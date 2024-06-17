#define SRP_SOURCE

#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "triangle.h"
#include "message_callback.h"
#include "utils.h"
#include "vec.h"
#include "defines.h"

static void drawBuffer(
	const SRPFramebuffer* fb, const SRPIndexBuffer* this,
	const SRPVertexBuffer* vb, SRPPrimitive primitive, size_t startIndex,
	size_t count, const SRPShaderProgram* sp
);
static uint64_t indexIndexBuffer(const SRPIndexBuffer* this, size_t index);
static SRPVertex* indexVertexBuffer(const SRPVertexBuffer* this, size_t index);

// An internal function to draw either index or vertex buffer
// If `ib == NULL`, draws the vertex buffer, else draws index buffer
// Used in `srpDrawVertexBuffer` and `srpDrawIndexBuffer`
// Created because vertex and index buffer drawing are very similar,
// with an intent to avoid code duplication
static void drawBuffer(
	const SRPFramebuffer* fb, const SRPIndexBuffer* ib,
	const SRPVertexBuffer* vb, SRPPrimitive primitive, size_t startIndex,
	size_t count, const SRPShaderProgram* sp
)
{
	const bool isDrawingIndexBuffer = (ib != NULL);

	if (primitive != PRIMITIVE_TRIANGLES)
	{
		srpMessageCallbackHelper(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Only triangles are implemented"
		);
		return;
	}

	// Index of the last vertex that will be touched
	// That is, only vertices with indices [startIndex, endIndex] will be drawn
	const size_t endIndex = startIndex + count - 1;
	const size_t bufferSize = (isDrawingIndexBuffer) ? ib->nIndices : vb->nVertices;
	if (endIndex >= bufferSize)
	{
		const char* errorMessage = (isDrawingIndexBuffer) ? \
			"Attempt to OOB access index buffer (read) at indices %i-%i (size: %i)\n" : \
			"Attempt to OOB access vertex buffer (read) at indices %i-%i (size: %i)\n";
		srpMessageCallbackHelper(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			errorMessage, startIndex, endIndex, bufferSize
		);
		return;
	}

	// Allocate memory for three vertex shader output variables (triangle = 3 vertices)
	SRPVertexVariable* triangleOutputVertexVariables = \
		SRP_MALLOC(sp->vs.nBytesPerOutputVariables * 3);
	size_t primitiveID = 0;

	for (size_t i = startIndex; i <= endIndex; i += 3)
	{
		SRPvsInput vsIn[3];
		SRPvsOutput vsOut[3];
		for (uint8_t j = 0; j < 3; j++)
		{
			size_t vertexIndex;
			if (isDrawingIndexBuffer)
				vertexIndex = indexIndexBuffer(ib, i+j);
			else
				vertexIndex = i+j;
			SRPVertex* pVertex = indexVertexBuffer(vb, vertexIndex);
			SRPVertexVariable* pOutputVertexVariables = (SRPVertexVariable*) \
				INDEX_VOID_PTR(triangleOutputVertexVariables, j, sp->vs.nBytesPerOutputVariables);

			vsIn[j] = (SRPvsInput) {
				.vertexID = i+j,
				.pVertex = pVertex,
				.uniform = sp->uniform
			};
			vsOut[j] = (SRPvsOutput) {
				.position = {0},
				.pOutputVariables = pOutputVertexVariables
			};

			sp->vs.shader(&vsIn[j], &vsOut[j]);

			// Perspective divide
			vsOut[j].position[0] = vsOut[j].position[0] / vsOut[j].position[3],
			vsOut[j].position[1] = vsOut[j].position[1] / vsOut[j].position[3];
			vsOut[j].position[2] = vsOut[j].position[2] / vsOut[j].position[3];
			vsOut[j].position[3] = 1.;
		}
		drawTriangle(fb, vsOut, sp, primitiveID);
		primitiveID++;
	}

	SRP_FREE(triangleOutputVertexVariables);
}

SRPVertexBuffer* srpNewVertexBuffer(
	size_t nBytesPerVertex, size_t nBytesData, const void* data
)
{
	SRPVertexBuffer* this = SRP_MALLOC(sizeof(SRPVertexBuffer));

	this->nBytesPerVertex = nBytesPerVertex;
	this->nBytesData = nBytesData;
	this->nVertices = nBytesData / nBytesPerVertex;
	this->data = SRP_MALLOC(nBytesData);
	memcpy(this->data, data, nBytesData);

	return this;
}

void srpFreeVertexBuffer(SRPVertexBuffer* this)
{
	SRP_FREE(this->data);
	SRP_FREE(this);
}

void srpDrawVertexBuffer(
	const SRPFramebuffer* fb, const SRPVertexBuffer* this, SRPPrimitive primitive,
	size_t startIndex, size_t count, const SRPShaderProgram* sp
)
{
	drawBuffer(fb, NULL, this, primitive, startIndex, count, sp);
}

static SRPVertex* indexVertexBuffer(const SRPVertexBuffer* this, size_t index)
{
	return (SRPVertex*) INDEX_VOID_PTR(this->data, index, this->nBytesPerVertex);
}

SRPIndexBuffer* srpNewIndexBuffer(
	Type indicesType, size_t nBytesData, const void* data
)
{
	SRPIndexBuffer* this = SRP_MALLOC(sizeof(SRPIndexBuffer));

	this->indicesType = indicesType;
	this->nBytesPerIndex = SIZEOF_TYPE(indicesType);
	this->nIndices = nBytesData / this->nBytesPerIndex;
	this->data = SRP_MALLOC(nBytesData);
	memcpy(this->data, data, nBytesData);

	return this;
}

void srpFreeIndexBuffer(SRPIndexBuffer* this)
{
	SRP_FREE(this->data);
	SRP_FREE(this);
}

static uint64_t indexIndexBuffer(const SRPIndexBuffer* this, size_t index)
{
	void* pIndex = INDEX_VOID_PTR(this->data, index, this->nBytesPerIndex);
	uint64_t ret;
	switch (this->indicesType)
	{
		case TYPE_UINT8:
			ret = (uint64_t) (*(uint8_t*) pIndex);
			break;
		case TYPE_UINT16:
			ret = (uint64_t) (*(uint16_t*) pIndex);
			break;
		case TYPE_UINT32:
			ret = (uint64_t) (*(uint32_t*) pIndex);
			break;
		case TYPE_UINT64:
			ret = (uint64_t) (*(uint64_t*) pIndex);
			break;
		default:
			srpMessageCallbackHelper(
				MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
				"Unexpected type (%i)", this->indicesType
			);
			ret = 0;
	}
	return ret;
}

// @brief Draw an index buffer with specified primitive mode
void srpDrawIndexBuffer(
	const SRPFramebuffer* fb, const SRPIndexBuffer* this,
	const SRPVertexBuffer* vb, SRPPrimitive primitive, size_t startIbIndex,
	size_t count, const SRPShaderProgram* sp
)
{
	drawBuffer(fb, this, vb, primitive, startIbIndex, count, sp);
}


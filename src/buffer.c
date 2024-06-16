#define SRP_SOURCE

#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "triangle.h"
#include "message_callback.h"
#include "utils.h"
#include "vec.h"
#include "defines.h"

static uint64_t indexIndexBuffer(SRPIndexBuffer* this, size_t index);
static SRPVertex* indexVertexBuffer(SRPVertexBuffer* this, size_t index);

SRPVertexBuffer* srpNewVertexBuffer(size_t nBytesPerVertex, size_t nBytesData, void* data)
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

static SRPVertex* indexVertexBuffer(SRPVertexBuffer* this, size_t index)
{
	return (SRPVertex*) ((uint8_t*) this->data + this->nBytesPerVertex * index);
}

SRPIndexBuffer* srpNewIndexBuffer(Type indicesType, size_t nBytesData, void* data)
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

static uint64_t indexIndexBuffer(SRPIndexBuffer* this, size_t index)
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
			messageCallback(
				MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
				"Unexpected type (%i)", this->indicesType
			);
			ret = 0;
	}
	return ret;
}

// @brief Draw an index buffer with specified primitive mode
void srpDrawIndexBuffer(
	SRPFramebuffer* fb, SRPIndexBuffer* this, SRPVertexBuffer* vb, SRPPrimitive primitive,
	size_t startIbIndex, size_t count, SRPShaderProgram* sp
)
{
	if (primitive != PRIMITIVE_TRIANGLES)
	{
		messageCallback(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Only triangles are implemented"
		);
		return;
	}

	size_t endIbIndex = startIbIndex + count;
	if (endIbIndex > this->nIndices)
	{
		messageCallback(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Attempt to OOB access index buffer"
		);
		return;
	}

	VSOutputVariable* triangleVSOutputVariables = \
		SRP_MALLOC(sp->vs.nBytesPerOutputVariables * 3);
	size_t primitiveID = 0;

	for (size_t i = startIbIndex; i < endIbIndex; i += 3)
	{
		SRPvsInput vsIn[3];
		SRPvsOutput vsOut[3];
		for (size_t j = 0; j < 3; j++)
		{
			uint64_t vertexIndex = indexIndexBuffer(this, i + j);
			SRPVertex* pInVertex = indexVertexBuffer(vb, vertexIndex);
			VSOutputVariable* pVSOutputVariables = (VSOutputVariable*) \
				INDEX_VOID_PTR(triangleVSOutputVariables, j, sp->vs.nBytesPerOutputVariables);

			vsIn[j] = (SRPvsInput) {
				.vertexID = i+j,
				.pVertex = pInVertex,
				.uniform = sp->uniform
			};
			vsOut[j] = (SRPvsOutput) {
				.position = {0},
				.pOutputVariables = pVSOutputVariables
			};

			sp->vs.shader(&vsIn[j], &vsOut[j]);
			vsOut[j].position = (vec4d) {
				vsOut[j].position.x / vsOut[j].position.w,
				vsOut[j].position.y / vsOut[j].position.w,
				vsOut[j].position.z / vsOut[j].position.w,
				1.
			};
		}
		drawTriangle(fb, vsOut, sp, primitiveID);
		primitiveID++;
	}

	SRP_FREE(triangleVSOutputVariables);
}


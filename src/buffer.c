#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "shaders.h"
#include "Type.h"
#include "log.h"
#include "memoryUtils/memoryUtils.h"
#include "buffer.h"
#include "triangle.h"
#include "voidptr.h"

VertexBuffer* newVertexBuffer(
	size_t nBytesPerVertex, size_t nBytesData, void* data, 
	size_t nAttributes, VertexAttribute* attributes
)
{
	VertexBuffer* this = xmalloc(sizeof(VertexBuffer));

	this->nBytesPerVertex = nBytesPerVertex;
	this->nBytesData = nBytesData;
	this->nVertices = nBytesData / nBytesPerVertex;
	this->data = xmalloc(nBytesData);
	memcpy(this->data, data, nBytesData);
	this->nAttributes = nAttributes;
	this->attributes = xmalloc(nAttributes * sizeof(VertexAttribute));
	memcpy(this->attributes, attributes, nAttributes * sizeof(VertexAttribute));

	return this;
}

void freeVertexBuffer(VertexBuffer* this)
{
	xfree(this->data);
	xfree(this->attributes);
	xfree(this);
}

static Vertex* indexVertexBuffer(VertexBuffer* this, size_t index)
{
	return (Vertex*) ((uint8_t*) this->data + this->nBytesPerVertex * index);
}

// @brief Draw vertex array without calling vertex and geometry shaders
// Needed because geometry shader can output multiple primitives to draw
// Intended to use inside `drawIndexBuffer` after calling these shaders
static void drawRawVertexBuffer(
	Framebuffer* fb, GSOutput* gsOutput, ShaderProgram* sp, 
	Primitive primitive
)
{
	assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");

	GeometryShader* gs = &sp->geometryShader;
	size_t n = (gs->nOutputVertices == 0) ? 3 : gs->nOutputVertices;
	for (size_t i = 0; i < n; i += 3)
	{
		GSOutput* gsOutputTriangle = (GSOutput*) \
			INDEX_VOID_PTR(gsOutput, i, gs->nBytesPerOutputVertex);
		drawTriangle(fb, gsOutputTriangle, sp);
	}
}

IndexBuffer* newIndexBuffer(Type indicesType, size_t nBytesData, void* data)
{
	IndexBuffer* this = xmalloc(sizeof(IndexBuffer));

	this->indicesType = indicesType;
	this->nBytesPerIndex = SIZEOF_TYPE(indicesType);
	this->nIndices = nBytesData / this->nBytesPerIndex;
	this->data = xmalloc(nBytesData);
	memcpy(this->data, data, nBytesData);

	return this;
}

void freeIndexBuffer(IndexBuffer* this)
{
	xfree(this->data);
	xfree(this);
}

static uint64_t indexIndexBuffer(IndexBuffer* this, size_t index)
{
	void* pIndex = ((uint8_t*) this->data + this->nBytesPerIndex * index);
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
			LOGE("Unhandled type (%i) in %s", this->indicesType, __func__);
			ret = 0;
	}
	return ret;
}

// @brief Draw an index buffer with specified primitive mode
void drawIndexBuffer(
	Framebuffer* fb, IndexBuffer* this, VertexBuffer* vb, Primitive primitive, 
	size_t startIndex, size_t count, ShaderProgram* sp
)
{
	assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");

	VertexShader* vs = &sp->vertexShader;
	GeometryShader* gs = &sp->geometryShader;

	VSOutput* triangleVsOutput = xmalloc(vs->nBytesPerOutputVertex * 3);
	GSOutput* gsOutput;
	if (gs->shader == NULL)
		gsOutput = (GSOutput*) triangleVsOutput;
	else
		gsOutput = xmalloc(
			gs->nBytesPerOutputVertex * \
			gs->nOutputVertices
		);

#ifndef NDEBUG
	assert(
		gs->inputPrimitive == PRIMITIVE_ANY ||
		(
			primitive == gs->inputPrimitive && \
			"Input primitive type for geometry shader and primitive type \
			passed mismatch"
		)
	);
#endif

	size_t endIndex = startIndex + count;
	assert(endIndex <= this->nIndices);

	for (size_t i = startIndex; i < endIndex; i += 3)
	{
		for (size_t j = 0; j < 3; j++)
		{
			uint64_t vertexIndex = indexIndexBuffer(this, i + j);
			Vertex* pVertex = indexVertexBuffer(vb, vertexIndex);
			VSOutput* pVsOutput = (VSOutput*) \
				INDEX_VOID_PTR(triangleVsOutput, j, vs->nBytesPerOutputVertex);
			vs->shader(sp, pVertex, pVsOutput, i+j);
		}

		Primitive newPrimitive = \
			(gs->outputPrimitive == PRIMITIVE_ANY) ? \
			primitive : gs->outputPrimitive;
		if (gs->shader != NULL)
			gs->shader(sp, triangleVsOutput, gsOutput);

		drawRawVertexBuffer(fb, gsOutput, sp, newPrimitive);
	}

	// That means a distinct buffer for GS was allocated
	if ((void*) triangleVsOutput != (void*) gsOutput)
		xfree(gsOutput);
	xfree(triangleVsOutput);
}


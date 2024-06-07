#include <string.h>
#include "buffer.h"
#include "memoryUtils/memoryUtils.h"
#include "triangle.h"
#include "voidptr.h"
#include "log.h"

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

	VSOutput* triangleVsOutput = xmalloc(sp->vs.nBytesPerOutputVertex * 3);

	size_t endIndex = startIndex + count;
	assert(endIndex <= this->nIndices);

	for (size_t i = startIndex; i < endIndex; i += 3)
	{
		for (size_t j = 0; j < 3; j++)
		{
			uint64_t vertexIndex = indexIndexBuffer(this, i + j);
			Vertex* pVertex = indexVertexBuffer(vb, vertexIndex);
			VSOutput* pVsOutput = (VSOutput*) \
				INDEX_VOID_PTR(triangleVsOutput, j, sp->vs.nBytesPerOutputVertex);
			sp->vs.shader(sp, pVertex, pVsOutput, i+j);
		}
		drawTriangle(fb, triangleVsOutput, sp);
	}

	xfree(triangleVsOutput);
}


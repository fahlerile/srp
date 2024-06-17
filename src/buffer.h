#pragma once

#include "framebuffer.h"
#include "primitive.h"
#include "shaders.h"

// Stores vertex data
// Similar to VBO in OpenGL
typedef struct SRPVertexBuffer
{
	size_t nBytesPerVertex;
	size_t nBytesData;
	size_t nVertices;
	SRPVertex* data;
} SRPVertexBuffer;

// Stores indices to vertices stored in `VertexBuffer`
// Similar to EBO in OpenGL
typedef struct SRPIndexBuffer
{
	Type indicesType;
	size_t nBytesPerIndex;
	size_t nIndices;
	void* data;
} SRPIndexBuffer;

// TODO: drawVertexBuffer
SRPVertexBuffer* srpNewVertexBuffer(
	size_t nBytesPerVertex, size_t nBytesData, const void* data
);
void srpFreeVertexBuffer(SRPVertexBuffer* this);
void srpDrawVertexBuffer(
	const SRPFramebuffer* fb, const SRPVertexBuffer* this, SRPPrimitive primitive,
	size_t startIndex, size_t count, const SRPShaderProgram* sp
);

SRPIndexBuffer* srpNewIndexBuffer(
	Type indicesType, size_t nBytesData, const void* data
);
void srpFreeIndexBuffer(SRPIndexBuffer* this);
void srpDrawIndexBuffer(
	const SRPFramebuffer* fb, const SRPIndexBuffer* this,
	const SRPVertexBuffer* vb, SRPPrimitive primitive, size_t startIndex,
	size_t count, const SRPShaderProgram* sp
);


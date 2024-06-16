#pragma once

#include "Framebuffer.h"
#include "Primitive.h"
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
SRPVertexBuffer* srpNewVertexBuffer(size_t nBytesPerVertex, size_t nBytesData, void* data);
void srpFreeVertexBuffer(SRPVertexBuffer* this);

SRPIndexBuffer* srpNewIndexBuffer(Type indicesType, size_t nBytesData, void* data);
void srpFreeIndexBuffer(SRPIndexBuffer* this);
void srpDrawIndexBuffer(
	SRPFramebuffer* fb, SRPIndexBuffer* this, SRPVertexBuffer* vb,
	SRPPrimitive primitive, size_t startIndex, size_t count, SRPShaderProgram* sp
);


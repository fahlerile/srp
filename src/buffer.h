#pragma once
#include "Framebuffer.h"
#include "Primitive.h"
#include "Vertex.h"
#include "shaders.h"

// Stores vertex data
// Similar to VBO in OpenGL
typedef struct
{
	size_t nBytesPerVertex;
	size_t nBytesData;
	size_t nVertices;
	Vertex* data;
	size_t nAttributes;
	VertexAttribute* attributes;
} VertexBuffer;

// Stores indices to vertices stored in `VertexBuffer`
// Similar to EBO in OpenGL
typedef struct
{
	Type indicesType;
	size_t nBytesPerIndex;
	size_t nIndices;
	void* data;
} IndexBuffer;

VertexBuffer* newVertexBuffer(
	size_t nBytesPerVertex, size_t nBytesData, void* data, 
	size_t nAttributes, VertexAttribute* attributes
);
void freeVertexBuffer(VertexBuffer* this);
static Vertex* indexVertexBuffer(VertexBuffer* this, size_t index);

IndexBuffer* newIndexBuffer(Type indicesType, size_t nBytesData, void* data);
void freeIndexBuffer(IndexBuffer* this);
static uint64_t indexIndexBuffer(IndexBuffer* this, size_t index);
void drawIndexBuffer(
	Framebuffer* fb, IndexBuffer* this, VertexBuffer* vb, Primitive primitive, 
	size_t startIndex, size_t count, ShaderProgram* sp
);


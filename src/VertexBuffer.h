#pragma once
#include "Vector/Vector.h"
#include "DynamicArray/DynamicArray.h"
#include "Uniforms.h"

typedef struct
{
    void* buffer;
    size_t nBytesPerVertex;
    size_t nVertices;
    size_t* attributeOffsets;
    size_t nAttributes;
} VertexBuffer;

#include "Shaders.h"

typedef enum
{
    DRAW_MODE_LINES = 0,
    DRAW_MODE_TRIANGLES
} DrawMode;

// `attributeOffsets` is assumed to be sorted
VertexBuffer* newVertexBuffer(
    void* data, size_t nBytesPerVertex, size_t nVertices,
    size_t* attributeOffsets, size_t nAttributes
);
void freeVertexBuffer(VertexBuffer* this);

void* VertexBufferGetVertexPointer(VertexBuffer* vertexBuffer, size_t i);
void* VertexPointerGetAttributePointerByIndex(
    VertexBuffer* vertexBuffer, void* p_vertex, size_t attributeI
);

void drawVertexBuffer(
    VertexBuffer* vertexBuffer, DrawMode drawMode, size_t startIndex, size_t count
);


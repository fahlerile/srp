#pragma once
#include "DynamicArray/DynamicArray.h"

typedef enum
{
    TYPE_DOUBLE,
    TYPE_UINT8
} AttributeType;

typedef struct
{
    void* buffer;
    size_t nBytesPerVertex;
    size_t nVertices;
    size_t* attributeOffsets;
    size_t nAttributes;
} VertexBuffer;

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


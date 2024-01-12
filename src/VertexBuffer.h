#pragma once
#include "DynamicArray/DynamicArray.h"

typedef enum
{
    TYPE_DOUBLE,
    TYPE_UINT8
} AttributeType;

typedef struct
{
    AttributeType type;
    size_t countOfElements;
    size_t offset;
} VertexAttribute;

typedef struct
{
    void* buffer;
    size_t nBytesPerVertex;
    size_t nVertices;
    DynamicArray* vertexAttributes;  // VertexAttribute
    size_t nAttributes;
} VertexBuffer;

VertexBuffer* newVertexBuffer(
    void* data, size_t nBytesPerVertex, size_t nVertices
);
void VertexBufferConfigureAttribute(
    VertexBuffer* this, size_t attributeIndex,
    size_t countOfElementsInAttribute, AttributeType attributeType,
    size_t attributeOffset
);

void* VertexBufferGetVertexPointer(VertexBuffer* VertexBuffer, size_t i);

void freeVertexBuffer(VertexBuffer* this);


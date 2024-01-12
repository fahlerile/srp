#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "memoryUtils/memoryUtils.h"
#include "VertexBuffer.h"

VertexBuffer* newVertexBuffer(
    void* data, size_t nBytesPerVertex, size_t nVertices
)
{
    VertexBuffer* this = xmalloc(sizeof(VertexBuffer));

    this->buffer = xmalloc(nBytesPerVertex * nVertices);
    memcpy(this->buffer, data, nBytesPerVertex * nVertices);
    this->nBytesPerVertex = nBytesPerVertex;
    this->nVertices = nVertices;
    this->vertexAttributes = newDynamicArray(1, sizeof(VertexAttribute), NULL);

    return this;
}

void VertexBufferConfigureAttribute(
    VertexBuffer* this, size_t attributeIndex,
    size_t countOfElementsInAttribute, AttributeType attributeType,
    size_t attributeOffset
)
{
    VertexAttribute attr = {
        .type = attributeType,
        .countOfElements = countOfElementsInAttribute,
        .offset = attributeOffset
    };
    setInDynamicArray(this->vertexAttributes, &attr, attributeIndex);
}

void* VertexBufferGetVertexPointer(VertexBuffer* vertexBuffer, size_t i)
{
    assert(vertexBuffer->nVertices > i);
    return indexVoidPointer(
        vertexBuffer->buffer, i, vertexBuffer->nBytesPerVertex
    );
}

void freeVertexBuffer(VertexBuffer* this)
{
    freeDynamicArray(this->vertexAttributes);
    xfree(this->buffer);
    xfree(this);
}


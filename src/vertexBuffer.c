#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "memoryUtils/memoryUtils.h"
#include "VertexBuffer.h"

VertexBuffer* newVertexBuffer(
    void* data, size_t nBytesPerVertex, size_t nVertices,
    size_t* attributeOffsets, size_t nAttributes
)
{
    VertexBuffer* this = xmalloc(sizeof(VertexBuffer));

    this->buffer = xmalloc(nBytesPerVertex * nVertices);
    memcpy(this->buffer, data, nBytesPerVertex * nVertices);
    this->nBytesPerVertex = nBytesPerVertex;
    this->nVertices = nVertices;

    this->attributeOffsets = xmalloc(sizeof(size_t) * nAttributes);
    memcpy(
        this->attributeOffsets, attributeOffsets,
        sizeof(size_t) * nAttributes
    );
    this->nAttributes = nAttributes;

    return this;
}

void freeVertexBuffer(VertexBuffer* this)
{
    xfree(this->buffer);
    xfree(this);
}

void* VertexBufferGetVertexPointer(VertexBuffer* this, size_t i)
{
    assert(this->nVertices > i);
    return indexVoidPointer(
        this->buffer, i, this->nBytesPerVertex
    );
}

void* VertexPointerGetAttributePointerByIndex(
    VertexBuffer* this, void* p_vertex, size_t attributeI
)
{
    assert(attributeI < this->nAttributes);
    return ((char*) p_vertex)[this->attributeOffsets[attributeI]];
}


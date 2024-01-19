#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "draw.h"
#include "memoryUtils/memoryUtils.h"
#include "VertexBuffer.h"
#include "Context.h"

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
    xfree(this->attributeOffsets);
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
    return ((char*) p_vertex) + this->attributeOffsets[attributeI];
}

void drawVertexBuffer(
    DrawMode drawMode, size_t startIndex, size_t count, 
    VertexBuffer* vertexBuffer, VertexShaderType vertexShader, FragmentShaderType fragmentShader
)
{
    assert(drawMode == DRAW_MODE_TRIANGLES && "Only triangles are implemented");

    for (size_t i = startIndex, n = startIndex + count; i < n; i += 3)
    {
        Vector4d transformedPositionsHomogenous[3] = {
            {0., 0., 0., 0.},
            {0., 0., 0., 0.},
            {0., 0., 0., 0.}
        };
        Vector3d transformedPositions[3];

        for (size_t j = 0; j < 3; j++)
        {
            vertexShader(
                VertexBufferGetVertexPointer(vertexBuffer, i+j),
                vertexBuffer, &(transformedPositionsHomogenous[j])
            );
            transformedPositions[j] = \
                Vector4dHomogenousDivide(transformedPositionsHomogenous[j]);
        }
        drawTriangle(transformedPositions);
    }
}


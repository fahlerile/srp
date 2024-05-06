#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "Shaders.h"
#include "memoryUtils/memoryUtils.h"
#include "buffer.h"
#include "triangle.h"

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
    xfree(this);
}

// @brief Draw vertex array without calling vertex and geometry shaders
// Needed because geometry shader can output multiple primitives to draw
// Intended to use inside `drawIndexBuffer` after calling these shaders
static void drawRawVertexBuffer(GSOutput* gsOutput, ShaderProgram* sp, Primitive primitive)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");

    GeometryShader* gs = &sp->geometryShader;
    size_t n = (gs->nOutputVertices == 0) ? 3 : gs->nOutputVertices;
    for (size_t i = 0; i < n; i += 3)
    {
        GSOutput* gsOutputTriangle = (GSOutput*) (
            (uint8_t*) gsOutput + (i * gs->nBytesPerOutputVertex)
        );
        drawTriangle(gsOutputTriangle, sp);
    }
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

// @brief Draw an index buffer with specified primitive mode
void drawIndexBuffer(
    IndexBuffer* this, VertexBuffer* vb, Primitive primitive, 
    size_t startIndex, size_t count, ShaderProgram* sp
)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");

    VertexShader* vs = &sp->vertexShader;
    GeometryShader* gs = &sp->geometryShader;

    void* triangleVsOutput = xmalloc(vs->nBytesPerOutputVertex * 3);
    void* gsOutput;
    if (gs->shader == NULL)
        gsOutput = triangleVsOutput;
    else
        gsOutput = xmalloc(
            gs->nBytesPerOutputVertex * \
            gs->nOutputVertices
        );

#ifndef NDEBUG
    assert(
        gs->inputPrimitive == PRIMITIVE_ANY ||
        (
            primitive == gs->inputPrimitive && \
            "Input primitive type for geometry shader and primitive type \
            passed mismatch"
        )
    );
#endif

    size_t endIndex = startIndex + count;
    assert(endIndex <= this->nIndices);

    for (size_t i = startIndex; i < endIndex; i += 3)
    {
        for (size_t j = 0; j < 3; j++)
        {
            // TODO not necessarily size_t!
            size_t vertexIndex = *(size_t*) ((uint8_t*) this->data + this->nBytesPerIndex * (i+j));

            Vertex* pVertex = (Vertex*) ((uint8_t*) vb->data + vb->nBytesPerVertex * vertexIndex);
            VSOutput* pVsOutput = (VSOutput*) (
                (uint8_t*) triangleVsOutput + vs->nBytesPerOutputVertex * j
            );
            vs->shader(sp, pVertex, pVsOutput);
        }

        Primitive newPrimitive = \
            (gs->outputPrimitive == PRIMITIVE_ANY) ? \
            primitive : gs->outputPrimitive;
        if (gs->shader != NULL)
            gs->shader(sp, triangleVsOutput, gsOutput);

        drawRawVertexBuffer(gsOutput, sp, newPrimitive);
    }

    // That means a distinct buffer for GS was allocated
    if ((void*) triangleVsOutput != (void*) gsOutput)
        xfree(gsOutput);
    xfree(triangleVsOutput);
}


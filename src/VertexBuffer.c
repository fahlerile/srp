#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "memoryUtils/memoryUtils.h"
#include "VertexBuffer.h"
#include "log.h"
#include "Context.h"
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

// @brief Draw a vertex buffer with specified primitive mode
// If sp->geometryShader.shader is NULL (so if geometry shader is not set),
// then geometry shader is set to a default one
void drawVertexBuffer(
    VertexBuffer* this, Primitive primitive, size_t startIndex, size_t count, 
    ShaderProgram* sp
)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");

    VertexShaderType* vs = &sp->vertexShader;
    GeometryShaderType* gs = &sp->geometryShader;

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
    assert(endIndex <= this->nVertices);

    for (size_t i = startIndex; i < endIndex; i += 3)
    {
        for (size_t j = 0; j < 3; j++)
        {
            void* pVertex = (uint8_t*) this->data + this->nBytesPerVertex * (i+j);
            void* pVsOutput = (uint8_t*) triangleVsOutput + vs->nBytesPerOutputVertex * j;
            vs->shader(sp, pVertex, pVsOutput);
        }

        Primitive newPrimitive = \
            (gs->outputPrimitive == PRIMITIVE_ANY) ? \
            primitive : gs->outputPrimitive;
        if (gs->shader != NULL)
            gs->shader(sp, triangleVsOutput, gsOutput);

        drawRawVertexBuffer(gsOutput, sp, newPrimitive);
    }

    if (triangleVsOutput != gsOutput)
        xfree(gsOutput);
    xfree(triangleVsOutput);
}

// @brief Draw vertex array without calling vertex and geometry shaders
// Needed because geometry shader can output multiple primitives to draw
// Intended to use inside `drawVertexBuffer` after calling these shaders
static void drawRawVertexBuffer(void* gsOutput, ShaderProgram* sp, Primitive primitive)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");

    GeometryShaderType* gs = &sp->geometryShader;
    size_t n = (gs->nOutputVertices == 0) ? 3 : gs->nOutputVertices;
    for (size_t i = 0; i < n; i += 3)
    {
        void* gsOutputTriangle = \
            (uint8_t*) gsOutput + (i * gs->nBytesPerOutputVertex);
        drawTriangle(gsOutputTriangle, sp);
    }
}


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
    this->attributes = attributes;

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
    
    void* triangleVsOutput = xmalloc(sp->vertexShader.nBytesPerOutputVertex * 3);
    void* gsOutput;
    if (sp->geometryShader.shader == NULL)
    {
        sp->geometryShader.nBytesPerOutputVertex = \
            sp->vertexShader.nBytesPerOutputVertex;
        sp->geometryShader.nOutputAttributes = \
            sp->vertexShader.nOutputAttributes;
        // points to the same buffer!
        sp->geometryShader.outputAttributes = \
            sp->vertexShader.outputAttributes;
        sp->geometryShader.indexOfOutputPositionAttribute = \
            sp->vertexShader.indexOfOutputPositionAttribute;
        // this assumes that primitive == triangle
        sp->geometryShader.nOutputVertices = 3;
        sp->geometryShader.inputPrimitive = primitive;
        sp->geometryShader.outputPrimitive = primitive;
        gsOutput = triangleVsOutput;
    }
    else
        gsOutput = xmalloc(
            sp->geometryShader.nBytesPerOutputVertex * \
            sp->geometryShader.nOutputVertices
        );

#ifndef NDEBUG
    assert(
        primitive == sp->geometryShader.inputPrimitive && \
        "Input primitive type for geometry shader and primitive type passed \
        to drawVertexBuffer mismatch"
    );
#endif

    size_t endIndex = startIndex + count;
    assert(endIndex <= this->nVertices);

    for (size_t i = startIndex; i < endIndex; i += 3)
    {
        for (size_t j = 0; j < 3; j++)
        {
            void* pVertex = (uint8_t*) this->data + (this->nBytesPerVertex * (i+j));
            sp->vertexShader.shader(
                sp, pVertex, (char*) triangleVsOutput + sp->vertexShader.nBytesPerOutputVertex * j
            );
        }

        Primitive newPrimitive = sp->geometryShader.outputPrimitive;;
        if (sp->geometryShader.shader != NULL)
            sp->geometryShader.shader(sp, triangleVsOutput, gsOutput);

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

    size_t n = sp->geometryShader.nOutputVertices;
    for (size_t i = 0; i < n; i += 3)
    {
        drawTriangle(
            (uint8_t*) gsOutput + (i * sp->geometryShader.nBytesPerOutputVertex), sp
        );
    }
}


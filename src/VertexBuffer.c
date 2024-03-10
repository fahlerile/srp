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

void drawVertexBuffer(
    VertexBuffer* this, Primitive primitive, size_t startIndex, size_t count, 
    ShaderProgram* sp
)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");
#ifndef NDEBUG
    if (sp->geometryShader.shader != NULL)
        assert(
            primitive == sp->geometryShader.inputPrimitive && \
            "Input primitive type for geometry shader and primitive passed to \
            drawVertexBuffer mismatch"
        );
#endif

    size_t endIndex = startIndex + count;
    assert(endIndex <= this->nVertices);

    void* triangleVsOutput = xmalloc(sp->vertexShader.nBytesPerVertex * 3);
    void* gsOutput;
    if (sp->geometryShader.shader == NULL)
        gsOutput = triangleVsOutput;
    else
        gsOutput = xmalloc(
            sp->geometryShader.nBytesPerVertex * \
            sp->geometryShader.nVertices
        );

    for (size_t i = startIndex; i < endIndex; i += 3)
    {
        for (size_t j = 0; j < 3; j++)
        {
            void* pVertex = (uint8_t*) this->data + (this->nBytesPerVertex * (i+j));
            sp->vertexShader.shader(
                pVertex, (char*) triangleVsOutput + \
                sp->vertexShader.nBytesPerVertex * j
            );
        }

        Primitive newPrimitive;
        if (sp->geometryShader.shader != NULL)
        {
            sp->geometryShader.shader(triangleVsOutput, gsOutput);
            newPrimitive = sp->geometryShader.outputPrimitive;
        }
        else
            newPrimitive = primitive;

        drawPrimitive(gsOutput, sp, newPrimitive);
    }

    if (triangleVsOutput != gsOutput)
        xfree(gsOutput);
    xfree(triangleVsOutput);
}

static void drawPrimitive(void* gsOutput, ShaderProgram* sp, Primitive primitive)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");
    
    switch (primitive)
    {
        case PRIMITIVE_TRIANGLES:
            drawTriangle(gsOutput, sp);
            break;
        default:
            LOGE("Unknown primitive type (%i) in drawPrimitive", primitive);
            break;
    }
}


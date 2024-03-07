#include <assert.h>
#include <stdint.h>
#include "memoryUtils/memoryUtils.h"
#include "VertexBuffer.h"

VertexBuffer* newVertexBuffer(
    size_t nBytesPerVertex, size_t nBytesData, void* data, 
    size_t nAttributes, VertexAttribute* attributes
)
{
    VertexBuffer* this = xmalloc(sizeof(VertexBuffer));

    this->nBytesPerVertex = nBytesPerVertex;
    this->nBytesData = nBytesData;
    this->nVertices = nBytesData / nBytesPerVertex;
    this->data = data;
    this->nAttributes = nAttributes;
    this->attributes = attributes;

    return this;
}

void drawVertexBuffer(
    VertexBuffer* this, Primitive primitive, size_t startIndex, size_t count, 
    ShaderProgram* shaderProgram
)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");
#ifndef NDEBUG
    if (shaderProgram->geometryShader.shader != NULL)
        assert(
            primitive == shaderProgram->geometryShader.inputPrimitive && \
            "Primitive type mismatches in geometryShader and call to drawVertexBuffer"
        );
#endif

    size_t endIndex = startIndex + count;
    void* triangleVsOutput = xmalloc(shaderProgram->vertexShader.nBytesPerVertex * 3);
    void* gsOutput;
    if (shaderProgram->geometryShader.shader == NULL)
        gsOutput = triangleVsOutput;
    else
        gsOutput = xmalloc(
            shaderProgram->geometryShader.nBytesPerVertex * \
            shaderProgram->geometryShader.nVertices
        );

    assert(startIndex + count <= this->nVertices);
    for (size_t i = startIndex; i < endIndex; i += 3)
    {
        for (size_t j = 0; j < 3; j++)
        {
            void* pVertex = (uint8_t*) this->data + (this->nBytesPerVertex * (i+j));
            shaderProgram->vertexShader.shader(
                pVertex, (char*) triangleVsOutput + \
                shaderProgram->vertexShader.nBytesPerVertex * j
            );
        }

        if (shaderProgram->geometryShader.shader != NULL)
            shaderProgram->geometryShader.shader(triangleVsOutput, gsOutput);
    }

    drawPrimitive(gsOutput, shaderProgram, primitive);
}


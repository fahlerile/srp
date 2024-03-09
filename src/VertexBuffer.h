#pragma once
#include <stddef.h>
#include "Type.h"
#include "Shaders.h"

typedef struct
{
    size_t nBytesPerVertex;
    size_t nBytesData;
    size_t nVertices;
    void* data;
    size_t nAttributes;
    VertexAttribute* attributes;
} VertexBuffer;

VertexBuffer* newVertexBuffer(
    size_t nBytesPerVertex, size_t nBytesData, void* data, 
    size_t nAttributes, VertexAttribute* attributes
);
void freeVertexBuffer(VertexBuffer* this);

void drawVertexBuffer(
    VertexBuffer* this, Primitive drawMode, size_t startIndex, size_t count, 
    ShaderProgram* shaderProgram
);

static void drawPrimitive(void* gsOutput, ShaderProgram* sp, Primitive primitive);


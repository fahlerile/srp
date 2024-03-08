#pragma once
#include "VertexAttribute.h"
#include "Primitive.h"

typedef struct
{
    void (*shader)(void* pVertex, void* pOutput);
    size_t nBytesPerVertex;
    size_t nAttributes;
    VertexAttribute* attributes;
    size_t indexOfPositionAttribute;
} VertexShader;

typedef struct
{
    void (*shader)(void* pInput, void* pOutput);
    size_t nBytesPerVertex;
    size_t nAttributes;
    VertexAttribute* attributes;
    size_t indexOfPositionAttribute;

    // Geometry shader specific
    size_t nVertices;
    Primitive inputPrimitive;
    Primitive outputPrimitive;
} GeometryShader;

typedef struct
{
    void (*shader)();
} FragmentShader;

typedef struct
{
    VertexShader vertexShader;
    GeometryShader geometryShader;
    FragmentShader fragmentShader;
} ShaderProgram;


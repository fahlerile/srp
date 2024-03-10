#pragma once
#include "VertexAttribute.h"
#include "Primitive.h"
#include "Color/Color.h"

typedef struct
{
    void (*shader)(void* sp, void* pVertex, void* pOutput);
    size_t nBytesPerVertex;
    size_t nAttributes;
    VertexAttribute* attributes;
    size_t indexOfPositionAttribute;
} VertexShaderType;

typedef struct
{
    void (*shader)(void* sp, void* pInput, void* pOutput);
    size_t nBytesPerVertex;
    size_t nAttributes;
    VertexAttribute* attributes;
    size_t indexOfPositionAttribute;

    // Geometry shader specific
    size_t nVertices;
    Primitive inputPrimitive;
    Primitive outputPrimitive;
} GeometryShaderType;

typedef struct
{
    void (*shader)(void* sp, void* pInterpolated, Color* color);
} FragmentShaderType;

typedef struct
{
    VertexShaderType vertexShader;
    GeometryShaderType geometryShader;
    FragmentShaderType fragmentShader;
} ShaderProgram;


#pragma once
#include "VertexAttribute.h"
#include "Primitive.h"
#include "Color/Color.h"

typedef struct
{
    void (*shader)(void* sp, void* pVertex, void* pOutput);
    size_t nBytesPerOutputVertex;
    size_t nOutputAttributes;
    VertexAttribute* outputAttributes;
    size_t indexOfOutputPositionAttribute;
} VertexShaderType;

typedef struct
{
    void (*shader)(void* sp, void* pInput, void* pOutput);
    size_t nBytesPerOutputVertex;
    size_t nOutputAttributes;
    VertexAttribute* outputAttributes;
    size_t indexOfOutputPositionAttribute;

    // Geometry shader specific
    // If both `inputPrimitive` and `outputPrimitive` are `PRIMITIVE_ANY` 
    // (so geometryShader was default-initialized) then `nOutputVertices`
    // is not valid and should be set accordingly to primitive type that 
    // is being processed
    size_t nOutputVertices;
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

void shaderProgramSetDefaultGeometryShader(ShaderProgram* sp);


#pragma once
#include <stddef.h>
#include "VertexBuffer.h"
#include "Type.h"

typedef struct
{
    size_t nBytes;
    size_t nAttributes;
    size_t* attributeOffsets;
    Type* attributeTypes;
} VertexShaderOutputInformation;

// The first element in the output buffer MUST ALWAYS be the Vector3d position
typedef void (*VertexShaderType)(
    Uniforms* uniform, void* p_vertex, VertexBuffer* vertexBuffer,
    void* outputBuffer
);
typedef void (*FragmentShaderType)(
    void* interpolatedVSOutput, Color* color
);

void* vertexShaderOutputGetAttributePointerByIndex(void* vsOutput, size_t index);

// `vsOutput` is an array of 3 vertex shader outputs
void interpolateVertexShaderOutputInTriangle(
    void* threeVSOutput, Vector3d barycentricCoordinates, void* interpolatedVSOutput
);


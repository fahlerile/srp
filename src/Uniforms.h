#pragma once
#include "DynamicArray/DynamicArray.h"

// Assumes that `data` is contiguous -> no gaps between uniforms
typedef struct
{
    void* data;
    size_t bytesAllocated;
    DynamicArray* offsets;  // `size_t`
    size_t nElements;
} Uniforms;

Uniforms* newUniforms();
void freeUniforms(Uniforms* this);

void addUniform(Uniforms* this, void* element, size_t nBytes);
static void reallocUniforms(Uniforms* this);


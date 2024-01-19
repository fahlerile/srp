#pragma once
#include "DynamicArray/DynamicArray.h"

// Assumes that `data` is contiguous -> no gaps between uniforms
// Assumes that the last element in the `offsets` array is the index of the first free byte
typedef struct
{
    void* data;
    size_t bytesAllocated;
    DynamicArray* offsets;  // `size_t`
    size_t nElements;
} Uniforms;

Uniforms* newUniforms();
void freeUniforms(Uniforms* this);
static void reallocUniforms(Uniforms* this);

// Returns the index of the added uniform
size_t addUniform(Uniforms* this, void* element, size_t nBytes);
void* getUniform(Uniforms* this, size_t index);


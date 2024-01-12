#pragma once

// Assumes that `data` is contiguous -> no gaps between uniforms
typedef struct
{
    void* data;
    size_t bytesAllocated;
    DynamicArray* offsets;  // `size_t`
    size_t nElements;
} Uniforms;

Uniforms* newUniforms();
void freeUniforms();

void addUniform(Uniforms* this, void* element, size_t nBytes);
void reallocUniforms(Uniforms* this);


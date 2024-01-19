#pragma once
#include "DynamicArray/DynamicArray.h"

// The user is responsible for freeing all the resources allocated by
// uniforms added by `addUniform`
// `pointers` store NULL if there is nothing at index
typedef struct
{
    DynamicArray* pointers;  // `void*`
} Uniforms;

Uniforms* newUniforms(size_t nUniforms);
void freeUniforms(Uniforms* this);
void reallocUniforms(Uniforms* this, size_t n);

void addUniform(Uniforms* this, size_t index, void* p_element, size_t nBytes);
void deleteUniform(Uniforms* this, size_t index);
void* getUniform(Uniforms* this, size_t index);


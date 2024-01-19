#include <string.h>
#include <stdint.h>
#include "memoryUtils/memoryUtils.h"
#include "Uniforms.h"

Uniforms* newUniforms()
{
    Uniforms* this = xmalloc(sizeof(Uniforms));

    this->bytesAllocated = 64;
    this->data = xmalloc(this->bytesAllocated);
    this->offsets = newDynamicArray(1, sizeof(size_t), NULL);
    addToDynamicArray(this->offsets, &((size_t) {0}));
    this->nElements = 0;
    
    return this;
}

void addUniform(Uniforms* this, void* element, size_t nBytes)
{
    size_t indexOfTheFirstFreeByte = \
        *(size_t*) indexDynamicArray(this->offsets, this->nElements);

    if (indexOfTheFirstFreeByte + 1 + nBytes > this->bytesAllocated)
        reallocUniforms(this);

    memcpy((uint8_t*) this->data + indexOfTheFirstFreeByte, element, nBytes);
    addToDynamicArray(this->offsets, &((size_t) {indexOfTheFirstFreeByte + nBytes}));
    this->nElements += 1;
}

static void reallocUniforms(Uniforms* this)
{
    this->bytesAllocated *= 2;
    this->data = xrealloc(this->data, this->bytesAllocated);
}

void freeUniforms(Uniforms* this)
{
    if (this->data != NULL)
        xfree(this->data);
    freeDynamicArray(this->offsets);
    xfree(this);
}


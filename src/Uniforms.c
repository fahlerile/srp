#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "memoryUtils/memoryUtils.h"
#include "Uniforms.h"

Uniforms* newUniforms(size_t nUniforms)
{
    Uniforms* this = xmalloc(sizeof(Uniforms));
    this->pointers = newDynamicArray(nUniforms, sizeof(void*), NULL);
    this->pointers->size = nUniforms;
    return this;
}

void freeUniforms(Uniforms* this)
{
    // free all allocated buffers (do not free unused/NULL ones)
    for (size_t i = 0; i < this->pointers->size; i++)
    {
        void* ptr = *(void**) indexDynamicArray(this->pointers, i);
        if (ptr != NULL)
            xfree(ptr);
    }
    freeDynamicArray(this->pointers);
    xfree(this);
}

void reallocUniforms(Uniforms* this, size_t n)
{
    reallocateDynamicArray(this->pointers, n);
    this->pointers->size = n;
}

void addUniform(Uniforms* this, size_t index, void* p_element, size_t nBytes)
{
    // assert that this slot is not already in use
    assert(*(void**) indexDynamicArray(this->pointers, index) == NULL);

    void* buffer = xmalloc(nBytes);
    memcpy(buffer, p_element, nBytes);
    setInDynamicArray(this->pointers, &buffer, index);
}

void modifyUniform(Uniforms* this, size_t index, void* p_element, size_t nBytes)
{
    assert(*(void**) indexDynamicArray(this->pointers, index) != NULL);

    void* buffer = *(void**) indexDynamicArray(this->pointers, index);
    memcpy(buffer, p_element, nBytes);
}

void deleteUniform(Uniforms* this, size_t index)
{
    setInDynamicArray(this->pointers, &((void*){NULL}), index);
}

void* getUniform(Uniforms* this, size_t index)
{
    void* ptr = *(void**) indexDynamicArray(this->pointers, index);
    assert(ptr != NULL);
    return ptr;
}


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "DynamicArray.h"
#include "memoryUtils/memoryUtils.h"

#define indexVoidPtr(ptr, i, nBytesPerElement) ((char*) ptr + (i * nBytesPerElement))

DynamicArray* newDynamicArray(size_t nElements, size_t nBytesPerElement, freeCallbackFunctionType freeCallback)
{
    DynamicArray* this = xmalloc(sizeof(DynamicArray));
    this->data = xcalloc(nElements, nBytesPerElement);
    this->size = 0;
    this->allocated = nElements;
    this->nBytesPerElement = nBytesPerElement;
    this->freeCallback = freeCallback;
    return this;
}

static void reallocateDynamicArray(DynamicArray* this)
{
    this->data = xrealloc(this->data, (this->allocated * 2) * this->nBytesPerElement);
    this->allocated = this->allocated * 2;
}

void addToDynamicArray(DynamicArray* this, void* element)
{
    if (this->size+1 > this->allocated)
        reallocateDynamicArray(this);

    memcpy(indexVoidPtr(this->data, this->size, this->nBytesPerElement), element, this->nBytesPerElement);
    this->size += 1;
}

void deleteLastInDynamicArray(DynamicArray* this)
{
    this->size -= 1;
}

void setInDynamicArray(DynamicArray* this, void* element, size_t i)
{
    assert(i < this->size);
    memcpy(indexVoidPtr(this->data, i, this->nBytesPerElement), element, this->nBytesPerElement);
}

void* indexDynamicArray(DynamicArray* this, size_t i)
{
    assert(i < this->size);
    return indexVoidPtr(this->data, i, this->nBytesPerElement);
}

void freeDynamicArray(DynamicArray* this)
{
    if (this->freeCallback != NULL)
    {
        for (size_t i = 0; i < this->size; i++)
            this->freeCallback(indexDynamicArray(this, i));
    }
    xfree(this->data);
    xfree(this);
}

DynamicArray* copyDynamicArray(DynamicArray* this)
{
    DynamicArray* arr = newDynamicArray(this->allocated, this->nBytesPerElement, this->freeCallback);
    memcpy(arr->data, this->data, this->nBytesPerElement * this->size);
    arr->size = this->size;
    return arr;
}


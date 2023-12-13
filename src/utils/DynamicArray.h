#pragma once
#include <stddef.h>

typedef void (*freeCallbackFunctionType)(void*);

typedef struct
{
    void* data;
    size_t size;
    size_t allocated;
    size_t nBytesPerElement;
    // callback function called by `freeDynamicArray` on every element of the array. Argument type is a pointer to the stored type
    freeCallbackFunctionType freeCallback;
} DynamicArray;

DynamicArray* newDynamicArray(size_t nElements, size_t nBytesPerElement, freeCallbackFunctionType freeCallback);
static void reallocateDynamicArray(DynamicArray* this);
void addToDynamicArray(DynamicArray* this, void* element);
void deleteLastInDynamicArray(DynamicArray* this);
void setInDynamicArray(DynamicArray* this, void* element, size_t i);
void* indexDynamicArray(DynamicArray* this, size_t i);
void freeDynamicArray(DynamicArray* this);


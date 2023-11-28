#pragma once
#include <stddef.h>

typedef struct
{
    void* data;
    size_t size;
    size_t allocated;
    size_t nBytesPerElement;
} DynamicArray;

DynamicArray* newDynamicArray(size_t nElements, size_t nBytesPerElement);
static void reallocateDynamicArray(DynamicArray* this);
void addToDynamicArray(DynamicArray* this, void* element);
void delInDynamicArray(DynamicArray* this);  // "delete" last element
void setInDynamicArray(DynamicArray* this, void* element, size_t i);
void* indexDynamicArray(DynamicArray* this, size_t i);
void freeDynamicArray(DynamicArray* this);

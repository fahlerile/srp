#pragma once
#include <stddef.h>

/*
 *  NOTE: It is absolutely safe to `addToDynamicArray` values allocated on the stack. What this function (and every other function manipulating the array) does is it copies the data pointed by a pointer passed to a function. So the following code is safe:
 *  int a = 6;
 *  addToDynamicArray(arr, &a);
 */

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
void freeDynamicArray(DynamicArray* this);

void addToDynamicArray(DynamicArray* this, void* element);
void deleteLastInDynamicArray(DynamicArray* this);
void setInDynamicArray(DynamicArray* this, void* element, size_t i);
void* indexDynamicArray(DynamicArray* this, size_t i);
DynamicArray* copyDynamicArray(DynamicArray* this);


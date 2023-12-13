#include <assert.h>
#include <string.h>
#include "utils/DynamicArray.h"
#include "utils/memoryUtils.h"

typedef struct __attribute__((packed))
{
    int a;
    double b;
} TestStruct;

void testInt()
{
    DynamicArray* arr = newDynamicArray(3, sizeof(int), NULL);
    assert(arr->size == 0);
    assert(arr->allocated == 3);
    assert(arr->nBytesPerElement == sizeof(int));

    addToDynamicArray(arr, &(int) {4});
    addToDynamicArray(arr, &(int) {6});
    addToDynamicArray(arr, &(int) {2});
    assert(arr->size == 3);
    assert(arr->allocated == 3);

    addToDynamicArray(arr, &(int) {1});
    addToDynamicArray(arr, &(int) {9});
    assert(arr->size == 5);
    assert(arr->allocated == 6);

    int expected[5] = {4, 6, 2, 1, 9};
    assert(memcmp(arr->data, (void*) expected, sizeof(expected)) == 0);

    deleteLastInDynamicArray(arr);
    assert(arr->size == 4);
    assert(*(int*) indexDynamicArray(arr, 0) == 4);
    assert(*(int*) indexDynamicArray(arr, 1) == 6);
    assert(*(int*) indexDynamicArray(arr, 3) == 1);

    setInDynamicArray(arr, &(int) {727}, 1);
    assert(*(int*) indexDynamicArray(arr, 1) == 727);

    freeDynamicArray(arr);
}

void testStruct()
{
    DynamicArray* arr = newDynamicArray(3, sizeof(TestStruct), NULL);
    assert(arr->size == 0);
    assert(arr->allocated == 3);
    assert(arr->nBytesPerElement == sizeof(TestStruct));

    addToDynamicArray(arr, &(TestStruct) {1, 4.3});
    addToDynamicArray(arr, &(TestStruct) {4, 6.2});
    addToDynamicArray(arr, &(TestStruct) {9, 2.3});
    assert(arr->size == 3);
    assert(arr->allocated == 3);

    addToDynamicArray(arr, &(TestStruct) {5, 1.7});
    addToDynamicArray(arr, &(TestStruct) {16, 9.1});
    assert(arr->size == 5);
    assert(arr->allocated == 6);

    TestStruct expected[5] = {
        {1, 4.3}, {4, 6.2}, {9, 2.3}, {5, 1.7}, {16, 9.1}
    };
    assert(memcmp(arr->data, (void*) expected, sizeof(expected)) == 0);

    deleteLastInDynamicArray(arr);
    assert(arr->size == 4);
    TestStruct a = *(TestStruct*) indexDynamicArray(arr, 0);
    assert(a.a == 1);
    assert(a.b == 4.3);
    a = *(TestStruct*) indexDynamicArray(arr, 1);
    assert(a.a == 4);
    assert(a.b == 6.2);
    a = *(TestStruct*) indexDynamicArray(arr, 3);
    assert(a.a == 5);
    assert(a.b == 1.7);

    setInDynamicArray(arr, &(TestStruct) {727, 727.2}, 1);
    a = *(TestStruct*) indexDynamicArray(arr, 1);
    assert(a.a == 727);
    assert(a.b == 727.2);

    freeDynamicArray(arr);
}

void freeCallback(void* p_p_int)
{
    xfree(*(int**) p_p_int);
}

void testFreeCallback()
{
    DynamicArray* arr = newDynamicArray(3, sizeof(int*), freeCallback);

    for (size_t i = 0; i < 10; i++)
    {
        int* elem = xmalloc(sizeof(int));
        *elem = 5;
        addToDynamicArray(arr, &elem);
    }

    freeDynamicArray(arr);
}

int main()
{
    testInt();
    testStruct();
    testFreeCallback();
}

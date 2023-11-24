#include <assert.h>
#include <string.h>
#include "utils/DynamicArray.h"

typedef struct __attribute__((packed))
{
    int a;
    double b;
} TestStruct;

int test_int()
{
    DynamicArray* arr = newDynamicArray(3, sizeof(int));
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

    delInDynamicArray(arr);
    assert(arr->size == 4);
    assert(*(int*) indexDynamicArray(arr, 0) == 4);
    assert(*(int*) indexDynamicArray(arr, 1) == 6);
    assert(*(int*) indexDynamicArray(arr, 3) == 1);

    setInDynamicArray(arr, &(int) {727}, 1);
    assert(*(int*) indexDynamicArray(arr, 1) == 727);

    freeDynamicArray(arr);
}

int test_double()
{
    DynamicArray* arr = newDynamicArray(3, sizeof(double));
    assert(arr->size == 0);
    assert(arr->allocated == 3);
    assert(arr->nBytesPerElement == sizeof(double));

    addToDynamicArray(arr, &(double) {4.3});
    addToDynamicArray(arr, &(double) {6.2});
    addToDynamicArray(arr, &(double) {2.3});
    assert(arr->size == 3);
    assert(arr->allocated == 3);

    addToDynamicArray(arr, &(double) {1.7});
    addToDynamicArray(arr, &(double) {9.1});
    assert(arr->size == 5);
    assert(arr->allocated == 6);

    double expected[5] = {4.3, 6.2, 2.3, 1.7, 9.1};
    assert(memcmp(arr->data, (void*) expected, sizeof(expected)) == 0);

    delInDynamicArray(arr);
    assert(arr->size == 4);
    assert(*(double*) indexDynamicArray(arr, 0) == 4.3);
    assert(*(double*) indexDynamicArray(arr, 1) == 6.2);
    assert(*(double*) indexDynamicArray(arr, 3) == 1.7);

    setInDynamicArray(arr, &(double) {727.2}, 1);
    assert(*(double*) indexDynamicArray(arr, 1) == 727.2);

    freeDynamicArray(arr);
}

int test_struct()
{
    DynamicArray* arr = newDynamicArray(3, sizeof(TestStruct));
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

    delInDynamicArray(arr);
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

int main()
{
    test_int();
    test_double();
    test_struct();
}

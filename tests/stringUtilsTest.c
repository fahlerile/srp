#include <string.h>
#include <assert.h>
#include "utils/stringUtils.h"

void test()
{
    const char* str = "a string I want to tokenize lololol";
    DynamicArray* arr = splitString(str, " ");

    assert(strcmp(*(char**) indexDynamicArray(arr, 0), "a") == 0);
    assert(strcmp(*(char**) indexDynamicArray(arr, 1), "string") == 0);
    assert(strcmp(*(char**) indexDynamicArray(arr, 2), "I") == 0);
    assert(strcmp(*(char**) indexDynamicArray(arr, 3), "want") == 0);
    assert(strcmp(*(char**) indexDynamicArray(arr, 4), "to") == 0);
    assert(strcmp(*(char**) indexDynamicArray(arr, 5), "tokenize") == 0);
    assert(strcmp(*(char**) indexDynamicArray(arr, 6), "lololol") == 0);
}

int main()
{
    test();
}


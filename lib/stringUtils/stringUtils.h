#pragma once
#include "DynamicArray/DynamicArray.h"

/**
 * @brief Split a `char*` by multiple single-char delimiters
 * @param str A string (`char*`) to be tokenized,ff
 * @param delim A string containing all the single-char delimiters
 * @return A `DynamicArray` of `char*` which should be freed with `freeDynamicArray`
 */
DynamicArray* splitString(const char* str, char* delim);


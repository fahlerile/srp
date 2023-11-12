#pragma once
#include <stdint.h>

// Wrapper functions for standard memory allocation functions,
// but with error handling (abort if NULL)

void* allocate(size_t n);                                // malloc
void* reallocate(void* ptr, size_t new_n);               // realloc
void* allocate_and_zero(size_t n_elem, size_t n_bytes);  // calloc

#include <stdio.h>
#include <stdlib.h>

void* allocate(size_t n)
{
    void *p = malloc(n);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to malloc %zu bytes, aborting\n", n);
        abort();
    }
    return p;
}

void* reallocate(void* ptr, size_t new_n)
{
    void *p = realloc(ptr, new_n);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to realloc for %zu bytes for %p, aborting\n", new_n, ptr);
        abort();
    }
    return p;
}

void* allocate_and_zero(size_t n_elem, size_t n_bytes)
{
    void *p = calloc(n_elem, n_bytes);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to calloc %zu bytes for %zu elements (total %zu bytes), aborting\n",
                n_bytes, n_elem, n_bytes * n_elem);
        abort();
    }
    return p;
}

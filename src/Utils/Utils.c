#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"

void* allocate(size_t n)
{
    void *p = malloc(n);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to allocate %zu bytes, aborting\n", n);
        abort();
    }
    return p;
}

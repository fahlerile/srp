#include "utils/utils.h"

void* xmalloc(size_t n)
{
    void *p = malloc(n);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to malloc %zu bytes, aborting\n", n);
        abort();
    }
    LOGM("malloc(%zu) = %p\n", n, p);
    return p;
}

void* xrealloc(void* ptr, size_t new_n)
{
    void *p = realloc(ptr, new_n);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to realloc for %zu bytes for %p, aborting\n", new_n, ptr);
        abort();
    }
    LOGM("realloc(%p, %zu) = %p\n", ptr, new_n, p);
    return p;
}

void* xcalloc(size_t n_elem, size_t n_bytes)
{
    void *p = calloc(n_elem, n_bytes);
    if (p == NULL)
    {
        fprintf(stderr, "Failed to calloc %zu bytes for %zu elements (total %zu bytes), aborting\n",
                n_bytes, n_elem, n_bytes * n_elem);
        abort();
    }
    LOGM("calloc(%zu, %zu) = %p\n", n_elem, n_bytes, p);
    return p;
}

void xfree(void* ptr)
{
    LOGM("free(%p)\n", ptr);
    free(ptr);
}

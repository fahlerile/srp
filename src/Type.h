#pragma once
#include "log.h"
#include <stddef.h>

// Types that can be stored in vertex attributes
typedef enum
{
    TYPE_DOUBLE = 0,
    TYPE_SIZE_T
} Type;

static inline size_t SIZEOF_TYPE(Type a)
{
    switch (a)
    {
        case TYPE_DOUBLE:
            return sizeof(double);
        case TYPE_SIZE_T:
            return sizeof(size_t);
        default:
            LOGE("Unknown type (%i) in %s", a, __func__);
            return 0;
    }
}


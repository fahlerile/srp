#pragma once
#include "log.h"
#include <stddef.h>
#include <stdint.h>

// Types that can be stored in vertex attributes
typedef enum
{
    TYPE_UINT8 = 0,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_FLOAT,
    TYPE_DOUBLE
} Type;

static inline size_t SIZEOF_TYPE(Type a)
{
    switch (a)
    {
        case TYPE_UINT8:
            return sizeof(uint8_t);
        case TYPE_UINT16:
            return sizeof(uint16_t);
        case TYPE_UINT32:
            return sizeof(uint32_t);
        case TYPE_UINT64:
            return sizeof(uint64_t);
        case TYPE_FLOAT:
            return sizeof(float);
        case TYPE_DOUBLE:
            return sizeof(double);
        default:
            LOGE("Unknown type (%i) in %s", a, __func__);
            return 0;
    }
}


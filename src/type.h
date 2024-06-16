#pragma once

#include <stdint.h>
#include <stddef.h>

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

size_t SIZEOF_TYPE(Type type);


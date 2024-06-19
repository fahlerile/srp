// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  Functions and types to help dealing with opaque types */

#include <stdint.h>
#include <stddef.h>

/** An enumeration to hold types. This is needed for internal handling of
 *  opaque types */
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


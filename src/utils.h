// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  Various utility macros */

#ifdef SRP_SOURCE

#include <stddef.h>

#define INDEX_VOID_PTR(ptr, idx, nBytesPerElement) \
	( ( (uint8_t*) (ptr) ) + (idx) * (nBytesPerElement) )
#define ADD_VOID_PTR(ptr, nBytes) \
	( ( (uint8_t*) (ptr) ) + (nBytes) )

#endif  // ifdef SRP_SOURCE


// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  Private header for `include/buffer.h` */

#include "buffer.h"

/** @ingroup Buffer_internal
 *  @{ */

struct SRPVertexBuffer
{
	size_t nBytesPerVertex;  /**< How many bytes does one vertex occupy */
	size_t nVertices;        /**< How many vertices does this vertex buffer contain */
	size_t nBytesAllocated;  /**< How many bytes was already allocated for `data` */
	SRPVertex* data;         /**< Pointer to the vertex data */
};

struct SRPIndexBuffer
{
	SRPType indicesType;     /**< Type of stored indices @see srpIndexBufferCopyData */
	size_t nBytesPerIndex;   /**< How many bytes does one index occupy */
	size_t nIndices;         /**< How many indices does this index buffer contain */
	size_t nBytesAllocated;  /**< How many bytes was already allocated for `data` */
	void* data;              /**< Pointer to the index data */
};

/** @} */  // ingroup Buffer_internal


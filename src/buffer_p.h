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
	size_t nBytesPerVertex;
	size_t nBytesData;
	size_t nVertices;
	SRPVertex* data;
};

struct SRPIndexBuffer
{
	SRPType indicesType;
	size_t nBytesPerIndex;
	size_t nIndices;
	void* data;
};

/** @} */  // infroup Buffer_internal


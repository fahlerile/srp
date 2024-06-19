// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  Typedefs related to vertex handling */

#include <stddef.h>
#include "type.h"

/** Represents the vertex as stored in `SRPVertexBuffer`
 *  @see `SRPVertexBuffer` */
typedef struct SRPVertex SRPVertex;

/** Represents a vertex variable as outputted by vertex shader
 *  @see `SRPvsOutput` */
typedef struct SRPVertexVariable SRPVertexVariable;

/** Holds nformation needed to interpolate vertex variables in-primitive
 *  #see `SRPVertexVariable` */
typedef struct
{
	size_t nItems;
	Type type;
} SRPVertexVariableInformation;


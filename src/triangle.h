// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  Triangle rasterization */

#ifdef SRP_SOURCE

#include "framebuffer.h"
#include "shaders.h"
#include "vec.h"

/** Draw the triangle that is specified by three vertices to the framebuffer
 *  @param[in] `fb` The framebuffer to draw to
 *  @param[in] `vertices` Pointer to an array of 3 `SRPvsOutput`s
 *  @param[in] `sp` The shader program to use
 *  @param[in] `primitiveID` Primitive ID of this triangle */
void drawTriangle(
	const SRPFramebuffer* fb, const SRPvsOutput vertices[3],
	const SRPShaderProgram* restrict sp, size_t primitiveID
);

#endif  // ifdef SRP_SOURCE


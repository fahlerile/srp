// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  \ref SRPPrimitive */

/** An enumeration to hold drawing primitives
 *  @see `srpDrawVertexBuffer` and `srpDrawIndexBuffer` */
typedef enum SRPPrimitive
{
	PRIMITIVE_POINTS,
	PRIMITIVE_LINES,
	PRIMITIVE_LINE_STRIP,
	PRIMITIVE_LINE_LOOP,
	PRIMITIVE_TRIANGLES,
	PRIMITIVE_TRIANGLE_STRIP,
	PRIMITIVE_TRIANGLE_FAN
} SRPPrimitive;


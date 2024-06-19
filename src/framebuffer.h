// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

/** @file
 *  SRPFramebuffer and related functions */

#pragma once

#include <stddef.h>
#include <stdint.h>

/** @ingroup Framebuffer
 *  @{ */

/** Holds RBGA8888 color buffer and depth buffer */
typedef struct SRPFramebuffer
{
	size_t width;
	size_t height;
	size_t size;
	uint32_t* color;
	double* depth;
} SRPFramebuffer;

/** Create a framebuffer
 *  @param[in] width Width of a new framebuffer, in pixels
 *  @param[in] height Height of a new framebuffer, in pixels
 *  @return A pointer to the created framebuffer */
SRPFramebuffer* srpNewFramebuffer(size_t width, size_t height);

/** Free a framebuffer
 *  @param[in] this The pointer to SRPFramebuffer, as returned
 *                    from srpNewFramebuffer() */
void srpFreeFramebuffer(SRPFramebuffer* this);

/** Clear a framebuffer: fill the color with black and depth with -1
 *  @param[in] this The pointer to SRPFramebuffer, as returned
 *                    from srpNewFramebuffer() */
void framebufferClear(const SRPFramebuffer* this);

#ifdef SRP_SOURCE

/** Draw a pixel in a framebuffer
 *  @param[in] this The pointer to SRPFramebuffer, as returned 
 *                    from srpNewFramebuffer
 *  @param[in] x,y Position at which to draw the pixel. X-axis points
 *                 from left to right, Y-axis - from top to bottom
 *  @param[in] depth Depth value to assign to the pixel, assumed to be inside
 *                   the [-1, 1] interval. If it is less than currently assigned
 *                   depth buffer value to this pixel, nothing is drawn.
 *  @param[in] color RBGA8888 color to draw. */
void framebufferDrawPixel(
	const SRPFramebuffer* this, size_t x, size_t y, double depth,
	uint32_t color
);

/** Convert Normalized Device Coordinates to screen-space coordiantes
 *  @param[in] this The pointer to SRPFramebuffer, as returned
 *                    from srpNewFramebuffer
 *  @param[in] NDC Pointer to 3-element double array containing NDC position
 *  @param[out] SS Pointer to 3-element double array that will contain SS
 *                   position after the call. The z-component is the same as
 *                   z-component of NDC coordinates. */
void framebufferNDCToScreenSpace
	(const SRPFramebuffer* this, const double* NDC, double* SS);

#endif  // ifdef SRP_SOURCE

/** @} */  // defgroup Framebuffer


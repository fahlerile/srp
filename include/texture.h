// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  SRPTexture and related functions */

#include <stdint.h>
#include <stddef.h>
#include "color.h"

/** @ingroup Texture
 *  @{ */

/** An enumeration to hold texture wrapping modes
 *  @see SRPTexture */
typedef enum
{
	TW_REPEAT,
	TW_CLAMP_TO_EDGE
} SRPTextureWrappingMode;

/** An enumeration to hold texture filtering modes
 *  @see SRPTexture */
typedef enum
{
	TF_NEAREST
} SRPTextureFilteringMode;

/** A structure to represent a texture */
typedef struct SRPTexture SRPTexture;

/** Initialize a texture by loading an image
 *  @param[in] image A filesystem path to an image. Most popular image types
 *					 are supported
 *	@param[in] wrappingModeX,wrappingModeY Wrapping modes to use in X and Y axes
 *	@param[in] filteringModeMagnifying,filteringModeMinifying
 *	           Texture filtering mode to use when magnifying or minifying the texture
 *	@return A pointer to the constructed texture */
SRPTexture* srpNewTexture(
	const char* image,
	SRPTextureWrappingMode wrappingModeX, SRPTextureWrappingMode wrappingModeY,
	SRPTextureFilteringMode filteringModeMagnifying,
	SRPTextureFilteringMode filteringModeMinifying
);
/** Free a texture
 *  @param[in] this A pointer to the texture, as returned by srpNewTexture() */
void srpFreeTexture(SRPTexture* this);

/** Get a texel color at u, v by filtering the texture using specified modes
 *  @param[in] this A pointer to the texture, as returned by srpNewTexture()
 *  @param[in] u,v Coordinates of the requested texel
 *  @param[out] out An array of 4 values each in [0, 1] interval, representing
 *                  RGBA8888 color (0.0 = 0x0; 1.0 = 0xFF) */
void srpTextureGetFilteredColor(
	const SRPTexture* this, double u, double v, double out[4]
);

/** @} */  // defgroup Texture


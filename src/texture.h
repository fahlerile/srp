#pragma once

#include <stdint.h>
#include <stddef.h>
#include "color.h"

typedef enum
{
	TW_REPEAT,
	TW_CLAMP_TO_EDGE
} SRPTextureWrappingMode;

typedef enum
{
	TF_NEAREST,
	// TF_LINEAR  // TODO
} SRPTextureFilteringMode;

typedef struct
{
	uint8_t* data;
	int width, height;
	SRPTextureWrappingMode wrappingModeX;
	SRPTextureWrappingMode wrappingModeY;
	SRPTextureFilteringMode filteringModeMagnifying;
	SRPTextureFilteringMode filteringModeMinifying;
} SRPTexture;

SRPTexture* srpNewTexture(
	const char* image,
	SRPTextureWrappingMode wrappingModeX, SRPTextureWrappingMode wrappingModeY,
	SRPTextureFilteringMode filteringModeMagnifying,
	SRPTextureFilteringMode filteringModeMinifying
);
void srpFreeTexture(SRPTexture* this);

void srpTextureGetFilteredColor(
	const SRPTexture* this, double u, double v, double out[4]
);


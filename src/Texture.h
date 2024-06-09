#pragma once
#include "Color/Color.h"
#include <stdint.h>
#include <stddef.h>

typedef enum
{
	TW_REPEAT,
	TW_CLAMP_TO_EDGE
} TextureWrappingMode;

typedef enum
{
	TF_NEAREST,
	// TF_LINEAR  // TODO
} TextureFilteringMode;

typedef struct
{
	uint8_t* data;
	int width, height;
	TextureWrappingMode wrappingModeX;
	TextureWrappingMode wrappingModeY;
	TextureFilteringMode filteringModeMagnifying;
	TextureFilteringMode filteringModeMinifying;
} Texture;

Texture* newTexture(
	const char* image,
	TextureWrappingMode wrappingModeX, TextureWrappingMode wrappingModeY,
	TextureFilteringMode filteringModeMagnifying,
	TextureFilteringMode filteringModeMinifying
);
void freeTexture(Texture* this);

Color textureGetFilteredColor(Texture* this, double u, double v);
static Color textureGetColor(Texture* this, size_t x, size_t y);


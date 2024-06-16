#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "math_utils.h"
#include "stb_image.h"
#include "utils.h"
#include "Texture.h"

#define N_CHANNELS_REQUESTED 3

Texture* newTexture(
	const char* image,
	TextureWrappingMode wrappingModeX, TextureWrappingMode wrappingModeY,
	TextureFilteringMode filteringModeMagnifying,
	TextureFilteringMode filteringModeMinifying
)
{
	Texture* this = malloc(sizeof(Texture));
	this->data = stbi_load(image, &this->width, &this->height, NULL, N_CHANNELS_REQUESTED);
	if (this->data == NULL)
	{
		fprintf(
			stderr, "%s: failed to load image \"%s\"; %s\n",
			__func__, image, stbi_failure_reason()
		);
		return NULL;
	}
	this->wrappingModeX = wrappingModeX;
	this->wrappingModeY = wrappingModeY;
	this->filteringModeMagnifying = filteringModeMagnifying;
	this->filteringModeMinifying = filteringModeMinifying;
	return this;
}

void freeTexture(Texture* this)
{
	stbi_image_free(this->data);
	free(this);
}

// TODO: now using only filteringModeMagnifying
// how to know if texture is magnified or minified?
// TODO: too many conditionals?
Color textureGetFilteredColor(Texture* this, double u, double v)
{
	if (u < 0 || u > 1)
	{
		switch (this->wrappingModeX)
		{
		case TW_REPEAT:
			u = FRACTIONAL(u);
			break;
		case TW_CLAMP_TO_EDGE:
			u = CLAMP(0., 1., u);
			break;
		}
	}
	if (v < 0 || v > 1)
	{
		switch (this->wrappingModeY)
		{
		case TW_REPEAT:
			v = FRACTIONAL(v);
			break;
		case TW_CLAMP_TO_EDGE:
			v = CLAMP(0., 1., v);
			break;
		}
	}

	double x = (this->width-1) * u;
	double y = (this->height-1) * (1-v);
	size_t x_min, y_min, x_max, y_max;

	switch (this->filteringModeMagnifying)
	{
	case TF_NEAREST:
		return textureGetColor(this, round(x), round(y));
	}
}

static Color textureGetColor(Texture* this, size_t x, size_t y)
{
	uint8_t* start = INDEX_VOID_PTR(this->data, x + y * this->width, N_CHANNELS_REQUESTED);
	return (Color) {
		start[0],
		start[1],
		start[2],
		(N_CHANNELS_REQUESTED == 3) ? 255 : start[3]
	};
}


#pragma once
#include "Utils/Utils.h"

typedef struct
{
    Color* data;
    Vector2i dimensions;
} Texture;

Texture* newTexture(const char* filename);
void freeTexture(Texture* this);

size_t texturePixelToIndex(Texture* this, Vector2i point);
Vector2i textureUVToPixel(Texture* this, Vector2d uvPoint);
size_t textureUVToIndex(Texture* this, Vector2d uvPoint);

Color textureGetColorAtPixel(Texture* this, Vector2i point);
Color textureGetColorAtUV(Texture* this, Vector2d uvPoint);

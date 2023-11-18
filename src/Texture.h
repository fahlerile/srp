#pragma once
#include "Utils/Utils.h"

typedef struct
{
    Color* data;
    Vector2i dimensions;
} Texture;

Texture* newTexture(const char* filename);
void freeTexture(Texture* this);

size_t texturePointToIndex(Texture* this, Vector2i point);
Vector2i textureUVToPoint(Texture* this, Vector2d uvPoint);
size_t textureUVToIndex(Texture* this, Vector2d uvPoint);

Color textureGetColorAtPoint(Texture* this, Vector2i point);
Color textureGetColorAtUV(Texture* this, Vector2d uvPoint);

#include "Texture.h"
#include "Utils/Utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture* newTexture(const char* filename)
{
    Texture* this = allocate(sizeof(Texture));
    int w, h, n;
    this->data = (Color*) stbi_load(filename, &w, &h, &n, 0);
    if (this->data == NULL)
    {
        fprintf(stderr, "Failed to open %s, aborting...\n", filename);
        abort();
    }
    assert(n == 4);
    this->dimensions = (Vector2i) {w, h};
    return this;
}

void freeTexture(Texture* this)
{
    stbi_image_free(this->data);
    free(this);
}

size_t texturePointToIndex(Texture* this, Vector2i point)
{
    return point.y * this->dimensions.x + point.x;
}

Vector2i textureUVToPoint(Texture* this, Vector2d uvPoint)
{
    return (Vector2i) {
        this->dimensions.x * uvPoint.x,
        this->dimensions.y * (1 - uvPoint.y),
    };
}

size_t textureUVToIndex(Texture* this, Vector2d uvPoint)
{
    return texturePointToIndex(this, textureUVToPoint(this, uvPoint));
}

Color textureGetColorAtPoint(Texture* this, Vector2i point)
{
    return this->data[texturePointToIndex(this, point)];
}

Color textureGetColorAtUV(Texture* this, Vector2d uvPoint)
{
    return this->data[textureUVToIndex(this, uvPoint)];
}
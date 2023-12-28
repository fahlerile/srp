#include "Texture.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture* newTexture(const char* filename)
{
    Texture* this = xmalloc(sizeof(Texture));
    int w, h, n;
    this->data = (Color*) stbi_load(filename, &w, &h, &n, 4);
    if (this->data == NULL)
    {
        fprintf(stderr, "Failed to open %s, aborting...\n", filename);
        abort();
    }
    this->dimensions = (Vector2i) {w, h};
    return this;
}

void freeTexture(Texture* this)
{
    stbi_image_free(this->data);
    xfree(this);
}

size_t texturePixelToIndex(Texture* this, Vector2i point)
{
    return point.y * this->dimensions.x + point.x;
}

Vector2i textureUVToPixel(Texture* this, Vector2d uvPoint)
{
    return (Vector2i) {
        this->dimensions.x * uvPoint.x,
        this->dimensions.y * (1 - uvPoint.y),
    };
}

size_t textureUVToIndex(Texture* this, Vector2d uvPoint)
{
    return texturePixelToIndex(this, textureUVToPixel(this, uvPoint));
}

Color textureGetColorAtPixel(Texture* this, Vector2i point)
{
    return this->data[texturePixelToIndex(this, point)];
}

Color textureGetColorAtUV(Texture* this, Vector2d uvPoint)
{
    return this->data[textureUVToIndex(this, uvPoint)];
}

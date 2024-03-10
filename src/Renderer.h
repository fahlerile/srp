#pragma once
#include "SDL2/SDL.h"
#include "Vector/Vector.h"

// @brief A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Surface* pixels;
    double* zBuffer;
    size_t zBufferSize;
    Vector2i dimensions;
} Renderer;

Renderer* newRenderer(const char* window_title, int width, int height, uint32_t flags);
void freeRenderer(Renderer* this);

void rendererSetDrawColor(Renderer* this, Color color);
void rendererDrawPixel(Renderer* this, Vector2i point, double depth, Color color);

int rendererSaveBuffer(Renderer* this, const char* filename);
void rendererSwapBuffer(Renderer* this);
void rendererClearBuffer(Renderer* this, Color color);

static inline size_t rendererPointToZBufferIndex(Renderer* this, Vector2i point)
{
    assert(0 <= point.x && point.x < this->dimensions.x);
    assert(0 <= point.y && point.y < this->dimensions.y);
    return point.y * this->dimensions.x + point.x;
}


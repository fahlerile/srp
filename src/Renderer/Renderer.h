#pragma once
#include "SDL2/SDL.h"
#include "Utils/Utils.h"

// A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* internal_window;
    SDL_Renderer* internal_renderer;
} Renderer;

// May return NULL on error
// Returned pointer should be `free`d
Renderer* newRenderer(int width, int height, int flags);

void setColor(Renderer* this, Color color);

void drawPixel(Renderer* this, int x, int y, Color color);

// Takes an array of vertices in NDC (point is represented as `Vector3d`)
// with amount of vertices
void drawPolygon(Array* vertices);

void clearBuffer(Renderer* this, Color color);
void swapBuffer(Renderer* this);
void updateBuffer(Renderer* this);

#pragma once
#include "SDL2/SDL.h"
#include "Utils/Utils.h"

// A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* internal_window;
    SDL_Renderer* internal_renderer;
} Renderer;

#include "Triangle.h"  // circular dependency

// May return NULL on error
Renderer* newRenderer(int width, int height, int flags);
void freeRenderer(Renderer* this);

Vector2i rendererGetWindowDimensions(Renderer* this);

void rendererSetColor(Renderer* this, Color color);
void rendererDrawPixel(Renderer* this, Vector2i point, Color color);

void rendererDrawTriangle(Renderer* this, Triangle* tri);

// A function to put draw calls in
void rendererDrawToBuffer(Renderer* this);
// Save buffer as a BMP screenshot
void rendererSaveBuffer(Renderer* this, const char* filename);
void rendererSwapBuffer(Renderer* this);
void rendererClearBuffer(Renderer* this, Color color);

#pragma once
#include "SDL2/SDL.h"
#include "Utils/Utils.h"

// A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* internal_window;
    SDL_Renderer* internal_renderer;
} Renderer;

#include "Polygon.h"  // circular dependency

// May return NULL on error
Renderer* newRenderer(int width, int height, int flags);
void freeRenderer(Renderer* this);

Vector2i getWindowDimensions(Renderer* this);

void setColor(Renderer* this, Color color);

void drawPixel(Renderer* this, Vector2i point, Color color);
void drawPolygon(Renderer* this, Polygon* polygon);

void drawToBuffer(Renderer* this);
// Save buffer as a BMP screenshot
void saveBuffer(Renderer* this, const char* filename);
void swapBuffer(Renderer* this);
void clearBuffer(Renderer* this, Color color);

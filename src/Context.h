#pragma once

#include <stdbool.h>
#include "Renderer.h"
#include "SDL2/SDL.h"

typedef enum
{
    drawingModeFill = 0,
    drawingModeLine
} drawingMode;

typedef struct
{
    bool running;
    Renderer* renderer;
    SDL_Event event;
    drawingMode drawingMode;
} Context;

extern Context context;

void constructContext(Context* this);
void destroyContext();

void pollEvents();


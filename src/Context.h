#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Renderer.h"

#ifndef NDEBUG
typedef struct
{
    bool colorRasterizerTiles;
} DebugOptions;
#endif

typedef struct
{
    bool running;
    Renderer* renderer;
    SDL_Event event;
#ifndef NDEBUG
    DebugOptions debug;
#endif
} Context;

extern Context context;

void constructContext(Context* this);
void destroyContext();

void pollEvents();


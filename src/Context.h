#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Renderer.h"

typedef struct
{
    bool running;
    Renderer* renderer;
    SDL_Event event;
} Context;

extern Context context;

void constructContext(Context* this);
void destroyContext();

void pollEvents();


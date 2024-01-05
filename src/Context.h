#pragma once

#include <stdbool.h>
#include "Face.h"
#include "Renderer.h"
#include "SDL2/SDL.h"

typedef struct
{
    bool running;
    Renderer* renderer;
    SDL_Event event;
    faceModeType faceMode;
} Context;

extern Context context;

void constructContext(Context* this);
void destroyContext();

void pollEvents();


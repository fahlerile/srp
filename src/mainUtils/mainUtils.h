#pragma once
#include <stdbool.h>
#include "SDL2/SDL.h"

Renderer* initialize(int width, int height, int flags);
void pollEvents(bool* running, SDL_Event* event);
void deinitialize(Renderer* renderer);

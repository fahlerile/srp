#pragma once
#include <stdbool.h>
#include "SDL2/SDL.h"

Renderer* Initialize(int width, int height, int flags);
void PollEvents(bool* running, SDL_Event* event);
void Deinitialize(Renderer* renderer);

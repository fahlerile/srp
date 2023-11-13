#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "Renderer/Renderer.h"
#include "MainUtils/MainUtils.h"

int main(int argc, char** argv)
{
    Renderer* renderer = initialize(800, 600, 0);
    SDL_Event event;
    bool running = true;

    while (running)
    {
        PollEvents(&running, &event);
        clearBuffer(renderer, (Color) {0, 0, 0, 255});
        updateBuffer(renderer);
        swapBuffer(renderer);
    }

    deinitialize(renderer);
}

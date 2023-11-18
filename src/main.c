#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "Renderer/Renderer.h"
#include "MainUtils/MainUtils.h"

int main(int argc, char** argv)
{
    Renderer* renderer = initialize(256, 256, 0);
    SDL_Event event;
    bool running = true;

    // Draw once & save - then loop forever
    rendererClearBuffer(renderer, (Color) {0, 0, 0, 255});
    rendererDrawToBuffer(renderer);
    rendererSaveBuffer(renderer, "screenshot.bmp");
    rendererSwapBuffer(renderer);
    while (running)
    {
        pollEvents(&running, &event);
        rendererSwapBuffer(renderer);
    }

    deinitialize(renderer);
}

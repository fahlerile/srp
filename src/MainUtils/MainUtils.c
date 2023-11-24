#include "SDL2/SDL.h"
#include "Renderer.h"
#include "mainUtils.h"
#include "utils/utils.h"
#include "errorcodes.h"

Renderer* initialize(int width, int height, int flags)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL library: %s\n", SDL_GetError());
        exit(SDL_INIT_FAIL);
    }

    Renderer* renderer = newRenderer(width, height, flags);

    if (renderer == NULL)
    {
        fprintf(stderr, "Failed to initialize SDL_Window or SDL_Renderer: %s\n", SDL_GetError());
        exit(SDL_WINDOW_OR_RENDERER_INIT_FAIL);
    }

    return renderer;
}

void pollEvents(bool* running, SDL_Event* p_event)
{
    while (SDL_PollEvent(p_event) != 0)
    {
        if (p_event->type == SDL_QUIT)
            *running = false;
    }
}

void deinitialize(Renderer* renderer)
{
    freeRenderer(renderer);
}

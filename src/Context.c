#include <stdlib.h>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Context.h"
#include "utils.h"
#include "errorcodes.h"
#include "constants.h"

void constructContext(Context* this)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL library: %s\n", SDL_GetError());
        exit(SDL_INIT_FAIL);
    }

    this->renderer = newRenderer("Rasterizer", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_RENDERER_SDL_FLAGS);
    if (this->renderer == NULL)
    {
        fprintf(stderr, "Failed to initialize SDL_Window or SDL_Renderer: %s\n", SDL_GetError());
        exit(SDL_WINDOW_OR_RENDERER_INIT_FAIL);
    }

    atexit(destroyContext);
    this->running = true;

#ifndef NDEBUG
    this->debug.colorRasterizerTiles = false;
#endif
}

void pollEvents()
{
    SDL_Event* const e = &context.event;
    while (SDL_PollEvent(e) != 0)
    {
        if (
            e->type == SDL_QUIT || \
            (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
        )
        {
            context.running = false;
        }
        if (e->type == SDL_KEYDOWN)
        {
            switch (e->key.keysym.sym)
            {
#ifndef NDEBUG
                case SDLK_SPACE:
                    context.debug.colorRasterizerTiles = \
                        !context.debug.colorRasterizerTiles;
#endif
            }
        }
    }
}

void destroyContext()
{
    freeRenderer(context.renderer);
}


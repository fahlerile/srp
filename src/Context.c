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

    this->renderer = newRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_RENDERER_SDL_FLAGS);

    if (this->renderer == NULL)
    {
        fprintf(stderr, "Failed to initialize SDL_Window or SDL_Renderer: %s\n", SDL_GetError());
        exit(SDL_WINDOW_OR_RENDERER_INIT_FAIL);
    }

    atexit(destroyContext);

    this->running = true;
    this->drawingMode = DRAWING_MODE;
}

void pollEvents()
{
    while (SDL_PollEvent(&context.event) != 0)
    {
        if (context.event.type == SDL_QUIT ||
            (context.event.type == SDL_KEYDOWN &&
             context.event.key.keysym.sym == SDLK_ESCAPE))
        {
            context.running = false;
        }
    }
}

void destroyContext()
{
    freeRenderer(context.renderer);
}


#include "Renderer.h"
#include "Utils/Utils.h"

Renderer* NewRenderer(int width, int height, int flags)
{
    SDL_Window* int_window;
    SDL_Renderer* int_renderer;
    if (SDL_CreateWindowAndRenderer(width, height, flags, &int_window, &int_renderer) != 0)
        return NULL;

    Renderer* renderer = allocate(sizeof(Renderer));
    *renderer = (Renderer) {
        .internal_window = int_window,
        .internal_renderer = int_renderer,
    };
    return renderer;
}

void SetColor(Renderer* this, Color color)
{
    SDL_SetRenderDrawColor(this->internal_renderer, color.r, color.g, color.b, color.a);
}

void PutPixel(Renderer* this, int x, int y, Color color)
{
    SetColor(this, color);
    SDL_RenderDrawPoint(this->internal_renderer, x, y);
}

void ClearBuffer(Renderer* this, Color color)
{
    SetColor(this, color);
    SDL_RenderClear(this->internal_renderer);
}

void Update(Renderer* this)
{
    Color color = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 255
    };

    for (int x = 100; x < 500; x++)
    {
        for (int y = 200; y < 400; y++)
            PutPixel(this, x, y, color);
    }
}

void SwapBuffer(Renderer* this)
{
    SDL_RenderPresent(this->internal_renderer);
}

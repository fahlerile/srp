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
    SDL_SetRenderDrawColor(this->internal_renderer,
                           color.r, color.g, color.b, color.a);
}

void DrawPixel(Renderer* this, int x, int y, Color color)
{
    SetColor(this, color);
    SDL_RenderDrawPoint(this->internal_renderer, x, y);
}

// vertices - array of `Vector3d`
void DrawPolygon(Array* vertices)
{
    // loop over the bounding box
    // determine if the current point is inside the polygon using the cross product test
}

void ClearBuffer(Renderer* this, Color color)
{
    SetColor(this, color);
    SDL_RenderClear(this->internal_renderer);
}

void Update(Renderer* this)
{
    Vector3d data[3] = {
        (Vector3d) { 0.,   0.5, 0.},
        (Vector3d) {-0.5, -0.5, 0.},
        (Vector3d) { 0.5, -0.5, 0.}
    };

    Array vertices = {
        .data = data,
        .n = 3
    };

    DrawPolygon(&vertices);
}

void SwapBuffer(Renderer* this)
{
    SDL_RenderPresent(this->internal_renderer);
}

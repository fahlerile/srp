#include "Renderer.h"
#include "Utils/Utils.h"

Renderer* newRenderer(int width, int height, int flags)
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

void freeRenderer(Renderer* this)
{
    free(this);
}

Vector2i getWindowDimensions(Renderer* this)
{
    int x, y;
    SDL_GetWindowSize(this->internal_window, &x, &y);
    return (Vector2i) {x, y};
}

void setColor(Renderer* this, Color color)
{
    SDL_SetRenderDrawColor(this->internal_renderer,
                           color.r, color.g, color.b, color.a);
}

void drawPixel(Renderer* this, Vector2i point, Color color)
{
    setColor(this, color);
    SDL_RenderDrawPoint(this->internal_renderer, point.x, point.y);
}

// `polygon` should have NDC vertices!
void drawPolygon(Renderer* this, Polygon* polygon)
{
    NDCtoScreenSpacePolygon(this, polygon);
    Vector3d minPoint, maxPoint;
    getBoundingPointsPolygon(polygon, &minPoint, &maxPoint);

    for (int x = minPoint.x; x < maxPoint.x; x++)
    {
        for (int y = minPoint.y; x < maxPoint.y; y++)
        {
            if (isPointInsidePolygon((Vector3d) {x, y, 0.}, polygon))
                drawPixel(this, (Vector2i) {x, y}, (Color) {255, 255, 255, 255});
        }
    }
}

void clearBuffer(Renderer* this, Color color)
{
    setColor(this, color);
    SDL_RenderClear(this->internal_renderer);
}

void drawToBuffer(Renderer* this)
{
    Vector3d vertices[3] = {
        (Vector3d) { 0.,   0.5, 0.},
        (Vector3d) {-0.5, -0.5, 0.},
        (Vector3d) { 0.5, -0.5, 0.}
    };
    Polygon* triangle = newPolygon(vertices, NULL, 3);
    drawPolygon(this, triangle);
    freePolygon(triangle);
}

void swapBuffer(Renderer* this)
{
    SDL_RenderPresent(this->internal_renderer);
}

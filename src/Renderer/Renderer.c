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
    Vector3d minPoint, maxPoint;
    getBoundingPointsPolygon(polygon, &minPoint, &maxPoint);

    double* barycentricCoordinatesX2 = allocate(polygon->n * sizeof(double));
    for (int x = minPoint.x; x < maxPoint.x; x++)
    {
        for (int y = minPoint.y; y < maxPoint.y; y++)
        {
            calculateBarycentricCoordinatesX2Polygon(polygon, (Vector3d) {x, y, 0.}, barycentricCoordinatesX2);
            double sum = sumOfArrayDouble(barycentricCoordinatesX2, polygon->n);
            if (sum == polygon->areaX2)
                drawPixel(this, (Vector2i) {x, y}, polygon->colors[0]);
        }
    }
    free(barycentricCoordinatesX2);
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
        (Vector3d) { 0.5, -0.5, 0.},
        (Vector3d) {-0.5, -0.5, 0.}
    };
    Color colors[3] = {
        (Color) {255, 255, 255, 255},
        (Color) {255, 255, 255, 255},
        (Color) {255, 255, 255, 255}
    };

    Polygon* triangle = newPolygon(vertices, colors, this, 3);
    drawPolygon(this, triangle);
    freePolygon(triangle);
}

void swapBuffer(Renderer* this)
{
    SDL_RenderPresent(this->internal_renderer);
}

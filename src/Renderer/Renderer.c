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

void drawTriangle(Renderer* this, Triangle* tri)
{
    Vector3d minPoint, maxPoint;
    getBoundingPointsTriangle(tri, &minPoint, &maxPoint);

    // Set initial barycentric coordinates
    Vector3d barycentricCoordinates = Vector3dAdd(
        Vector3dAdd(
            tri->baryCoordsZero,
            Vector3dMultiplyD(tri->baryDeltaX, minPoint.x)
        ),
        Vector3dMultiplyD(tri->baryDeltaY, minPoint.y)
    );

    for (int y = minPoint.y; y < maxPoint.y; y++)
    {
        for (int x = minPoint.x; x < maxPoint.x; x++)
        {
            double sumBarycentric = 0;
            for (size_t i = 0; i < 3; i++)
            {
                size_t edgeIndex = (i+1) % 3;  // index of edge in front of current vertex
                bool isWrongHalfPlane = Vector3dIndex(barycentricCoordinates, i) < 0;
                bool doesLieOnEdgeNotFlatTopOrLeft = Vector3dIndex(barycentricCoordinates, i) == 0 && \
                                                     !isEdgeFlatTopOrLeftTriangle(tri->edgeVectors[edgeIndex]);
                if (isWrongHalfPlane || doesLieOnEdgeNotFlatTopOrLeft)
                    goto nextPixel;
                sumBarycentric += Vector3dIndex(barycentricCoordinates, i);
            }

            if (roughlyEqualD(sumBarycentric, 1))
                drawPixel(this, (Vector2i) {x, y}, mixColorsBaryCoordTriangle(tri, barycentricCoordinates));

            nextPixel:
            barycentricCoordinates = Vector3dAdd(barycentricCoordinates, tri->baryDeltaX);
        }
        barycentricCoordinates = Vector3dSubtract(barycentricCoordinates, Vector3dMultiplyD(tri->baryDeltaX, maxPoint.x - minPoint.x));
        barycentricCoordinates = Vector3dAdd(barycentricCoordinates, tri->baryDeltaY);
    }
}

void drawToBuffer(Renderer* this)
{
    Color white = {255, 255, 255, 255};

    Vector3d v0 = {-0.5, -0.5, 0.};
    Vector3d v1 = { 0.,   0.5, 0.};
    Vector3d v2 = { 0.5, -0.5, 0.};

    Vector3d vertices[3] = {v0, v1, v2};
    Color colors[3] = {
        {255,   0,   0, 255},
        {  0, 255,   0, 255},
        {  0,   0, 255, 255}
    };

    Triangle* triangle = newTriangle(vertices, colors, this);

    drawTriangle(this, triangle);
    freeTriangle(triangle);
}

void saveBuffer(Renderer* this, const char* filename)
{
    Vector2i dimensions = getWindowDimensions(this);
    SDL_Surface* shot = SDL_CreateRGBSurface(0, dimensions.x, dimensions.y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(this->internal_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, shot->pixels, shot->pitch);
    SDL_SaveBMP(shot, filename);
    SDL_FreeSurface(shot);
}

void swapBuffer(Renderer* this)
{
    SDL_RenderPresent(this->internal_renderer);
}

void clearBuffer(Renderer* this, Color color)
{
    setColor(this, color);
    SDL_RenderClear(this->internal_renderer);
}

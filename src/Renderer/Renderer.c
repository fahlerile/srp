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

void drawPolygon(Renderer* this, Polygon* polygon)
{
    Vector3d minPoint, maxPoint;
    getBoundingPointsPolygon(polygon, &minPoint, &maxPoint);

    double* barycentricCoordinates = allocate(polygon->n * sizeof(double));
    for (int x = minPoint.x; x < maxPoint.x; x++)
    {
        for (int y = minPoint.y; y < maxPoint.y; y++)
        {
            double sumBarycentric = 0;
            for (size_t i = 0; i < polygon->n; i++)
            {
                size_t index = (i+1) % polygon->n;
                Vector3d nextVertexToPoint = Vector3dSubtract((Vector3d) {x, y, 0}, polygon->vertices[index]);
                nextVertexToPoint.z = 0;
                Vector3d edge = polygon->edgeVectors[index];
                edge.z = 0;
                barycentricCoordinates[i] = Vector3dCross(edge, nextVertexToPoint).z / polygon->areaX2;

                if (barycentricCoordinates[i] < 0 || (barycentricCoordinates[i] == 0 && !isEdgeFlatTopOrLeftPolygon(polygon->edgeVectors[index])))
                    goto nextPixel;

                sumBarycentric += barycentricCoordinates[i];
            }

            if (roughlyEqualD(sumBarycentric, 1))
                drawPixel(this, (Vector2i) {x, y}, mixColorsBaryCoordPolygon(polygon, barycentricCoordinates));

            nextPixel:;
        }
    }
    free(barycentricCoordinates);
}

void drawToBuffer(Renderer* this)
{
    Color white = {255, 255, 255, 255};
    Color red = {255, 0, 0, 255};

    Vector3d v0 = {-0.5, -0.5, 0.};
    Vector3d v1 = {-0.5,  0.5, 0.};
    Vector3d v2 = { 0.5,  0.5, 0.};
    Vector3d v3 = { 0.5, -0.5, 0.};

    Vector3d vertices1[3] = {v0, v1, v3};
    Color colors1[3] = {white, white, white};

    Vector3d vertices2[3] = {v1, v2, v3};
    Color colors2[3] = {red, red, red};

    Polygon* triangle1 = newPolygon(vertices1, colors1, this, 3);
    Polygon* triangle2 = newPolygon(vertices2, colors2, this, 3);

    drawPolygon(this, triangle1);
    saveBuffer(this, "screenshot1.bmp");
    drawPolygon(this, triangle2);
    saveBuffer(this, "screenshot2.bmp");

    freePolygon(triangle1);
    freePolygon(triangle2);
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

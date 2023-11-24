#pragma once
#include "utils/utils.h"
#include "Texture.h"
#include "Renderer.h"

typedef enum
{
    TriangleColored,
    TriangleTextured
} TriangleType;

typedef struct
{
    Vector3d vertices[3];  // viewport coordinates (except Z-coordinate, it is NDC)
    Vector3d edgeVectors[3];

    Vector3d minBoundingPoint;
    Vector3d maxBoundingPoint;

    union
    {
        Color* colors;
        struct
        {
        Texture* texture;
        Vector2d* UV;
        };
    };
    TriangleType type;

    // Barycentric coordinates for minBoundingPoint
    Vector3d baryCoordsInitial;
    // Delta values for of barycentric coordinates (incremental computation)
    Vector3d baryDeltaX;
    Vector3d baryDeltaY;
} Triangle;

Triangle* newTriangle(Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV, Renderer* renderer);
void freeTriangle(Triangle* this);
void triangleReinitialize(Triangle* this, Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV, Renderer* renderer);

static void triangleSetVertices(Triangle* this, Vector3d* vertices, Renderer* renderer);
static void triangleSetColorOrTexture(Triangle* this, Color* colors, Texture* texture, Vector2d* UV);
static void triangleCalculateEdgeVectors(Triangle* this);
static void triangleCalculateBaryDeltasAndInitial(Triangle* this);
static void triangleGetBoundingPoints(Triangle* this);
static Vector3d triangleInitializeBarycentricCoordinates(Triangle* this, Vector3d baryCoordsZero, Vector3d point);
static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge);

static Color triangleInterpolateColor(Triangle* this, Vector3d barycentricCoordinates);
static Vector2d triangleInterpolateUV(Triangle* this, Vector3d barycentricCoordinates);

void triangleDraw(Triangle* this, Renderer* renderer);


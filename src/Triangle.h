#pragma once
#include "Utils/Utils.h"
#include "Texture.h"

typedef enum
{
    TriangleColored,
    TriangleTextured
} TriangleType;

typedef struct
{
    Vector3d vertices[3];
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

    // Barycentric coordinates for minBoundingPoint, used for incremental computation
    Vector3d baryCoordsInitial;
    // Delta values for incremental computation of barycentric coordinates
    Vector3d baryDeltaX;
    Vector3d baryDeltaY;
} Triangle;

#include "Renderer.h"  // circular dependency

// `vertices` must be NDC and clockwise!
// colors OR (UV AND texture) should be NULL, the other one shouldn't
Triangle* newTriangle(Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV, Renderer* renderer);
void freeTriangle(Triangle* this);

Color triangleInterpolateColor(Triangle* this, Vector3d barycentricCoordinates);
Vector2d triangleInterpolateUV(Triangle* this, Vector3d barycentricCoordinates);

void triangleDraw(Triangle* this, Renderer* renderer);

static void triangleGetBoundingPoints(Triangle* this, Vector3d* min, Vector3d* max);
static Vector3d triangleInitializeBarycentricCoordinates(Triangle* this, Vector3d baryCoordsZero, Vector3d point);
static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge);

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
    Vector3d* vertices;
    Vector3d* edgeVectors;
    double areaX2;

    Color* colors;
    Texture* texture;
    Vector2d* UV;
    TriangleType type;

    // Barycentric coordinates for point (0, 0), used for incremental computation
    Vector3d baryCoordsZero;
    // Delta values for incremental computation of barycentric coordinates
    Vector3d baryDeltaX;
    Vector3d baryDeltaY;
} Triangle;

#include "Renderer.h"  // circular dependency

// `vertices` must be NDC and clockwise!
Triangle* newTriangle(Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV,
                      TriangleType type, Renderer* renderer);
void freeTriangle(Triangle* this);

// min = {min_x, min_y, min_z},
// max = {max_x, max_y, max_z}
void triangleGetBoundingPoints(Triangle* this, Vector3d* min, Vector3d* max);

Color triangleInterpolateColor(Triangle* this, Vector3d barycentricCoordinates);
Vector2d triangleInterpolateUV(Triangle* this, Vector3d barycentricCoordinates);

bool triangleIsEdgeFlatTopOrLeft(Vector3d edge);

Vector3d triangleInitializeBarycentricCoordinates(Triangle* this, Vector3d point);

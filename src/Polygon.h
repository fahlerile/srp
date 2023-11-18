#pragma once
#include "Utils/Utils.h"

// Should be convex
typedef struct
{
    Vector3d* vertices;
    Vector3d* edgeVectors;
    Color* colors;
    double areaX2;
    // Barycentric coordinates for point (0, 0), used for incremental computation
    Vector3d baryCoordsZero;
    // Delta values for incremental computation of barycentric coordinates
    Vector3d baryDeltaX;
    Vector3d baryDeltaY;
} Triangle;

#include "Renderer/Renderer.h"  // circular dependency

// `vertices` must be NDC and clockwise!
Triangle* newTriangle(Vector3d* vertices, Color* colors, Renderer* renderer);
void freeTriangle(Triangle* this);

// min = {min_x, min_y, min_z},
// max = {max_x, max_y, max_z}
void getBoundingPointsTriangle(Triangle* this, Vector3d* min, Vector3d* max);

// Mix polygon's vertices' colors with weights (barycentrtic coordinates)
Color mixColorsBaryCoordTriangle(Triangle* this, Vector3d barycentricCoordinates);

bool isEdgeFlatTopOrLeftTriangle(Vector3d edge);

Vector3d initializeBarycentricCoordinatesTriangle(Triangle* this, Vector3d point);

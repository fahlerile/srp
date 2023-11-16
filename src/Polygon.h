#pragma once
#include "Utils/Utils.h"

// Should be convex
typedef struct
{
    Vector3d* vertices;
    Vector3d* edgeVectors;
    Color* colors;
    double areaX2;
    size_t n;
} Polygon;

#include "Renderer/Renderer.h"  // circular dependency

// `vertices` must be NDC and clockwise!
Polygon* newPolygon(Vector3d* vertices, Color* colors, Renderer* renderer, size_t n);
void freePolygon(Polygon* this);

// min = {min_x, min_y, min_z},
// max = {max_x, max_y, max_z}
void getBoundingPointsPolygon(Polygon* this, Vector3d* min, Vector3d* max);

void calculateBarycentricCoordinatesPolygon(Polygon* this, Vector3d point, double* barycentricCoordinates);
// Mix polygon's vertices' colors with weights (barycentrtic coordinates)
Color mixColorsBaryCoordPolygon(Polygon* this, double* barycentricCoordinates);

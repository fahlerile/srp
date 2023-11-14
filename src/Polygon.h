#pragma once
#include "Utils/Utils.h"

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
void getBoundingPointsPolygon(Polygon* polygon, Vector3d* min, Vector3d* max);

bool isPointInsidePolygon(Vector3d point, Polygon* polygon);

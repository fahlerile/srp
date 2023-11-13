#pragma once
#include "Utils/Utils.h"

typedef struct
{
    Vector3d* vertices;
    Vector3d* edgeVectors;
    Color* colors;
    double area;
    size_t n;
} Polygon;

#include "Renderer/Renderer.h"  // circular dependency

Polygon* newPolygon(Vector3d* vertices, Color* colors, size_t n);
void freePolygon(Polygon* this);

// Convert `Polygon` vertices from NDC to screen space IN-PLACE
void NDCtoScreenSpacePolygon(Renderer* renderer, Polygon* polygon);

// min = {min_x, min_y, min_z},
// max = {max_x, max_y, max_z}
void getBoundingPointsPolygon(Polygon* polygon, Vector3d* min, Vector3d* max);

bool isPointInsidePolygon(Vector3d point, Polygon* polygon);

#pragma once
#include "utils.h"

typedef struct
{
    Vector4d* position;
    Vector2d* UV;
    Vector3d* normal;
} Vertex;

typedef struct
{
    Vertex vertices[3];
} Triangle;

// UNTESTED
// Triangulate a CONVEX face given its ORDERED ((anti-)clockwise) vertices (`Vertex`)
// Uses splitting by diagonals
// Returns a `DynamicArray` of `Triangle`'s
DynamicArray* triangulateFace(DynamicArray* vertices);
bool areAllVerticesOfATriangleOutsideOfUnitCube(Triangle* this);
void drawTriangle(Triangle* triangle);

static void triangleDrawEdges(Triangle* triangle);
static void triangleFill(Triangle* triangle);

static void triangleConvertVerticesToScreenSpace(
    Triangle* triangle, Vector3d* SSVertices
);
static void triangleGetBoundingPoints(
    Vector3d* SSVertices, Vector3d* min, Vector3d* max
);
static void triangleCalculateEdgeVectors(
    Vector3d* SSVertices, Vector3d* edgeVectors
);
static void triangleCalculateBaryDeltasAndInitial(
    Triangle* triangle, Vector3d* SSVertices, Vector3d* edgeVectors,
    Vector3d* baryCoordsInitial, Vector3d* baryDeltaX,
    Vector3d* baryDeltaY, Vector3d minBoundingPoint
);
static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge);
static Vector3d triangleInitializeBarycentricCoordinates(
    Vector3d baryCoordsZero, Vector3d baryDeltaX,
    Vector3d baryDeltaY, Vector3d point
);
// static Color triangleInterpolateColor(Triangle* triangle, Vector3d barycentricCoordinates);
// static Vector2d triangleInterpolateUV(Triangle* triangle, Vector3d barycentricCoordinates);


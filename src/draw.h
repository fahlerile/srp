#pragma once
#include "VertexBuffer.h" 
#include "Vector/Vector.h"

void drawTriangle(Vector3d* NDCPositions);

// Transform a plain array of N NDC coordinates to a plain array of N screen space coordinates
static void transformPositionsToScreenSpace(
    Vector3d* NDCPositions, size_t n, Vector3d* SSPositions
);

// Get two 2D (X, Y) bounding points for a plain array of N screen space vertex positions
static void getBoundingPoints(
    Vector3d* SSPositions, size_t n, 
    Vector3d* minBoundingPoint, Vector3d* maxBoundingPoint
);

// Calculate edge vectors for a plain array of N screen space vertex positions
static void calculateEdgeVectors(
    Vector3d* SSPositions, size_t n, Vector3d* edgeVectors
);

// Calculate per-pixel delta values for barycentric coordinates for a triangle and barycentric coordinates for point (0, 0)
static void triangleCalculateBarycenticDeltas(
    Vector3d* SSPositions, Vector3d* edgeVectors,
    Vector3d* barycentricDeltaX, Vector3d* barycentricDeltaY,
    Vector3d* barycentricCoordinatesZero
);
// Calculate barycentric coordinates in a triangle for a point given per-pixel delta values
static Vector3d triangleCalculateBarycentricCoordinatesForPoint(
    Vector3d barycentricCoordinatesZero, Vector3d barycentricDeltaX,
    Vector3d barycentricDeltaY, Vector3d point
);

static bool triangleIsEdgeFlatTopOrLeft(Vector3d edgeVector);


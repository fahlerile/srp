#pragma once
#include "VertexBuffer.h" 
#include "Vector/Vector.h"

// `vsOutput` is an array of 3 vertex shader outputs 
// NDC vertex positions are assumed to be first at every VS output (see the comment above `VertexShaderType` definition)
void drawTriangle(void* vsOutput);

static double signedAreaParallelogram(Vector2d a, Vector2d b);

// Transform a plain array of N NDC coordinates to a plain array of N screen space coordinates
static void transformPositionsToScreenSpace(
    Vector3d* NDCPositions, size_t n, Vector3d* SSPositions
);

// Get two 2D (X, Y) bounding points for a plain array of N screen space vertex positions
static void triangleGetBoundingPoints(
    Vector3d* SSPositions, Vector2d* minBoundingPoint, Vector2d* maxBoundingPoint
);

// Calculate edge vectors for a plain array of N screen space vertex positions
static void triangleCalculateEdgeVectors(Vector3d* SSPositions, Vector2d* edgeVectors);
static void calculatePositiveOnPlusYAndSlopeSigns(
    Vector3d* SSPositions, Vector2d* edgeVectors,
    bool* positiveOnPlusY, bool* edgeSlopePositiveOrZero
);

// Calculate per-pixel delta values for barycentric coordinates for a triangle and barycentric coordinates for point (0, 0)
static void triangleCalculateBarycenticDeltasAndBarycentricCoordinatesForPoint(
    Vector3d* SSPositions, Vector2d* edgeVectors,
    double* barycentricDeltaX, double* barycentricDeltaY,
    double* barycentricCoordinates, Vector2d* point
);
// Calculate barycentric coordinates in a triangle for a point given per-pixel delta values
static Vector3d triangleCalculateBarycentricCoordinatesForPoint(
    Vector3d barycentricCoordinatesZero, Vector3d barycentricDeltaX,
    Vector3d barycentricDeltaY, Vector3d point
);

static void loopOverTileAndFillNoCheck(
    int xTile, int yTile, Vector2i tileDimensions, Vector2d minBoundingPoint
);
static void loopOverTileAndFill(
    int xTile, int yTile, Vector2i tileDimensions, Vector2d minBoundingPoint
);

static bool triangleIsEdgeFlatTopOrLeft(Vector2d edgeVector);


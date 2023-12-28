#pragma once
#include "Face.h"

void drawTriangle(Face* this);
void drawLineBresenham(Vector2i p1, Vector2i p2);

static void triangleDrawEdges(Face* this);
static void triangleFill(Face* this);

static void triangleConvertVerticesToScreenSpace(
    Face* this, Vector3d* SSVertices
);
static void triangleGetBoundingPoints(
    Vector3d* SSVertices, Vector3d* min, Vector3d* max
);
static void triangleCalculateEdgeVectors(
    Face* this, Vector3d* SSVertices, Vector3d* edgeVectors
);
static void triangleCalculateBaryDeltasAndInitial(
    Face* this, Vector3d* SSVertices, Vector3d* edgeVectors,
    Vector3d* baryCoordsInitial, Vector3d* baryDeltaX,
    Vector3d* baryDeltaY, Vector3d minBoundingPoint
);
static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge);
static Vector3d triangleInitializeBarycentricCoordinates(
    Face* this, Vector3d baryCoordsZero, Vector3d baryDeltaX,
    Vector3d baryDeltaY, Vector3d point
);
// static Color triangleInterpolateColor(Face* this, Vector3d barycentricCoordinates);
// static Vector2d triangleInterpolateUV(Face* this, Vector3d barycentricCoordinates);


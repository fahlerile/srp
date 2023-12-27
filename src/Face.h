#pragma once
#include "utils/utils.h"

typedef struct
{
    Vector4d* position;
    Vector2d* UV;
    Vector3d* normal;
} Vertex;

typedef struct
{
    DynamicArray* vertices;  // `Vertex`
} Face;

typedef enum
{
    faceModeFill = 0,
    faceModeLine
} faceModeType;

Face* newFace(DynamicArray* vertices);
Face* copyFace(Face* this);
DynamicArray* triangulateFace(Face* this);
void freeFace(Face* this);

void drawFace(Face* this);
void drawTriangle(Face* this);
void drawLineBresenham(Vector2d p1, Vector2d p2);

static void triangleDrawEdges(Face* this);
static void triangleFill(Face* this);

static void triangleConvertVerticesToScreenSpace(
    Face* this, Vector3d* SSVertices
);
static void triangleGetBoundingPoints(
    Face* triangle, Vector3d* SSVertices, Vector3d* min, Vector3d* max
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


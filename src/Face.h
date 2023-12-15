#pragma once
#include "utils/utils.h"
#include "Texture.h"
#include "Renderer.h"

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

Face* newFace(DynamicArray* vertices);
Face* copyFace(Face* this);
DynamicArray* triangulateFace(Face* this);
void freeFace(Face* this);

void drawFace(Face* this, Renderer* renderer);
void drawTriangle(Face* this, Renderer* renderer);

static void triangleConvertVerticesToScreenSpace(Face* this, Renderer* renderer, Vector3d* SSVertices);
static void triangleGetBoundingPoints(Face* triangle, Vector3d* SSVertices, Vector3d* min, Vector3d* max);
static void triangleCalculateEdgeVectors(Face* this, Vector3d* SSVertices, Vector3d* edgeVectors);
static void triangleCalculateBaryDeltasAndInitial(Face* this, Vector3d* SSVertices, Vector3d* edgeVectors, Vector3d* baryCoordsInitial, Vector3d* baryDeltaX, Vector3d* baryDeltaY, Vector3d minBoundingPoint);
static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge);
static Vector3d triangleInitializeBarycentricCoordinates(Face* this, Vector3d baryCoordsZero, Vector3d baryDeltaX, Vector3d baryDeltaY, Vector3d point);
// static Color triangleInterpolateColor(Face* this, Vector3d barycentricCoordinates);
// static Vector2d triangleInterpolateUV(Face* this, Vector3d barycentricCoordinates);


#pragma once
#include "utils.h"

#define LOG_FACE(p_face, LOG_FN) \
    do \
    { \
        for (size_t i = 0; i < (p_face)->vertices->size; i++) \
        { \
            Vertex* v = indexDynamicArray((p_face)->vertices, i); \
            LOG_FN("VERTEX %zu (POSITION, UV, NORMAL)\n", i); \
            LOG_VECTOR4D(*(v->position), LOG_FN); \
            LOG_VECTOR2D(*(v->UV), LOG_FN); \
            LOG_VECTOR3D(*(v->normal), LOG_FN); \
        } \
    } while (false)

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

bool areAllVerticesOfAFaceOutsideOfUnitCube(Face* this);


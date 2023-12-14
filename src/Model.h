#pragma once
#include "utils/DynamicArray.h"
#include "utils/Vectors/Vectors.h"
#include "utils/Matrix.h"

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
Face* faceCopy(Face* this);
void freeFace(Face* this);

typedef struct
{
    DynamicArray* vertexPositions;  // `Vector4d`
    DynamicArray* UVs;              // `Vector2d`
    DynamicArray* normals;          // `Vector3d`
    DynamicArray* matrices;         // `Matrix4`
    DynamicArray* faces;            // `Face*`
} Model;

Model* newModel(const char* filename);
void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale);
void modelRender(Model* this, Matrix4* view, Matrix4* projection);
void freeModel(Model* this);

static void modelParseObj(Model* this, const char* filename);

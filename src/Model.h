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
    DynamicArray* vertices;  // an array of `Vertex`
} Face;

typedef struct
{
    DynamicArray* vertexPositions;  // an array of `Vector4d`
    DynamicArray* UVs;              // an array of `Vector2d`
    DynamicArray* normals;          // an array of `Vector3d`
    DynamicArray* matrices;         // an array of `Matrix4`
} Model;

Model* newModel(const char* filename);
void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale);
void modelRender(Model* this, Matrix4* view, Matrix4* projection);
void freeModel(Model* this);

static void modelParseObj(Model* this, const char* filename);

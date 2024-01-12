#pragma once
#include "DynamicArray/DynamicArray.h"
#include "Matrix/Matrix.h"

typedef struct
{
    DynamicArray* vertexPositions;  // `Vector4d`
    DynamicArray* textureCoords;    // `Vector3d`
    DynamicArray* normals;          // `Vector3d`
    DynamicArray* matrices;         // `Matrix4`
    DynamicArray* triangles;        // `Triangle`
} Model;

Model* newModel(const char* filename);
void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale);
void modelRender(Model* this, Matrix4* view, Matrix4* projection);
void freeModel(Model* this);

static void modelParseObj(Model* this, const char* filename);

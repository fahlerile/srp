#pragma once
#include "Renderer.h"
#include "DynamicArray/DynamicArray.h"
#include "Vector/Vector.h"
#include "Matrix/Matrix.h"

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
void modelRender(Model* this, Matrix4* view, Matrix4* projection, Renderer* renderer);
void freeModel(Model* this);

static void modelParseObj(Model* this, const char* filename);

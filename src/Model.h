#pragma once
#include "utils/DynamicArray.h"
#include "utils/Vectors/Vector3.h"

typedef struct
{
    DynamicArray* matrices;  // an array of `Matrix4`
} Model;

Model* newModel(const char* filename);
void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale);
void freeModel(Model* this);

#pragma once
#include "Utils/DynamicArray.h"
#include "Utils/Matrix.h"
#include "Model.h"

typedef struct
{
    DynamicArray* models;
    Matrix4 viewMatrix;
} Scene;

Scene* newScene(Matrix4 viewMatrix);
void sceneAddModel(Scene* this, Model* model);
void sceneRender(Scene* this);
void freeSceneAndModels(Scene* this);

#pragma once
#include "Renderer.h"
#include "Model.h"
#include "utils/DynamicArray.h"
#include "utils/Matrix.h"

typedef struct
{
    DynamicArray* models;  // an array of `Model*`
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
} Scene;

Scene* newScene(Matrix4 viewMatrix, Matrix4 projectionMatrix);
void sceneAddModel(Scene* this, Model* model);
void sceneRender(Scene* this, Renderer* renderer);
void freeSceneAndModels(Scene* this);

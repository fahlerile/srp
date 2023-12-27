#pragma once
#include "Renderer.h"
#include "Model.h"
#include "DynamicArray/DynamicArray.h"
#include "Matrix/Matrix.h"

typedef struct
{
    DynamicArray* models;  // an array of `Model*`
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
} Scene;

Scene* newScene(Matrix4 viewMatrix, Matrix4 projectionMatrix);
void sceneAddModel(Scene* this, Model* model);
void sceneRender(Scene* this);
void freeSceneAndModels(Scene* this);

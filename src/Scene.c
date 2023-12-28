#include "utils.h"
#include "Scene.h"
#include "Model.h"

static void sceneFreeCallback(void* p_model)
{
    freeModel(*(Model**) p_model);
}

Scene* newScene(Matrix4 viewMatrix, Matrix4 projectionMatrix)
{
    Scene* this = xmalloc(sizeof(Scene));
    this->models = newDynamicArray(10, sizeof(Model*), sceneFreeCallback);
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    return this;
}

void sceneAddModel(Scene* this, Model* model)
{
    addToDynamicArray(this->models, &model);
}

void sceneRender(Scene* this)
{
    for (size_t i = 0; i < this->models->size; i++)
        modelRender(*(Model**) indexDynamicArray(this->models, i), &this->viewMatrix, &this->projectionMatrix);
}

void freeSceneAndModels(Scene* this)
{
    freeDynamicArray(this->models);
    xfree(this);
}

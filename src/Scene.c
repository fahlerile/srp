#include "Scene.h"
#include "Model.h"
#include "utils/utils.h"

Scene* newScene(Matrix4 viewMatrix, Matrix4 projectionMatrix)
{
    Scene* this = xmalloc(sizeof(Scene));
    this->models = newDynamicArray(10, sizeof(Model*));
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
    // We store Model* in `models` array and we get a
    // pointer to stored type from `indexDynamicArray`,
    // so we need to dereference it to get a Model*
    for (size_t i = 0; i < this->models->size; i++)
        freeModel(*(Model**) indexDynamicArray(this->models, i));
    freeDynamicArray(this->models);
    xfree(this);
}

#include "Scene.h"
#include "Model.h"
#include "Utils/Utils.h"

Scene* newScene(Matrix4 viewMatrix)
{
    Scene* this = xmalloc(sizeof(Scene));
    this->models = newDynamicArray(10, sizeof(Model*));
    this->viewMatrix = viewMatrix;
    return this;
}

void sceneAddModel(Scene* this, Model* model)
{
    addToDynamicArray(this->models, model);
}

void sceneRender(Scene* this)
{

}

void freeSceneAndModels(Scene* this)
{
    freeDynamicArray(this->models);
    xfree(this);
}

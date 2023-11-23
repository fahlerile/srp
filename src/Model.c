#include "Model.h"
#include "Utils/Utils.h"

Model* newModel(const char* filename)
{
    Model* this = xmalloc(sizeof(Model));
    this->matrices = newDynamicArray(10, sizeof(Matrix4));
    return this;
}

void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale)
{
    Matrix4 mat = Matrix4ConstructTRS(position, rotation, scale);
    addToDynamicArray(this->matrices, &mat);
}

void freeModel(Model* this)
{
    freeDynamicArray(this->matrices);
    xfree(this);
}

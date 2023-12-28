#include "Face.h"
#include "draw.h"
#include "DynamicArray/DynamicArray.h"

Face* newFace(DynamicArray* vertices)
{
    Face* this = xmalloc(sizeof(Face));
    this->vertices = vertices;
    return this;
}

Face* copyFace(Face* this)
{
    DynamicArray* copy = copyDynamicArray(this->vertices);
    return newFace(copy);
}

DynamicArray* triangulateFace(Face* this)
{
    // TODO
    LOGE("triangulateFace: NOT IMPLEMENTED!\n");
    return NULL;
}

void freeFace(Face* this)
{
    freeDynamicArray(this->vertices);
    xfree(this);
}

void drawFace(Face* this)
{   
    // TODO
    // DynamicArray* triangulatedFace = faceTriangulate(this);  // `Face*`
    // for (size_t i = 0; i < triangulatedFace->size; i++)
    // {
    //     Face* triangle = *(Face**) indexDynamicArray(triangulatedFace, i);
    //     drawTriangle(triangle, renderer);
    // }
    
    drawTriangle(this);
}

bool areAllVerticesOfAFaceOutsideOfUnitCube(Face* this)
{
    for (size_t i = 0; i < this->vertices->size; i++)
    {
        Vector4d position = *((Vertex*) indexDynamicArray(this->vertices, i))->position;
        if ((position.x >= -1. && position.x <= 1.) &&
            (position.y >= -1. && position.y <= 1.) &&
            (position.z >= -1. && position.z <= 1.))
        {
            return false;
        }
    }
    return true;
}


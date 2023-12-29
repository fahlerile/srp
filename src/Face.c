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

// Triangulate a face. FACE SHOULD BE A CONVEX POLYGON!
// Uses a simple "sun rays" algorithm
DynamicArray* triangulateFace(Face* this)
{
    LOGE("triangulateFace: NOT IMPLEMENTED!\n");
    return NULL;

    // if (this->vertices->size == 3)
    // {
    //     DynamicArray(1, sizeof(Face*));
    // }

    DynamicArray* triangles = newDynamicArray(2, sizeof(Face*), NULL);
    for (size_t i = 2, n = this->vertices->size; i < n; i++)
    {
        // TRIANGLE 0 i-1 i
    }
}

void freeFace(Face* this)
{
    freeDynamicArray(this->vertices);
    xfree(this);
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


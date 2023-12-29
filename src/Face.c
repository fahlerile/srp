#include "Face.h"
#include "draw.h"
#include "DynamicArray/DynamicArray.h"
#include "log.h"
#include <strings.h>

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

// Triangulate a face. FACE SHOULD BE A CONVEX POLYGON WITH CLOCKWISE VERTEX ORDERING!
// Uses a simple "sun rays" algorithm
DynamicArray* triangulateFace(Face* this)
{
    LOGW("triangulateFace called!\n");

    DynamicArray* triangles = newDynamicArray(2, sizeof(Face*), faceFreeCallbackForDynamicArray);
    Vertex vertex0 = *(Vertex*) indexDynamicArray(this->vertices, 0);

    for (size_t i = 2, n = this->vertices->size; i < n; i++)
    {
        DynamicArray* vertices = newDynamicArray(3, sizeof(Vertex), NULL);
        addToDynamicArray(vertices, &vertex0);
        addToDynamicArray(vertices, indexDynamicArray(this->vertices, i-1));
        addToDynamicArray(vertices, indexDynamicArray(this->vertices, i));

        Face* triangle = newFace(vertices);
        addToDynamicArray(triangles, &triangle);
    }

    return triangles;
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

void faceFreeCallbackForDynamicArray(void* p_p_face)
{
    freeFace(*(Face**) p_p_face);
}


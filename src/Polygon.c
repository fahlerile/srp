#include <float.h>
#include "Polygon.h"
#include "Utils/NDC.h"
#include "Utils/Array.h"
#include "Utils/Utils.h"

Triangle* newTriangle(Vector3d* vertices, Color* colors, Renderer* renderer)
{
    Triangle* this = allocate(sizeof(Triangle));

    this->vertices = vertices;
    for (size_t i = 0; i < 3; i++)
        this->vertices[i] = NDCtoScreenSpace(renderer, vertices[i]);

    this->edgeVectors = allocate(3 * sizeof(Vector3d));
    this->colors = colors;

    // Calculate `edgeVectors`
    for (size_t i = 0; i < 3; i++)
        this->edgeVectors[i] = Vector3dSubtract(vertices[(i+1 == 3) ? 0 : i+1], vertices[i]);

    // Calculate `areaX2`
    this->areaX2 = Vector3dMagnitude(
        Vector3dCross(this->edgeVectors[0], this->edgeVectors[2])
    );

    // Calculate `baryCoordsZero`
    this->baryCoordsZero = (Vector3d) {
        Vector3dCross(
            Vector3dZeroZ(this->edgeVectors[1]),
            Vector3dZeroZ(Vector3dNegate(this->vertices[1]))
        ).z,
        Vector3dCross(
            Vector3dZeroZ(this->edgeVectors[2]),
            Vector3dZeroZ(Vector3dNegate(this->vertices[2]))
        ).z,
        Vector3dCross(
            Vector3dZeroZ(this->edgeVectors[0]),
            Vector3dZeroZ(Vector3dNegate(this->vertices[0]))
        ).z
    };

    // Calculate `baryDelta`'s
    this->baryDeltaX = (Vector3d) {
        this->vertices[1].y - this->vertices[2].y,
        this->vertices[2].y - this->vertices[0].y,
        this->vertices[1].y - this->vertices[1].y
    };

    this->baryDeltaY = (Vector3d) {
        this->vertices[2].x - this->vertices[1].x,
        this->vertices[0].x - this->vertices[2].x,
        this->vertices[1].x - this->vertices[0].x
    };

    return this;
}

void freeTriangle(Triangle* this)
{
    free(this->edgeVectors);
    free(this);
}

void getBoundingPointsTriangle(Triangle* this, Vector3d* min, Vector3d* max)
{
    Vector3d res_min = {DBL_MAX, DBL_MAX, DBL_MAX};
    Vector3d res_max = {0};

    for (size_t i = 0; i < 3; i++)
    {
        if (this->vertices[i].x < res_min.x)
            res_min.x = this->vertices[i].x;
        if (this->vertices[i].y < res_min.y)
            res_min.y = this->vertices[i].y;
        if (this->vertices[i].z < res_min.z)
            res_min.z = this->vertices[i].z;

        if (this->vertices[i].x > res_max.x)
            res_max.x = this->vertices[i].x;
        if (this->vertices[i].y > res_max.y)
            res_max.y = this->vertices[i].y;
        if (this->vertices[i].z > res_max.z)
            res_max.z = this->vertices[i].z;
    }

    *min = res_min;
    *max = res_max;
}

Color mixColorsBaryCoordTriangle(Triangle* this, Vector3d barycentricCoordinates)
{
    Vector4d resColorV = {0};  // result color vector
    for (size_t i = 0; i < 3; i++)
    {
        Vector4d curColorAsVector4d = ColorToVector4d(this->colors[i]);
        Vector4d weighted = Vector4dMultiplyD(curColorAsVector4d, Vector3dIndex(barycentricCoordinates, i));
        resColorV = Vector4dAdd(resColorV, weighted);
    }
    Color color = Vector4dToColor(resColorV);
    return color;
}

bool isEdgeFlatTopOrLeftTriangle(Vector3d edge)
{
    return ((edge.x > 0) && (edge.y == 0)) ||  // is top
           (edge.y < 0);                       // is left
}

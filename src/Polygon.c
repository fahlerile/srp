#include <float.h>
#include "Polygon.h"
#include "Utils/NDC.h"
#include "Utils/Array.h"
#include "Utils/Utils.h"

Polygon* newPolygon(Vector3d* vertices, Color* colors, Renderer* renderer, size_t n)
{
    assert(n >= 3);

    Polygon* this = allocate(sizeof(Polygon));

    this->vertices = vertices;
    for (size_t i = 0; i < n; i++)
        this->vertices[i] = NDCtoScreenSpace(renderer, vertices[i]);

    this->edgeVectors = allocate(n * sizeof(Vector3d));
    this->colors = colors;
    this->n = n;

    // Calculate `edgeVectors`
    for (size_t i = 0; i < n; i++)
        this->edgeVectors[i] = Vector3dSubtract(vertices[(i == n-1) ? 0 : i+1], vertices[i]);

    // Calculate `areaX2`
    if (n == 3)
    {
        this->areaX2 = Vector3dMagnitude(
            Vector3dCross(this->edgeVectors[0], Vector3dNegate(this->edgeVectors[2]))
        );
    }
    else  // NOT TESTED!
    {
        this->areaX2 = 0;
        for (size_t i = 2; i < n; i++)
        {
            double areaOfThisSubtriangleX2 = Vector3dMagnitude(Vector3dCross(
                Vector3dSubtract(this->vertices[0], this->vertices[i]),
                Vector3dNegate(this->edgeVectors[i-1])
            ));
            this->areaX2 += areaOfThisSubtriangleX2;
        }
    }

    return this;
}

void freePolygon(Polygon* this)
{
    free(this->edgeVectors);
    free(this);
}

void getBoundingPointsPolygon(Polygon* polygon, Vector3d* min, Vector3d* max)
{
    Vector3d res_min = {DBL_MAX, DBL_MAX, DBL_MAX};
    Vector3d res_max = {0};

    for (size_t i = 0; i < polygon->n; i++)
    {
        if (polygon->vertices[i].x < res_min.x)
            res_min.x = polygon->vertices[i].x;
        if (polygon->vertices[i].y < res_min.y)
            res_min.y = polygon->vertices[i].y;
        if (polygon->vertices[i].z < res_min.z)
            res_min.z = polygon->vertices[i].z;

        if (polygon->vertices[i].x > res_max.x)
            res_max.x = polygon->vertices[i].x;
        if (polygon->vertices[i].y > res_max.y)
            res_max.y = polygon->vertices[i].y;
        if (polygon->vertices[i].z > res_max.z)
            res_max.z = polygon->vertices[i].z;
    }

    *min = res_min;
    *max = res_max;
}

bool isPointInsidePolygon(Vector3d point, Polygon* polygon)
{
    double* barycentricCoordinatesX2 = allocate(polygon->n * sizeof(double));
    for (size_t i = 0; i < polygon->n; i++)
    {
        Vector3d toPoint = Vector3dSubtract(point, polygon->vertices[i]);
        barycentricCoordinatesX2[i] = Vector3dMagnitude(Vector3dCross(toPoint, polygon->edgeVectors[i]));
        if (barycentricCoordinatesX2[i] < 0)
        {
            free(barycentricCoordinatesX2);
            return false;
        }
    }

    double sum = sumOfArrayDouble(barycentricCoordinatesX2, polygon->n);
    free(barycentricCoordinatesX2);
    if (sum == polygon->areaX2)
        return true;
    return false;
}

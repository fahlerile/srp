#include "Polygon.h"
#include "Utils/NDC.h"
#include "Utils/Array.h"
#include "Utils/Utils.h"

Polygon* newPolygon(Vector3d* vertices, Color* colors, size_t n)
{
    Polygon* this = allocate(sizeof(Polygon));
    this->vertices = vertices;
    this->edgeVectors = allocate(n * sizeof(Vector3d));
    this->colors = colors;
    this->n = n;

    // Calculate `edgeVectors`
    for (size_t i = 0; i < n; i++)
        this->edgeVectors[i] = Vector3dSubtract(vertices[i+1], vertices[i]);

    // Calculate the area


    return this;
}

void freePolygon(Polygon* this)
{
    free(this->edgeVectors);
    free(this);
}

void NDCtoScreenSpacePolygon(Renderer* renderer, Polygon* polygon)
{
    for (size_t i = 0; i < polygon->n; i++)
        polygon->vertices[i] = NDCtoScreenSpace(renderer, polygon->vertices[i]);
}

void getBoundingPointsPolygon(Polygon* polygon, Vector3d* min, Vector3d* max)
{
    Vector3d res_min, res_max = {0};

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
    double* barycentricCoordinatesMultipliedByTwo = allocate(polygon->n * sizeof(double));
    for (size_t i = 0; i < polygon->n; i++)
    {
        Vector3d toPoint = Vector3dSubtract(point, polygon->vertices[i]);
        barycentricCoordinatesMultipliedByTwo[i] = MagnitudeVector3d(Vector3dCross(toPoint, polygon->edgeVectors[i]));
        if (barycentricCoordinatesMultipliedByTwo[i] < 0)
        {
            free(barycentricCoordinatesMultipliedByTwo);
            return false;
        }
    }

    double sum = sumOfArrayDouble(barycentricCoordinatesMultipliedByTwo, polygon->n);
    free(barycentricCoordinatesMultipliedByTwo);
    if (sum == polygon->area * 2)
        return true;
    return false;
}

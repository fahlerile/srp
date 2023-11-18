#include <float.h>
#include "Triangle.h"
#include "Utils/NDC.h"
#include "Utils/Array.h"
#include "Utils/Utils.h"

Triangle* newTriangle(Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV,
                      TriangleType type, Renderer* renderer)
{
    Triangle* this = allocate(sizeof(Triangle));

    this->vertices = vertices;
    for (size_t i = 0; i < 3; i++)
        this->vertices[i] = NDCtoScreenSpace(renderer, vertices[i]);

    this->edgeVectors = allocate(3 * sizeof(Vector3d));
    this->colors = colors;
    this->texture = texture;
    this->UV = UV;
    this->type = type;

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
    this->baryCoordsZero = Vector3dDivideD(this->baryCoordsZero, this->areaX2);

    // Calculate `baryDelta`'s
    this->baryDeltaX = (Vector3d) {
        this->vertices[1].y - this->vertices[2].y,
        this->vertices[2].y - this->vertices[0].y,
        this->vertices[0].y - this->vertices[1].y
    };
    this->baryDeltaX = Vector3dDivideD(this->baryDeltaX, this->areaX2);

    this->baryDeltaY = (Vector3d) {
        this->vertices[2].x - this->vertices[1].x,
        this->vertices[0].x - this->vertices[2].x,
        this->vertices[1].x - this->vertices[0].x
    };
    this->baryDeltaY = Vector3dDivideD(this->baryDeltaY, this->areaX2);

    return this;
}

void freeTriangle(Triangle* this)
{
    free(this->edgeVectors);
    free(this);
}

void triangleGetBoundingPoints(Triangle* this, Vector3d* min, Vector3d* max)
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

Color triangleInterpolateColor(Triangle* this, Vector3d barycentricCoordinates)
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

Vector2d triangleInterpolateUV(Triangle* this, Vector3d barycentricCoordinates)
{
    Vector2d resUV = {0};
    for (size_t i = 0; i < 3; i++)
    {
        Vector2d weighted = Vector2dMultiplyD(this->UV[i], Vector3dIndex(barycentricCoordinates, i));
        resUV = Vector2dAdd(resUV, weighted);
    }
    return resUV;
}

bool triangleIsEdgeFlatTopOrLeft(Vector3d edge)
{
    return ((edge.x > 0) && (edge.y == 0)) ||  // is top
           (edge.y < 0);                       // is left
}

Vector3d triangleInitializeBarycentricCoordinates(Triangle* this, Vector3d point)
{
    return Vector3dAdd(
        Vector3dAdd(
            this->baryCoordsZero,
            Vector3dMultiplyD(this->baryDeltaX, point.x)
        ),
        Vector3dMultiplyD(this->baryDeltaY, point.y)
    );
}

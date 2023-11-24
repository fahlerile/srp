#include <float.h>
#include "Triangle.h"
#include "utils/NDC.h"
#include "utils/utils.h"

Triangle* newTriangle(Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV, Renderer* renderer)
{
    Triangle* this = xmalloc(sizeof(Triangle));
    triangleReinitialize(this, vertices, colors, texture, UV, renderer);
    return this;
}

void freeTriangle(Triangle* this)
{
    xfree(this);
}

void triangleReinitialize(Triangle* this, Vector3d* vertices, Color* colors, Texture* texture, Vector2d* UV, Renderer* renderer)
{
    triangleSetColorOrTexture(this, colors, texture, UV);
    triangleSetVertices(this, vertices, renderer);
    triangleCalculateEdgeVectors(this);
    triangleGetBoundingPoints(this);
    triangleCalculateBaryDeltasAndInitial(this);
}

static void triangleSetVertices(Triangle* this, Vector3d* vertices, Renderer* renderer)
{
    for (size_t i = 0; i < 3; i++)
        this->vertices[i] = NDCtoScreenSpace(renderer, vertices[i]);
}

static void triangleSetColorOrTexture(Triangle* this, Color* colors, Texture* texture, Vector2d* UV)
{
    if (colors != NULL)
    {
        this->colors = colors;
        this->type = TriangleColored;
    }
    else if (texture != NULL && UV != NULL)
    {
        this->texture = texture;
        this->UV = UV;
        this->type = TriangleTextured;
    }
    else
    {
        LOGE("triangleSetColorOrTexture() invalid call");
        abort();
    }
}

static void triangleCalculateEdgeVectors(Triangle* this)
{
    for (size_t i = 0; i < 3; i++)
        this->edgeVectors[i] = Vector3dSubtract(this->vertices[(i+1 == 3) ? 0 : i+1], this->vertices[i]);
}

static void triangleCalculateBaryDeltasAndInitial(Triangle* this)
{
    double areaX2 = Vector3dMagnitude(
        Vector3dCross(this->edgeVectors[0], this->edgeVectors[2])
    );

    Vector3d baryCoordsZero = (Vector3d) {
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
    baryCoordsZero = Vector3dDivideD(baryCoordsZero, areaX2);

    this->baryDeltaX = (Vector3d) {
        this->vertices[1].y - this->vertices[2].y,
        this->vertices[2].y - this->vertices[0].y,
        this->vertices[0].y - this->vertices[1].y
    };
    this->baryDeltaX = Vector3dDivideD(this->baryDeltaX, areaX2);

    this->baryDeltaY = (Vector3d) {
        this->vertices[2].x - this->vertices[1].x,
        this->vertices[0].x - this->vertices[2].x,
        this->vertices[1].x - this->vertices[0].x
    };
    this->baryDeltaY = Vector3dDivideD(this->baryDeltaY, areaX2);

    this->baryCoordsInitial = triangleInitializeBarycentricCoordinates(this, baryCoordsZero, this->minBoundingPoint);
}

static Color triangleInterpolateColor(Triangle* this, Vector3d barycentricCoordinates)
{
    Vector4d resColorV = {0};  // result color as Vector4d
    for (size_t i = 0; i < 3; i++)
    {
        Vector4d curColorAsVector4d = ColorToVector4d(this->colors[i]);
        Vector4d weighted = Vector4dMultiplyD(curColorAsVector4d, Vector3dIndex(barycentricCoordinates, i));
        resColorV = Vector4dAdd(resColorV, weighted);
    }
    Color color = Vector4dToColor(resColorV);
    return color;
}

static Vector2d triangleInterpolateUV(Triangle* this, Vector3d barycentricCoordinates)
{
    Vector2d resUV = {0};
    for (size_t i = 0; i < 3; i++)
    {
        Vector2d weighted = Vector2dMultiplyD(this->UV[i], Vector3dIndex(barycentricCoordinates, i));
        resUV = Vector2dAdd(resUV, weighted);
    }
    return resUV;
}

void triangleDraw(Triangle* this, Renderer* renderer)
{
    Vector3d barycentricCoordinates = this->baryCoordsInitial;

    for (double y = this->minBoundingPoint.y; y < this->maxBoundingPoint.y; y += 1.0)
    {
        Vector3d bCoordsAtBeginningOfRow = barycentricCoordinates;
        for (double x = this->minBoundingPoint.x; x < this->maxBoundingPoint.x; x += 1.0)
        {
            double sumBarycentric = 0;
            for (size_t i = 0; i < 3; i++)
            {
                size_t edgeIndex = (i+1) % 3;  // index of edge in front of current vertex
                bool isWrongHalfPlane = Vector3dIndex(barycentricCoordinates, i) < 0;
                bool doesLieOnEdgeNotFlatTopOrLeft = Vector3dIndex(barycentricCoordinates, i) == 0 && \
                                                     !triangleIsEdgeFlatTopOrLeft(this->edgeVectors[edgeIndex]);
                if (isWrongHalfPlane || doesLieOnEdgeNotFlatTopOrLeft)
                    goto nextPixel;
                sumBarycentric += Vector3dIndex(barycentricCoordinates, i);
            }

            if (roughlyEqualD(sumBarycentric, 1))
            {
                Color color;
                if (this->type == TriangleTextured)
                    color = textureGetColorAtUV(this->texture, triangleInterpolateUV(this, barycentricCoordinates));
                else
                    color = triangleInterpolateColor(this, barycentricCoordinates);
                rendererDrawPixel(renderer, (Vector2i) {x, y}, color);
            }

            nextPixel:
            barycentricCoordinates = Vector3dAdd(barycentricCoordinates, this->baryDeltaX);
        }
        barycentricCoordinates = bCoordsAtBeginningOfRow;
        barycentricCoordinates = Vector3dAdd(barycentricCoordinates, this->baryDeltaY);
    }
}

static void triangleGetBoundingPoints(Triangle* this)
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

    this->minBoundingPoint = Vector3dAdd(res_min, (Vector3d) {0.5, 0.5, 0});
    this->maxBoundingPoint = Vector3dAdd(res_max, (Vector3d) {0.5, 0.5, 0});
}

static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge)
{
    return ((edge.x > 0) && (edge.y == 0)) ||  // is top
           (edge.y < 0);                       // is left
}

static Vector3d triangleInitializeBarycentricCoordinates(Triangle* this, Vector3d baryCoordsZero, Vector3d point)
{
    return Vector3dAdd(
        Vector3dAdd(
            baryCoordsZero,
            Vector3dMultiplyD(this->baryDeltaX, point.x)
        ),
        Vector3dMultiplyD(this->baryDeltaY, point.y)
    );
}

#include "Renderer.h"
#include "Vector/Vector2.h"
#include "utils.h"
#include "Context.h"
#include "draw.h"

void drawTriangle(Face* this)
{
    assert(this->vertices->size == 3);

    if (context.faceMode == faceModeLine)
        triangleDrawEdges(this);
    else if (context.faceMode == faceModeFill)
        triangleFill(this);
    else
        LOGE("drawTriangle: unknown polygonMode!\n");
}

// See https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLineBresenham(Vector2i p1, Vector2i p2)
{
    Color color = {255, 255, 255, 255};

    if (p1.x > p2.x)
        SWAP(p1, p2);

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    bool steep = abs(dy) > abs(dx);
    if (steep)
    {
        SWAP(dx, dy);
        SWAP(p1.x, p1.y);
        SWAP(p2.x, p2.y);
    }

    if (p1.x > p2.x)
    {
        SWAP(p1, p2);
        dx = -dx;
        dy = -dy;
    }

    int D = 2*dy - dx;
    int y = p1.y;
    int yIncrement = 1;
    if (dy < 0 || (dx < 0 && steep))
    {
        yIncrement = -1;
        dy = -dy;
    }

    for (int x = p1.x; x <= p2.x; x++)
    {
        rendererDrawPixel(
            context.renderer,
            (steep) ? (Vector2i) {y, x} : (Vector2i) {x, y},
            color
        );
        if (D > 0)
        {
            y += yIncrement;
            D -= 2*dx;
        }
        D += 2*dy;
    }
}

static void triangleDrawEdges(Face* this)
{
    Vector3d SSVertices[3];  // Screen Space Vertices
    triangleConvertVerticesToScreenSpace(this, SSVertices);
    
    Vector2i points[3] = {
        Vector2dRound((Vector2d) {SSVertices[0].x, SSVertices[0].y}),
        Vector2dRound((Vector2d) {SSVertices[1].x, SSVertices[1].y}),
        Vector2dRound((Vector2d) {SSVertices[2].x, SSVertices[2].y})
    };

    drawLineBresenham(points[0], points[1]);
    drawLineBresenham(points[1], points[2]);
    drawLineBresenham(points[0], points[2]);
}

static void triangleFill(Face* this)
{
    Vector3d SSVertices[3];  // Screen Space Vertices
    Vector3d minBoundingPoint, maxBoundingPoint;
    Vector3d edgeVectors[3];
    Vector3d baryCoordsInitial, baryDeltaX, baryDeltaY;

    triangleConvertVerticesToScreenSpace(this, SSVertices);
    triangleGetBoundingPoints(SSVertices, &minBoundingPoint, &maxBoundingPoint);
    triangleCalculateEdgeVectors(this, SSVertices, edgeVectors);
    triangleCalculateBaryDeltasAndInitial(this, SSVertices, edgeVectors, &baryCoordsInitial, &baryDeltaX, &baryDeltaY, minBoundingPoint);

    Vector3d barycentricCoordinates = baryCoordsInitial;
    for (double y = minBoundingPoint.y; y < maxBoundingPoint.y; y += 1.0)
    {
        Vector3d bCoordsAtBeginningOfRow = barycentricCoordinates;
        for (double x = minBoundingPoint.x; x < maxBoundingPoint.x; x += 1.0)
        {
            double sumBarycentric = 0;
            for (size_t i = 0; i < 3; i++)
            {
                size_t edgeIndex = (i+1) % 3;  // index of edge in front of current vertex
                bool isWrongHalfPlane = Vector3dIndex(barycentricCoordinates, i) < 0;
                bool doesLieOnEdgeNotFlatTopOrLeft = Vector3dIndex(barycentricCoordinates, i) == 0 && \
                                                     !triangleIsEdgeFlatTopOrLeft(edgeVectors[edgeIndex]);
                if (isWrongHalfPlane || doesLieOnEdgeNotFlatTopOrLeft)
                    goto nextPixel;
                sumBarycentric += Vector3dIndex(barycentricCoordinates, i);
            }

            if (ROUGHLY_EQUAL(sumBarycentric, 1))
            {
                // TODO
                // if (type == TriangleTextured)
                //     color = textureGetColorAtUV(texture, triangleInterpolateUV(this, barycentricCoordinates));
                // else
                //     color = triangleInterpolateColor(this, barycentricCoordinates);
                Color color = {255, 255, 255, 255};
                rendererDrawPixel(context.renderer, (Vector2i) {x, y}, color);
            }

            nextPixel:
            barycentricCoordinates = Vector3dAdd(barycentricCoordinates, baryDeltaX);
        }
        barycentricCoordinates = bCoordsAtBeginningOfRow;
        barycentricCoordinates = Vector3dAdd(barycentricCoordinates, baryDeltaY);
    }
}

static void triangleConvertVerticesToScreenSpace(Face* this, Vector3d* SSVertices)
{
    for (size_t i = 0; i < this->vertices->size; i++)
    {
        SSVertices[i] = NDCtoScreenSpace(
            context.renderer,
            *((Vertex*) indexDynamicArray(this->vertices, i))->position
        );
    }
}

static void triangleGetBoundingPoints(Vector3d* SSVertices, Vector3d* min, Vector3d* max)
{
    Vector3d res_min = {DBL_MAX, DBL_MAX, DBL_MAX};
    Vector3d res_max = {0};

    for (size_t i = 0; i < 3; i++)
    {
        if (SSVertices[i].x < res_min.x)
            res_min.x = SSVertices[i].x;
        if (SSVertices[i].y < res_min.y)
            res_min.y = SSVertices[i].y;
        if (SSVertices[i].z < res_min.z)
            res_min.z = SSVertices[i].z;

        if (SSVertices[i].x > res_max.x)
            res_max.x = SSVertices[i].x;
        if (SSVertices[i].y > res_max.y)
            res_max.y = SSVertices[i].y;
        if (SSVertices[i].z > res_max.z)
            res_max.z = SSVertices[i].z;
    }

    // We want to loop over CENTERS of pixels
    *min = Vector3dAdd(res_min, (Vector3d) {0.5, 0.5, 0});
    *max = Vector3dAdd(res_max, (Vector3d) {0.5, 0.5, 0});
}

static void triangleCalculateEdgeVectors(Face* this, Vector3d* SSVertices, Vector3d* edgeVectors)
{
    for (size_t i = 0; i < 3; i++)
        edgeVectors[i] = Vector3dSubtract(SSVertices[(i+1 == 3) ? 0 : i+1], SSVertices[i]);
}

static void triangleCalculateBaryDeltasAndInitial(Face* this, Vector3d* SSVertices, Vector3d* edgeVectors, Vector3d* baryCoordsInitial, Vector3d* baryDeltaX, Vector3d* baryDeltaY, Vector3d minBoundingPoint)
{
    double areaX2 = Vector3dMagnitude(
        Vector3dCross(edgeVectors[0], edgeVectors[2])
    );

    Vector3d baryCoordsZero = (Vector3d) {
        Vector3dCross(
            Vector3dZeroZ(edgeVectors[1]),
            Vector3dZeroZ(Vector3dNegate(SSVertices[1]))
        ).z,
        Vector3dCross(
            Vector3dZeroZ(edgeVectors[2]),
            Vector3dZeroZ(Vector3dNegate(SSVertices[2]))
        ).z,
        Vector3dCross(
            Vector3dZeroZ(edgeVectors[0]),
            Vector3dZeroZ(Vector3dNegate(SSVertices[0]))
        ).z
    };
    baryCoordsZero = Vector3dDivideD(baryCoordsZero, areaX2);

    // see /docs/Triangle.md
    *baryDeltaX = (Vector3d) {
        SSVertices[1].y - SSVertices[2].y,
        SSVertices[2].y - SSVertices[0].y,
        SSVertices[0].y - SSVertices[1].y
    };
    *baryDeltaX = Vector3dDivideD(*baryDeltaX, areaX2);

    *baryDeltaY = (Vector3d) {
        SSVertices[2].x - SSVertices[1].x,
        SSVertices[0].x - SSVertices[2].x,
        SSVertices[1].x - SSVertices[0].x
    };
    *baryDeltaY = Vector3dDivideD(*baryDeltaY, areaX2);

    *baryCoordsInitial = triangleInitializeBarycentricCoordinates(this, baryCoordsZero, *baryDeltaX, *baryDeltaY, minBoundingPoint);
}

static bool triangleIsEdgeFlatTopOrLeft(Vector3d edge)
{
    return ((edge.x > 0) && (edge.y == 0)) ||  // is top
           (edge.y < 0);                       // is left
}

// Initialize barycentric coordinates for `point`
static Vector3d triangleInitializeBarycentricCoordinates(Face* this, Vector3d baryCoordsZero, Vector3d baryDeltaX, Vector3d baryDeltaY, Vector3d point)
{
    return Vector3dAdd(
        Vector3dAdd(
            baryCoordsZero,
            Vector3dMultiplyD(baryDeltaX, point.x)
        ),
        Vector3dMultiplyD(baryDeltaY, point.y)
    );
}

// static Color triangleInterpolateColor(Face* this, Vector3d barycentricCoordinates)
// {
//     Vector4d resColorV = {0};  // result color as Vector4d
//     for (size_t i = 0; i < 3; i++)
//     {
//         Vector4d curColorAsVector4d = ColorToVector4d(this->colors[i]);
//         Vector4d weighted = Vector4dMultiplyD(curColorAsVector4d, Vector3dIndex(barycentricCoordinates, i));
//         resColorV = Vector4dAdd(resColorV, weighted);
//     }
//     Color color = Vector4dToColor(resColorV);
//     return color;
// }
// 
// static Vector2d triangleInterpolateUV(Face* this, Vector3d barycentricCoordinates)
// {
//     Vector2d resUV = {0};
//     for (size_t i = 0; i < 3; i++)
//     {
//         Vector2d weighted = Vector2dMultiplyD(this->UV[i], Vector3dIndex(barycentricCoordinates, i));
//         resUV = Vector2dAdd(resUV, weighted);
//     }
//     return resUV;
// }


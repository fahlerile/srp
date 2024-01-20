#include <stdint.h>
#include <assert.h>
#include "Vector/Vector.h"
#include "Context.h"
#include "utils.h"
#include "draw.h"

void drawTriangle(void* vsOutput)
{
    Vector3d NDCPositions[3];
    for (size_t i = 0; i < 3; i++)
    {
        NDCPositions[i] = *(Vector3d*) (vertexShaderOutputGetAttributePointerByIndex(
            (uint8_t*) vsOutput + (context.vertexShaderOutputInformation.nBytes * i),
            0
        ));
    }

    Vector3d SSPositions[3];
    Vector3d edgeVectors[3];
    Vector3d minBoundingPoint, maxBoundingPoint;
    Vector3d barycentricCoordinates, barycentricDeltaX, barycentricDeltaY;

    transformPositionsToScreenSpace(NDCPositions, 3, SSPositions);
    getBoundingPoints(SSPositions, 3, &minBoundingPoint, &maxBoundingPoint);
    // We want to loop over centers of pixels, not over their edge
    minBoundingPoint = Vector3dAdd(minBoundingPoint, (Vector3d) {0.5, 0.5, 0.});
    maxBoundingPoint = Vector3dAdd(maxBoundingPoint, (Vector3d) {0.5, 0.5, 0.});

    calculateEdgeVectors(SSPositions, 3, edgeVectors);

    triangleCalculateBarycenticDeltas(
        SSPositions, edgeVectors, &barycentricDeltaX, &barycentricDeltaY,
        &barycentricCoordinates
    );
    barycentricCoordinates = triangleCalculateBarycentricCoordinatesForPoint(
        barycentricCoordinates, barycentricDeltaX, barycentricDeltaY,
        minBoundingPoint
    );

    for (double y = minBoundingPoint.y; y < maxBoundingPoint.y; y += 1.)
    {
        Vector3d barycentricCoordinatesAtBeginningOfTheRow = \
            barycentricCoordinates;
        for (double x = minBoundingPoint.x; x < maxBoundingPoint.x; x += 1.)
        {
            double barycentricCoordinatesSum = 0.;
            for (size_t i = 0; i < 3; i++)
            {
                // index of edge in front of current vertex
                size_t edgeIndex = (i+1) % 3;
                bool isWrongHalfPlane = Vector3dIndex(barycentricCoordinates, i) < 0;
                bool doesLieOnEdgeNotFlatTopOrLeft = \
                    Vector3dIndex(barycentricCoordinates, i) == 0 && \
                    !triangleIsEdgeFlatTopOrLeft(edgeVectors[edgeIndex]);
                if (isWrongHalfPlane || doesLieOnEdgeNotFlatTopOrLeft)
                    goto nextPixel;
                barycentricCoordinatesSum += Vector3dIndex(barycentricCoordinates, i);
            }

            if (ROUGHLY_EQUAL(barycentricCoordinatesSum, 1.))
            {
                uint8_t interpolatedVSOutput[context.vertexShaderOutputInformation.nBytes];
                interpolateVertexShaderOutputInTriangle(
                    vsOutput, barycentricCoordinates, &interpolatedVSOutput
                );

                Color color = {0};
                context.fragmentShader(interpolatedVSOutput, &color);
                rendererDrawPixel(context.renderer, (Vector2i) {x, y}, color);
            }
            
nextPixel:
            barycentricCoordinates = Vector3dAdd(
                barycentricCoordinates, barycentricDeltaX
            );
        }
        barycentricCoordinates = barycentricCoordinatesAtBeginningOfTheRow;
        barycentricCoordinates = Vector3dAdd(
            barycentricCoordinates, barycentricDeltaY
        );
    }
}

static void transformPositionsToScreenSpace(
    Vector3d* NDCPositions, size_t n, Vector3d* SSPositions
)
{
    for (size_t i = 0; i < n; i++)
    {
        SSPositions[i] = \
            NDCToScreenSpace(context.renderer, NDCPositions[i]);
    }
}

static void getBoundingPoints(
    Vector3d* SSPositions, size_t n, 
    Vector3d* minBoundingPoint, Vector3d* maxBoundingPoint
)
{
    Vector3d min = {DBL_MAX, DBL_MAX, DBL_MAX};
    Vector3d max = {0};

    for (size_t i = 0; i < n; i++)
    {
        if (SSPositions[i].x < min.x)
            min.x = SSPositions[i].x;
        if (SSPositions[i].y < min.y)
            min.y = SSPositions[i].y;
        if (SSPositions[i].z < min.z)
            min.z = SSPositions[i].z;

        if (SSPositions[i].x > max.x)
            max.x = SSPositions[i].x;
        if (SSPositions[i].y > max.y)
            max.y = SSPositions[i].y;
        if (SSPositions[i].z > max.z)
            max.z = SSPositions[i].z;
    }

    *minBoundingPoint = min;
    *maxBoundingPoint = max;
}

static void calculateEdgeVectors(
    Vector3d* SSPositions, size_t n, Vector3d* edgeVectors
)
{
    for (size_t i = 0; i < n; i++)
    {
        edgeVectors[i] = Vector3dSubtract(
            SSPositions[(i+1 == n) ? 0 : i+1], SSPositions[i]
        );
    }
}

static void triangleCalculateBarycenticDeltas(
    Vector3d* SSPositions, Vector3d* edgeVectors,
    Vector3d* barycentricDeltaX, Vector3d* barycentricDeltaY,
    Vector3d* barycentricCoordinatesZero
)
{
    double areaX2 = Vector3dMagnitude(
        Vector3dCross(edgeVectors[0], edgeVectors[2])
    );

    *barycentricCoordinatesZero = (Vector3d) {
        Vector3dCross(
            Vector3dZeroZ(edgeVectors[1]),
            Vector3dZeroZ(Vector3dNegate(SSPositions[1]))
        ).z / areaX2,
        Vector3dCross(
            Vector3dZeroZ(edgeVectors[2]),
            Vector3dZeroZ(Vector3dNegate(SSPositions[2]))
        ).z / areaX2,
        Vector3dCross(
            Vector3dZeroZ(edgeVectors[0]),
            Vector3dZeroZ(Vector3dNegate(SSPositions[0]))
        ).z / areaX2
    };

    // see /docs/Triangle.md
    *barycentricDeltaX = (Vector3d) {
        SSPositions[1].y - SSPositions[2].y,
        SSPositions[2].y - SSPositions[0].y,
        SSPositions[0].y - SSPositions[1].y
    };
    *barycentricDeltaX = Vector3dDivideD(*barycentricDeltaX, areaX2);

    *barycentricDeltaY = (Vector3d) {
        SSPositions[2].x - SSPositions[1].x,
        SSPositions[0].x - SSPositions[2].x,
        SSPositions[1].x - SSPositions[0].x
    };
    *barycentricDeltaY = Vector3dDivideD(*barycentricDeltaY, areaX2);
}

static Vector3d triangleCalculateBarycentricCoordinatesForPoint(
    Vector3d barycentricCoordinatesZero, Vector3d barycentricDeltaX,
    Vector3d barycentricDeltaY, Vector3d point
)
{
    return Vector3dAdd(
        Vector3dAdd(
            barycentricCoordinatesZero,
            Vector3dMultiplyD(barycentricDeltaX, point.x)
        ),
        Vector3dMultiplyD(barycentricDeltaY, point.y)
    );
}

static bool triangleIsEdgeFlatTopOrLeft(Vector3d edgeVector)
{
    // is flat top OR is left
    return ( (edgeVector.x > 0) && (edgeVector.y == 0) ) || (edgeVector.y < 0);
}


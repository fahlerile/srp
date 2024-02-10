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
    double barycentricCoordinates[3] = {0};
    double barycentricDeltaX[3] = {0};
    double barycentricDeltaY[3] = {0};

    transformPositionsToScreenSpace(NDCPositions, 3, SSPositions);
    getBoundingPoints(SSPositions, 3, &minBoundingPoint, &maxBoundingPoint);
    calculateEdgeVectors(SSPositions, 3, edgeVectors);
    triangleCalculateBarycenticDeltasAndBarycentricCoordinatesForPoint(
        SSPositions, edgeVectors, barycentricDeltaX, barycentricDeltaY,
        barycentricCoordinates, &minBoundingPoint
    );

    double barycentricCoordinatesAtBeginningOfTheRow[3];
    bool isEdgeNotFlatTopOrLeft[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        barycentricCoordinatesAtBeginningOfTheRow[i] = barycentricCoordinates[i];
        isEdgeNotFlatTopOrLeft[i] = !triangleIsEdgeFlatTopOrLeft(edgeVectors[i]);
    }

    for (size_t y = minBoundingPoint.y; y < maxBoundingPoint.y; y += 1)
    {
        for (size_t x = minBoundingPoint.x; x < maxBoundingPoint.x; x += 1)
        {
            // Rasterization rules
            // If current point lies on the edge that is not flat top or left, do not draw the point
            for (uint8_t i = 0; i < 3; i++)
            {
                if (barycentricCoordinates[i] == 0 && isEdgeNotFlatTopOrLeft[i])
                    goto nextPixel;
            }

            if (barycentricCoordinates[0] >= 0 && barycentricCoordinates[1] >= 0 && \
                barycentricCoordinates[2] >= 0)
            { 
                // uint8_t interpolatedVSOutput[context.vertexShaderOutputInformation.nBytes];
                // interpolateVertexShaderOutputInTriangle(
                //     vsOutput, barycentricCoordinates, &interpolatedVSOutput
                // );
                Color color = {255, 255, 255, 255};
                // context.fragmentShader(interpolatedVSOutput, &color);
                rendererDrawPixel(context.renderer, (Vector2i) {x, y}, color);
            }

nextPixel:
            for (uint8_t i = 0; i < 3; i++)
                barycentricCoordinates[i] += barycentricDeltaX[i];
        }
        for (uint8_t i = 0; i < 3; i++)
        {
            barycentricCoordinatesAtBeginningOfTheRow[i] += barycentricDeltaY[i];;
            barycentricCoordinates[i] = barycentricCoordinatesAtBeginningOfTheRow[i];
        }
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

static void triangleCalculateBarycenticDeltasAndBarycentricCoordinatesForPoint(
    Vector3d* SSPositions, Vector3d* edgeVectors,
    double* barycentricDeltaX, double* barycentricDeltaY,
    double* barycentricCoordinates, Vector3d* point
)
{
    double areaX2 = Vector3dMagnitude(Vector3dCross(edgeVectors[0], edgeVectors[2]));
    barycentricCoordinates[0] = (
        edgeVectors[1].x * (point->y - SSPositions[1].y) - \
        edgeVectors[1].y * (point->x - SSPositions[1].x)
    ) / areaX2;
    barycentricCoordinates[1] = (
        edgeVectors[2].x * (point->y - SSPositions[2].y) - \
        edgeVectors[2].y * (point->x - SSPositions[2].x)
    ) / areaX2;
    barycentricCoordinates[2] = (
        edgeVectors[0].x * (point->y - SSPositions[0].y) - \
        edgeVectors[0].y * (point->x - SSPositions[0].x)
    ) / areaX2;

    // see /docs/Triangle.md
    barycentricDeltaX[0] = (SSPositions[1].y - SSPositions[2].y) / areaX2;
    barycentricDeltaX[1] = (SSPositions[2].y - SSPositions[0].y) / areaX2;
    barycentricDeltaX[2] = (SSPositions[0].y - SSPositions[1].y) / areaX2;

    barycentricDeltaY[0] = (SSPositions[2].x - SSPositions[1].x) / areaX2;
    barycentricDeltaY[1] = (SSPositions[0].x - SSPositions[2].x) / areaX2;
    barycentricDeltaY[2] = (SSPositions[1].x - SSPositions[0].x) / areaX2;
}

static bool triangleIsEdgeFlatTopOrLeft(Vector3d edgeVector)
{
    // is flat top OR is left
    return ( (edgeVector.x > 0) && (edgeVector.y == 0) ) || (edgeVector.y < 0);
}


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
    Vector2d edgeVectors[3];
    Vector2d minBoundingPoint, maxBoundingPoint;
    double barycentricCoordinates[3];
    double barycentricDeltaX[3];
    double barycentricDeltaY[3];

    bool positiveOnPlusY[3];
    bool edgeSlopePositiveOrZero[3];

    transformPositionsToScreenSpace(NDCPositions, 3, SSPositions);
    triangleGetBoundingPoints(SSPositions, &minBoundingPoint, &maxBoundingPoint);
    triangleCalculateEdgeVectors(SSPositions, edgeVectors);
    triangleCalculateBarycenticDeltasAndBarycentricCoordinatesForPoint(
        SSPositions, edgeVectors, barycentricDeltaX, barycentricDeltaY,
        barycentricCoordinates, &minBoundingPoint
    );
    triangleCalculatePositiveOnPlusYAndSlopeSigns(
        SSPositions, edgeVectors, positiveOnPlusY, edgeSlopePositiveOrZero
    );

    Vector2d boundingBoxDimensions = {
        maxBoundingPoint.x - minBoundingPoint.x,
        maxBoundingPoint.y - minBoundingPoint.y
    };
    Vector2i tileDimensions = {32, 32};
    Vector2i nTilesInBox = {
        ceil(boundingBoxDimensions.x / tileDimensions.x),
        ceil(boundingBoxDimensions.y / tileDimensions.y)
    };

    double barycentricDeltaTileX[3] = {
        barycentricDeltaX[0] * tileDimensions.x,
        barycentricDeltaX[1] * tileDimensions.x,
        barycentricDeltaX[2] * tileDimensions.x
    };
    double barycentricDeltaTileY[3] = {
        barycentricDeltaY[0] * tileDimensions.y,
        barycentricDeltaY[1] * tileDimensions.y,
        barycentricDeltaY[2] * tileDimensions.y
    };

    double barycentricCoordinatesAtBeginningOfTheRow[3];
    for (uint8_t i = 0; i < 3; i++)
        barycentricCoordinatesAtBeginningOfTheRow[i] = barycentricCoordinates[i];

    for (int yTile = 0; yTile < nTilesInBox.y; yTile++)
    {
        for (int xTile = 0; xTile < nTilesInBox.x; xTile++)
        {
            Vector2d startPoint = {
                minBoundingPoint.x + xTile * tileDimensions.x,
                minBoundingPoint.y + yTile * tileDimensions.y
            };
            Vector2d endPoint = {
                startPoint.x + tileDimensions.x,
                startPoint.y + tileDimensions.y
            };

            bool rejected[3] = {0};
            bool accepted[3] = {0};
            for (uint8_t iEdge = 0; iEdge < 3; iEdge++)
            {
                // barycentric coordinates for acception and rejection points
                double barycentricAcception, barycentricRejection;
                uint8_t iBarycentric = (iEdge + 2) % 3;
                if (positiveOnPlusY[iEdge])
                {
                    if (edgeSlopePositiveOrZero[iEdge])
                    {
                        barycentricAcception = \
                            barycentricCoordinates[iBarycentric] + \
                            barycentricDeltaTileX[iBarycentric];
                        barycentricRejection = \
                            barycentricCoordinates[iBarycentric] + \
                            barycentricDeltaTileY[iBarycentric];
                    }
                    else  // negative slope
                    {
                        barycentricAcception = \
                            barycentricCoordinates[iBarycentric];
                        barycentricRejection = \
                            barycentricCoordinates[iBarycentric] + \
                            barycentricDeltaTileX[iBarycentric] + \
                            barycentricDeltaTileY[iBarycentric];
                    }
                }
                else  // negative on +y
                {
                    if (edgeSlopePositiveOrZero[iEdge])
                    {
                        barycentricAcception = \
                            barycentricCoordinates[iBarycentric] + \
                            barycentricDeltaTileY[iBarycentric];
                        barycentricRejection = \
                            barycentricCoordinates[iBarycentric] + \
                            barycentricDeltaTileX[iBarycentric];
                    }
                    else  // negative slope
                    {
                        barycentricAcception = \
                            barycentricCoordinates[iBarycentric] + \
                            barycentricDeltaTileX[iBarycentric] + \
                            barycentricDeltaTileY[iBarycentric];
                        barycentricRejection = \
                            barycentricCoordinates[iBarycentric];
                    }
                }

                if (barycentricAcception > 0)
                    accepted[iEdge] = true;
                else if (barycentricRejection <= 0)
                    rejected[iEdge] = true;
            }

            double tempBaryCoords[3];
            memcpy(tempBaryCoords, barycentricCoordinates, sizeof(double) * 3);

            if (accepted[0] && accepted[1] && accepted[2])
                loopOverTileAndFillNoCheck(
                    startPoint, endPoint, tempBaryCoords,
                    barycentricDeltaX, barycentricDeltaY
                );
            else if (!(rejected[0] || rejected[1] || rejected[2]))
                loopOverTileAndFill(
                    startPoint, endPoint, edgeVectors, tempBaryCoords,
                    barycentricDeltaX, barycentricDeltaY
                );

            for (uint8_t i = 0; i < 3; i++)
                barycentricCoordinates[i] += barycentricDeltaTileX[i];
        }

        for (uint8_t i = 0; i < 3; i++)
        {
            barycentricCoordinatesAtBeginningOfTheRow[i] += barycentricDeltaTileY[i];
            barycentricCoordinates[i] = barycentricCoordinatesAtBeginningOfTheRow[i];
        }
    }
}

static void loopOverTileAndFillNoCheck(
    const Vector2d startPoint, const Vector2d endPoint,
    double* barycentricCoordinates, const double* barycentricDeltaX,
    const double* barycentricDeltaY
)
{
    double barycentricCoordinatesAtBeginningOfTheRow[3];
    for (uint8_t i = 0; i < 3; i++)
        barycentricCoordinatesAtBeginningOfTheRow[i] = barycentricCoordinates[i];

    for (size_t y = startPoint.y; y < endPoint.y; y += 1)
    {
        for (size_t x = startPoint.x; x < endPoint.x; x += 1)
        {
            // uint8_t interpolatedVSOutput[context.vertexShaderOutputInformation.nBytes];
            // interpolateVertexShaderOutputInTriangle(
            //     vsOutput, barycentricCoordinates, &interpolatedVSOutput
            // );
            Color color = {255, 255, 255, 255};
            // context.fragmentShader(interpolatedVSOutput, &color);
            rendererDrawPixel(context.renderer, (Vector2i) {x, y}, color);

            for (uint8_t i = 0; i < 3; i++)
                barycentricCoordinates[i] += barycentricDeltaX[i];
        }
        for (uint8_t i = 0; i < 3; i++)
        {
            barycentricCoordinatesAtBeginningOfTheRow[i] += barycentricDeltaY[i];
            barycentricCoordinates[i] = barycentricCoordinatesAtBeginningOfTheRow[i];
        }
    }
}

static void loopOverTileAndFill(
    const Vector2d startPoint, const Vector2d endPoint, const Vector2d* edgeVectors,
    double* barycentricCoordinates, const double* barycentricDeltaX, 
    const double* barycentricDeltaY
)
{
    double barycentricCoordinatesAtBeginningOfTheRow[3];
    bool isEdgeNotFlatTopOrLeft[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        barycentricCoordinatesAtBeginningOfTheRow[i] = barycentricCoordinates[i];
        isEdgeNotFlatTopOrLeft[i] = !triangleIsEdgeFlatTopOrLeft(edgeVectors[i]);
    }

    for (size_t y = startPoint.y; y < endPoint.y; y += 1)
    {
        for (size_t x = startPoint.x; x < endPoint.x; x += 1)
        {
            // Rasterization rules
            // If current point lies on the edge that is not flat top or left, do not draw the point
            for (uint8_t i = 0; i < 3; i++)
            {
                if (barycentricCoordinates[i] == 0 && isEdgeNotFlatTopOrLeft[i])
                    goto nextPixel;
            }

            if (barycentricCoordinates[0] >= 0 && \
                barycentricCoordinates[1] >= 0 && \
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
            barycentricCoordinatesAtBeginningOfTheRow[i] += barycentricDeltaY[i];
            barycentricCoordinates[i] = barycentricCoordinatesAtBeginningOfTheRow[i];
        }
    }
}

static double signedAreaParallelogram(Vector2d a, Vector2d b)
{
    return a.y * b.x - a.x * b.y;
}

static void triangleCalculatePositiveOnPlusYAndSlopeSigns(
    Vector3d* SSPositions, Vector2d* edgeVectors,
    bool* positiveOnPlusY, bool* edgeSlopePositiveOrZero
)
{
    for (uint8_t iEdge = 0; iEdge < 3; iEdge++)
    {
        uint8_t iFirstVertexOnEdge = iEdge;
        uint8_t iSecondVertexOnEdge = (iFirstVertexOnEdge + 1) % 3;
        uint8_t iThirdVertex = (iFirstVertexOnEdge + 2) % 3;

        double dx = SSPositions[iFirstVertexOnEdge].x - SSPositions[iSecondVertexOnEdge].x;
        double dy = SSPositions[iFirstVertexOnEdge].y - SSPositions[iSecondVertexOnEdge].y;
        double F = \
            SSPositions[iThirdVertex].x * dy - SSPositions[iThirdVertex].y * dx + \
            SSPositions[iFirstVertexOnEdge].y * dx - SSPositions[iFirstVertexOnEdge].x * dy;
        assert(F != 0 && "The triangle is degenerate");

        if (dx > 0)
        {
            if (F < 0)
                positiveOnPlusY[iEdge] = true;
            else
                positiveOnPlusY[iEdge] = false;
        }
        else
        {
            if (F < 0)
                positiveOnPlusY[iEdge] = false;
            else
                positiveOnPlusY[iEdge] = true;
        }

        double slope = edgeVectors[iEdge].y / edgeVectors[iEdge].x;
        edgeSlopePositiveOrZero[iEdge] = slope >= 0;
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

static void triangleGetBoundingPoints(
    Vector3d* SSPositions, Vector2d* minBoundingPoint, Vector2d* maxBoundingPoint
)
{
    Vector2d min = {DBL_MAX, DBL_MAX};
    Vector2d max = {0};

    for (uint8_t i = 0; i < 3; i++)
    {
        if (SSPositions[i].x < min.x)
            min.x = SSPositions[i].x;
        if (SSPositions[i].y < min.y)
            min.y = SSPositions[i].y;

        if (SSPositions[i].x > max.x)
            max.x = SSPositions[i].x;
        if (SSPositions[i].y > max.y)
            max.y = SSPositions[i].y;
    }

    *minBoundingPoint = min;
    *maxBoundingPoint = max;
}

static void triangleCalculateEdgeVectors(Vector3d* SSPositions, Vector2d* edgeVectors)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        uint8_t iNextVertex = (i + 1) % 3;
        edgeVectors[i] = (Vector2d) {
            SSPositions[iNextVertex].x - SSPositions[i].x,
            SSPositions[iNextVertex].y - SSPositions[i].y
        };
    }
}

static void triangleCalculateBarycenticDeltasAndBarycentricCoordinatesForPoint(
    Vector3d* SSPositions, Vector2d* edgeVectors,
    double* barycentricDeltaX, double* barycentricDeltaY,
    double* barycentricCoordinates, Vector2d* point
)
{
    double areaX2 = fabs(signedAreaParallelogram(edgeVectors[0], edgeVectors[2]));

    Vector2d AP = {
        point->x - SSPositions[0].x,
        point->y - SSPositions[0].y
    };
    Vector2d BP = {
        point->x - SSPositions[1].x,
        point->y - SSPositions[1].y
    };
    Vector2d CP = {
        point->x - SSPositions[2].x,
        point->y - SSPositions[2].y
    };

    barycentricCoordinates[0] = signedAreaParallelogram(BP, edgeVectors[1]) / areaX2;
    barycentricCoordinates[1] = signedAreaParallelogram(CP, edgeVectors[2]) / areaX2;
    barycentricCoordinates[2] = signedAreaParallelogram(AP, edgeVectors[0]) / areaX2;

    // see /docs/Triangle.md
    barycentricDeltaX[0] = -edgeVectors[1].y / areaX2;
    barycentricDeltaX[1] = -edgeVectors[2].y / areaX2;
    barycentricDeltaX[2] = -edgeVectors[0].y / areaX2;

    barycentricDeltaY[0] = edgeVectors[1].x / areaX2;
    barycentricDeltaY[1] = -edgeVectors[2].x / areaX2;
    barycentricDeltaY[2] = edgeVectors[0].x / areaX2;
}

static bool triangleIsEdgeFlatTopOrLeft(Vector2d edgeVector)
{
    // is flat top OR is left
    return ( (edgeVector.x > 0) && (edgeVector.y == 0) ) || (edgeVector.y < 0);
}


#pragma once
#include <stdbool.h>
#include "Vector/Vector.h"
#include "Shaders.h"

typedef struct
{
    double barycentricCoordinates[3];
    double barycentricCoordinatesCopy[3];
    double barycentricDeltaX[3];
    double barycentricDeltaY[3];
    double barycentricTileDeltaX[3];
    double barycentricTileDeltaY[3];

    bool posOnPlusY[3];
    bool slopeSign[3];
    bool isEdgeNotFlatTopOrLeft[3];

    Vector2i tileDimensions;
    Vector2i nTilesInBox;
    Vector2d minBP;
} triangleData;

void drawTriangle(const void* restrict gsOutput, const ShaderProgram* restrict sp);

static void drawTrianglePreparation(
    const void* restrict gsOutput, const ShaderProgram* restrict sp,
    triangleData* restrict data
);
static void drawTriangleRasterization(
    const void* restrict gsOutput, triangleData* restrict data,
    const ShaderProgram* restrict sp
);

static void NDCToScreenSpaceArray(
    const Vector3d* restrict NDCPositions, Vector3d* restrict SSPositions,
    const size_t n
);
static void getBoundingBoxArray(
    const Vector3d* restrict SSPositions, Vector2d* restrict min,
    Vector2d* restrict max, Vector2i* restrict BBDimensions, const size_t n
);
static void calculateEdgeVectors(
    const Vector3d* restrict SSPositions, Vector3d* restrict edgeVectors,
    const size_t n
);
static void calculatePositiveOnPlusYAndSlopeSignsArray(
    const Vector3d* restrict SSPositions, const Vector3d* restrict edgeVectors,
    bool* restrict posOnPlusY, bool* restrict slopeSigns, const size_t n
);
static double signedAreaParallelogram(
    const Vector3d* restrict a, const Vector3d* restrict b
);
static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
    const Vector3d* restrict SSPositions, const Vector3d* restrict edgeVectors,
    const Vector2d point, double* restrict barycentricCoordinates,
    double* restrict barycentricDeltaX, double* restrict barycentricDeltaY
);
static void calculateTileDimensionsAndNTilesInBoundingBox(
    const Vector2i BBDimensions, Vector2i* restrict tileDimensions,
    Vector2i* restrict nTiles
);
static void calculateBarycentricTileDeltas(
    const Vector2i tileDimensions, const double* restrict barycentricDeltaX,
    const double* restrict barycentricDeltaY, double* restrict barycentricTileDeltaX,
    double* restrict barycentricTileDeltaY
);
static bool triangleIsEdgeFlatTopOrLeft(const Vector3d* restrict edgeVector);

static void triangleRejectionAcceptionTests(
    const triangleData* restrict data, bool* restrict rejected,
    bool* restrict accepted
);
static void triangleLoopOverTileAndFill(
    const bool check, const Vector2d* restrict startPoint,
    const Vector2d* restrict endPoint, triangleData* restrict data,
    const ShaderProgram* restrict sp, const void* restrict gsOutput
);

static void triangleInterpolateGsOutput(
    const void* gsOutput, const double barycentricCoordinates[3],
    const ShaderProgram* restrict sp, void* pInterpolatedBuffer
);


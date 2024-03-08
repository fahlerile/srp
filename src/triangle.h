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

void drawTriangle(void* gsOutput, ShaderProgram* sp);

static void drawTrianglePreparation(
    void* gsOutput, ShaderProgram* sp, triangleData* returnData
);
static void drawTriangleRasterization(triangleData* data);

static void NDCToScreenSpaceArray(
    Vector3d* NDCPositions, Vector3d* SSPositions, size_t n
);
static void getBoundingBoxArray(
    Vector3d* SSPositions, Vector2d* min, Vector2d* max,
    Vector2i* BBDimensions, size_t n
);
static void calculateEdgeVectors(
    Vector3d* SSPositions, Vector3d* edgeVectors, size_t n
);
static void calculatePositiveOnPlusYAndSlopeSignsArray(
    Vector3d* SSPositions, Vector3d* edgeVectors,
    bool* posOnPlusY, bool* slopeSigns, size_t n
);
static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
    Vector3d* SSPositions, Vector3d* edgeVectors, Vector2d point,
    double* barycentricCoordinates, double* barycentricDeltaX, 
    double* barycentricDeltaY
);
static void calculateTileDimensionsAndNTilesInBoundingBox(
    Vector2i BBDimensions, Vector2i* tileDimensions, Vector2i* NTiles
);
static void calculateBarycentricTileDeltas(
    Vector2i tileDimensions, double* barycentricDeltaX, double* barycentricDeltaY,
    double* barycentricTileDeltaX, double* barycentricTileDeltaY
);
static bool triangleIsEdgeFlatTopOrLeft(Vector3d edgeVector);

static void triangleRejectionAcceptionTests(
    triangleData* data, bool* rejected, bool* accepted
);
static void triangleLoopOverTileAndFill(
    bool check, Vector2d startPoint, Vector2d endPoint, triangleData* data
);



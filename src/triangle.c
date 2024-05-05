#include <assert.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include "Context.h"
#include "NDC.h"
#include "triangle.h"
#include "Shaders.h"
#include "log.h"

void drawTriangle(const GSOutput* restrict gsOutput, const ShaderProgram* restrict sp)
{
    triangleData data;
    drawTrianglePreparation(gsOutput, sp, &data);
    drawTriangleRasterization(gsOutput, &data, sp);
}

static void drawTrianglePreparation(
    const GSOutput* restrict gsOutput, const ShaderProgram* restrict sp,
    triangleData* restrict data
)
{
    VertexAttribute positionAttribute = sp->geometryShader.outputAttributes[
        sp->geometryShader.indexOfOutputPositionAttribute
    ];

    // TODO add this assert to docs
    assert(positionAttribute.nItems == 3);
    assert(positionAttribute.type == TYPE_DOUBLE);

    size_t positionOffsetBytes = positionAttribute.offsetBytes;
    Vector3d NDCPositions[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        void* pVertex = (uint8_t*) gsOutput + (i * sp->geometryShader.nBytesPerOutputVertex);
        NDCPositions[i] = \
            *(Vector3d*) ((uint8_t*) pVertex + positionOffsetBytes);
    }

    Vector3d SSPositions[3], edgeVectors[3];
    Vector2d maxBP;
    Vector2i BBDimensions;

    NDCToScreenSpaceArray(NDCPositions, SSPositions, 3);
    getBoundingBoxArray(SSPositions, &data->minBP, &maxBP, &BBDimensions, 3);
    calculateEdgeVectors(SSPositions, edgeVectors, 3);
    calculatePositiveOnPlusYAndSlopeSignsArray(
        SSPositions, edgeVectors, 
        data->posOnPlusY, data->slopeSign, 3
    );

    calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
        SSPositions, edgeVectors, data->minBP, data->barycentricCoordinates, 
        data->barycentricDeltaX, data->barycentricDeltaY
    );

    calculateTileDimensionsAndNTilesInBoundingBox(
        BBDimensions, &data->tileDimensions, &data->nTilesInBox
    );
    calculateBarycentricTileDeltas(
        data->tileDimensions, data->barycentricDeltaX, data->barycentricDeltaY,
        data->barycentricTileDeltaX, data->barycentricTileDeltaY
    );

    for (uint8_t i = 0; i < 3; i++)
        data->isEdgeNotFlatTopOrLeft[i] = \
            !triangleIsEdgeFlatTopOrLeft(&edgeVectors[i]);
}

static void drawTriangleRasterization(
    const GSOutput* restrict gsOutput, triangleData* restrict data,
    const ShaderProgram* restrict sp
)
{
    double barycentricCoordinatesAtBeginningOfTheRow[3];
    memcpy(
        barycentricCoordinatesAtBeginningOfTheRow,
        data->barycentricCoordinates,
        sizeof(double) * 3
    );

    for (int yTile = 0; yTile < data->nTilesInBox.y; yTile++)
    {
        for (int xTile = 0; xTile < data->nTilesInBox.x; xTile++)
        {
            Vector2d startPoint = {
                data->minBP.x + xTile * data->tileDimensions.x,
                data->minBP.y + yTile * data->tileDimensions.y
            };
            Vector2d endPoint = {
                startPoint.x + data->tileDimensions.x,
                startPoint.y + data->tileDimensions.y
            };

            bool rejected[3], accepted[3];
            triangleRejectionAcceptionTests(data, rejected, accepted);

            bool fullyAccepted = accepted[0] && accepted[1] && accepted[2];
            bool fullyRejected = rejected[0] || rejected[1] || rejected[2];
            bool check = !fullyAccepted;

            if (fullyAccepted || !fullyRejected)
                triangleLoopOverTileAndFill(
                    check, &startPoint, &endPoint, data, sp, gsOutput
                );

            for (uint8_t i = 0; i < 3; i++)
                data->barycentricCoordinates[i] += data->barycentricTileDeltaX[i];
        }

        for (uint8_t i = 0; i < 3; i++)
            barycentricCoordinatesAtBeginningOfTheRow[i] += \
                data->barycentricTileDeltaY[i];

        memcpy(
            data->barycentricCoordinates,
            barycentricCoordinatesAtBeginningOfTheRow,
            sizeof(double) * 3
        );
    }
}

static void NDCToScreenSpaceArray(
    const Vector3d* restrict NDCPositions, Vector3d* restrict SSPositions,
    const size_t n
)
{
    for (size_t i = 0; i < n; i++)
        SSPositions[i] = NDCToScreenSpace(context.renderer, NDCPositions[i]);
}

static void getBoundingBoxArray(
    const Vector3d* restrict SSPositions, Vector2d* restrict min,
    Vector2d* restrict max, Vector2i* restrict BBDimensions, const size_t n
)
{
    double xmin = SSPositions[0].x, xmax = SSPositions[0].x;
    double ymin = SSPositions[0].y, ymax = SSPositions[0].y;

    for (size_t i = 0; i < n; i++)
    {
        double x = SSPositions[i].x;
        double y = SSPositions[i].y;

        if (x < xmin)
            xmin = x;
        else if (x > xmax)
            xmax = x;
        if (y < ymin)
            ymin = y;
        else if (y > ymax)
            ymax = y;
    }

    *min = (Vector2d) {xmin, ymin};
    *max = (Vector2d) {xmax, ymax};
    *BBDimensions = (Vector2i) {
        xmax - xmin,
        ymax - ymin,
    };
}

static void calculateEdgeVectors(
    const Vector3d* restrict SSPositions, Vector3d* restrict edgeVectors,
    const size_t n
)
{
    for (size_t i = 0; i < n; i++)
    {
        edgeVectors[i] = Vector3dSubtract(
            SSPositions[(i+1) % n], SSPositions[i]
        );
    }
}

static void calculatePositiveOnPlusYAndSlopeSignsArray(
    const Vector3d* restrict SSPositions, const Vector3d* restrict edgeVectors,
    bool* restrict posOnPlusY, bool* restrict slopeSigns, const size_t n
)
{
    for (uint8_t iEdge = 0; iEdge < n; iEdge++)
    {
        uint8_t iFirstVertexOnEdge = iEdge;
        uint8_t iSecondVertexOnEdge = (iFirstVertexOnEdge + 1) % n;
        uint8_t iThirdVertex = (iFirstVertexOnEdge + 2) % n;

        double dx = SSPositions[iFirstVertexOnEdge].x - \
                    SSPositions[iSecondVertexOnEdge].x;
        double dy = SSPositions[iFirstVertexOnEdge].y - \
                    SSPositions[iSecondVertexOnEdge].y;
        double F = \
            SSPositions[iThirdVertex].x * dy - SSPositions[iThirdVertex].y * dx + \
            SSPositions[iFirstVertexOnEdge].y * dx - SSPositions[iFirstVertexOnEdge].x * dy;
        assert(F != 0 && "The triangle is degenerate");

        if (dx > 0)
        {
            if (F < 0)
                posOnPlusY[iEdge] = true;
            else
                posOnPlusY[iEdge] = false;
        }
        else
        {
            if (F < 0)
                posOnPlusY[iEdge] = false;
            else
                posOnPlusY[iEdge] = true;
        }

        double slope = edgeVectors[iEdge].y / edgeVectors[iEdge].x;
        slopeSigns[iEdge] = slope >= 0;
    }
}

static double signedAreaParallelogram(
    const Vector3d* restrict a, const Vector3d* restrict b
)
{
    return a->y * b->x - a->x * b->y;
}

static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
    const Vector3d* restrict SSPositions, const Vector3d* restrict edgeVectors,
    const Vector2d point, double* restrict barycentricCoordinates,
    double* restrict barycentricDeltaX, double* restrict barycentricDeltaY
)
{
    double areaX2 = fabs(signedAreaParallelogram(&edgeVectors[0], &edgeVectors[2]));

    Vector3d AP = {
        point.x - SSPositions[0].x,
        point.y - SSPositions[0].y,
        0
    };
    Vector3d BP = {
        point.x - SSPositions[1].x,
        point.y - SSPositions[1].y,
        0
    };
    Vector3d CP = {
        point.x - SSPositions[2].x,
        point.y - SSPositions[2].y,
        0
    };

    barycentricCoordinates[0] = signedAreaParallelogram(&BP, &edgeVectors[1]) / areaX2;
    barycentricCoordinates[1] = signedAreaParallelogram(&CP, &edgeVectors[2]) / areaX2;
    barycentricCoordinates[2] = signedAreaParallelogram(&AP, &edgeVectors[0]) / areaX2;

    barycentricDeltaX[0] = -edgeVectors[1].y / areaX2;
    barycentricDeltaX[1] = -edgeVectors[2].y / areaX2;
    barycentricDeltaX[2] = -edgeVectors[0].y / areaX2;

    barycentricDeltaY[0] = edgeVectors[1].x / areaX2;
    barycentricDeltaY[1] = edgeVectors[2].x / areaX2;
    barycentricDeltaY[2] = edgeVectors[0].x / areaX2;
}

static void calculateTileDimensionsAndNTilesInBoundingBox(
    const Vector2i BBDimensions, Vector2i* restrict tileDimensions,
    Vector2i* restrict nTiles
)
{
    // linear regression (boundingBoxDimension -> tileDimension)
    const double coefficient = 0.021;
    const double intercept = 14.87;
    *tileDimensions = (Vector2i) {
        round(coefficient * BBDimensions.x + intercept),
        round(coefficient * BBDimensions.y + intercept)
    };
    *nTiles = (Vector2i) {
        ceil((double) BBDimensions.x / tileDimensions->x),
        ceil((double) BBDimensions.y / tileDimensions->y)
    };
}

static void calculateBarycentricTileDeltas(
    const Vector2i tileDimensions, const double* restrict barycentricDeltaX,
    const double* restrict barycentricDeltaY, double* restrict barycentricTileDeltaX,
    double* restrict barycentricTileDeltaY
)
{
    barycentricTileDeltaX[0] = barycentricDeltaX[0] * tileDimensions.x;
    barycentricTileDeltaX[1] = barycentricDeltaX[1] * tileDimensions.x;
    barycentricTileDeltaX[2] = barycentricDeltaX[2] * tileDimensions.x;

    barycentricTileDeltaY[0] = barycentricDeltaY[0] * tileDimensions.y;
    barycentricTileDeltaY[1] = barycentricDeltaY[1] * tileDimensions.y;
    barycentricTileDeltaY[2] = barycentricDeltaY[2] * tileDimensions.y;
}

static bool triangleIsEdgeFlatTopOrLeft(const Vector3d* restrict edgeVector)
{
    return ((edgeVector->x > 0) && (edgeVector->y == 0)) || (edgeVector->y < 0);
}

static void triangleRejectionAcceptionTests(
    const triangleData* restrict data, bool* restrict rejected,
    bool* restrict accepted
)
{
    for (uint8_t iEdge = 0; iEdge < 3; iEdge++)
    {
        // barycentric coordinates for acception and rejection points
        double barycentricAcception, barycentricRejection;
        uint8_t iBarycentric = (iEdge + 2) % 3;
        if (data->posOnPlusY[iEdge])
        {
            if (data->slopeSign[iEdge])
            {
                barycentricAcception = \
                    data->barycentricCoordinates[iBarycentric] + \
                    data->barycentricTileDeltaX[iBarycentric];
                barycentricRejection = \
                    data->barycentricCoordinates[iBarycentric] + \
                    data->barycentricTileDeltaY[iBarycentric];
            }
            else  // negative slope
            {
                barycentricAcception = \
                    data->barycentricCoordinates[iBarycentric];
                barycentricRejection = \
                    data->barycentricCoordinates[iBarycentric] + \
                    data->barycentricTileDeltaX[iBarycentric] + \
                    data->barycentricTileDeltaY[iBarycentric];
            }
        }
        else  // negative on +y
        {
            if (data->slopeSign[iEdge])
            {
                barycentricAcception = \
                    data->barycentricCoordinates[iBarycentric] + \
                    data->barycentricTileDeltaY[iBarycentric];
                barycentricRejection = \
                    data->barycentricCoordinates[iBarycentric] + \
                    data->barycentricTileDeltaX[iBarycentric];
            }
            else  // negative slope
            {
                barycentricAcception = \
                    data->barycentricCoordinates[iBarycentric] + \
                    data->barycentricTileDeltaX[iBarycentric] + \
                    data->barycentricTileDeltaY[iBarycentric];
                barycentricRejection = \
                    data->barycentricCoordinates[iBarycentric];
            }
        }

        if (barycentricAcception > 0)
        {
            accepted[iEdge] = true;
            rejected[iEdge] = false;
        }
        else if (barycentricRejection <= 0)
        {
            accepted[iEdge] = false;
            rejected[iEdge] = true;
        }
        else  // TODO how tile can be neither accepted nor rejected?
        {
            accepted[iEdge] = false;
            rejected[iEdge] = false;
        }
    }
}

static void triangleLoopOverTileAndFill(
    const bool check, const Vector2d* restrict startPoint,
    const Vector2d* restrict endPoint, triangleData* restrict data,
    const ShaderProgram* restrict sp, const void* restrict gsOutput
)
{
    double barycentricCoordinatesAtBeginningOfTheRow[3];
    memcpy(
        data->barycentricCoordinatesCopy,
        data->barycentricCoordinates,
        sizeof(double) * 3
    );
    memcpy(
        barycentricCoordinatesAtBeginningOfTheRow,
        data->barycentricCoordinatesCopy,
        sizeof(double) * 3
    );

    for (size_t y = startPoint->y; y < endPoint->y; y += 1)
    {
        for (size_t x = startPoint->x; x < endPoint->x; x += 1)
        {
            if (check)
            {
                // If current point lies on the edge that is not flat top or 
                // left, do not draw the point (rasterization rules)
                for (uint8_t i = 0; i < 3; i++)
                    if (data->barycentricCoordinatesCopy[i] == 0 && \
                        data->isEdgeNotFlatTopOrLeft[i])
                        goto nextPixel;

                if (data->barycentricCoordinatesCopy[0] < 0 || \
                    data->barycentricCoordinatesCopy[1] < 0 || \
                    data->barycentricCoordinatesCopy[2] < 0)
                    goto nextPixel;
            }

            Color color;
            double depth;
            {
                // TODO avoid VLA (custom allocator?)
                uint8_t interpolatedBuffer[sp->geometryShader.nBytesPerOutputVertex];
                Interpolated* pInterpolated = (Interpolated*) interpolatedBuffer;
                triangleInterpolateGsOutput(
                    gsOutput, data->barycentricCoordinatesCopy, sp, pInterpolated
                );
                depth = ((double*) (
                    (uint8_t*) pInterpolated + sp->geometryShader.outputAttributes[
                        sp->geometryShader.indexOfOutputPositionAttribute
                    ].offsetBytes
                ))[2];

#ifndef NDEBUG
                if (context.debug.colorRasterizerTiles)
                {
                    if (check)
                        color = (Color) {255, 0, 0, 255};
                    else
                        color = (Color) {0, 255, 0, 255};
                }
                else
#endif
                {
                    sp->fragmentShader.shader(sp, pInterpolated, &color);
                }
            }

            rendererDrawPixel(context.renderer, (Vector2i) {x, y}, depth, color);

nextPixel:
            for (uint8_t i = 0; i < 3; i++)
                data->barycentricCoordinatesCopy[i] += data->barycentricDeltaX[i];
        }

        for (uint8_t i = 0; i < 3; i++)
            barycentricCoordinatesAtBeginningOfTheRow[i] += \
                data->barycentricDeltaY[i];

        memcpy(
            data->barycentricCoordinatesCopy,
            barycentricCoordinatesAtBeginningOfTheRow,
            sizeof(double) * 3
        );
    }
}

// @brief Interpolates geometry shader output in triangle
// Invalidates the whole buffer pointed to by `gsOutput`
// Interpolated data is stored in the first vertex of `gsOutput`
static void triangleInterpolateGsOutput(
    const void* gsOutput, const double barycentricCoordinates[3],
    const ShaderProgram* restrict sp, Interpolated* pInterpolatedBuffer
)
{
    // gsOutput =
    // (                           V0                            )  (       V1      ) (       V2      )
    // (          A0V0          )      (           AnV0          )  (A0V1, ..., AnV1) (A0V2, ..., AnV2)
    // V0A0E0, V0A0E1, ... V0A0En, ... V0AnE0, V0AnE1, ..., V0AnEn, ...

    // points to the beginning of attribute
    const void* pAttrVoid = gsOutput;
    // points to attribute in output buffer
    void* pInterpolatedAttrVoid = pInterpolatedBuffer;
    const GeometryShader* gs = &sp->geometryShader;

    for (size_t attrI = 0; attrI < gs->nOutputAttributes; attrI++)
    {
        VertexAttribute* attr = &gs->outputAttributes[attrI];
        size_t elemSize = 0;
        switch (attr->type)
        {
        case TYPE_DOUBLE:
        {
            elemSize = sizeof(double);
            double* pInterpolatedAttr = (double*) pInterpolatedAttrVoid;
            double* AV0 = (double*) pAttrVoid;
            double* AV1 = (double*) ((uint8_t*) AV0 + gs->nBytesPerOutputVertex);
            double* AV2 = (double*) ((uint8_t*) AV1 + gs->nBytesPerOutputVertex);
            for (size_t elemI = 0; elemI < attr->nItems; elemI++)
            {
                pInterpolatedAttr[elemI] = \
                    AV0[elemI] * barycentricCoordinates[0] + \
                    AV1[elemI] * barycentricCoordinates[1] + \
                    AV2[elemI] * barycentricCoordinates[2];
            }
            break;
        }
        default:
            LOGE("Unknown type (%i) in %s", attr->type, __func__);
            memset(pInterpolatedBuffer, 0, gs->nBytesPerOutputVertex);
            return;
        }
        size_t attrSize = elemSize * attr->nItems;
        pAttrVoid = (uint8_t*) pAttrVoid + attrSize;
        pInterpolatedAttrVoid = (uint8_t*) pInterpolatedAttrVoid + attrSize;
    }
}


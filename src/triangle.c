#include <assert.h>
#include "Vector/Vector.h"
#include "triangle.h"
#include "shaders.h"
#include "voidptr.h"
#include "utils.h"

void drawTriangle(
    Framebuffer* fb, const GSOutput* restrict gsOutput, 
    const ShaderProgram* restrict sp
)
{
    const GeometryShader* gs = &sp->geometryShader;
    VertexAttribute positionAttribute = gs->outputAttributes[
        gs->indexOfOutputPositionAttribute
    ];

    // TODO add this assert to docs
    assert(positionAttribute.nItems == 3);
    assert(positionAttribute.type == TYPE_DOUBLE);

    Vector3d NDCPositions[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        Vertex* pVertex = (Vertex*) INDEX_VOID_PTR(gsOutput, i, gs->nBytesPerOutputVertex);
        NDCPositions[i] = *(Vector3d*) VOID_PTR_ADD(pVertex, positionAttribute.offsetBytes);
    }

    // Do not traverse triangles with clockwise vertices
    Vector3d e0 = Vector3dSubtract(NDCPositions[1], NDCPositions[0]);
    Vector3d e1 = Vector3dSubtract(NDCPositions[2], NDCPositions[1]);
    double normal = signedAreaParallelogram(&e0, &e1);
    if (normal < 0)
        return;

    Vector3d SSPositions[3], edgeVectors[3];
    for (size_t i = 0; i < 3; i++)
        SSPositions[i] = \
            framebufferNDCToScreenSpace(fb, &NDCPositions[i]);
    for (size_t i = 0; i < 3; i++)
        edgeVectors[i] = Vector3dSubtract(
            SSPositions[(i+1) % 3], SSPositions[i]
        );

    Vector2d minBoundingPoint = {
        MIN(SSPositions[0].x, MIN(SSPositions[1].x, SSPositions[2].x)),
        MIN(SSPositions[0].y, MIN(SSPositions[1].y, SSPositions[2].y))
    };
    Vector2d maxBoundingPoint = {
        MAX(SSPositions[0].x, MAX(SSPositions[1].x, SSPositions[2].x)),
        MAX(SSPositions[0].y, MAX(SSPositions[1].y, SSPositions[2].y))
    };

    double barycentricCoordinates[3];
    double barycentricDeltaX[3], barycentricDeltaY[3];
    calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
        SSPositions, edgeVectors, minBoundingPoint, barycentricCoordinates,
        barycentricDeltaX, barycentricDeltaY
    );

    double barycentricCoordinatesAtBeginningOfTheRow[3];
    bool isEdgeNotFlatTopOrLeft[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        barycentricCoordinatesAtBeginningOfTheRow[i] = barycentricCoordinates[i];
        isEdgeNotFlatTopOrLeft[i] = !triangleIsEdgeFlatTopOrLeft(&edgeVectors[i]);
    }

    for (size_t y = minBoundingPoint.y; y < maxBoundingPoint.y; y += 1)
    {
        for (size_t x = minBoundingPoint.x; x < maxBoundingPoint.x; x += 1)
        {
            for (uint8_t i = 0; i < 3; i++)
            {
                // TODO are rasterization rules working? ROUGHLY_EQUAL here?
                if (barycentricCoordinates[i] == 0 && isEdgeNotFlatTopOrLeft[i])
                    goto nextPixel;
            }

            if (barycentricCoordinates[0] >= 0 && \
                barycentricCoordinates[1] >= 0 && \
                barycentricCoordinates[2] >= 0)
            {
                // TODO avoid VLA (custom allocator?)
                uint8_t interpolatedBuffer[sp->geometryShader.nBytesPerOutputVertex];
                Interpolated* pInterpolated = (Interpolated*) interpolatedBuffer;
                triangleInterpolateGsOutput(
                    gsOutput, barycentricCoordinates, sp, pInterpolated
                );
                double depth = ((double*) (
                    (uint8_t*) pInterpolated + sp->geometryShader.outputAttributes[
                        sp->geometryShader.indexOfOutputPositionAttribute
                    ].offsetBytes
                ))[2];

                Color color;
                sp->fragmentShader.shader(sp, pInterpolated, &color);
                framebufferDrawPixel(fb, x, y, depth, &color);
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

static double signedAreaParallelogram(
    const Vector3d* restrict a, const Vector3d* restrict b
)
{
    return a->x * b->y - a->y * b->x;
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

    barycentricDeltaX[0] = edgeVectors[1].y / areaX2;
    barycentricDeltaX[1] = edgeVectors[2].y / areaX2;
    barycentricDeltaX[2] = edgeVectors[0].y / areaX2;

    barycentricDeltaY[0] = -edgeVectors[1].x / areaX2;
    barycentricDeltaY[1] = -edgeVectors[2].x / areaX2;
    barycentricDeltaY[2] = -edgeVectors[0].x / areaX2;
}

static bool triangleIsEdgeFlatTopOrLeft(const Vector3d* restrict edgeVector)
{
    return ((edgeVector->x > 0) && (edgeVector->y == 0)) || (edgeVector->y < 0);
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
    // [V]ertex, [A]ttribute, [E]lement

    const GeometryShader* gs = &sp->geometryShader;
    // points to the beginning of attribute
    const void* pAttrVoid = gsOutput;
    // points to attribute in output buffer
    void* pInterpolatedAttrVoid = pInterpolatedBuffer;

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


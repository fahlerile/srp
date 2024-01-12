#include <stdint.h>
#include <assert.h>
#include "Vector/Vector.h"
#include "VertexBuffer.h" 
#include "Context.h"
#include "utils.h"
#include "draw.h"

static void geometryShader(
    void* vertex, Vector4d* transformedPosition, VertexBuffer* vertexBuffer
)
{

}

void drawVertexBuffer(
    DrawMode drawMode, size_t startIndex,
    size_t count, VertexBuffer* vertexBuffer
)
{
    assert(drawMode == DRAW_MODE_TRIANGLES && "Only triangles are implemented");

    for (size_t i = startIndex, n = startIndex + count; i < n; i += 3)
    {
        Vector4d transformedPositionsHomogenous[3];
        Vector3d transformedPositions[3];

        for (size_t j = 0; j < 3; j++)
        {
            geometryShader(
                VertexBufferGetVertexPointer(vertexBuffer, i+j),
                transformedPositionsHomogenous[j]
            );
            transformedPositions[j] = \
                Vector4dHomogenousDivide(transformedPositionsHomogenous[j]);
        }
        drawTriangle(transformedPositions, vertexBuffer);
    }
}

static void drawTriangle(
    Vector3d* NDCPositions, VertexBuffer* vertexBuffer
)
{
    Vector3d SSPositions[3];
    Vector3d edgeVectors[3];
    Vector2d minBoundingPoint, maxBoundingPoint;
    Vector3d barycentricCoordinates, barycentricDeltaX, barycentricDeltaY;

    triangleTransformPositionsToScreenSpace(NDCPositions, SSPositions);
    triangleGetBoundingPoints(SSPositions, &minBoundingPoint, &maxBoundingPoint);
    // We want to loop over centers of pixels, not over their bound
    minBoundingPoint = Vector2dAdd(minBoundingPoint, (Vector2d) {0.5, 0.5});
    maxBoundingPoint = Vector2dAdd(maxBoundingPoint, (Vector2d) {0.5, 0.5});

    triangleCalculateEdgeVectors(SSPositions, edgeVectors);
    triangleCalculateBarycenticDeltas(&barycentricDeltaX, &barycentricDeltaY);
    triangleCalculateBarycentricCoordinatesForPoint(
        barycentricDeltaX, barycentricDeltaY, minBoundingPoint,
        &barycentricCoordinates
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
                // Color color;
                // fragmentShader(&color);
                Color color = {255, 255, 255, 255};
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


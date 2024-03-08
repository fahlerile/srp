#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "memoryUtils/memoryUtils.h"
#include "VertexBuffer.h"
#include "log.h"
#include "NDC.h"
#include "Context.h"

VertexBuffer* newVertexBuffer(
    size_t nBytesPerVertex, size_t nBytesData, void* data, 
    size_t nAttributes, VertexAttribute* attributes
)
{
    VertexBuffer* this = xmalloc(sizeof(VertexBuffer));

    this->nBytesPerVertex = nBytesPerVertex;
    this->nBytesData = nBytesData;
    this->nVertices = nBytesData / nBytesPerVertex;
    this->data = xmalloc(nBytesData);
    memcpy(this->data, data, nBytesData);
    this->nAttributes = nAttributes;
    this->attributes = attributes;

    return this;
}

void freeVertexBuffer(VertexBuffer* this)
{
    xfree(this->data);
    xfree(this);
}

void drawVertexBuffer(
    VertexBuffer* this, Primitive primitive, size_t startIndex, size_t count, 
    ShaderProgram* sp
)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");
#ifndef NDEBUG
    if (sp->geometryShader.shader != NULL)
        assert(
            primitive == sp->geometryShader.inputPrimitive && \
            "Input primitive type for geometry shader and primitive passed to \
            drawVertexBuffer mismatch"
        );
#endif

    size_t endIndex = startIndex + count;
    void* triangleVsOutput = xmalloc(sp->vertexShader.nBytesPerVertex * 3);
    void* gsOutput;
    if (sp->geometryShader.shader == NULL)
        gsOutput = triangleVsOutput;
    else
        gsOutput = xmalloc(
            sp->geometryShader.nBytesPerVertex * \
            sp->geometryShader.nVertices
        );

    assert(startIndex + count <= this->nVertices);
    for (size_t i = startIndex; i < endIndex; i += 3)
    {
        for (size_t j = 0; j < 3; j++)
        {
            void* pVertex = (uint8_t*) this->data + (this->nBytesPerVertex * (i+j));
            sp->vertexShader.shader(
                pVertex, (char*) triangleVsOutput + \
                sp->vertexShader.nBytesPerVertex * j
            );
        }

        Primitive newPrimitive;
        if (sp->geometryShader.shader != NULL)
        {
            sp->geometryShader.shader(triangleVsOutput, gsOutput);
            newPrimitive = sp->geometryShader.outputPrimitive;
        }
        else
            newPrimitive = primitive;

        drawPrimitive(gsOutput, sp, newPrimitive);
    }
}

static void drawPrimitive(void* gsOutput, ShaderProgram* sp, Primitive primitive)
{
    assert(primitive == PRIMITIVE_TRIANGLES && "Only triangles are implemented");
    
    switch (primitive)
    {
        case PRIMITIVE_TRIANGLES:
            drawTriangle(gsOutput, sp);
            break;
        default:
            LOGE("Unknown primitive type (%i) in drawPrimitive", primitive);
            break;
    }
}

static void drawTriangle(void* gsOutput, ShaderProgram* sp)
{
    triangleData data;
    drawTrianglePreparation(gsOutput, sp, &data);
    drawTriangleRasterization(&data);
}

static void drawTrianglePreparation(
    void* gsOutput, ShaderProgram* sp, triangleData* returnData
)
{
    VertexAttribute positionAttribute;
    if (sp->geometryShader.shader == NULL)
        positionAttribute = sp->vertexShader.attributes[
            sp->geometryShader.indexOfPositionAttribute
        ];
    else
        positionAttribute = sp->geometryShader.attributes[
            sp->geometryShader.indexOfPositionAttribute
        ];

    // TODO add this assert to docs
    assert(positionAttribute.nItems == 3);
    assert(positionAttribute.type == TYPE_DOUBLE);

    size_t positionOffsetBytes = positionAttribute.offsetBytes;
    Vector3d* NDCPositions = (Vector3d*) ((uint8_t*) gsOutput + positionOffsetBytes);

    Vector3d SSPositions[3], edgeVectors[3];
    Vector3d minBP, maxBP;
    bool posOnPlusY[3], slopeSigns[3];

    NDCToScreenSpaceArray(NDCPositions, SSPositions, 3);
    getBoundingBoxArray(SSPositions, &minBP, &maxBP, 3);
    calculateEdgeVectors(SSPositions, edgeVectors, 3);
    calculatePositiveOnPlusYAndSlopeSignsArray(
        SSPositions, edgeVectors, posOnPlusY, slopeSigns, 3
    );

    calculateBarycentricCoordinatesForPoint();
    calculateBarycentricDeltas();

    calculateTileDimensionsAndNTilesInBoundingBox();
    calculateBarycentricTileDeltas();
}

static void drawTriangleRasterization(triangleData* data)
{

}

static void NDCToScreenSpaceArray(
    Vector3d* NDCPositions, Vector3d* SSPositions, size_t n
)
{
    for (size_t i = 0; i < n; i++)
        SSPositions[i] = NDCToScreenSpace(context.renderer, NDCPositions[i]);
}

static void getBoundingBoxArray(
    Vector3d* SSPositions, Vector3d* min, Vector3d* max, size_t n
)
{
    double xmin = SSPositions[0].x, xmax = SSPositions[0].x;
    double ymin = SSPositions[0].y, ymax = SSPositions[0].y;
    double zmin = SSPositions[0].z, zmax = SSPositions[0].z;

    for (size_t i = 0; i < n; i++)
    {
        double x = SSPositions[i].x;
        double y = SSPositions[i].y;
        double z = SSPositions[i].z;

        if (x < xmin)
            xmin = x;
        else if (x > xmax)
            xmax = x;
        if (y < ymin)
            ymin = y;
        else if (y > ymax)
            ymax = y;
        if (z < zmin)
            zmin = z;
        else if (z > zmax)
            zmax = z;
    }
    *min = (Vector3d) {xmin, ymin, zmin};
    *max = (Vector3d) {xmax, ymax, zmax};
}

static void calculateEdgeVectors(
    Vector3d* SSPositions, Vector3d* edgeVectors, size_t n
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
    Vector3d* SSPositions, Vector3d* edgeVectors,
    bool* posOnPlusY, bool* slopeSigns, size_t n
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


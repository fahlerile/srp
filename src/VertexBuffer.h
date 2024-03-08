#pragma once
#include <stddef.h>
#include "Vector/Vector.h"
#include "Type.h"
#include "Shaders.h"

typedef enum
{
    PRIMITIVE_TRIANGLES
} Primitive;

typedef struct
{
    size_t nItems;
    Type type;
    size_t offsetBytes;
} VertexAttribute;

typedef struct
{
    size_t nBytesPerVertex;
    size_t nBytesData;
    size_t nVertices;
    void* data;
    size_t nAttributes;
    VertexAttribute* attributes;
} VertexBuffer;

VertexBuffer* newVertexBuffer(
    size_t nBytesPerVertex, size_t nBytesData, void* data, 
    size_t nAttributes, VertexAttribute* attributes
);
void freeVertexBuffer(VertexBuffer* this);

void drawVertexBuffer(
    VertexBuffer* this, Primitive drawMode, size_t startIndex, size_t count, 
    ShaderProgram* shaderProgram
);

static void drawPrimitive(void* gsOutput, ShaderProgram* sp, Primitive primitive);

typedef struct
{
    Vector3d* positions;    // vertices' coordinates in screen space
    Vector3d* edgeVectors;  // 0->1, 1->2, 2->0

    double* barycentricMinBoundingPoint;
    double* barycentricDeltaX;
    double* barycentricDeltaY;

    Vector2i minBoundingPoint;
    Vector2i tileDimensions;
    Vector2i nTiles;
    double* barycentricDeltaTileX;
    double* barycentricDeltaTileY;
    bool* positiveOnPlusY;
    bool* edgeSlopeIsPositiveOrZero;
} triangleData;

static void drawTriangle(void* gsOutput, ShaderProgram* sp);
static void drawTrianglePreparation(void* gsOutput, triangleData* returnData);
static void drawTriangleRasterization(triangleData* data);


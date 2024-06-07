#pragma once
#include "Vector/Vector.h"
#include "Framebuffer.h"
#include "shaders.h"

typedef struct
{
	double barycentricCoordinates[3];
	double barycentricDeltaX[3];
	double barycentricDeltaY[3];
	bool isEdgeNotFlatTopOrLeft[3];
	Vector2d minBP, maxBP;
} triangleData;

void drawTriangle(
	Framebuffer* fb, const VSOutput vertices[3], const ShaderProgram* restrict sp,
	size_t primitiveID
);

static double signedAreaParallelogram(
	const Vector3d* restrict a, const Vector3d* restrict b
);
static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
	const Vector3d* restrict SSPositions, const Vector3d* restrict edgeVectors,
	const Vector2d point, double* restrict barycentricCoordinates,
	double* restrict barycentricDeltaX, double* restrict barycentricDeltaY
);
static bool triangleIsEdgeFlatTopOrLeft(const Vector3d* restrict edgeVector);

static void triangleInterpolatePositionAndVertexVariables(
	const VSOutput vertices[3], const double barycentricCoordinates[3],
	const ShaderProgram* restrict sp, Interpolated* pInterpolatedBuffer,
	Vector4d* interpolatedPosition
);


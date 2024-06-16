#pragma once

#include "Framebuffer.h"
#include "shaders.h"
#include "vec.h"

void drawTriangle(
	Framebuffer* fb, const VSOutput vertices[3], const ShaderProgram* restrict sp,
	size_t primitiveID
);

static double signedAreaParallelogram(
	const vec3d* restrict a, const vec3d* restrict b
);
static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
	const vec3d* restrict SSPositions, const vec3d* restrict edgeVectors,
	const vec2d point, double* restrict barycentricCoordinates,
	double* restrict barycentricDeltaX, double* restrict barycentricDeltaY
);
static bool triangleIsEdgeFlatTopOrLeft(const vec3d* restrict edgeVector);

static void triangleInterpolatePositionAndVertexVariables(
	const VSOutput vertices[3], const double barycentricCoordinates[3],
	const ShaderProgram* restrict sp, Interpolated* pInterpolatedBuffer,
	vec4d* interpolatedPosition
);


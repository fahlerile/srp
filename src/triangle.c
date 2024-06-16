#define SRP_SOURCE

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "triangle.h"
#include "vertex.h"
#include "message_callback.h"
#include "math_utils.h"
#include "shaders.h"
#include "color.h"
#include "utils.h"

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
	const SRPvsOutput vertices[3], const double barycentricCoordinates[3],
	const SRPShaderProgram* restrict sp, SRPInterpolated* pInterpolatedBuffer,
	vec4d* interpolatedPosition
);

void drawTriangle(
	SRPFramebuffer* fb, const SRPvsOutput vertices[3],
	const SRPShaderProgram* restrict sp, size_t primitiveID
)
{
	vec3d NDCPositions[3];
	for (uint8_t i = 0; i < 3; i++)
		NDCPositions[i] = (vec3d) {
			vertices[i].position.x,
			vertices[i].position.y,
			vertices[i].position.z
		};

	// Do not traverse triangles with clockwise vertices
	// TODO: why compute these two edge vectors twice?
	vec3d e0 = vec3dSubtract(NDCPositions[1], NDCPositions[0]);
	vec3d e1 = vec3dSubtract(NDCPositions[2], NDCPositions[1]);
	double normal = signedAreaParallelogram(&e0, &e1);
	if (normal < 0)
		return;

	vec3d SSPositions[3], edgeVectors[3];
	for (size_t i = 0; i < 3; i++)
		framebufferNDCToScreenSpace(
			fb, (double*) &NDCPositions[i], (double*) &SSPositions[i]
		);
	for (size_t i = 0; i < 3; i++)
		edgeVectors[i] = vec3dSubtract(SSPositions[(i+1) % 3], SSPositions[i]);

	vec2d minBoundingPoint = {
		MIN(SSPositions[0].x, MIN(SSPositions[1].x, SSPositions[2].x)),
		MIN(SSPositions[0].y, MIN(SSPositions[1].y, SSPositions[2].y))
	};
	vec2d maxBoundingPoint = {
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
				// TODO: avoid VLA (custom allocator?)
				uint8_t interpolatedBuffer[sp->vs.nBytesPerOutputVariables];
				SRPInterpolated* pInterpolated = (SRPInterpolated*) interpolatedBuffer;
				vec4d interpolatedPosition = {0};
				triangleInterpolatePositionAndVertexVariables(
					vertices, barycentricCoordinates, sp, pInterpolated,
					&interpolatedPosition
				);

				// TODO: fix rasterizer to accept both cw and ccw vertices
				// and add correct `frontFacing` here!
				SRPfsInput fsIn = {
					.uniform = sp->uniform,
					.interpolated = pInterpolated,
					.fragCoord = interpolatedPosition,
					.frontFacing = true,
					.primitiveID = primitiveID,
				};
				SRPfsOutput fsOut = {0};

				sp->fs.shader(&fsIn, &fsOut);

				SRPColor color = {
					CLAMP(0, 255, fsOut.color.x * 255),
					CLAMP(0, 255, fsOut.color.y * 255),
					CLAMP(0, 255, fsOut.color.z * 255),
					CLAMP(0, 255, fsOut.color.w * 255)
				};
				double depth = (fsOut.fragDepth == 0) ? fsIn.fragCoord.z : fsOut.fragDepth;

				framebufferDrawPixel(fb, x, y, depth, SRP_COLOR_TO_UINT32_T(color));
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
	const vec3d* restrict a, const vec3d* restrict b
)
{
	return a->x * b->y - a->y * b->x;
}

static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
	const vec3d* restrict SSPositions, const vec3d* restrict edgeVectors,
	const vec2d point, double* restrict barycentricCoordinates,
	double* restrict barycentricDeltaX, double* restrict barycentricDeltaY
)
{
	double areaX2 = fabs(signedAreaParallelogram(&edgeVectors[0], &edgeVectors[2]));

	vec3d AP = {
		point.x - SSPositions[0].x,
		point.y - SSPositions[0].y,
		0
	};
	vec3d BP = {
		point.x - SSPositions[1].x,
		point.y - SSPositions[1].y,
		0
	};
	vec3d CP = {
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

static bool triangleIsEdgeFlatTopOrLeft(const vec3d* restrict edgeVector)
{
	return ((edgeVector->x > 0) && (edgeVector->y == 0)) || (edgeVector->y < 0);
}

// @brief Interpolates variables in triangle
static void triangleInterpolatePositionAndVertexVariables(
	const SRPvsOutput vertices[3], const double barycentricCoordinates[3],
	const SRPShaderProgram* restrict sp, SRPInterpolated* pInterpolatedBuffer,
	vec4d* pInterpolatedPosition
)
{
	// *vertices.pOutputVariables =
	// (						   V0							 )	(		V1		) (		  V2	  )
	// (		  V0A0			) ...  (		   V0An			 )	(V1A0, ..., V1An) (V2A0, ..., V2An)
	// V0A0E0, V0A0E1, ... V0A0En, ... V0AnE0, V0AnE1, ..., V0AnEn, ...
	// [V]ertex, [A]ttribute, [E]lement

	// points to attribute in output buffer
	void* pInterpolatedAttrVoid = pInterpolatedBuffer;

	// interpolate position
	for (uint8_t i = 0; i < 4; i++)
	{
		((double*) pInterpolatedPosition)[i] = \
			((double*) &vertices[0].position)[i] * barycentricCoordinates[0] + \
			((double*) &vertices[1].position)[i] * barycentricCoordinates[1] + \
			((double*) &vertices[2].position)[i] * barycentricCoordinates[2];
	}

	// interpolate variables (attributes)
	for (size_t attrI = 0; attrI < sp->vs.nOutputVariables; attrI++)
	{
		SRPVertexVariable* attr = &sp->vs.outputVariables[attrI];
		size_t elemSize = 0;
		switch (attr->type)
		{
		case TYPE_DOUBLE:
		{
			elemSize = sizeof(double);
			double* pInterpolatedAttr = (double*) pInterpolatedAttrVoid;

			// pointers to the current attribute of 0th, 1st and 2nd vertices
			double* AV0 = (double*) \
				ADD_VOID_PTR(vertices[0].pOutputVariables, attr->offsetBytes);
			double* AV1 = (double*) \
				ADD_VOID_PTR(vertices[1].pOutputVariables, attr->offsetBytes);
			double* AV2 = (double*) \
				ADD_VOID_PTR(vertices[2].pOutputVariables, attr->offsetBytes);

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
			messageCallback(
				MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
				"Unexpected type (%i)", attr->type
			);
		}
		size_t attrSize = elemSize * attr->nItems;
		pInterpolatedAttrVoid = (uint8_t*) pInterpolatedAttrVoid + attrSize;
	}
}


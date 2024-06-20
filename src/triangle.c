// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "triangle.h"
#include "context.h"
#include "vertex.h"
#include "framebuffer_p.h"
#include "message_callback_p.h"
#include "shaders.h"
#include "color.h"
#include "utils.h"
#include "math_utils.h"
#include "vec.h"

/** @file
 *  Triangle rasteization & data interpolation */

/** @ingroup Rasterization
 *  @{ */

/** Get a parallelogram's signed area. The two vectors define a parallelogram.
 *  Used for barycentric coordinates' initialization in
 *  calculateBarycentricCoordinatesForPointAndBarycentricDeltas() */
static double signedAreaParallelogram(
	const vec3d* restrict a, const vec3d* restrict b
);

/** Calculate barycentric coordinates for a point and barycentric coordinates'
 *  delta values.
 *  @param[in] SSPositions Vertices' positions in screen-space
 *  @param[in] edgeVectors Triangle's edge vectors, where 0th element is a vector
 *                         pointing from vertex 0 to vertex 1, 1th - from vertex
 *                         1 to vertex 2 and so on.
 *  @param[in] point A point for which to calculate the barycentric coordinates
 *  @param[out] barycentricCoordinates A pointer to 3-element `double` array to
 *                                     where output barycentric coordinates to
 *  @param[out] barycentricDeltaX,barycentricDeltaY
 *              A pointer to 3-element `double` array to where to output the X
 *              and Y barycentric delta values. These show how much does the
 *              barycentric coordinates change if we move one pixel to the +X or
 *              to the +Y direction. Used in incremental computation of
 *              barycentric coordinates when looping over triangle's bounding
 *              box */
static void calculateBarycentricCoordinatesForPointAndBarycentricDeltas(
	const vec3d* restrict SSPositions, const vec3d* restrict edgeVectors,
	vec2d point, double* restrict barycentricCoordinates,
	double* restrict barycentricDeltaX, double* restrict barycentricDeltaY
);

/** Check if a triangle's edge is flat top or left. Used in rasterization rules.
 *  @param[in] A pointer to an edge vector (pointing from one vertex to the other)
 *  @return Whether or not this edge is flat top or left
 *  @todo Does it matter if I pass edge 0->1 or 1->0 (numbers = vertex indices)? */
static bool triangleIsEdgeFlatTopOrLeft(const vec3d* restrict edgeVector);

/** Interpolate the fragment position and vertex variables inside the triangle.
 *  @param[in] vertices A pointer to an array of 3 vertices
 *  @param[in] barycentricCoordinates Barycentric coordinates of the fragment
 *  @param[in] sp A pointer to shader program to use
 *  @param[out] pInterpolatedBuffer A pointer to the buffer where interpolated
 *              variables will appear. Must be big enough to hold all of them
 *  @param[out] interpolatedPosition A pointer to vec4d where interpolated
 *              position will appear. */
static void triangleInterpolatePositionAndVertexVariables(
	const SRPvsOutput vertices[3], const double barycentricCoordinates[3],
	const SRPShaderProgram* restrict sp, SRPInterpolated* pInterpolatedBuffer,
	vec4d* interpolatedPosition
);

/** @} */  // ingroup Rasterization

void drawTriangle(
	const SRPFramebuffer* fb, const SRPvsOutput vertices[3],
	const SRPShaderProgram* restrict sp, size_t primitiveID
)
{
	vec3d NDCPositions[3];
	for (uint8_t i = 0; i < 3; i++)
		NDCPositions[i] = (vec3d) {
			vertices[i].position[0],
			vertices[i].position[1],
			vertices[i].position[2]
		};

	// Do not traverse triangles with clockwise vertices
	/** @todo Why compute these two edge vectors twice? */
	vec3d e0 = vec3dSubtract(NDCPositions[1], NDCPositions[0]);
	vec3d e1 = vec3dSubtract(NDCPositions[2], NDCPositions[1]);
	double normal = signedAreaParallelogram(&e0, &e1);
	if (normal < 0)
		return;

	vec3d SSPositions[3], edgeVectors[3];
	for (size_t i = 0; i < 3; i++)
		srpFramebufferNDCToScreenSpace(
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
				/** @todo Are rasterization rules working? Rough equality here? */
				if (barycentricCoordinates[i] == 0 && isEdgeNotFlatTopOrLeft[i])
					goto nextPixel;
			}

			if (barycentricCoordinates[0] >= 0 && \
				barycentricCoordinates[1] >= 0 && \
				barycentricCoordinates[2] >= 0)
			{
				/** @todo Avoid VLA (custom allocator?) */
				uint8_t interpolatedBuffer[sp->vs.nBytesPerOutputVariables];
				SRPInterpolated* pInterpolated = (SRPInterpolated*) interpolatedBuffer;
				vec4d interpolatedPosition = {0};
				triangleInterpolatePositionAndVertexVariables(
					vertices, barycentricCoordinates, sp, pInterpolated,
					&interpolatedPosition
				);

				/** @todo fix rasterizer to accept both cw and ccw vertices
				 *  and add correct `frontFacing` here! */
				SRPfsInput fsIn = {
					.uniform = sp->uniform,
					.interpolated = pInterpolated,
					.fragCoord = {
						interpolatedPosition.x,
						interpolatedPosition.y,
						interpolatedPosition.z,
						interpolatedPosition.w
					},
					.frontFacing = true,
					.primitiveID = primitiveID,
				};
				SRPfsOutput fsOut = {0};

				sp->fs.shader(&fsIn, &fsOut);

				SRPColor color = {
					CLAMP(0, 255, fsOut.color[0] * 255),
					CLAMP(0, 255, fsOut.color[1] * 255),
					CLAMP(0, 255, fsOut.color[2] * 255),
					CLAMP(0, 255, fsOut.color[3] * 255)
				};
				double depth = (fsOut.fragDepth == 0) ? fsIn.fragCoord[2] : fsOut.fragDepth;

				srpFramebufferDrawPixel(fb, x, y, depth, SRP_COLOR_TO_UINT32_T(color));
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

/** @todo Document the algorithm and why it works. P.S: I have no idea */
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

	// Points to current attribute in output buffer
	void* pInterpolatedAttrVoid = pInterpolatedBuffer;

	// Vertices' Z values without negatives, in preserved order
	// Needed because perspective correction does not like negative values
	double noNegativeZ[3];
	if (srpContext.attributeInterpolation == SRP_ATTR_INTERPOLATION_PERSPECTIVE)
	{
		noNegativeZ[0] = (vertices[0].position[2] + 1) / 2;
		noNegativeZ[1] = (vertices[1].position[2] + 1) / 2;
		noNegativeZ[2] = (vertices[2].position[2] + 1) / 2;
	}

	// Interpolate position
	for (uint8_t i = 0; i < 2; i++)
	{
		((double*) pInterpolatedPosition)[i] = \
			((double*) &vertices[0].position)[i] * barycentricCoordinates[0] + \
			((double*) &vertices[1].position)[i] * barycentricCoordinates[1] + \
			((double*) &vertices[2].position)[i] * barycentricCoordinates[2];
	}
	pInterpolatedPosition->w = 1;
	if (srpContext.attributeInterpolation == SRP_ATTR_INTERPOLATION_PERSPECTIVE)
	{
		pInterpolatedPosition->z = 1 / (
			(1 / noNegativeZ[0]) * barycentricCoordinates[0] + \
			(1 / noNegativeZ[1]) * barycentricCoordinates[1] + \
			(1 / noNegativeZ[2]) * barycentricCoordinates[2]
		);
	}
	else  // affine
	{
		pInterpolatedPosition->z = (
			vertices[0].position[2] * barycentricCoordinates[0] + \
			vertices[1].position[2] * barycentricCoordinates[1] + \
			vertices[2].position[2] * barycentricCoordinates[2]
		);
	}

	// Interpolate variables (attributes)
	size_t attrOffsetBytes = 0;
	for (size_t attrI = 0; attrI < sp->vs.nOutputVariables; attrI++)
	{
		SRPVertexVariableInformation* attr = &sp->vs.outputVariables[attrI];
		size_t elemSize = 0;
		switch (attr->type)
		{
		case TYPE_DOUBLE:
		{
			elemSize = sizeof(double);
			double* pInterpolatedAttr = (double*) pInterpolatedAttrVoid;

			// pointers to the current attribute of 0th, 1st and 2nd vertices
			double* AV0 = (double*) \
				ADD_VOID_PTR(vertices[0].pOutputVariables, attrOffsetBytes);
			double* AV1 = (double*) \
				ADD_VOID_PTR(vertices[1].pOutputVariables, attrOffsetBytes);
			double* AV2 = (double*) \
				ADD_VOID_PTR(vertices[2].pOutputVariables, attrOffsetBytes);

			for (size_t elemI = 0; elemI < attr->nItems; elemI++)
			{
				if (srpContext.attributeInterpolation == SRP_ATTR_INTERPOLATION_PERSPECTIVE)
				{
					pInterpolatedAttr[elemI] = pInterpolatedPosition->z * (
						(AV0[elemI] / noNegativeZ[0]) * barycentricCoordinates[0] + \
						(AV1[elemI] / noNegativeZ[1]) * barycentricCoordinates[1] + \
						(AV2[elemI] / noNegativeZ[2]) * barycentricCoordinates[2]
					);
				}
				else  // affine
				{
					pInterpolatedAttr[elemI] = \
						AV0[elemI] * barycentricCoordinates[0] + \
						AV1[elemI] * barycentricCoordinates[1] + \
						AV2[elemI] * barycentricCoordinates[2];
				}
			}
			break;
		}
		default:
			srpMessageCallbackHelper(
				MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
				"Unexpected type (%i)", attr->type
			);
		}
		size_t attrSize = elemSize * attr->nItems;
		pInterpolatedAttrVoid = (uint8_t*) pInterpolatedAttrVoid + attrSize;
		attrOffsetBytes += attrSize;
	}
}


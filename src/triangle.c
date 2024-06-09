#include <stdint.h>
#include <string.h>
#include "triangle.h"
#include "Vector/Vector4.h"
#include "Vertex.h"
#include "math_utils.h"
#include "shaders.h"
#include "log.h"
#include "voidptr.h"

void drawTriangle(
	Framebuffer* fb, const VSOutput vertices[3], const ShaderProgram* restrict sp,
	size_t primitiveID
)
{
	Vector3d NDCPositions[3];
	for (uint8_t i = 0; i < 3; i++)
		NDCPositions[i] = Vector4dHomogenousDivide(vertices[i].position);

	// Do not traverse triangles with clockwise vertices
	Vector3d e0 = Vector3dSubtract(NDCPositions[1], NDCPositions[0]);
	Vector3d e1 = Vector3dSubtract(NDCPositions[2], NDCPositions[1]);
	double normal = signedAreaParallelogram(&e0, &e1);
	if (normal < 0)
		return;

	Vector3d SSPositions[3], edgeVectors[3];
	for (size_t i = 0; i < 3; i++)
		framebufferNDCToScreenSpace(
			fb, (double*) &NDCPositions[i], (double*) &SSPositions[i]
		);
	for (size_t i = 0; i < 3; i++)
		edgeVectors[i] = Vector3dSubtract(SSPositions[(i+1) % 3], SSPositions[i]);

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
				// TODO: avoid VLA (custom allocator?)
				uint8_t interpolatedBuffer[sp->vs.nBytesPerOutputVariables];
				Interpolated* pInterpolated = (Interpolated*) interpolatedBuffer;
				Vector4d interpolatedPosition = {0};
				triangleInterpolatePositionAndVertexVariables(
					vertices, barycentricCoordinates, sp, pInterpolated,
					&interpolatedPosition
				);

				// TODO: fix rasterizer to accept both cw and ccw vertices
				// and add correct `frontFacing` here!
				FSInput fsIn = {
					.uniforms = sp->uniforms,
					.interpolated = pInterpolated,
					.fragCoord = interpolatedPosition,
					.frontFacing = true,
					.primitiveID = primitiveID,
				};
				FSOutput fsOut = {0};

				sp->fs.shader(&fsIn, &fsOut);

				Color color = {
					CLAMP(0, 255, fsOut.color.x * 255),
					CLAMP(0, 255, fsOut.color.y * 255),
					CLAMP(0, 255, fsOut.color.z * 255),
					CLAMP(0, 255, fsOut.color.w * 255)
				};
				double depth = (fsOut.fragDepth == 0) ? fsIn.fragCoord.z : fsOut.fragDepth;

				framebufferDrawPixel(fb, x, y, depth, ColorToUint32RGBA(&color));
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

// @brief Interpolates variables in triangle
static void triangleInterpolatePositionAndVertexVariables(
	const VSOutput vertices[3], const double barycentricCoordinates[3],
	const ShaderProgram* restrict sp, Interpolated* pInterpolatedBuffer,
	Vector4d* pInterpolatedPosition
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
		VertexVariable* attr = &sp->vs.outputVariables[attrI];
		size_t elemSize = 0;
		switch (attr->type)
		{
		case TYPE_DOUBLE:
		{
			elemSize = sizeof(double);
			double* pInterpolatedAttr = (double*) pInterpolatedAttrVoid;

			// pointers to the current attribute of 0th, 1st and 2nd vertices
			double* AV0 = (double*) \
				INDEX_VOID_PTR(vertices[0].pOutputVariables, attr->offsetBytes, 1);
			double* AV1 = (double*) \
				INDEX_VOID_PTR(vertices[1].pOutputVariables, attr->offsetBytes, 1);
			double* AV2 = (double*) \
				INDEX_VOID_PTR(vertices[2].pOutputVariables, attr->offsetBytes, 1);

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
			memset(pInterpolatedBuffer, 0, sp->vs.nBytesPerOutputVariables);
			return;
		}
		size_t attrSize = elemSize * attr->nItems;
		pInterpolatedAttrVoid = (uint8_t*) pInterpolatedAttrVoid + attrSize;
	}
}


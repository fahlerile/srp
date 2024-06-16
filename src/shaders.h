#pragma once

#include <stdbool.h>
#include "vertex.h"
#include "vec.h"

typedef struct SRPInterpolated SRPInterpolated;

typedef struct SRPUniform SRPUniform;


typedef struct
{
	SRPUniform* uniform;
	SRPVertex* pVertex;
	size_t vertexID;
} SRPvsInput;

typedef struct VSOutputVariable VSOutputVariable;

typedef struct
{
	vec4d position;
	VSOutputVariable* pOutputVariables;
} SRPvsOutput;

typedef struct
{
	void (*shader)(
		SRPvsInput* in, SRPvsOutput* out
	);
	size_t nBytesPerOutputVariables;
	size_t nOutputVariables;
	SRPVertexVariable* outputVariables;
} SRPVertexShader;


typedef struct
{
	SRPUniform* uniform;
	SRPInterpolated* interpolated;
	vec4d fragCoord;
	bool frontFacing;
	size_t primitiveID;
} SRPfsInput;

typedef struct
{
	vec4d color;
	double fragDepth;
} SRPfsOutput;

typedef struct
{
	void (*shader)(SRPfsInput* in, SRPfsOutput* out);
} SRPFragmentShader;


typedef struct SRPShaderProgram
{
	SRPUniform* uniform;

	SRPVertexShader vs;
	SRPFragmentShader fs;
} SRPShaderProgram;


#pragma once

#include <stdbool.h>
#include "vertex.h"

typedef struct SRPInterpolated SRPInterpolated;

typedef struct SRPUniform SRPUniform;


typedef struct
{
	SRPUniform* uniform;
	SRPVertex* pVertex;
	size_t vertexID;
} SRPvsInput;

typedef struct
{
	double position[4];
	SRPVertexVariable* pOutputVariables;
} SRPvsOutput;

// TODO: are nBytesPerOutputVariables and outputVariables[i].offsetBytes
// useless? Can I compute those? Where would I? Do I need to?
typedef struct
{
	void (*shader)(
		SRPvsInput* in, SRPvsOutput* out
	);
	size_t nBytesPerOutputVariables;
	size_t nOutputVariables;
	SRPVertexVariableInformation* outputVariables;
} SRPVertexShader;


typedef struct
{
	SRPUniform* uniform;
	SRPInterpolated* interpolated;
	double fragCoord[4];
	bool frontFacing;
	size_t primitiveID;
} SRPfsInput;

typedef struct
{
	double color[4];
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


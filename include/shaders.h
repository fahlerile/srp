// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  Typedefs related to shaders */

#include <stdbool.h>
#include "vertex.h"

/** @ingroup Shaders
 *  @{ */

/** Represents user's shader uniform
 *  @see SRPShaderProgram */
typedef struct SRPUniform SRPUniform;


/** Holds inputs to vertex shader
 *  @see SRPVertexShader */
typedef struct SRPvsInput
{
	SRPUniform* uniform;
	SRPVertex* pVertex;
	size_t vertexID;
} SRPvsInput;

/** Holds outputs from vertex shader
 *  @see SRPVertexShader */
typedef struct SRPvsOutput
{
	double position[4];
	SRPVertexVariable* pOutputVariables;
} SRPvsOutput;

/** Represents the vertex shader
 *  @see SRPShaderProgram */
typedef struct SRPVertexShader
{
	void (*shader)(SRPvsInput* in, SRPvsOutput* out);
	size_t nOutputVariables;
	SRPVertexVariableInformation* outputVariables;
	size_t nBytesPerOutputVariables;
} SRPVertexShader;


/** Holds inputs to fragment shader
 *  @see SRPFragmentShader */
typedef struct SRPfsInput
{
	SRPUniform* uniform;
	SRPInterpolated* interpolated;
	double fragCoord[4];
	bool frontFacing;
	size_t primitiveID;
} SRPfsInput;

/** Holds outputs from fragment shader
 *  @see SRPFragmentShader */
typedef struct SRPfsOutput
{
	double color[4];
	double fragDepth;
} SRPfsOutput;

/** Represents the fragment shader
 *  @see ShaderProgram */
typedef struct SRPFragmentShader
{
	void (*shader)(SRPfsInput* in, SRPfsOutput* out);
} SRPFragmentShader;


/** Holds shaders and a uniform.
 *  While not being a program (is not compiled or anything), the naming is
 *  chosen because it is similar to OpenGL's shader program 
 *  @see srpDrawVertexBuffer srpDrawIndexBuffer */
typedef struct SRPShaderProgram
{
	SRPUniform* uniform;

	SRPVertexShader vs;
	SRPFragmentShader fs;
} SRPShaderProgram;

/** @} */  // defgroup Shaders


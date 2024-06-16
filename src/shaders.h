#pragma once
#include <stdbool.h>
#include "Vertex.h"
#include "vec.h"

typedef struct Interpolated Interpolated;

// Assumed to be defined by the user
// TODO: multiple uniform types?
typedef struct Uniforms Uniforms;



typedef struct
{
	Uniforms* uniforms;
	Vertex* pVertex;
	size_t vertexID;
} VSInput;

typedef struct VSOutputVariable VSOutputVariable;

typedef struct
{
	vec4d position;
	VSOutputVariable* pOutputVariables;
} VSOutput;

typedef struct
{
	void (*shader)(
		VSInput* in, VSOutput* out
	);
	size_t nBytesPerOutputVariables;
	size_t nOutputVariables;
	VertexVariable* outputVariables;
} VertexShader;


typedef struct
{
	Uniforms* uniforms;
	Interpolated* interpolated;
	vec4d fragCoord;
	bool frontFacing;
	size_t primitiveID;
} FSInput;

typedef struct
{
	vec4d color;
	double fragDepth;
} FSOutput;

typedef struct
{
	void (*shader)(FSInput* in, FSOutput* out);
} FragmentShader;


typedef struct ShaderProgram
{
	Uniforms* uniforms;

	VertexShader vs;
	FragmentShader fs;
} ShaderProgram;


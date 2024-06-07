#pragma once
#include "Vertex.h"

typedef struct VSOutput VSOutput;
typedef struct Interpolated Interpolated;

// Assumed to be defined by the user
// TODO: multiple uniform types?
typedef struct Uniforms Uniforms;

// Circular dependency
typedef struct ShaderProgram ShaderProgram;

typedef struct
{
	void (*shader)(
		const ShaderProgram* sp, const Vertex* pVertex, VSOutput* pOutput,
		size_t vertexIndex
	);
	size_t nBytesPerOutputVertex;
	size_t nOutputAttributes;
	VertexAttribute* outputAttributes;
	size_t indexOfOutputPositionAttribute;
} VertexShader;

typedef struct
{
	void (*shader)(
		const ShaderProgram* sp, const Interpolated* pInterpolated, double* color
	);
} FragmentShader;

struct ShaderProgram
{
	Uniforms* uniforms;

	VertexShader vs;
	FragmentShader fs;
};


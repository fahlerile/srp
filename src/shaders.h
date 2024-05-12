#pragma once
#include "Vertex.h"
#include "Primitive.h"
#include "Color/Color.h"

typedef struct VSOutput VSOutput;
typedef struct GSOutput GSOutput;
typedef struct Interpolated Interpolated;

typedef struct Uniforms Uniforms;

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
	void (*shader)(const ShaderProgram* sp, const VSOutput* pInput, GSOutput* pOutput);
	size_t nBytesPerOutputVertex;
	size_t nOutputAttributes;
	VertexAttribute* outputAttributes;
	size_t indexOfOutputPositionAttribute;

	// Geometry shader specific
	// If both `inputPrimitive` and `outputPrimitive` are `PRIMITIVE_ANY` 
	// (so geometryShader was default-initialized) then `nOutputVertices`
	// is not valid and should be set accordingly to primitive type that 
	// is being processed
	size_t nOutputVertices;
	Primitive inputPrimitive;
	Primitive outputPrimitive;
} GeometryShader;

typedef struct
{
	void (*shader)(
		const ShaderProgram* sp, const Interpolated* pInterpolated, Color* pColor
	);
} FragmentShader;

struct ShaderProgram
{
	Uniforms* uniforms;

	VertexShader vertexShader;
	GeometryShader geometryShader;
	FragmentShader fragmentShader;
};

void shaderProgramSetDefaultGeometryShader(ShaderProgram* sp);


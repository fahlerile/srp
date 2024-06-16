#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "defines.h"

#pragma pack(push, 1)

typedef struct vec2 { float x, y; } vec2;
typedef struct vec2d { double x, y; } vec2d;

typedef struct vec3 { float x, y, z; } vec3;
typedef struct vec3d { double x, y, z; } vec3d;

typedef struct vec4 { float x, y, z, w; } vec4;
typedef struct vec4d { double x, y, z, w; } vec4d;
#define VEC4D_ZERO (vec4d) {0, 0, 0, 0}

#pragma pack(pop)

SRP_FORCEINLINE vec3d vec3dSubtract(vec3d a, vec3d b)
{
	return (vec3d) {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	};
}


SRP_FORCEINLINE vec4d vec4dAdd(vec4d a, vec4d b)
{
	return (vec4d) {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	};
}

SRP_FORCEINLINE vec4d vec4dMultiplyScalar(vec4d a, double b)
{
	return (vec4d) {
		a.x * b,
		a.y * b,
		a.z * b,
		a.w * b
	};
}

SRP_FORCEINLINE double vec4dIndex(vec4d a, uint8_t index)
{
	assert(index <= 3);
	return ((double*) &a)[index];
}


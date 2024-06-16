#pragma once

#include <stdint.h>

#pragma pack(push, 1)

typedef struct vec2 { float x, y; } vec2;
typedef struct vec2d { double x, y; } vec2d;

typedef struct vec3 { float x, y, z; } vec3;
typedef struct vec3d { double x, y, z; } vec3d;

typedef struct vec4 { float x, y, z, w; } vec4;
typedef struct vec4d { double x, y, z, w; } vec4d;
#define VEC4D_ZERO (vec4d) {0, 0, 0, 0}

#pragma pack(pop)

vec3d vec3dSubtract(vec3d a, vec3d b);

vec4d vec4dAdd(vec4d a, vec4d b);
vec4d vec4dMultiplyScalar(vec4d a, double b);
double vec4dIndex(vec4d a, uint8_t index);


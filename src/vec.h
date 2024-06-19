// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  `vec2`, vec2d, vec3, vec3d, vec4, vec4d */

#include <stdint.h>

/** @ingroup Vector
 *  @{ */

#pragma pack(push, 1)

typedef struct vec2 { float x, y; } vec2;
typedef struct vec2d { double x, y; } vec2d;
#define VEC2_ZERO (vec2) {0, 0}
#define VEC2D_ZERO (vec2d) {0, 0}

typedef struct vec3 { float x, y, z; } vec3;
typedef struct vec3d { double x, y, z; } vec3d;
#define VEC3_ZERO (vec3) {0, 0, 0}
#define VEC3D_ZERO (vec3d) {0, 0, 0}

typedef struct vec4 { float x, y, z, w; } vec4;
typedef struct vec4d { double x, y, z, w; } vec4d;
#define VEC4_ZERO (vec4) {0, 0, 0, 0}
#define VEC4D_ZERO (vec4d) {0, 0, 0, 0}

#pragma pack(pop)

/** Subtract two `vec3d`s */
vec3d vec3dSubtract(vec3d a, vec3d b);

/** Add two `vec4d`s */
vec4d vec4dAdd(vec4d a, vec4d b);
/** Multiply a `vec4d` by a `double` scalar */
vec4d vec4dMultiplyScalar(vec4d a, double b);
/** Index a `vec4d` */
double vec4dIndex(vec4d a, uint8_t index);

/** @} */  // defgroup Vector


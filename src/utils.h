#pragma once

#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "Color/Color.h"
#include "Matrix/Matrix.h"
#include "Vector/Vector.h"
#include "memoryUtils/memoryUtils.h"
#include "DynamicArray/DynamicArray.h"
#include "Renderer.h"
#include "log.h"
#include "constants.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define RADIANS(x) ((M_PI / 180) * (x))
#define ROUGHLY_EQUAL(a, b) \
    (fabs((a) - (b)) < TOLERANCE)

#define SWAP(a, b) \
    do \
    { \
        unsigned char temp[(sizeof(a) == sizeof(b)) ? (int) sizeof(a) : -1]; \
        memcpy(temp, &a, sizeof(a)); \
        memcpy(&a, &b, sizeof(a)); \
        memcpy(&b, temp, sizeof(a)); \
    } while (false)

inline static Vector3d NDCtoScreenSpace(Renderer* renderer, Vector4d NDC)
{
    Vector2i dimensions = rendererGetWindowDimensions(renderer);
    return (Vector3d) {
         ((double) dimensions.x / 2) * (NDC.x + 1),
        -((double) dimensions.y / 2) * (NDC.y - 1),
        NDC.z
    };
}


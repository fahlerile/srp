#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "Color/Color.h"
#include "Matrix/Matrix.h"
#include "Vector/Vector.h"
#include "memoryUtils/memoryUtils.h"
#include "DynamicArray/DynamicArray.h"
#include "log.h"

#include "Renderer.h"
#include "constants.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define RADIANS(x) ((M_PI / 180) * (x))
#define ROUGHLY_EQUAL(a, b) (fabs((a) - (b)) < TOLERANCE)

#define INDEX_VOID_PTR(ptr, i, bytesPerElem) \
    ((uint8_t*) (ptr) + (i) * (bytesPerElem))
#define VOID_PTR_ADD(ptr, increment) \
    ((uint8_t*) (ptr) + (increment))

#define SWAP(a, b) \
    do \
    { \
        unsigned char temp[(sizeof(a) == sizeof(b)) ? (int) sizeof(a) : -1]; \
        memcpy(temp, &a, sizeof(a)); \
        memcpy(&a, &b, sizeof(a)); \
        memcpy(&b, temp, sizeof(a)); \
    } while (false)


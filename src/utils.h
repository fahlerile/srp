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

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
#define TOLERANCE 10e-6

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define CLAMP_MIN(min, val) ( ((val) < (min)) ? (min) : (val) )
#define CLAMP_MAX(max, val) ( ((val) > (max)) ? (max) : (val) )
#define CLAMP(min, max, val) \
	(\
		((val) < (min)) ? (min) : (\
			((val) > (max)) ? (max) : (val)\
		)\
	)

#define RADIANS(x) ((M_PI / 180) * (x))
#define ROUGHLY_EQUAL(a, b) (fabs((a) - (b)) < TOLERANCE)

#define SWAP(a, b) \
	do \
	{ \
		unsigned char temp[(sizeof(a) == sizeof(b)) ? (int) sizeof(a) : -1]; \
		memcpy(temp, &a, sizeof(a)); \
		memcpy(&a, &b, sizeof(a)); \
		memcpy(&b, temp, sizeof(a)); \
	} while (false)


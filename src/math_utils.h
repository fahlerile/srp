#pragma once

#ifdef SRP_SOURCE

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define CLAMP(min, max, value) \
	( \
		(value < min) ? \
			(min) : \
			(value > max) ? \
				(max) : \
				(value) \
	)

#define FRACTIONAL(x) ( (x) - floor(x) )

#endif  // ifdef SRP_SOURCE

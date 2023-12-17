#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t r, g, b, a;
} Color;

#include "Vector/Vector4.h"  // circular dependency

Vector4d ColorToVector4d(Color color);

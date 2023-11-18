#pragma once

typedef struct
{
    int x, y, z, w;
} Vector4i;

typedef struct
{
    double x, y, z, w;
} Vector4d;

#include "Utils/Color.h"  // circular dependency

double   Vector4dMagnitude(Vector4d a);
Vector4d Vector4dAdd      (Vector4d a, Vector4d b);
Vector4d Vector4dSubtract (Vector4d a, Vector4d b);
Vector4d Vector4dMultiplyD(Vector4d a, double b);
Vector4d Vector4dDivideD  (Vector4d a, double b);
Vector4d Vector4dNegate   (Vector4d a);
double   Vector4dDot      (Vector4d a, Vector4d b);

Color Vector4dToColor(Vector4d a);

double Vector4dIndex(Vector4d a, size_t i);

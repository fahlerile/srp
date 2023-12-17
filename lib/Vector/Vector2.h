#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    int x, y;
} Vector2i;

typedef struct
{
    double x, y;
} Vector2d;

double   Vector2dMagnitude(Vector2d a);
Vector2d Vector2dNormalize(Vector2d a);
Vector2d Vector2dAdd      (Vector2d a, Vector2d b);
Vector2d Vector2dSubtract (Vector2d a, Vector2d b);
Vector2d Vector2dMultiplyD(Vector2d a, double b);
Vector2d Vector2dDivideD  (Vector2d a, double b);
Vector2d Vector2dNegate   (Vector2d a);
double   Vector2dDot      (Vector2d a, Vector2d b);

double Vector2dIndex(Vector2d a, size_t i);
bool Vector2dEqual(Vector2d a, Vector2d b);

#pragma once

typedef struct
{
    int x, y;
} Vector2i;

typedef struct
{
    double x, y;
} Vector2d;

double   MagnitudeVector2d(Vector2d a);
Vector2d Vector2dAdd      (Vector2d a, Vector2d b);
Vector2d Vector2dSubtract (Vector2d a, Vector2d b);
double   Vector2dDot      (Vector2d a, Vector2d b);

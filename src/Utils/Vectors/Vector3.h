#pragma once

typedef struct
{
    int x, y, z;
} Vector3i;

typedef struct
{
    double x, y, z;
} Vector3d;

double   MagnitudeVector3d(Vector3d a);
Vector3d Vector3dAdd      (Vector3d a, Vector3d b);
Vector3d Vector3dSubtract (Vector3d a, Vector3d b);
double   Vector3dDot      (Vector3d a, Vector3d b);
Vector3d Vector3dCross    (Vector3d a, Vector3d b);

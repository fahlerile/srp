#pragma once

typedef struct
{
    int x, y, z;
} Vector3i;

typedef struct
{
    double x, y, z;
} Vector3d;

double   Vector3dMagnitude(Vector3d a);
Vector3d Vector3dAdd      (Vector3d a, Vector3d b);
Vector3d Vector3dSubtract (Vector3d a, Vector3d b);
Vector3d Vector3dMultiplyD(Vector3d a, double b);
Vector3d Vector3dDivideD  (Vector3d a, double b);
Vector3d Vector3dNegate   (Vector3d a);
double   Vector3dDot      (Vector3d a, Vector3d b);
Vector3d Vector3dCross    (Vector3d a, Vector3d b);

#include <math.h>
#include "Vector3.h"
#include "Utils/Utils.h"

double MagnitudeVector3d(Vector3d a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3d Vector3dAdd(Vector3d a, Vector3d b)
{
    return (Vector3d) {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3d Vector3dSubtract(Vector3d a, Vector3d b)
{
    return (Vector3d) {a.x - b.x, a.y - b.y, a.z - b.z};
}

double Vector3dDot(Vector3d a, Vector3d b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3d Vector3dCross(Vector3d a, Vector3d b)
{
    return (Vector3d) {
        a.y * b.z - a.z * b.y,
        a.x * b.z - a.z * b.x,
        a.x * b.y - a.y * b.x
    };
}

#include <math.h>
#include "Vector4.h"
#include "Utils/Utils.h"

double Vector4dMagnitude(Vector4d a)
{
    return sqrt(a.x * a.x +
                a.y * a.y +
                a.z * a.z +
                a.w * a.w);
}

Vector4d Vector4dAdd(Vector4d a, Vector4d b)
{
    return (Vector4d) {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    };
}

Vector4d Vector4dSubtract(Vector4d a, Vector4d b)
{
    return (Vector4d) {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    };
}

Vector4d Vector4dMultiplyD(Vector4d a, double b)
{
    return (Vector4d) {
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b
    };
}

Vector4d Vector4dDivideD(Vector4d a, double b)
{
    return (Vector4d) {
        a.x / b,
        a.y / b,
        a.z / b,
        a.w / b
    };
}

Vector4d Vector4dNegate(Vector4d a)
{
    return (Vector4d) {
        -a.x,
        -a.y,
        -a.z,
        -a.w
    };
}

double Vector4dDot(Vector4d a, Vector4d b)
{
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z +
           a.w * b.w;
}

Color Vector4dToColor(Vector4d a)
{
    assert(a.x >= 0 && a.y >= 0 && a.z >= 0 && a.w >= 0);
    return (Color) {
        (a.x > 255) ? 255 : a.x,
        (a.y > 255) ? 255 : a.y,
        (a.z > 255) ? 255 : a.z,
        (a.w > 255) ? 255 : a.w
    };
}

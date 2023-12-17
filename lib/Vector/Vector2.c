#include "Vector2.h"
#include "utils/utils.h"

double Vector2dMagnitude(Vector2d a)
{
    return sqrt(a.x * a.x +
                a.y * a.y);
}

Vector2d Vector2dNormalize(Vector2d a)
{
    return Vector2dDivideD(a, Vector2dMagnitude(a));
}

Vector2d Vector2dAdd(Vector2d a, Vector2d b)
{
    return (Vector2d) {
        a.x + b.x,
        a.y + b.y
    };
}

Vector2d Vector2dSubtract(Vector2d a, Vector2d b)
{
    return (Vector2d) {
        a.x - b.x,
        a.y - b.y
    };
}

Vector2d Vector2dMultiplyD(Vector2d a, double b)
{
    return (Vector2d) {
        a.x * b,
        a.y * b
    };
}

Vector2d Vector2dDivideD  (Vector2d a, double b)
{
    return (Vector2d) {
        a.x / b,
        a.y / b
    };
}

Vector2d Vector2dNegate(Vector2d a)
{
    return (Vector2d) {-a.x, -a.y};
}

double Vector2dDot(Vector2d a, Vector2d b)
{
    return a.x * b.x +
           a.y * b.y;
}

double Vector2dIndex(Vector2d a, size_t i)
{
    switch (i)
    {
        case 0:
            return a.x;
        case 1:
            return a.y;
        default:
            LOGE("Vector2d invalid index %zu", i);
            abort();
    }
}

bool Vector2dEqual(Vector2d a, Vector2d b)
{
    return (a.x == b.x &&
            a.y == b.y);
}

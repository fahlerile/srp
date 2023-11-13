#include <math.h>
#include "Vector2.h"
#include "Utils/Utils.h"

double MagnitudeVector2d(Vector2d a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

Vector2d Vector2dAdd(Vector2d a, Vector2d b)
{
    return (Vector2d) {a.x + b.x, a.y + b.y};
}

Vector2d Vector2dSubtract(Vector2d a, Vector2d b)
{
    return (Vector2d) {a.x - b.x, a.y - b.y};
}

double Vector2dDot(Vector2d a, Vector2d b)
{
    return a.x * b.x + a.y * b.y;
}

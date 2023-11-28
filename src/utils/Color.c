#include "Color.h"

Vector4d ColorToVector4d(Color color)
{
    return (Vector4d) {
        color.r,
        color.g,
        color.b,
        color.a
    };
}

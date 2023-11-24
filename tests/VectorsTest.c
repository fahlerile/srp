#include <assert.h>
#include <math.h>
#include "utils/Vectors/Vectors.h"
#include "utils/compares.h"

int test_2()
{
    Vector2d a = {3.0, 4.0};
    Vector2d b = {10.2, 5.4};

    assert(Vector2dMagnitude(a) == 5.0);

    Vector2d c = Vector2dAdd(a, b);
    assert(c.x == (3.0 + 10.2));
    assert(c.y == (4.0 + 5.4));

    c = Vector2dSubtract(a, b);
    assert(c.x == (3.0 - 10.2));
    assert(c.y == (4.0 - 5.4));

    c = Vector2dMultiplyD(a, 2.0);
    assert(c.x == (3.0 * 2.0));
    assert(c.y == (4.0 * 2.0));

    c = Vector2dDivideD(a, 2.0);
    assert(c.x == (3.0 / 2.0));
    assert(c.y == (4.0 / 2.0));

    c = Vector2dNegate(a);
    assert(c.x == -3.0);
    assert(c.y == -4.0);

    assert(roughlyEqualD(Vector2dDot(a, b), 52.2) == true);
    assert(Vector2dIndex(a, 1) == 4.0);
}

int test_3()
{
    Vector3d a = {3.0, 4.0, 5.0};
    Vector3d b = {10.2, 5.4, 5.2};

    assert(Vector3dMagnitude(a) == sqrt(50));

    Vector3d c = Vector3dAdd(a, b);
    assert(c.x == (3.0 + 10.2));
    assert(c.y == (4.0 + 5.4));
    assert(c.z == (5.0 + 5.2));

    c = Vector3dSubtract(a, b);
    assert(c.x == (3.0 - 10.2));
    assert(c.y == (4.0 - 5.4));
    assert(c.z == (5.0 - 5.2));

    c = Vector3dMultiplyD(a, 2.0);
    assert(c.x == (3.0 * 2.0));
    assert(c.y == (4.0 * 2.0));
    assert(c.z == (5.0 * 2.0));

    c = Vector3dDivideD(a, 2.0);
    assert(c.x == (3.0 / 2.0));
    assert(c.y == (4.0 / 2.0));
    assert(c.z == (5.0 / 2.0));

    c = Vector3dNegate(a);
    assert(c.x == -3.0);
    assert(c.y == -4.0);
    assert(c.z == -5.0);

    assert(roughlyEqualD(Vector3dDot(a, b), 78.2) == true);
    assert(Vector3dIndex(a, 2) == 5.0);
}

int test_4()
{
    Vector4d a = {3.0, 4.0, 5.0, 2.3};
    Vector4d b = {10.2, 5.4, 5.2, 7.3};

    assert(Vector4dMagnitude(a) == sqrt(55.29));

    Vector4d c = Vector4dAdd(a, b);
    assert(c.x == (3.0 + 10.2));
    assert(c.y == (4.0 + 5.4));
    assert(c.z == (5.0 + 5.2));
    assert(c.w == (2.3 + 7.3));

    c = Vector4dSubtract(a, b);
    assert(c.x == (3.0 - 10.2));
    assert(c.y == (4.0 - 5.4));
    assert(c.z == (5.0 - 5.2));
    assert(c.w == (2.3 - 7.3));

    c = Vector4dMultiplyD(a, 2.0);
    assert(c.x == (3.0 * 2.0));
    assert(c.y == (4.0 * 2.0));
    assert(c.z == (5.0 * 2.0));
    assert(c.w == (2.3 * 2.0));

    c = Vector4dDivideD(a, 2.0);
    assert(c.x == (3.0 / 2.0));
    assert(c.y == (4.0 / 2.0));
    assert(c.z == (5.0 / 2.0));
    assert(c.w == (2.3 / 2.0));

    c = Vector4dNegate(a);
    assert(c.x == -3.0);
    assert(c.y == -4.0);
    assert(c.z == -5.0);
    assert(c.w == -2.3);

    assert(roughlyEqualD(Vector4dDot(a, b), 94.99) == true);
    assert(Vector4dIndex(a, 3) == 2.3);
}

int main()
{
    test_2();
    test_3();
    test_4();
}

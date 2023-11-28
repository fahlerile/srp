#include <assert.h>
#include <math.h>
#include "utils/Vectors/Vectors.h"
#include "utils/compares.h"

void test2()
{
    Vector2d a = {3., 4.};
    Vector2d b = {10.2, 5.4};

    assert(Vector2dMagnitude(a) == 5.);
    assert(Vector2dEqual(Vector2dNormalize(a), (Vector2d) {3. / 5., 4. / 5.}));

    Vector2d c = Vector2dAdd(a, b);
    assert(Vector2dEqual(c, (Vector2d) {3. + 10.2, 4. + 5.4}));

    c = Vector2dSubtract(a, b);
    assert(Vector2dEqual(c, (Vector2d) {3. - 10.2, 4. - 5.4}));

    c = Vector2dMultiplyD(a, 2.);
    assert(Vector2dEqual(c, (Vector2d) {3. * 2., 4. * 2.}));

    c = Vector2dDivideD(a, 2.);
    assert(Vector2dEqual(c, (Vector2d) {3. / 2., 4. / 2.}));

    c = Vector2dNegate(a);
    assert(Vector2dEqual(c, (Vector2d) {-3., -4.}));

    assert(roughlyEqualD(Vector2dDot(a, b), 52.2) == true);
    assert(Vector2dIndex(a, 1) == 4.);
}

void test3()
{
    Vector3d a = {3., 4., 5.};
    Vector3d b = {10.2, 5.4, 5.2};

    assert(Vector3dMagnitude(a) == sqrt(50));
    assert(Vector3dEqual(Vector3dNormalize(a), (Vector3d) {3. / sqrt(50), 4. / sqrt(50), 5. / sqrt(50)}));

    Vector3d c = Vector3dAdd(a, b);
    assert(Vector3dEqual(c, (Vector3d) {3. + 10.2, 4. + 5.4, 5. + 5.2}));

    c = Vector3dSubtract(a, b);
    assert(Vector3dEqual(c, (Vector3d) {3. - 10.2, 4. - 5.4, 5. - 5.2}));

    c = Vector3dMultiplyD(a, 2.);
    assert(Vector3dEqual(c, (Vector3d) {3. * 2., 4. * 2., 5. * 2.}));

    c = Vector3dDivideD(a, 2.);
    assert(Vector3dEqual(c, (Vector3d) {3. / 2., 4. / 2., 5. / 2.}));

    c = Vector3dNegate(a);
    assert(Vector3dEqual(c, (Vector3d) {-3., -4., -5.}));

    assert(roughlyEqualD(Vector3dDot(a, b), 78.2) == true);
    assert(Vector3dIndex(a, 2) == 5.);
}

void test4()
{
    Vector4d a = {3., 4., 5., 2.3};
    Vector4d b = {10.2, 5.4, 5.2, 7.3};

    assert(Vector4dMagnitude(a) == sqrt(55.29));
    assert(Vector4dEqual(Vector4dNormalize(a), (Vector4d) {3. / sqrt(55.29), 4. / sqrt(55.29), 5. / sqrt(55.29), 2.3 / sqrt(55.29)}));

    Vector4d c = Vector4dAdd(a, b);
    assert(Vector4dEqual(c, (Vector4d) {3. + 10.2, 4. + 5.4, 5. + 5.2, 2.3 + 7.3}));

    c = Vector4dSubtract(a, b);
    assert(Vector4dEqual(c, (Vector4d) {3. - 10.2, 4. - 5.4, 5. - 5.2, 2.3 - 7.3}));

    c = Vector4dMultiplyD(a, 2.);
    assert(Vector4dEqual(c, (Vector4d) {3. * 2., 4. * 2., 5. * 2., 2.3 * 2.}));

    c = Vector4dDivideD(a, 2.);
    assert(Vector4dEqual(c, (Vector4d) {3. / 2., 4. / 2., 5. / 2., 2.3 / 2.}));

    c = Vector4dNegate(a);
    assert(Vector4dEqual(c, (Vector4d) {-3., -4., -5., -2.3}));

    assert(roughlyEqualD(Vector4dDot(a, b), 94.99) == true);
    assert(Vector4dIndex(a, 3) == 2.3);
}

int main()
{
    test2();
    test3();
    test4();
}

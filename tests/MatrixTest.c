#include <stdbool.h>
#include <assert.h>
#include "utils/Matrix.h"

void testEqual()
{
    Matrix4 A = {{
        {0, 1, 2, 5},
        {2, 6, 2, 8},
        {3, 0, 3, 8},
        {1, 7, 2, 2}
    }};
    Matrix4 B = Matrix4ConstructIdentity();

    assert(Matrix4Equal(&A, &A));
    assert(Matrix4Equal(&A, &B) == false);
}

void testConstruct()
{
    Matrix4 A;

    A = Matrix4ConstructIdentity();
    assert(Matrix4Equal(
        &A,
        &(Matrix4) {{
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1},
        }}
    ));

    A = Matrix4ConstructScale((Vector3d) {2, 3, 4});
    assert(Matrix4Equal(
        &A,
        &(Matrix4) {{
            {2, 0, 0, 0},
            {0, 3, 0, 0},
            {0, 0, 4, 0},
            {0, 0, 0, 1},
        }}
    ));

    A = Matrix4ConstructTranslate((Vector3d) {2, 3, 4});
    assert(Matrix4Equal(
        &A,
        &(Matrix4) {{
            {1, 0, 0, 2},
            {0, 1, 0, 3},
            {0, 0, 1, 4},
            {0, 0, 0, 1},
        }}
    ));

    // I do not want to write this test :(
    // A = Matrix4ConstructRotate((Vector3d) {});
    // assert(Matrix4Equal(
    //     &A,
    //     &(Matrix4) {{
    //         {1, 0, 0, 0},
    //         {0, 1, 0, 0},
    //         {0, 0, 1, 0},
    //         {0, 0, 0, 1},
    //     }}
    // ));
}

void testMultiply()
{
    Matrix4 A = {{
        {4, 5, 2, 6},
        {1, 7, 8, 9},
        {2, 4, 2, 7},
        {2, 7, 2, 7}
    }};
    Matrix4 B = {{
        {7, 3, 2, 8},
        {4, 6, 8, 1},
        {4, 3, 2, 1},
        {9, 3, 8, 2}
    }};

    Matrix4 AB = Matrix4MultiplyMatrix4(&A, &B);
    Matrix4 BA = Matrix4MultiplyMatrix4(&B, &A);
    assert(Matrix4Equal(
        &AB,
        &(Matrix4) {{
            {110, 66, 100, 51},
            {148, 96, 146, 41},
            {101, 57,  96, 36},
            {113, 75, 120, 39},
        }}
    ));
    assert(Matrix4Equal(
        &BA,
        &(Matrix4) {{
            {51, 120, 58, 139},
            {40, 101, 74, 141},
            {25,  56, 38,  72},
            {59, 112, 62, 151},
        }}
    ));

    Vector4d v = {4, 2, 6, 1};
    assert(Vector4dEqual(
        Matrix4MultiplyVector4d(&A, v), (Vector4d) {44, 75, 35, 41}
    ));
}


int main()
{
    testEqual();
    testConstruct();
    testMultiply();
}

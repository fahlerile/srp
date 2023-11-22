#include <math.h>
#include <assert.h>
#include "Matrix.h"

static inline Vector4d Matrix4GetColumn(Matrix4* a, size_t i)
{
    assert(i <= 3);
    return (Vector4d) {
        a->data[0][i],
        a->data[1][i],
        a->data[2][i],
        a->data[3][i]
    };
}

static inline void Matrix4SetColumn(Matrix4* a, Vector4d column, size_t i)
{
    assert(i <= 3);
    a->data[0][i] = column.x;
    a->data[1][i] = column.y;
    a->data[2][i] = column.z;
    a->data[3][i] = column.w;
}

Vector4d Matrix4MultiplyVector4d(Matrix4* a, Vector4d b)
{
    Vector4d res = {0};
    for (size_t i = 0; i < 4; i++)
    {
        res = Vector4dAdd(res, Vector4dMultiplyD(
            Matrix4GetColumn(a, i), Vector4dIndex(b, i)
        ));
    }
    return res;
}

Matrix4 Matrix4MultiplyMatrix4(Matrix4* a, Matrix4* b)
{
    Matrix4 res = {0};
    for (size_t i = 0; i < 4; i++)
    {
        Vector4d column = Matrix4MultiplyVector4d(a, Matrix4GetColumn(b, i));
        Matrix4SetColumn(&res, column, i);
    }
    return res;
}

Matrix4 Matrix4ConstructIdentity()
{
    Matrix4 res = {
        .data = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    return res;
}

Matrix4 Matrix4ConstructScale(double scaleX, double scaleY, double scaleZ)
{
    Matrix4 res = {
        .data = {
            {scaleX,      0,      0, 0},
            {     0, scaleY,      0, 0},
            {     0,      0, scaleZ, 0},
            {     0,      0,    0,   1}
        }
    };
    return res;
}

Matrix4 Matrix4ConstructTranslate(double transX, double transY, double transZ)
{
    Matrix4 res = {
        .data = {
            {1, 0, 0, transX},
            {0, 1, 0, transY},
            {0, 0, 1, transZ},
            {0, 0, 0,      1}
        }
    };
    return res;
}

Matrix4 Matrix4ConstructRotate(double rotX, double rotY, double rotZ)
{
    Matrix4 res = {0};
    res.data[0][0] = cos(rotY) * cos(rotZ);
    res.data[0][1] = sin(rotX) * sin(rotY) * cos(rotZ) - cos(rotX) * sin(rotZ);
    res.data[0][2] = cos(rotX) * sin(rotY) * cos(rotZ) + sin(rotX) * sin(rotZ);
    res.data[0][3] = 0;
    res.data[1][0] = cos(rotY) * sin(rotZ);
    res.data[1][1] = sin(rotX) * sin(rotY) * sin(rotZ) + cos(rotX) * cos(rotZ);
    res.data[1][2] = cos(rotX) * sin(rotY) * sin(rotZ) - sin(rotX) * cos(rotZ);
    res.data[1][3] = 0;
    res.data[2][0] = -sin(rotY);
    res.data[2][1] = sin(rotX) * cos(rotY);
    res.data[2][2] = cos(rotX) * cos(rotY);
    res.data[2][3] = 0;
    res.data[3][0] = 0;
    res.data[3][1] = 0;
    res.data[3][2] = 0;
    res.data[3][3] = 1;
    return res;
}

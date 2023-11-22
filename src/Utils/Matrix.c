#include <math.h>
#include <assert.h>
#include "Matrix.h"

#define Matrix4GetIndex(x, y) (y*4 + x)

static inline Vector4d Matrix4GetColumn(Matrix4* a, size_t i)
{
    assert(i <= 3);
    return (Vector4d) {
        a->data[Matrix4GetIndex(i, 0)],
        a->data[Matrix4GetIndex(i, 1)],
        a->data[Matrix4GetIndex(i, 2)],
        a->data[Matrix4GetIndex(i, 3)]
    };
}

static inline void Matrix4SetColumn(Matrix4* a, Vector4d column, size_t i)
{
    assert(i <= 3);
    a->data[Matrix4GetIndex(i, 0)] = column.x;
    a->data[Matrix4GetIndex(i, 1)] = column.y;
    a->data[Matrix4GetIndex(i, 2)] = column.z;
    a->data[Matrix4GetIndex(i, 3)] = column.w;
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
    Matrix4 res = {0};
    res.data[Matrix4GetIndex(0, 0)] = 1;
    res.data[Matrix4GetIndex(1, 1)] = 1;
    res.data[Matrix4GetIndex(2, 2)] = 1;
    res.data[Matrix4GetIndex(3, 3)] = 1;
    return res;
}

Matrix4 Matrix4ConstructScale(double scaleX, double scaleY, double scaleZ)
{
    Matrix4 res = {0};
    res.data[Matrix4GetIndex(0, 0)] = scaleX;
    res.data[Matrix4GetIndex(1, 1)] = scaleY;
    res.data[Matrix4GetIndex(2, 2)] = scaleZ;
    res.data[Matrix4GetIndex(3, 3)] = 1;
    return res;
}

Matrix4 Matrix4ConstructTranslate(double transX, double transY, double transZ)
{
    Matrix4 res = {0};
    res.data[Matrix4GetIndex(3, 0)] = transX;
    res.data[Matrix4GetIndex(3, 1)] = transY;
    res.data[Matrix4GetIndex(3, 2)] = transZ;
    res.data[Matrix4GetIndex(3, 3)] = 1;
    return res;
}

Matrix4 Matrix4ConstructRotate(double rotX, double rotY, double rotZ)
{
    Matrix4 res = {0};
    res.data[Matrix4GetIndex(0, 0)] = cos(rotY) * cos(rotZ);
    res.data[Matrix4GetIndex(1, 0)] = sin(rotX) * sin(rotY) * cos(rotZ) - cos(rotX) * sin(rotZ);
    res.data[Matrix4GetIndex(2, 0)] = cos(rotX) * sin(rotY) * cos(rotZ) + sin(rotX) * sin(rotZ);
    res.data[Matrix4GetIndex(3, 0)] = 0;
    res.data[Matrix4GetIndex(0, 1)] = cos(rotY) * sin(rotZ);
    res.data[Matrix4GetIndex(1, 1)] = sin(rotX) * sin(rotY) * sin(rotZ) + cos(rotX) * cos(rotZ);
    res.data[Matrix4GetIndex(2, 1)] = cos(rotX) * sin(rotY) * sin(rotZ) - sin(rotX) * cos(rotZ);
    res.data[Matrix4GetIndex(3, 1)] = 0;
    res.data[Matrix4GetIndex(0, 2)] = -sin(rotY);
    res.data[Matrix4GetIndex(1, 2)] = sin(rotX) * cos(rotY);
    res.data[Matrix4GetIndex(2, 2)] = cos(rotX) * cos(rotY);
    res.data[Matrix4GetIndex(3, 2)] = 0;
    res.data[Matrix4GetIndex(0, 3)] = 0;
    res.data[Matrix4GetIndex(1, 3)] = 0;
    res.data[Matrix4GetIndex(2, 3)] = 0;
    res.data[Matrix4GetIndex(3, 3)] = 1;
    return res;
}

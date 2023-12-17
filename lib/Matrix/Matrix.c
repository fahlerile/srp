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

bool Matrix4Equal(Matrix4* a, Matrix4* b)
{
    return (a->data[0][0] == b->data[0][0] &&
            a->data[0][1] == b->data[0][1] &&
            a->data[0][2] == b->data[0][2] &&
            a->data[0][3] == b->data[0][3] &&
            a->data[1][0] == b->data[1][0] &&
            a->data[1][1] == b->data[1][1] &&
            a->data[1][2] == b->data[1][2] &&
            a->data[1][3] == b->data[1][3] &&
            a->data[2][0] == b->data[2][0] &&
            a->data[2][1] == b->data[2][1] &&
            a->data[2][2] == b->data[2][2] &&
            a->data[2][3] == b->data[2][3] &&
            a->data[3][0] == b->data[3][0] &&
            a->data[3][1] == b->data[3][1] &&
            a->data[3][2] == b->data[3][2] &&
            a->data[3][3] == b->data[3][3]);
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

Matrix4 Matrix4ConstructScale(Vector3d scale)
{
    Matrix4 res = {
        .data = {
            {scale.x,       0,       0, 0},
            {      0, scale.y,       0, 0},
            {      0,       0, scale.z, 0},
            {      0,       0,       0, 1}
        }
    };
    return res;
}

Matrix4 Matrix4ConstructTranslate(Vector3d trans)
{
    Matrix4 res = {
        .data = {
            {1, 0, 0, trans.x},
            {0, 1, 0, trans.y},
            {0, 0, 1, trans.z},
            {0, 0, 0,       1}
        }
    };
    return res;
}

Matrix4 Matrix4ConstructRotate(Vector3d rot)
{
    Matrix4 res = {0};
    res.data[0][0] = cos(rot.y) * cos(rot.z);
    res.data[0][1] = sin(rot.x) * sin(rot.y) * cos(rot.z) - cos(rot.x) * sin(rot.z);
    res.data[0][2] = cos(rot.x) * sin(rot.y) * cos(rot.z) + sin(rot.x) * sin(rot.z);
    res.data[0][3] = 0;
    res.data[1][0] = cos(rot.y) * sin(rot.z);
    res.data[1][1] = sin(rot.x) * sin(rot.y) * sin(rot.z) + cos(rot.x) * cos(rot.z);
    res.data[1][2] = cos(rot.x) * sin(rot.y) * sin(rot.z) - sin(rot.x) * cos(rot.z);
    res.data[1][3] = 0;
    res.data[2][0] = -sin(rot.y);
    res.data[2][1] = sin(rot.x) * cos(rot.y);
    res.data[2][2] = cos(rot.x) * cos(rot.y);
    res.data[2][3] = 0;
    res.data[3][0] = 0;
    res.data[3][1] = 0;
    res.data[3][2] = 0;
    res.data[3][3] = 1;
    return res;
}

Matrix4 Matrix4ConstructTRS(Vector3d trans, Vector3d rot, Vector3d scale)
{
    Matrix4 T = Matrix4ConstructTranslate(trans);
    Matrix4 R = Matrix4ConstructRotate(rot);
    Matrix4 S = Matrix4ConstructScale(scale);
    Matrix4 RS = Matrix4MultiplyMatrix4(&R, &S);
    return Matrix4MultiplyMatrix4(&T, &RS);
}

Matrix4 Matrix4ConstructView(Vector3d trans, Vector3d rot, Vector3d scale)
{
    return Matrix4ConstructTRS(Vector3dNegate(trans), Vector3dNegate(rot), scale);
}

Matrix4 Matrix4ConstructOrthogonalProjection(
    double left, double right,
    double bottom, double top,
    double near, double far
)
{
    Matrix4 res = {0};
    res.data[0][0] = 2 / (right - left);
    res.data[0][1] = 0;
    res.data[0][2] = 0;
    res.data[0][3] = -(2*left / right - left) - 1;
    res.data[1][0] = 0;
    res.data[1][1] = 2 / (top - bottom);
    res.data[1][2] = 0;
    res.data[1][3] = -(2*bottom / (top - bottom)) - 1;
    res.data[2][0] = 0;
    res.data[2][1] = 0;
    res.data[2][2] = -2 / (far - near);
    res.data[2][3] = -(2*near / (far - near)) - 1;
    res.data[3][0] = 0;
    res.data[3][1] = 0;
    res.data[3][2] = 0;
    res.data[3][3] = 1;
    return res;
}

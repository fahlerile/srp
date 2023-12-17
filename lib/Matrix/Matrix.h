#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "Vector/Vector.h"

typedef struct
{
    double data[4][4];
} Matrix4;

static inline Vector4d Matrix4GetColumn(Matrix4* a, size_t i);
static inline void Matrix4SetColumn(Matrix4* a, Vector4d column, size_t i);

bool Matrix4Equal(Matrix4* a, Matrix4* b);

Vector4d Matrix4MultiplyVector4d(Matrix4* a, Vector4d b);
Matrix4 Matrix4MultiplyMatrix4(Matrix4* a, Matrix4* b);

Matrix4 Matrix4ConstructIdentity();
Matrix4 Matrix4ConstructScale(Vector3d scale);
Matrix4 Matrix4ConstructTranslate(Vector3d trans);
Matrix4 Matrix4ConstructRotate(Vector3d rot);
Matrix4 Matrix4ConstructTRS(Vector3d trans, Vector3d rot, Vector3d scale);
Matrix4 Matrix4ConstructView(Vector3d trans, Vector3d rot, Vector3d scale);

Matrix4 Matrix4ConstructOrthogonalProjection(
    double left, double right,
    double bottom, double top,
    double near, double far
);

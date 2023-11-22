#pragma once
#include <stdint.h>
#include "Vectors/Vectors.h"

typedef struct
{
    double data[16];
} Matrix4;

static inline Vector4d Matrix4GetColumn(Matrix4* a, size_t i);
static inline void Matrix4SetColumn(Matrix4* a, Vector4d column, size_t i);

Vector4d Matrix4MultiplyVector4d(Matrix4* a, Vector4d b);
Matrix4 Matrix4MultiplyMatrix4(Matrix4* a, Matrix4* b);

Matrix4 Matrix4ConstructIdentity();
Matrix4 Matrix4ConstructScale(double scaleX, double scaleY, double scaleZ);
Matrix4 Matrix4ConstructTranslate(double transX, double transY, double transZ);
Matrix4 Matrix4ConstructRotate(double rotX, double rotY, double rotZ);
// Matrix4 Matrix4ConstructView();
// Matrix4 Matrix4ConstructProjection();

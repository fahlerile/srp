#pragma once

#include "vec.h"

// ROW MAJOR
typedef struct mat4 { float data[4][4]; } mat4;
typedef struct mat4d { double data[4][4]; } mat4d;

vec4d mat4dGetColumn(const mat4d* a, uint8_t index);
void mat4dSetColumn(mat4d* a, vec4d column, uint8_t index);

vec4d mat4dMultiplyVec4d(const mat4d* a, vec4d b);
mat4d mat4dMultiplyMat4d(const mat4d* a, mat4d* b);

mat4d mat4dConstructIdentity();
mat4d mat4dConstructScale(double x, double y, double z);
mat4d mat4dConstructTranslate(double x, double y, double z);
mat4d mat4dConstructRotate(double x, double y, double z);
mat4d mat4dConstructTRS(
	double transX, double transY, double transZ,
	double rotataionX, double rotataionY, double rotataionZ,
	double scaleX, double scaleY, double scaleZ
);
mat4d mat4dConstructView(
	double transX, double transY, double transZ,
	double rotataionX, double rotataionY, double rotataionZ,
	double scaleX, double scaleY, double scaleZ
);
mat4d mat4dConstructOrthogonalProjection(
	double x_min, double x_max,
	double y_min, double y_max,
	double z_min, double z_max
);
mat4d mat4dConstructPerspectiveProjection(
	double x_min_near, double x_max_near,
	double y_min_near, double y_max_near,
	double z_near, double z_far
);


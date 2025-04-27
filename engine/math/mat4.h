#ifndef __bse_math_mat4__
#define __bse_math_mat4__

#include <string.h>
#include <math.h>
#include "defs.h"
#include "vec3f.h"
#include "include.h"

typedef float Mat4[16];

inline void Mat4Identity(Mat4 mat)
{
  memset(mat, 0, sizeof(Mat4));
  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

void Mat4Multiply(Mat4 out, Mat4 a, Mat4 b);
void Mat4Translate(Mat4 mat, Vec3f trans);
void Mat4Scale(Mat4 mat, Vec3f scale);
void Mat4Rotate(Mat4 mat, Vec3f rotation);
void Mat4Perspective(Mat4 mat, float yfov, float a, float n, float f);
void Mat4Ortho(Mat4 mat, float l, float r, float b, float t, float n, float f);

#endif

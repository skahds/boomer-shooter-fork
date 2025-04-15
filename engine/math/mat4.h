#ifndef __bse_math_mat4__
#define __bse_math_mat4__

#include <string.h>
#include <math.h>
#include "defs.h"
#include "vec3f.h"

typedef float Mat4[16];

inline void mat4Identity(Mat4 mat)
{
  memset(mat, 0, sizeof(Mat4));
  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

void mat4Multiply(Mat4 out, Mat4 a, Mat4 b);
void mat4Translate(Mat4 mat, Vec3f trans);
void mat4Scale(Mat4 mat, Vec3f scale);
void mat4Rotate(Mat4 mat, Vec3f rotation);
void mat4Perspective(Mat4 mat, float yfov, float a, float n, float f);
void mat4Ortho(Mat4 mat, float l, float r, float b, float t, float n, float f);

#endif

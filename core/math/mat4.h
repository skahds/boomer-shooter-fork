#ifndef __bse_math_mat4__
#define __bse_math_mat4__

#include <string.h>
#include <math.h>
#include "defs.h"
#include "vec3f.h"
#include "include.h"

typedef float mat4_t[16];

void Mat4Identity(mat4_t mat);
void Mat4Multiply(mat4_t out, mat4_t a, mat4_t b);
void Mat4Translate(mat4_t mat, vec3f_t trans);
void Mat4Scale(mat4_t mat, vec3f_t scale);
void Mat4Rotate(mat4_t mat, vec3f_t rotation);
void Mat4Perspective(mat4_t mat, float yfov, float a, float n, float f);
void Mat4Ortho(mat4_t mat, float l, float r, float b, float t, float n, float f);

#endif

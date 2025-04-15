#include "mat4.h"

void mat4Multiply(Mat4 out, Mat4 a, Mat4 b)
{
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      out[col * 4 + row] = 0.0f;
      for (int k = 0; k < 4; k++) {
        out[col * 4 + row] += a[k * 4 + row] * b[col * 4 + k];
      }
    }
  }
}

void mat4Translate(Mat4 mat, Vec3f trans)
{
  mat4Identity(mat);
  mat[12] = trans.x;
  mat[13] = trans.y;
  mat[14] = trans.z;
}

void mat4Scale(Mat4 mat, Vec3f scale)
{
  mat4Identity(mat);
  mat[0] = scale.x;
  mat[5] = scale.y;
  mat[10] = scale.z;
}

void mat4Rotate(Mat4 mat, Vec3f rotation)
{
  float cosx = cosf(rotation.x);
  float sinx = sinf(rotation.x);

  float cosy = cosf(rotation.y);
  float siny = sinf(rotation.y);

  float cosz = cosf(rotation.z);
  float sinz = sinf(rotation.z);

  // Vec3f ra = (Vec3f){0, 1, 0};

  // mat4Identity(mat);

  mat[0] = cosy * cosz;
  mat[1] = cosy * sinz;
  mat[2] = -siny;
  mat[3] = 0;
  mat[4] = cosz * sinx * siny - cosx * sinz;
  mat[5] = cosx * cosz + sinx * siny * sinz;
  mat[6] = cosy * sinx;
  mat[7] = 0;
  mat[8] = cosx * cosz * siny + sinx * sinz;
  mat[9] = -cosz * sinx + cosx * siny * sinz;
  mat[10] = cosx * cosy;
  mat[11] = 0;
  mat[12] = 0;
  mat[13] = 0;
  mat[14] = 0;
  mat[15] = 1;
}

void mat4Perspective(Mat4 mat, float yfov, float a, float n, float f)
{
  // thank you love2d for letting me continue to not understand how this works
  // https://github.com/love2d/love/blob/main/src/common/Matrix.cpp#L459
  memset(mat, 0, sizeof(Mat4));

  float cotan = 1.0 / tanf(yfov * deg_to_rad * 0.5);
	mat[0] = cotan / a;
	mat[5] = cotan;
	mat[10] = (f + n) / (n - f);
	mat[11] = -1.0;
	mat[14] = 2.0 * n * f / (n - f);
}

void mat4Ortho(Mat4 mat, float l, float r, float b, float t, float n, float f)
{
  // https://github.com/love2d/love/blob/main/src/common/Matrix.cpp#L444
  mat4Identity(mat);

  mat[0] = 2.0 / (r - l);
	mat[5] = 2.0 / (t - b);
	mat[10] = -2.0 / (f - n);

	mat[12] = -(r + l) / (r - l);
	mat[13] = -(t + b) / (t - b);
	mat[14] = -(f + n) / (f - n);
}

#include "transform.h"

#include "mat4.h"

void transformToMatrix(Transform trans, Mat4 mat)
{
  Mat4 translation;
  mat4Translate(translation, trans.pos);

  Mat4 rotation;
  mat4Rotate(rotation, trans.rot);

  Mat4 scale;
  mat4Scale(scale, trans.scale);

  Mat4 inter;
  // mat4Multiply(inter, translation, rotation);

  mat4Multiply(inter, scale, rotation);
  mat4Multiply(mat, translation, inter);
}

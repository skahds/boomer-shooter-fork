#include "transform.h"

#include "mat4.h"

void TransformToMatrix(struct Transform trans, Mat4 mat)
{
  Mat4 translation;
  Mat4Translate(translation, trans.pos);

  Mat4 rotation;
  Mat4Rotate(rotation, trans.rot);

  Mat4 scale;
  Mat4Scale(scale, trans.scale);

  Mat4 inter;
  // mat4Multiply(inter, translation, rotation);

  // translation * rotation * scale
  Mat4Multiply(inter, rotation, scale);
  Mat4Multiply(mat, translation, inter);
}

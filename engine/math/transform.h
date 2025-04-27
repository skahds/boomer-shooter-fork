#ifndef __bse_math_transform__
#define __bse_math_transform__

#include "vec3f.h"
#include "mat4.h"

struct Transform
{
  Vec3f pos;
  Vec3f rot;
  Vec3f scale;
};

void TransformToMatrix(struct Transform trans, Mat4 mat);

#endif

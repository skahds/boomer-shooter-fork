#ifndef __bse_math_transform__
#define __bse_math_transform__

#include "vec3f.h"
#include "mat4.h"

typedef struct
{
  Vec3f pos;
  Vec3f rot;
  Vec3f scale;
} Transform;

void transformToMatrix(Transform trans, Mat4 mat);

#endif

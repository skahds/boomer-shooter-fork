#ifndef __bse_math_transform__
#define __bse_math_transform__

#include "vec3f.h"
#include "mat4.h"

struct Transform
{
  vec3f_t pos;
  vec3f_t rot;
  vec3f_t scale;
};

void TransformToMatrix(struct Transform trans, mat4_t mat);

#endif

#ifndef __bse_math_vec2f__
#define __bse_math_vec2f__

#include "include.h"

typedef struct
{
  union {
    float x;
    float w;
  };
  union {
    float y;
    float h;
  };
} Vec2f;

#endif

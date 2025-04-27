#ifndef __bse_math_vec2i__
#define __bse_math_vec2i__

#include "include.h"

typedef struct
{
  union {
    int x;
    int w;
  };
  union {
    int y;
    int h;
  };
} Vec2i;

#endif

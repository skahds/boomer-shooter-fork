#ifndef __bse_color__
#define __bse_color__

#include "math/vec4f.h"

typedef struct
{
  float r;
  float g;
  float b;
  float a;
} Color;

inline Color colorRgb(float r, float g, float b)
{
  return (Color){r, g, b, 1};
}

inline Color colorRgba(float r, float g, float b, float a)
{
  return (Color){r, g, b, a};
}

inline Vec4f colorToVec4f(Color color)
{
  return (Vec4f){color.r, color.g, color.b, color.a};
}

#endif

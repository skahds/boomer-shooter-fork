#include "color.h"

struct Color ColorFromRGBA(float r, float g, float b, float a)
{
  return (struct Color){r, g, b, a};
}

struct Color ColorFromRGB(float r, float g, float b)
{
  return (struct Color){r, g, b, 1};
}

struct Color ColorFromRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  return (struct Color){
    (float)r / UINT8_MAX,
    (float)g / UINT8_MAX,
    (float)b / UINT8_MAX,
    (float)a / UINT8_MAX,
  };
}

struct Color ColorFromRGB8(uint8_t r, uint8_t g, uint8_t b)
{
  return (struct Color){
    (float)r / UINT8_MAX,
    (float)g / UINT8_MAX,
    (float)b / UINT8_MAX,
    1,
  };
}

void ColorToBytes(
  struct Color c,
  uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
{
  if (r != NULL) *r = (uint8_t)(c.r * UINT8_MAX);
  if (g != NULL) *g = (uint8_t)(c.g * UINT8_MAX);
  if (b != NULL) *b = (uint8_t)(c.b * UINT8_MAX);
  if (a != NULL) *a = (uint8_t)(c.a * UINT8_MAX);
}

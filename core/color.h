#ifndef __engine_color__
#define __engine_color__

#include "include.h"

struct Color
{
  float r;
  float g;
  float b;
  float a;
};

struct Color ColorFromRGBA(float r, float g, float b, float a);
struct Color ColorFromRGB(float r, float g, float b);

struct Color ColorFromRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
struct Color ColorFromRGB8(uint8_t r, uint8_t g, uint8_t b);

void ColorToBytes(
  struct Color c,
  uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a
);

#endif

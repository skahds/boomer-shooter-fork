#ifndef __bse_texture__
#define __bse_texture__

#include "include.h"
#include "color.h"

typedef enum
{
  TexFormat_invalid = 0,
  TexFormat_r8,
  TexFormat_ra8,
  TexFormat_rgb8,
  TexFormat_rgba8,
} TextureFormat;

typedef struct
{
  uint8_t* data;
  int width;
  int height;
  int format;
  bool is_stbi;
} Image;

typedef struct
{
  uint32_t handle;
  int width;
  int height;
  int mipmaps;
  int format;
} Texture;

typedef struct
{
  int x;
  int y;
  int width;
  int height;
} Region;

Image imageLoad(const char* path);
Image imageSingleColor(Color color);
void imageDestroy(Image img);
bool imageIsValid(Image img);

Texture textureLoad(const char* path);
Texture textureFromImage(Image img);
Texture textureSingleColor(Color color);
void textureDestroy(Texture tex);
void textureBind(Texture tex, int slot);
bool textureIsValid(Texture tex);

#endif

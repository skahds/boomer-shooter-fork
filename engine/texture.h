#ifndef __engine_texture__
#define __engine_texture__

#include "include.h"
#include "image_format.h"
#include "image.h"

struct Texture
{
  uint32_t handle;
  int width;
  int height;
  enum ImageFormat format;
};

struct Texture TextureLoad(const char* path);
struct Texture TextureLoadFromImg(struct Image* img);
void TextureBind(struct Texture* tex, uint8_t slot);
void TextureDestroy(struct Texture* tex);

#endif

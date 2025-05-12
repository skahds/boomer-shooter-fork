#ifndef __engine_texture__
#define __engine_texture__

#include "include.h"
#include "gfx_types.h"
#include "image.h"
#include "math/vec2i.h"

// TODO: make this consistent with other implementation-defined stuff
struct Texture
{
  void* handle;
  Vec2i size;
  enum ImageFormat format;
  enum TextureFilter min_filter;
  enum TextureFilter mag_filter;
  enum TextureWrap x_wrap;
  enum TextureWrap y_wrap;
};

struct Texture TextureLoad(const char* path);
struct Texture TextureLoadFromImg(struct Image* img);
void TextureGenerateMipmaps(struct Texture* tex);
void TextureBind(struct Texture* tex, uint8_t slot);
void TextureSetFilter(
  struct Texture* tex,
  enum TextureFilter min,
  enum TextureFilter mag
);
void TextureSetWrap(
  struct Texture* tex,
  enum TextureWrap x_wrap,
  enum TextureWrap y_wrap
);
void TextureDestroy(struct Texture* tex);

#endif

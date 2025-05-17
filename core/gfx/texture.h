#ifndef __engine_texture__
#define __engine_texture__

#include "include.h"
#include "gfx_types.h"
#include "gfx.h"
#include "image.h"
#include "math/vec2i.h"

// TODO: make this consistent with other implementation-defined stuff
struct Texture
{
  void* handle;
  vec2i_t size;
  enum ImageFormat format;
  enum TextureFilter min_filter;
  enum TextureFilter mag_filter;
  enum TextureWrap x_wrap;
  enum TextureWrap y_wrap;
};

struct Texture TextureLoad(struct Renderer* r, const char* path);
struct Texture TextureLoadFromImg(struct Renderer* r, struct Image* img);
void TextureDestroy(struct Renderer* r, struct Texture* tex);
void TextureGenerateMipmaps(struct Renderer* r, struct Texture* tex);
void TextureBind(struct Renderer* r, struct Texture* tex, uint8_t slot);
void TextureSetFilter(
  struct Renderer* r,
  struct Texture* tex,
  enum TextureFilter min,
  enum TextureFilter mag
);
void TextureSetWrap(
  struct Renderer* r,
  struct Texture* tex,
  enum TextureWrap x_wrap,
  enum TextureWrap y_wrap
);

#endif

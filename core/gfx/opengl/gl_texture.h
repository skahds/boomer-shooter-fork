#ifndef __engine_gfx_opengl_texture__
#define __engine_gfx_opengl_texture__

#include "gfx/texture.h"

struct Texture gl_TextureLoadFromImg(struct Image* img);
void gl_TextureDestroy(struct Texture* tex);
void gl_TextureBind(struct Texture* tex, uint8_t slot);
void gl_TextureGenerateMipmaps(struct Texture* tex);
void gl_TextureSetFilter(
  struct Texture* tex,
  enum TextureFilter min,
  enum TextureFilter mag
);
void gl_TextureSetWrap(
  struct Texture* tex,
  enum TextureWrap x_wrap,
  enum TextureWrap y_wrap
);

#endif

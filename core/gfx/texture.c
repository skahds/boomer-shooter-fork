#include "texture.h"

struct Texture TextureLoad(
  struct Renderer* r,
  struct Vfs* vfs,
  const char* path)
{
  struct Image img = ImageLoad(vfs, path);
  struct Texture tex = TextureLoadFromImg(r, &img);
  ImageDestroy(&img);
  return tex;
}

struct Texture TextureLoadFromImg(struct Renderer* r, struct Image* img)
{
  return r->backend.texture_load_img(img);
}

void TextureDestroy(struct Renderer* r, struct Texture* tex)
{
  return r->backend.texture_destroy(tex);
}

void TextureGenerateMipmaps(struct Renderer* r, struct Texture* tex)
{
  return r->backend.texture_gen_mipmaps(tex);
}

void TextureBind(struct Renderer* r, struct Texture* tex, uint8_t slot)
{
  return r->backend.texture_bind(tex, slot);
}

void TextureSetFilter(
  struct Renderer* r,
  struct Texture* tex,
  enum TextureFilter min,
  enum TextureFilter mag)
{
  return r->backend.texture_set_filter(tex, min, mag);
}

void TextureSetWrap(
  struct Renderer* r,
  struct Texture* tex,
  enum TextureWrap x_wrap,
  enum TextureWrap y_wrap)
{
  return r->backend.texture_set_wrap(tex, x_wrap, y_wrap);
}

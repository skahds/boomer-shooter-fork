#include "texture.h"

#include "glad/glad.h"

static uint32_t GlFormatFromImageFormat(enum ImageFormat format)
{
  switch (format) {
    case IMAGE_FORMAT_R8: return GL_RED;
    case IMAGE_FORMAT_RA8: return GL_RG;
    case IMAGE_FORMAT_RGB8: return GL_RGB;
    case IMAGE_FORMAT_RGBA8: return GL_RGBA;
    case IMAGE_FORMAT_INVALID: return GL_RGBA;
  }
  return GL_RED;
}


struct Texture TextureLoad(const char* path)
{
  struct Image img = ImageLoad(path);
  struct Texture tex = TextureLoadFromImg(&img);
  ImageDestroy(&img);
  return tex;
}

struct Texture TextureLoadFromImg(struct Image* img)
{
  struct Texture tex;

  tex.width = img->width;
  tex.height = img->height;
  tex.format = img->format;

  uint32_t gl_format = GlFormatFromImageFormat(img->format);

  glGenTextures(1, &tex.handle);
  glBindTexture(GL_TEXTURE_2D, tex.handle);
  glTexImage2D(
    GL_TEXTURE_2D,
    0, gl_format, tex.width, tex.height,
    0, gl_format, GL_UNSIGNED_BYTE, img->data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  LogDebug("loaded texture %d", tex.handle);

  return tex;
}

void TextureBind(struct Texture* tex, uint8_t slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, tex->handle);
}

void TextureDestroy(struct Texture* tex)
{
  glDeleteTextures(1, &tex->handle);
  LogDebug("destroyed texture %d", tex->handle);
}

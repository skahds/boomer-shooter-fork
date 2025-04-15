#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"
#include "renderer.h"

uint32_t textureFormatToGlSizedFormat(TextureFormat format)
{
  switch (format) {
    case TexFormat_r8: return GL_R8;
    case TexFormat_ra8: return GL_RG8;
    case TexFormat_rgb8: return GL_RGB8;
    case TexFormat_rgba8: return GL_RGBA8;
    default:
      logFatal(1, "invalid texture format %d", format);
      return 0;
  }
}

uint32_t textureFormatToGlFormat(TextureFormat format)
{
  switch (format) {
    case TexFormat_r8: return GL_RED;
    case TexFormat_ra8: return GL_RG;
    case TexFormat_rgb8: return GL_RGB;
    case TexFormat_rgba8: return GL_RGBA;
    default:
      logFatal(1, "invalid texture format %d", format);
      return 0;
  }
}

Image imageLoad(const char* path)
{
  Image img;
  int channelc;
  img.is_stbi = true;
  img.data = stbi_load(path, &img.width, &img.height, &channelc, 0);
  if (!img.data) {
    logError("failed to load image '%s'.", path);
    // return something valid, cause crashing games sucks
    return imageSingleColor(colorRgb(1, 1, 1));
  }
  switch (channelc) {
    case 1: img.format = TexFormat_r8; break;
    case 2: img.format = TexFormat_ra8; break;
    case 3: img.format = TexFormat_rgb8; break;
    case 4: img.format = TexFormat_rgba8; break;
    default:
      logError(
        "failed to load image '%s'. unsupported channel count", path);
      stbi_image_free(img.data);
      img.data = NULL;
      img.width = 0;
      img.height = 0;
      img.format = TexFormat_invalid;
      break;
  }
  return img;
}

Image imageSingleColor(Color color)
{
  Image img;
  img.is_stbi = false;
  img.format = TexFormat_rgba8;
  img.width = 1;
  img.height = 1;

  uint8_t* data = createArr(uint8_t, 4);
  data[0] = (char)(color.r * UINT8_MAX);
  data[1] = (char)(color.g * UINT8_MAX);
  data[2] = (char)(color.b * UINT8_MAX);
  data[3] = (char)(color.a * UINT8_MAX);
  img.data = data;

  return img;
}

void imageDestroy(Image img)
{
  if (img.is_stbi)
    stbi_image_free(img.data);
  else
    destroy(img.data);
}

bool imageIsValid(Image img)
{
  return
    img.data != NULL &&
    img.width > 0 &&
    img.height > 0 &&
    img.format != TexFormat_invalid;
}

static Texture invalidTexture()
{
  Texture gput;
  memset(&gput, 0, sizeof(Texture));
  return gput;
}

Texture textureLoad(const char* path)
{
  Image img = imageLoad(path);
  if (!imageIsValid(img)) {
    logError("failed to load texture '%s'", path);
    return invalidTexture();
  }

  Texture tex = textureFromImage(img);
  imageDestroy(img);
  return tex;
}

Texture textureFromImage(Image img)
{
  Texture tex;
  glGenTextures(1, &tex.handle);
  glBindTexture(GL_TEXTURE_2D, tex.handle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  uint32_t gl_sized_format = textureFormatToGlSizedFormat(img.format);
  uint32_t gl_format = textureFormatToGlFormat(img.format);
  glTexImage2D(
    GL_TEXTURE_2D,
    0, gl_sized_format, img.width, img.height,
    0,
    gl_format, GL_UNSIGNED_BYTE, img.data);
  glGenerateMipmap(GL_TEXTURE_2D);

  logDebugInfo("creating texture %d", tex.handle);

  tex.width = img.width;
  tex.height = img.height;
  tex.mipmaps = 0;
  tex.format = tex.format;
  return tex;
}

Texture textureSingleColor(Color color)
{
  Image img = imageSingleColor(color);
  Texture tex = textureFromImage(img);
  imageDestroy(img);
  return tex;
}

void textureDestroy(Texture tex)
{
  logDebugInfo("destroying texture %d", tex.handle);
  glDeleteTextures(1, &tex.handle);
}

void textureBind(Texture tex, int slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, tex.handle);
}

bool textureIsValid(Texture tex)
{
  return
    tex.handle > 0 &&
    tex.width > 0 &&
    tex.height > 0 &&
    tex.format != TexFormat_invalid;
}

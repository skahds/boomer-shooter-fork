#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "mem.h"

static enum ImageFormat ImageFormatFromChannelCount(int channels)
{
  switch (channels) {
    case 1: return IMAGE_FORMAT_R8;
    case 2: return IMAGE_FORMAT_RA8;
    case 3: return IMAGE_FORMAT_RGB8;
    case 4: return IMAGE_FORMAT_RGBA8;
  }
  return IMAGE_FORMAT_INVALID;
}

struct Image ImageLoad(const char* path)
{
  int width;
  int height;
  int channel_count;
  uint8_t* data = stbi_load(path, &width, &height, &channel_count, 0);

  enum ImageFormat format = ImageFormatFromChannelCount(channel_count);

  LogDebug("loading image '%s'", path);

  return ImageLoadFromMemory(data, width, height, format);
}

struct Image ImageLoadFromMemory(
  uint8_t* data,
  int width,
  int height,
  enum ImageFormat format)
{
  struct Image img;
  img.data = data;
  img.width = width;
  img.height = height;
  img.format = format;
  return img;
}

void ImageDestroy(struct Image* img)
{
  Destroy(img->data);
  img->data = NULL;
  img->width = 0;
  img->height = 0;
  img->format = IMAGE_FORMAT_INVALID;
}

#include "image.h"

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

struct Image ImageLoad(struct Vfs* vfs, const char* path)
{
  vec2i_t size;
  int channel_count;
  size_t file_dat_len;
  uint8_t* file_dat = (uint8_t*)VfsReadFile(vfs, path, &file_dat_len);
  if (!file_dat)
    LogFatal(1, "could not load texture '%s'", path);
  uint8_t* data = stbi_load_from_memory(
    file_dat, file_dat_len, &size.x, &size.y, &channel_count, 0);
  Destroy(file_dat);
  // uint8_t* data = stbi_load(path, &size.x, &size.y, &channel_count, 0);

  enum ImageFormat format = ImageFormatFromChannelCount(channel_count);

  LogDebug("loading image '%s'", path);

  return ImageLoadFromMemory(data, size, format);
}

struct Image ImageLoadFromMemory(
  uint8_t* data,
  vec2i_t size,
  enum ImageFormat format)
{
  struct Image img;
  img.data = data;
  img.size = size;
  img.format = format;
  return img;
}

void ImageDestroy(struct Image* img)
{
  Destroy(img->data);
  img->data = NULL;
  img->size = (vec2i_t){0, 0};
  img->format = IMAGE_FORMAT_INVALID;
}

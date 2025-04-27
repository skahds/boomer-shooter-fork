#ifndef __engine_image__
#define __engine_image__

#include "include.h"
#include "image_format.h"

struct Image
{
  uint8_t* data;
  int width;
  int height;
  enum ImageFormat format;
};

struct Image ImageLoad(const char* path);
// will take ownership of data
struct Image ImageLoadFromMemory(
  uint8_t* data,
  int width,
  int height,
  enum ImageFormat format
);
void ImageDestroy(struct Image* img);

#endif

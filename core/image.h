#ifndef __engine_image__
#define __engine_image__

#include "include.h"
#include "gfx/gfx_types.h"
#include "math/vec2i.h"
#include "vfs.h"

struct Image
{
  uint8_t* data;
  vec2i_t size;
  enum ImageFormat format;
};

struct Image ImageLoad(struct Vfs* vfs, const char* path);
// will take ownership of data
struct Image ImageLoadFromMemory(
  uint8_t* data,
  vec2i_t size,
  enum ImageFormat format
);
void ImageDestroy(struct Image* img);

#endif

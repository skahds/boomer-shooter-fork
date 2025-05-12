#include "texture.h"

struct Texture TextureLoad(const char* path)
{
  struct Image img = ImageLoad(path);
  struct Texture tex = TextureLoadFromImg(&img);
  ImageDestroy(&img);
  return tex;
}


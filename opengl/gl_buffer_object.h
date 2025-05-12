#ifndef __engine_gfx_opengl_gl_buffer_object__
#define __engine_gfx_opengl_gl_buffer_object__

#include "include.h"
#include "gfx/buffer_object.h"

struct BufferObject
{
  enum BufferObjectType type;
  uint32_t handle;
};

#endif

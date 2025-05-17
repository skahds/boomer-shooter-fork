#ifndef __engine_gfx_opengl_gl_buffer_object__
#define __engine_gfx_opengl_gl_buffer_object__

#include "include.h"
#include "gfx/buffer_object.h"

struct BufferObject
{
  enum BufferObjectType type;
  uint32_t handle;
};

struct BufferObject* gl_BufferObjectCreate(enum BufferObjectType type);
void gl_BufferObjectDestroy(struct BufferObject* buf);
void gl_BufferObjectBind(struct BufferObject* buf);
void gl_BufferObjectUnbind(enum BufferObjectType type);
void gl_BufferObjectSet(
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode
);

#endif

#ifndef __engine_gfx_buffer_object__
#define __engine_gfx_buffer_object__

#include "include.h"
#include "gfx_types.h"

struct BufferObject;

struct BufferObject* BufferObjectCreate(enum BufferObjectType type);
void BufferObjectBind(struct BufferObject* buf);
void BufferObjectUnbind(enum BufferObjectType type);
void BufferObjectSet(
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode
);
void BufferObjectDestroy(struct BufferObject* buf);

#endif

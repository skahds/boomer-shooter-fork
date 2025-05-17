#ifndef __engine_gfx_buffer_object__
#define __engine_gfx_buffer_object__

#include "include.h"
#include "gfx_types.h"
#include "gfx.h"

struct BufferObject;

struct BufferObject* BufferObjectCreate(
  struct Renderer* r,
  enum BufferObjectType type
);
void BufferObjectDestroy(struct Renderer* r, struct BufferObject* buf);
void BufferObjectBind(struct Renderer* r, struct BufferObject* buf);
void BufferObjectUnbind(struct Renderer* r, enum BufferObjectType type);
void BufferObjectSet(
  struct Renderer* r,
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode
);

#endif

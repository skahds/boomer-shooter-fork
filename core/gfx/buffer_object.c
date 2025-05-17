#include "buffer_object.h"

struct BufferObject* BufferObjectCreate(
  struct Renderer* r,
  enum BufferObjectType type)
{
  return r->backend.buffer_object_create(type);
}

void BufferObjectDestroy(struct Renderer* r, struct BufferObject* buf)
{
  return r->backend.buffer_object_destroy(buf);
}

void BufferObjectBind(struct Renderer* r, struct BufferObject* buf)
{
  return r->backend.buffer_object_bind(buf);
}

void BufferObjectUnbind(struct Renderer* r, enum BufferObjectType type)
{
  return r->backend.buffer_object_unbind(type);
}

void BufferObjectSet(
  struct Renderer* r,
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode)
{
  return r->backend.buffer_object_set(buf, data, size, mode);
}

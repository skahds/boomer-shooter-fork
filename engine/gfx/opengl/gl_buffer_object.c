#include "gl_buffer_object.h"

#include <glad/glad.h>

#include "mem.h"
#include "gl_type_conv.h"

struct BufferObject* BufferObjectCreate(enum BufferObjectType type)
{
  struct BufferObject* buf = Create(struct BufferObject);
  buf->type = type;
  glGenBuffers(1, &buf->handle);
  LogDebug("created buffer object %d", buf->handle);
  return buf;
}

void BufferObjectBind(struct BufferObject* buf)
{
  glBindBuffer(BufferObjectTypeToOpenGl(buf->type), buf->handle);
}

void BufferObjectSet(
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode)
{
  uint32_t gl_mode = DrawModeToOpenGl(mode);
  uint32_t type = BufferObjectTypeToOpenGl(buf->type);
  glBindBuffer(type, buf->handle);
  glBufferData(type, size, data, gl_mode);
  glBindBuffer(type, 0);
  LogDebug("uploaded data to buffer object %d", buf->handle);
}

void BufferObjectDestroy(struct BufferObject* buf)
{
  glDeleteBuffers(1, &buf->handle);
  LogDebug("destroyed buffer object %d", buf->handle);
  Destroy(buf);
}

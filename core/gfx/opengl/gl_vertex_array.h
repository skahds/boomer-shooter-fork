#ifndef __engine_gfx_opengl_gl_vertex_array__
#define __engine_gfx_opengl_gl_vertex_array__

#include "include.h"

#include "gfx/vertex_array.h"

struct VertexArray
{
  uint32_t handle;
};

struct VertexArray* gl_VertexArrayCreate(const struct VertexFormat* fmt);
void gl_VertexArrayBind(struct VertexArray* varr);
void gl_VertexArrayDestroy(struct VertexArray* varr);
void gl_VertexArrayDraw(
  struct VertexArray* varr,
  size_t start,
  size_t count,
  enum IndexMode index_mode
);
void gl_VertexArrayDrawIndexed(
  struct VertexArray* varr,
  struct BufferObject* ebo,
  size_t count,
  enum GfxDataType type,
  enum IndexMode index_mode
);

#endif

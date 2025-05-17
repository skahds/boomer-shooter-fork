#include "vertex_array.h"

struct VertexFormat VertexFormatCreate(
  const struct VertexAttrib* attribs,
  size_t count)
{
  struct VertexFormat fmt;
  VertexFormatUpdate(&fmt, attribs, count);
  return fmt;
}

void VertexFormatUpdate(
  struct VertexFormat* fmt,
  const struct VertexAttrib* attribs,
  size_t count)
{
  fmt->attribs = attribs;
  fmt->attrib_count = count;

  size_t stride = 0;
  for (size_t i = 0; i < fmt->attrib_count; i++) {
    size_t size = GetGfxDataTypeSize(fmt->attribs[i].type);
    stride += size * fmt->attribs[i].components;
  }

  fmt->stride = stride;
}

struct VertexArray* VertexArrayCreate(
  struct Renderer* r,
  const struct VertexFormat* fmt)
{
  return r->backend.vertex_array_create(fmt);
}

void VertexArrayDestroy(struct Renderer* r, struct VertexArray* varr)
{
  return r->backend.vertex_array_destroy(varr);
}

void VertexArrayBind(struct Renderer* r, struct VertexArray* varr)
{
  return r->backend.vertex_array_bind(varr);
}

void VertexArrayDraw(
  struct Renderer* r,
  struct VertexArray* varr,
  size_t start,
  size_t count,
  enum IndexMode index_mode)
{
  return r->backend.vertex_array_draw(varr, start, count, index_mode);
}

void VertexArrayDrawIndexed(
  struct Renderer* r,
  struct VertexArray* varr,
  struct BufferObject* ebo,
  size_t count,
  enum GfxDataType type,
  enum IndexMode index_mode)
{
  return r->backend.vertex_array_draw_indexed(
    varr,
    ebo,
    count,
    type,
    index_mode
  );
}

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

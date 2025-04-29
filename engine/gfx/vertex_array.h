#ifndef __engine_gfx_vertex_array__
#define __engine_gfx_vertex_array__

#include "include.h"
#include "gfx_types.h"
#include "buffer_object.h"

struct VertexAttrib
{
  enum GfxDataType type : 8;
  uint8_t components; // is it a vec2, vec3, or whatever?
};

struct VertexFormat
{
  const struct VertexAttrib* attribs;
  size_t attrib_count;
  size_t stride;
};

struct VertexArray;

struct VertexFormat VertexFormatCreate(
  const struct VertexAttrib* attribs,
  size_t count
);
void VertexFormatUpdate(
  struct VertexFormat* fmt,
  const struct VertexAttrib* attribs,
  size_t count
);

struct VertexArray* VertexArrayCreate(const struct VertexFormat* fmt);
void VertexArrayDestroy(struct VertexArray* varr);
void VertexArrayBind(struct VertexArray* varr);
void VertexArrayDraw(
  struct VertexArray* varr,
  size_t start,
  size_t count,
  enum IndexMode index_mode
);
void VertexArrayDrawIndexed(
  struct VertexArray* varr,
  struct BufferObject* ebo,
  size_t count,
  enum GfxDataType type,
  enum IndexMode index_mode
);

#endif

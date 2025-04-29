#ifndef __engine_wrap_wrap_vertex_format__
#define __engine_wrap_wrap_vertex_format__

#include "include.h"
#include "wrap.h"
#include "gfx/vertex_array.h"

struct LuaVertexFormat
{
  struct VertexAttrib* attribs;
  size_t count;
  size_t capacity;
  struct VertexFormat fmt;
};

#endif

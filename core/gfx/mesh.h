#ifndef __engine_mesh__
#define __engine_mesh__

#include "include.h"
#include "buffer_object.h"
#include "vertex_array.h"

struct Mesh
{
  void* vertices;
  size_t vertex_count;
  uint16_t* indices;
  size_t index_count;

  struct VertexFormat* fmt;

  struct VertexArray* vao;
  struct BufferObject* vbo;
  struct BufferObject* ebo;
};

struct Mesh MeshCreate(struct VertexFormat* fmt);
void MeshDestroy(struct Mesh* m);
void MeshFinalize(struct Mesh* m, bool is_static);
void MeshDraw(struct Mesh* m);

#endif

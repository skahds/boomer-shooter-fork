#include "mesh.h"

struct Mesh MeshCreate(struct VertexFormat* fmt)
{
  struct Mesh m;
  m.vertices = NULL;
  m.vertex_count = 0;
  m.indices = NULL;
  m.index_count = 0;
  m.fmt = fmt;
  m.vao = NULL;
  m.vbo = NULL;
  m.ebo = NULL;
  return m;
}

void MeshDestroy(struct Mesh* m)
{
  m->vertices = NULL;
  m->vertex_count = 0;
  m->indices = NULL;
  m->index_count = 0;
  m->fmt = NULL;

  if (m->vao != NULL) {
    VertexArrayDestroy(m->vao);
  }
  if (m->vbo != NULL) {
    BufferObjectDestroy(m->vbo);
    m->vbo = NULL;
  }
  if (m->ebo != NULL) {
    BufferObjectDestroy(m->ebo);
    m->ebo = NULL;
  }
}

void MeshFinalize(struct Mesh* m, bool is_static)
{
  enum DrawMode mode = is_static ? DRAW_STATIC : DRAW_DYNAMIC;

  struct BufferObject* vbo = BufferObjectCreate(BUFFER_ARRAY);
  BufferObjectSet(vbo, m->vertices, m->fmt->stride * m->vertex_count, mode);
  BufferObjectBind(vbo);
  struct VertexArray* vao = VertexArrayCreate(m->fmt);
  m->vao = vao;
  m->vbo = vbo;

  if (m->index_count != 0) {
    struct BufferObject* ebo = BufferObjectCreate(BUFFER_INDEX);
    BufferObjectSet(ebo, m->indices, sizeof(uint16_t) * m->index_count, mode);
    m->ebo = ebo;
  }
}

void MeshDraw(struct Mesh* m)
{
  if (m->vao == NULL || m->vbo == NULL) {
    LogError("attempt to draw unfinalized mesh");
    return;
  }

  if (m->ebo != NULL) {
    VertexArrayDrawIndexed(
      m->vao,
      m->ebo,
      m->index_count,
      TYPE_USHORT,
      INDEX_TRIANGLES
    );
  } else {
    VertexArrayDraw(m->vao, 0, m->vertex_count, INDEX_TRIANGLES);
  }
}

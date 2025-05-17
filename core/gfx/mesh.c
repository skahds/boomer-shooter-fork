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

void MeshDestroy(struct Renderer* r, struct Mesh* m)
{
  m->vertices = NULL;
  m->vertex_count = 0;
  m->indices = NULL;
  m->index_count = 0;
  m->fmt = NULL;

  if (m->vao != NULL) {
    VertexArrayDestroy(r, m->vao);
  }
  if (m->vbo != NULL) {
    BufferObjectDestroy(r, m->vbo);
    m->vbo = NULL;
  }
  if (m->ebo != NULL) {
    BufferObjectDestroy(r, m->ebo);
    m->ebo = NULL;
  }
}

void MeshFinalize(struct Renderer* r, struct Mesh* m, bool is_static)
{
  enum DrawMode mode = is_static ? DRAW_STATIC : DRAW_DYNAMIC;

  struct BufferObject* vbo = BufferObjectCreate(r, BUFFER_ARRAY);
  BufferObjectSet(r, vbo, m->vertices, m->fmt->stride * m->vertex_count, mode);
  BufferObjectBind(r, vbo);
  struct VertexArray* vao = VertexArrayCreate(r, m->fmt);
  m->vao = vao;
  m->vbo = vbo;

  if (m->index_count != 0) {
    struct BufferObject* ebo = BufferObjectCreate(r, BUFFER_INDEX);
    BufferObjectSet(
      r,
      ebo,
      m->indices,
      sizeof(uint16_t) * m->index_count,
      mode
    );
    m->ebo = ebo;
  }
}

void MeshDraw(struct Renderer* r, struct Mesh* m)
{
  if (m->vao == NULL || m->vbo == NULL) {
    LogError("attempt to draw unfinalized mesh");
    return;
  }

  if (m->ebo != NULL) {
    VertexArrayDrawIndexed(
      r,
      m->vao,
      m->ebo,
      m->index_count,
      TYPE_USHORT,
      INDEX_TRIANGLES
    );
  } else {
    VertexArrayDraw(r, m->vao, 0, m->vertex_count, INDEX_TRIANGLES);
  }
}

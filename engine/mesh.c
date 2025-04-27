#include "mesh.h"

#include "mem.h"
#include <glad/glad.h>
#include <string.h>

struct DataTypeName
{
  const char* name;
  size_t len;
  enum DataType type;
};

struct DataTypeName type_names[] = {
  {"float", 5, TYPE_FLOAT},
  {"int", 3, TYPE_INT},
  {NULL, 0, 0},
};

enum DataType StringToDataType(const char* str, size_t len)
{
  for (struct DataTypeName* type = type_names; type->name != NULL; type++) {
    if (type->len == len &&
        memcmp(type->name, str, len) == 0) {
      return type->type;
    }
  }
  return TYPE_FLOAT;
}

size_t GetAttrSize(struct VertAttr attr)
{
  size_t tsize = 0;
  switch (attr.type) {
    case TYPE_FLOAT: tsize = sizeof(float); break;
    case TYPE_INT: tsize = sizeof(int); break;
  }
  return tsize;
}

struct MeshFormat MeshFormatCreate()
{
  struct MeshFormat fmt;

  fmt.attrs = NULL;
  fmt.count = 0;
  fmt.capacity = 0;

  return fmt;
}

void MeshFormatAddAttr(
  struct MeshFormat* fmt, enum DataType type, int count)
{
  if (fmt->count + 1 > fmt->capacity) {
    fmt->capacity = GrowCapacity(fmt->capacity);
    fmt->attrs = GrowArray(struct VertAttr, fmt->attrs, fmt->capacity);
  }

  struct VertAttr attr;
  attr.type = type;
  attr.count = count;

  fmt->attrs[fmt->count++] = attr;
  fmt->stride += GetAttrSize(attr) * attr.count;
}

void MeshFormatDestroy(struct MeshFormat* fmt)
{
  free(fmt->attrs);
  fmt->count = 0;
  fmt->capacity = 0;
}

struct Mesh MeshCreate(struct MeshFormat* fmt)
{
  struct Mesh m;
  m.vertices = NULL;
  m.vertex_count = 0;
  m.indices = NULL;
  m.index_count = 0;
  m.format = fmt;
  m.vao = 0;
  m.vbo = 0;
  m.ebo = 0;
  return m;
}

void MeshDestroy(struct Mesh* m)
{
  if (m->vao != 0) {
    glDeleteVertexArrays(1, &m->vao);
    LogDebug("deleted VAO %d", m->vao);
  }
  if (m->vbo != 0) {
    glDeleteBuffers(1, &m->vbo);
    LogDebug("deleted VBO %d", m->vbo);
  }
  if (m->ebo != 0) {
    glDeleteBuffers(1, &m->ebo);
    LogDebug("deleted EBO %d", m->ebo);
  }
}

void MeshFinalize(struct Mesh* m, bool is_static)
{
  uint32_t vao;
  glGenVertexArrays(1, &vao);
  LogDebug("created VAO %d", vao);

  uint32_t vbo;
  glGenBuffers(1, &vbo);
  LogDebug("created VBO %d", vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  uint32_t mode = is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
  glBufferData(GL_ARRAY_BUFFER,
    m->format->stride * m->vertex_count, m->vertices, mode);

  size_t offset = 0;
  for (int i = 0; i < m->format->count; i++) {
    struct VertAttr* attr = &m->format->attrs[i];
    uint16_t count = attr->count;

    uint32_t gl_type = GL_FLOAT;
    switch (attr->type) {
      case TYPE_FLOAT: gl_type = GL_FLOAT; break;
      case TYPE_INT: gl_type = GL_INT; break;
    }

    glVertexAttribPointer(
      i, count, gl_type, GL_FALSE, m->format->stride, (void*)offset);
    glEnableVertexAttribArray(i);
    offset += GetAttrSize(*attr) * attr->count;
  }

  if (m->index_count != 0) {
    uint32_t ebo;
    glGenBuffers(1, &ebo);
    LogDebug("created EBO %d", ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(uint16_t) * m->index_count, m->indices, mode);
    m->ebo = ebo;
  }

  m->vao = vao;
  m->vbo = vbo;
}

void MeshDraw(struct Mesh* m)
{
  if (m->vao == 0 || m->vbo == 0) {
    LogError("attempt to draw unfinalized mesh");
    return;
  }

  glBindVertexArray(m->vao);
  if (m->ebo != 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, m->vertex_count);
  }
}

#include "mesh.h"

#include "math/transform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

VertexAttribute simple_vertex_attrs[] = {
  (VertexAttribute){
    .size = sizeof(float) * 3,
    .components = 3,
  },
  (VertexAttribute){
    .size = sizeof(float) * 2,
    .components = 2,
  },
  (VertexAttribute){
    .size = sizeof(float) * 3,
    .components = 3,
  },
};

VertexFormat simple_vertex_format = (VertexFormat){
  .attr_count = 3,
  .attrs = simple_vertex_attrs,
  .vertex_size = sizeof(float) * 8,
};

static int attrTypeToGl(AttributeType type)
{
  switch (type) {
    case AttrType_float: return GL_FLOAT;
    case AttrType_int: return GL_INT;
  }
  return 0;
}

Mesh meshCreate(Material mat)
{
  Mesh mesh;

  mesh.vertices = NULL;
  mesh.vertices_count = 0;

  mesh.indices = NULL;
  mesh.indices_count = 0;

  mesh.triangle_count = 0;

  mesh.format = simple_vertex_format;

  mesh.material = mat;

  mesh.vao = 0;
  mesh.vbo = 0;
  mesh.ebo = 0;

  return mesh;
}

void meshDestroy(Mesh* mesh)
{
  if (mesh->vao > 0) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    if (mesh->ebo > 0) glDeleteBuffers(1, &mesh->ebo);
  }
}

void meshFinalize(Mesh* mesh, bool is_static)
{
  if (mesh->vao > 0) {
    logError("mesh already finalized (vao id %d)", mesh->vao);
    return;
  }

  VertexFormat format = mesh->format;

  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  int mode = is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

  size_t vertices_size = format.vertex_size * mesh->vertices_count;
  glBufferData(GL_ARRAY_BUFFER, vertices_size, mesh->vertices, mode);

  if (mesh->indices_count > 0 && mesh->indices != NULL) {
    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    size_t indices_size = sizeof(uint16_t) * mesh->indices_count;
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      indices_size, mesh->indices, mode);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->ebo = ebo;
  }

  size_t offset = 0;
  for (int i = 0; i < format.attr_count; i++) {
    VertexAttribute attr = format.attrs[i];
    int gl_type = attrTypeToGl(attr.type);
    glVertexAttribPointer(
      i, attr.components, gl_type, GL_FALSE,
      format.vertex_size,
      (void*)offset);
    glEnableVertexAttribArray(i);
    offset += attr.size;
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  mesh->vao = vao;
  mesh->vbo = vbo;
}

void meshDraw(const Renderer* r, Mesh* mesh, Vec3f pos, Vec3f rot)
{
  if (mesh->vao == 0) {
    logError("attempt to draw unfinalized mesh");
    return;
  }

  Transform trans;
  trans.pos = pos;
  trans.rot = rot;
  trans.scale = (Vec3f){1, 1, 1};

  Mat4 model;
  transformToMatrix(trans, model);
  // mat4Translate(model, x, y, z);

  shaderSendMat4(
    &mesh->material.shader,
    "projection", r->state->projection);
  shaderSendMat4(
    &mesh->material.shader,
    "view", r->state->view);
  shaderSendMat4(
    &mesh->material.shader,
    "model", model);

  materialApply(&mesh->material);

  glBindVertexArray(mesh->vao);
  if (mesh->ebo == 0) {
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertices_count);
  } else {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
}

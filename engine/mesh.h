#ifndef __bse_mesh__
#define __bse_mesh__

#include "include.h"
#include "renderer.h"
#include "material.h"
#include "math/vec2f.h"
#include "math/vec3f.h"

typedef struct
{
  Vec3f position;
  Vec2f uv;
  Vec3f normal;
} SimpleVertex;

typedef enum
{
  AttrType_float,
  AttrType_int,
} AttributeType;

typedef struct
{
  size_t size;
  AttributeType type;
  int components;
} VertexAttribute;

typedef struct
{
  int attr_count;
  VertexAttribute* attrs;
  size_t vertex_size;
} VertexFormat;

typedef struct
{
  void* vertices;
  int vertices_count;

  uint16_t* indices;
  int indices_count;

  int triangle_count;

  VertexFormat format;

  Material material;

  uint32_t vao;
  uint32_t vbo;
  uint32_t ebo;
} Mesh;

extern VertexFormat simple_vertex_format;

Mesh meshCreate(Material mat);
void meshDestroy(Mesh* mesh);
void meshFinalize(Mesh* mesh, bool is_static);
void meshDraw(const Renderer* r, Mesh* mesh, Vec3f pos, Vec3f rot);

#endif

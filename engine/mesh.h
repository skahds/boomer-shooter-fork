#ifndef __engine_mesh__
#define __engine_mesh__

#include "include.h"

enum DataType
{
  TYPE_FLOAT,
  TYPE_INT,
};

struct VertAttr
{
  enum DataType type;
  int count;
};

struct MeshFormat
{
  struct VertAttr* attrs;
  uint16_t count;
  uint16_t capacity;

  size_t stride;
};

struct Mesh
{
  void* vertices;
  uint16_t vertex_count;
  uint16_t* indices;
  uint16_t index_count;

  struct MeshFormat* format;

  uint32_t vao;
  uint32_t vbo;
  uint32_t ebo;
};

enum DataType StringToDataType(const char* str, size_t len);

size_t GetAttrSize(struct VertAttr attr);
struct MeshFormat MeshFormatCreate();
void MeshFormatAddAttr(
  struct MeshFormat* fmt, enum DataType type, int count);
void MeshFormatDestroy(struct MeshFormat* fmt);

struct Mesh MeshCreate(struct MeshFormat* fmt);
void MeshDestroy(struct Mesh* m);
void MeshFinalize(struct Mesh* m, bool is_static);
void MeshDraw(struct Mesh* m);

#endif

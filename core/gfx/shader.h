#ifndef __engine_gfx_shader__
#define __engine_gfx_shader__

#include "include.h"
#include "gfx.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/vec4f.h"
#include "math/vec2i.h"
#include "math/vec3i.h"
#include "math/vec4i.h"
#include "math/mat4.h"
#include "texture.h"

struct ShaderVar
{
  char* name;
  size_t len;
  uint32_t hash;

  int loc;
  int count;
  uint32_t type;
};

struct ShaderTable
{
  struct ShaderVar* vars;
  size_t count;
  size_t capacity;
};

struct Shader;

void ShaderTableAddVar(struct ShaderTable* t, struct ShaderVar var);
struct ShaderVar* ShaderTableFindVar(
  struct ShaderVar* vars, 
  size_t capacity,
  const char* name,
  size_t name_len,
  uint32_t name_hash
);
void ShaderTableDestroy(struct ShaderTable* t);
uint32_t HashVarName(const char* name, size_t len);

struct Shader* ShaderLoadFromFiles(
  struct Renderer* r,
  struct Vfs* vfs,
  const char* vert,
  const char* frag
);
struct Shader* ShaderLoadFromSource(
  struct Renderer* r,
  const char* vert,
  const char* frag
);
void ShaderSendInt(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  int i
);
void ShaderSendFloat(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  float f
);
void ShaderSendVec2f(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec2f_t v
);
void ShaderSendVec2i(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec2i_t v
);
void ShaderSendVec3f(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec3f_t v
);
void ShaderSendVec3i(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec3i_t v
);
void ShaderSendVec4f(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec4f_t v
);
void ShaderSendVec4i(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec4i_t v
);
void ShaderSendMat4(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  mat4_t m
);
void ShaderBind(struct Renderer* r, struct Shader* s);
void ShaderDestroy(struct Renderer* r, struct Shader* s);

#endif

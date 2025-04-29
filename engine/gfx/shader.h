#ifndef __engine_gfx_shader__
#define __engine_gfx_shader__

#include "include.h"
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

struct Shader
{
  void* handle;
  struct ShaderTable uniforms;
  struct ShaderTable attrs;
};

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

struct Shader ShaderLoadFromFiles(const char* vert, const char* frag);
struct Shader ShaderLoadFromSource(const char* vert, const char* frag);
void ShaderSendInt(struct Shader* s, const char* name, int i);
void ShaderSendFloat(struct Shader* s, const char* name, float f);
void ShaderSendVec2f(struct Shader* s, const char* name, Vec2f v);
void ShaderSendVec2i(struct Shader* s, const char* name, Vec2i v);
void ShaderSendVec3f(struct Shader* s, const char* name, Vec3f v);
void ShaderSendVec3i(struct Shader* s, const char* name, Vec3i v);
void ShaderSendVec4f(struct Shader* s, const char* name, Vec4f v);
void ShaderSendVec4i(struct Shader* s, const char* name, Vec4i v);
void ShaderSendMat4(struct Shader* s, const char* name, Mat4 m);
void ShaderBind(struct Shader* s);
void ShaderDestroy(struct Shader* s);

#endif

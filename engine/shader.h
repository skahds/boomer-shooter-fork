#ifndef __bse_shader__
#define __bse_shader__

#include "include.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/vec4f.h"
#include "math/vec2i.h"
#include "math/vec3i.h"
#include "math/vec4i.h"
#include "math/mat4.h"
#include "texture.h"

typedef struct
{
  char* chars;
  int len;
  uint32_t hash;
} UniformName;

typedef struct
{
  UniformName name;
  int count;
  uint32_t type;
  int location; 
} Uniform;

typedef struct
{
  Uniform* uniforms;
  int count;
  int capacity;
} UniformTable;

typedef struct
{
  uint32_t handle;
  UniformTable* uniform_table;
} Shader;

Shader shaderLoadFromFiles(const char* vert_path, const char* frag_path);
Shader shaderLoadFromSrc(const char* vert_src, const char* frag_src);
void shaderDestroy(Shader* s);
// void shaderSendInt(const Shader* s, const char* name, int* i);
// void shaderSendFloat(const Shader* s, const char* name, float* f);
Uniform* shaderGetUniform(const Shader* s, const char* name);
void shaderSendFloat(const Shader* s, const char* name, float f);
void shaderSendVec2f(const Shader* s, const char* name, Vec2f f);
void shaderSendVec3f(const Shader* s, const char* name, Vec3f f);
void shaderSendVec4f(const Shader* s, const char* name, Vec4f f);
void shaderSendInt(const Shader* s, const char* name, int i);
void shaderSendVec2i(const Shader* s, const char* name, Vec2i f);
void shaderSendVec3i(const Shader* s, const char* name, Vec3i f);
void shaderSendVec4i(const Shader* s, const char* name, Vec4i f);
void shaderSendMat4(const Shader* s, const  char* name, Mat4 mat);
void shaderUse(const Shader* s);

#endif

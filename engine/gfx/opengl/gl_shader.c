#include "gfx/shader.h"

#include <glad/glad.h>

#include "mem.h"

#define UNIFORM_NAME_MAX 512

static void FindShaderUniforms(struct Shader* s)
{
  uint32_t handle = *((uint32_t*)s->handle);

  s->uniforms.vars = NULL;
  s->uniforms.count = 0;
  s->uniforms.capacity = 0;

  int uniform_count;
  glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniform_count);

  for (int i = 0; i < uniform_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    struct ShaderVar var;
    var.loc = i;
    glGetActiveUniform(
       handle, i, UNIFORM_NAME_MAX, &name_len, &var.count, &var.type, name);

    var.name = CreateArray(char, name_len + 1);
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = HashVarName(name, name_len);

    ShaderTableAddVar(&s->uniforms, var);
  }
}

static void FindShaderAttribs(struct Shader* s)
{
  uint32_t handle = *((uint32_t*)s->handle);

  s->attrs.vars = NULL;
  s->attrs.count = 0;
  s->attrs.capacity = 0;

  int attr_count;
  glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &attr_count);

  for (int i = 0; i < attr_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    struct ShaderVar var;
    var.loc = i;
    glGetActiveAttrib(
       handle, i, UNIFORM_NAME_MAX, &name_len, &var.count, &var.type, name);

    var.name = CreateArray(char, name_len + 1);
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = HashVarName(name, name_len);

    ShaderTableAddVar(&s->attrs, var);
  }
}

static struct Shader ShaderCreate(uint32_t vert, uint32_t frag)
{
  struct Shader s;

  uint32_t handle = glCreateProgram();

  uint32_t* handle_ptr = Create(uint32_t);
  *handle_ptr = handle;

  s.handle = handle_ptr;

  glAttachShader(handle, vert);
  glAttachShader(handle, frag);
  glLinkProgram(handle);

  glDeleteShader(vert);
  glDeleteShader(frag);

  int status;
  glGetProgramiv(handle, GL_LINK_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetProgramInfoLog(handle, 512, NULL, msg);
    LogFatal(1, "failed to link shader program: %s", msg);
  }

  FindShaderUniforms(&s);
  FindShaderAttribs(&s);

  LogDebug("created shader %d", handle);

  return s;
}

static uint32_t CompileSource(const char* name, const char* src, uint32_t type)
{
  uint32_t h = glCreateShader(type);
  glShaderSource(h, 1, &src, NULL);
  glCompileShader(h);

  int status;
  glGetShaderiv(h, GL_COMPILE_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetShaderInfoLog(h, 512, NULL, msg);
    LogFatal(1, "failed to compile shader '%s': %s", name, msg);
  }

  return h;
}

struct Shader ShaderLoadFromFiles(const char* vert, const char* frag)
{
  char* vsrc = ReadFile(vert);
  char* fsrc = ReadFile(frag);
  uint32_t vertex_handle = CompileSource(vert, vsrc, GL_VERTEX_SHADER);
  uint32_t fragment_handle = CompileSource(frag, fsrc, GL_FRAGMENT_SHADER);
  Destroy(vsrc);
  Destroy(fsrc);

  return ShaderCreate(vertex_handle, fragment_handle);
}

struct Shader ShaderLoadFromSource(const char* vert, const char* frag)
{
  uint32_t vertex_handle = CompileSource("vertex", vert, GL_VERTEX_SHADER);
  uint32_t fragment_handle = CompileSource("fragment", frag, GL_FRAGMENT_SHADER);
  return ShaderCreate(vertex_handle, fragment_handle);
}

static struct ShaderVar* GetUniform(struct Shader* s, const char* name)
{
  size_t len = strlen(name);
  uint32_t hash = HashVarName(name, len);
  struct ShaderVar* var = ShaderTableFindVar(
    s->uniforms.vars,
    s->uniforms.capacity,
    name,
    len,
    hash
  );
  if (var->name == NULL) LogFatal(1, "uniform '%s' does not exist", name);
  return var;
}

void ShaderSendInt(struct Shader* s, const char* name, int i)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform1i(ptr->loc, i);
}

void ShaderSendFloat(struct Shader* s, const char* name, float f)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform1f(ptr->loc, f);
}

void ShaderSendVec2f(struct Shader* s, const char* name, Vec2f v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform2f(ptr->loc, v.x, v.y);
}

void ShaderSendVec2i(struct Shader* s, const char* name, Vec2i v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform2i(ptr->loc, v.x, v.y);
}

void ShaderSendVec3f(struct Shader* s, const char* name, Vec3f v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform3i(ptr->loc, v.x, v.y, v.z);
}

void ShaderSendVec3i(struct Shader* s, const char* name, Vec3i v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform3i(ptr->loc, v.x, v.y, v.z);
}

void ShaderSendVec4f(struct Shader* s, const char* name, Vec4f v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform4i(ptr->loc, v.x, v.y, v.z, v.w);
}

void ShaderSendVec4i(struct Shader* s, const char* name, Vec4i v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform4i(ptr->loc, v.x, v.y, v.z, v.w);
}

void ShaderSendMat4(struct Shader* s, const char* name, Mat4 m)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniformMatrix4fv(ptr->loc, 1, GL_FALSE, m);
}

void ShaderBind(struct Shader* s)
{
  glUseProgram(*((uint32_t*)s->handle));
}

void ShaderDestroy(struct Shader* s)
{
  uint32_t handle = *((uint32_t*)s->handle);
  glDeleteProgram(handle);
  LogDebug("destroyed shader %d", handle);

  ShaderTableDestroy(&s->uniforms);
  ShaderTableDestroy(&s->attrs);

  Destroy(s->handle);
  s->handle = NULL;
}

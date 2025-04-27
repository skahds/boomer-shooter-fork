#include "shader.h"

#include <glad/glad.h>

#include "mem.h"

#define UNIFORM_NAME_MAX 512

struct ShaderVar
{
  char* name;
  uint16_t len;
  uint32_t hash;

  int loc;
  int count;
  uint32_t type;
};

static char* ReadFile(const char* path)
{
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    LogError("could not open file '%s'", path);
    return NULL;
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char* buf = CreateArray(char, file_size + 1);
  if (buf == NULL) {
    LogError("error: file '%s' too large to read", path);
    return NULL;
  }

  size_t bytes_read = fread(buf, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    Destroy(buf);
    LogError("error: could not read file '%s'", path);
    return NULL;
  }

  buf[bytes_read] = '\0';

  fclose(file);

  LogDebug("loaded file '%s'", path);
  return buf;
}

static struct ShaderVar* FindShaderVar(
  struct ShaderVar* vars, 
  uint16_t capacity,
  const char* name,
  uint16_t name_len,
  uint32_t name_hash)
{
  if (capacity == 0) return NULL;

  uint32_t index = name_hash & (capacity - 1);

  while (true) {
    struct ShaderVar* var = &vars[index];
    if (var->name == NULL) {
      return var;
    } else if (
      name_hash == var->hash &&
      name_len == var->len &&
      memcmp(name, var->name, name_len) == 0
    ) {
      return var;
    }

    index = (index + 1) & (capacity - 1);
  }
}

static void GrowShaderTable(struct ShaderTable* t)
{
  uint16_t capacity = GrowCapacity(t->capacity);

  struct ShaderVar* vars = CreateArray(struct ShaderVar, capacity);
  for (int i = 0; i < capacity; i++) {
    vars[i].name = NULL;
  }

  for (int i = 0; i < t->capacity; i++) {
    struct ShaderVar* var = &t->vars[i];
    if (var->name == NULL) continue; // empty
    
    struct ShaderVar* new_position = FindShaderVar(
      vars, capacity, var->name, var->len, var->hash);
    *new_position = *var;
  }

  Destroy(t->vars);
  t->vars = vars;
  t->capacity = capacity;
}

static void AddShaderVar(struct ShaderTable* t, struct ShaderVar var)
{
  if (t->count + 1 > t->capacity * 0.75) {
    GrowShaderTable(t);
  }

  struct ShaderVar* ptr = FindShaderVar(
    t->vars, t->capacity, var.name, var.len, var.hash);
  *ptr = var;
  t->count++;
}

static void DestroyShaderTable(struct ShaderTable* t)
{
  for (int i = 0; i < t->capacity; i++) {
    struct ShaderVar* var = &t->vars[i];
    if (var->name == NULL) continue;
    Destroy(var->name);
  }
  Destroy(t->vars);

  t->vars = NULL;
  t->count = 0;
}

static uint32_t HashString(const char* str, uint16_t len)
{
  uint32_t hash = 2166136261u;
  for (uint16_t i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619;
  }
  return hash;
}

static void FindShaderUniforms(struct Shader* s)
{
  s->uniforms.vars = NULL;
  s->uniforms.count = 0;
  s->uniforms.capacity = 0;

  int uniform_count;
  glGetProgramiv(s->handle, GL_ACTIVE_UNIFORMS, &uniform_count);

  for (int i = 0; i < uniform_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    struct ShaderVar var;
    var.loc = i;
    glGetActiveUniform(
       s->handle, i, UNIFORM_NAME_MAX, &name_len, &var.count, &var.type, name);

    var.name = CreateArray(char, name_len + 1);
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = HashString(name, name_len);

    AddShaderVar(&s->uniforms, var);
  }
}

static void FindShaderAttribs(struct Shader* s)
{
  s->attrs.vars = NULL;
  s->attrs.count = 0;
  s->attrs.capacity = 0;

  int attr_count;
  glGetProgramiv(s->handle, GL_ACTIVE_ATTRIBUTES, &attr_count);

  for (int i = 0; i < attr_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    struct ShaderVar var;
    var.loc = i;
    glGetActiveAttrib(
       s->handle, i, UNIFORM_NAME_MAX, &name_len, &var.count, &var.type, name);

    var.name = CreateArray(char, name_len + 1);
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = HashString(name, name_len);

    AddShaderVar(&s->attrs, var);
  }
}

static struct Shader ShaderCreate(uint32_t vert, uint32_t frag)
{
  struct Shader s;

  s.handle = glCreateProgram();

  glAttachShader(s.handle, vert);
  glAttachShader(s.handle, frag);
  glLinkProgram(s.handle);

  glDeleteShader(vert);
  glDeleteShader(frag);

  int status;
  glGetProgramiv(s.handle, GL_LINK_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetProgramInfoLog(s.handle, 512, NULL, msg);
    LogFatal(1, "failed to link shader program: %s", msg);
  }

  FindShaderUniforms(&s);
  FindShaderAttribs(&s);

  LogDebug("created shader %d", s.handle);

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
  uint16_t len = strlen(name);
  uint32_t hash = HashString(name, len);
  struct ShaderVar* var =
    FindShaderVar(s->uniforms.vars, s->uniforms.capacity, name, len, hash);
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
  glUseProgram(s->handle);
}

void ShaderDestroy(struct Shader* s)
{
  glDeleteProgram(s->handle);
  LogDebug("deleted shader %d", s->handle);
  s->handle = 0;

  DestroyShaderTable(&s->uniforms);
  DestroyShaderTable(&s->attrs);
}

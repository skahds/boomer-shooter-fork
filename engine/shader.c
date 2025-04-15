#include "shader.h"

#include <string.h>

#include <glad/glad.h>

#define uniform_set_max_load 0.75

static Uniform* findUniform(
  Uniform* uniforms,
  int capacity,
  const char* name,
  int name_len,
  uint32_t name_hash)
{
  if (capacity == 0) return NULL;
  uint32_t index = name_hash & (capacity - 1);

  while (true) {
    Uniform* uniform = &uniforms[index];
    if (uniform->name.chars == NULL) {
      return uniform;
    } else if (
      name_hash == uniform->name.hash &&
      name_len == uniform->name.len &&
      memcmp(name, uniform->name.chars, name_len) == 0
    ) {
      return uniform;
    }

    index = (index + 1) & (capacity - 1);
  }
}

static void growUniformTable(UniformTable* uniform_table)
{
  int new_capacity = uniform_table->capacity == 0 
    ? 8
    : uniform_table->capacity * 2;
  Uniform* new_uniforms = createArr(Uniform, new_capacity);
  for (int i = 0; i < new_capacity; i++) {
    new_uniforms[i].name.chars = NULL; // mark as empty
  }

  for (int i = 0; i < uniform_table->capacity; i++) {
    Uniform* uniform = &uniform_table->uniforms[i];
    if (uniform->name.chars == NULL) {
      continue; // skip empty
    }

    Uniform* new_position = findUniform(
      new_uniforms,
      new_capacity,
      uniform->name.chars,
      uniform->name.len,
      uniform->name.hash
    );
    *new_position = *uniform;
  }

  if (uniform_table != NULL) destroy(uniform_table->uniforms);
  uniform_table->uniforms = new_uniforms;
  uniform_table->capacity = new_capacity;
}

static void addUniform(UniformTable* uniform_table, Uniform uniform)
{
  if (
    uniform_table->count + 1 > uniform_table->capacity * uniform_set_max_load
  ) {
    growUniformTable(uniform_table);
  }

  Uniform* ptr = findUniform(
    uniform_table->uniforms,
    uniform_table->capacity,
    uniform.name.chars,
    uniform.name.len,
    uniform.name.hash
  );
  *ptr = uniform;
  uniform_table->count++;
}

static uint32_t hashString(const char* str, int len)
{
  uint32_t hash = 2166136261u;
  for (int i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619;
  }
  return hash;
}

static void initShaderUniforms(Shader* s) 
{
  s->uniform_table = create(UniformTable);
  s->uniform_table->uniforms = NULL;
  s->uniform_table->count = 0;
  s->uniform_table->capacity = 0;

  int uniform_count;
  glGetProgramiv(s->handle, GL_ACTIVE_UNIFORMS, &uniform_count);

  for (int i = 0; i < uniform_count; i++) {
    const int name_max = 32;
    int name_length;
    char name[name_max];
    Uniform uniform;
    uniform.location = i;

    glGetActiveUniform(
      s->handle,
      i,
      name_max,
      &name_length,
      &uniform.count,
      &uniform.type,
      name
    );

    uniform.name.len = name_length;
    uniform.name.chars = createArr(char, name_length + 1);
    memcpy(uniform.name.chars, name, name_length);
    uniform.name.chars[name_length] = '\0';
    uniform.name.hash = hashString(uniform.name.chars, name_length);

    addUniform(s->uniform_table, uniform);
  }
}


static uint32_t createShader(const char* src, int type)
{
  uint32_t handle = glCreateShader(type);
  glShaderSource(handle, 1, &src, NULL);
  glCompileShader(handle);

  int success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(handle, 512, NULL, info_log);
    logError("failed to compile shader: %s", info_log);
  }
  return handle;
}

Shader shaderLoadFromFiles(const char* vert_path, const char* frag_path)
{
  char* vert_src = readFile(vert_path);
  char* frag_src = readFile(frag_path);

  Shader shader = shaderLoadFromSrc(vert_src, frag_src);

  destroy(vert_src);
  destroy(frag_src);

  return shader;
}

Shader shaderLoadFromSrc(const char* vert_src, const char* frag_src)
{
  uint32_t vert = createShader(vert_src, GL_VERTEX_SHADER);
  uint32_t frag = createShader(frag_src, GL_FRAGMENT_SHADER);

  uint32_t prog = glCreateProgram();
  logDebugInfo("create shader program %d", prog);

  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);

  int success;
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(prog, 512, NULL, info_log);
    logError("failed to link shader: %s", info_log);
  }

  glDeleteShader(vert);
  glDeleteShader(frag);

  Shader shader;
  shader.handle = prog;
  initShaderUniforms(&shader);

  return shader;
}

void shaderDestroy(Shader* s)
{
  logDebugInfo("destroy shader program %d", s->handle);

  glDeleteProgram(s->handle);
  
  for (int i = 0; i < s->uniform_table->capacity; i++) {
    Uniform* uniform = &s->uniform_table->uniforms[i];
    if (uniform->name.chars == NULL) {
      continue;
    }
    destroy(uniform->name.chars);
  }
  destroy(s->uniform_table->uniforms);
  destroy(s->uniform_table);
  s->uniform_table = NULL;
}

Uniform* shaderGetUniform(const Shader* s, const char* name)
{
  int name_len = strlen(name);
  Uniform* uniform = findUniform(
    s->uniform_table->uniforms,
    s->uniform_table->capacity,
    name,
    name_len,
    hashString(name, name_len)
  );
  if (uniform == NULL || uniform->name.chars == NULL) {
    logError("uniform '%s' does not exist", name);
    return NULL;
  }
  return uniform;
}

void shaderSendTexture(const Shader* s, const char* name, Texture tex)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex.handle);
  glUniform1i(loc, 0);
}

void shaderSendFloat(const Shader* s, const char* name, float f)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform1f(loc, f);
}

void shaderSendVec2f(const Shader* s, const char* name, Vec2f f)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  glUniform2f(loc, f.x, f.y);
}

void shaderSendVec3f(const Shader* s, const char* name, Vec3f f)
{

  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform3f(loc, f.x, f.y, f.z);
}

void shaderSendVec4f(const Shader* s, const char* name, Vec4f f)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform4f(loc, f.x, f.y, f.z, f.w);
}

void shaderSendInt(const Shader* s, const char* name, int i)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform1i(loc, i);
}

void shaderSendVec2i(const Shader* s, const char* name, Vec2i i)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform2i(loc, i.x, i.y);
}

void shaderSendVec3i(const Shader* s, const char* name, Vec3i i)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform3i(loc, i.x, i.y, i.z);
}

void shaderSendVec4i(const Shader* s, const char* name, Vec4i i)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniform4i(loc, i.x, i.y, i.z, i.w);
}

void shaderSendMat4(const Shader* s, const  char* name, Mat4 mat)
{
  Uniform* uniform = shaderGetUniform(s, name);
  if (!uniform) return;
  int loc = uniform->location;
  shaderUse(s);
  glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

void shaderUse(const Shader* s)
{
  glUseProgram(s->handle);
}

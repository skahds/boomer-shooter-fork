#include "shader.h"

#include <string.h>

#include "mem.h"

struct ShaderVar* ShaderTableFindVar(
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
    
    struct ShaderVar* new_position = ShaderTableFindVar(
      vars,
      capacity,
      var->name,
      var->len,
      var->hash
    );
    *new_position = *var;
  }

  Destroy(t->vars);
  t->vars = vars;
  t->capacity = capacity;
}

void ShaderTableAddVar(struct ShaderTable* t, struct ShaderVar var)
{
  if (t->count + 1 > t->capacity * 0.75) {
    GrowShaderTable(t);
  }

  struct ShaderVar* ptr = ShaderTableFindVar(
    t->vars,
    t->capacity,
    var.name,
    var.len,
    var.hash
  );
  *ptr = var;
  t->count++;
}

void ShaderTableDestroy(struct ShaderTable* t)
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

uint32_t HashVarName(const char* str, uint16_t len)
{
  uint32_t hash = 2166136261u;
  for (uint16_t i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619;
  }
  return hash;
}

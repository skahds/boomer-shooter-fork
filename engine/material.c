#include "material.h"

#include <glad/glad.h>

Material materialDefault(Renderer* renderer)
{
  Material mat;
  mat.shader = renderer->default_shader;
  mat.map = NULL;
  mat.map_count = 0;
  mat.map_capacity = 0;
  return mat;
}

MaterialMap* materialAddMapping(Material* mat, const char* uniform)
{
  if (mat->map_count + 1 > mat->map_capacity) {
    mat->map_capacity = mat->map_capacity == 0
      ? 8
      : mat->map_capacity * 2;
    mat->map = resizeArr(MaterialMap, mat->map, mat->map_capacity);
  }

  MaterialMap* map = &mat->map[mat->map_count++];
  map->uniform = uniform;
  return map;
}

void materialDestroy(Material* mat)
{
  destroy(mat->map);
  mat->map_count = 0;
  mat->map_capacity = 0;
}

void materialApply(Material* mat)
{
  shaderUse(&mat->shader);

  int texture_slot = 0;

  for (int i = 0; i < mat->map_count; i++) {
    MaterialMap* mapping = &mat->map[i];
    switch (mapping->data_type) {
      case MMType_float:
        shaderSendFloat(&mat->shader, mapping->uniform, mapping->data.f);
        break;
      case MMType_float2:
        shaderSendVec2f(&mat->shader, mapping->uniform, mapping->data.v2f);
        break;
      case MMType_float3:
        shaderSendVec3f(&mat->shader, mapping->uniform, mapping->data.v3f);
        break;
      case MMType_float4:
        shaderSendVec4f(&mat->shader, mapping->uniform, mapping->data.v4f);
        break;
      case MMType_int:
        shaderSendInt(&mat->shader, mapping->uniform, mapping->data.i);
        break;
      case MMType_int2:
        shaderSendVec2i(&mat->shader, mapping->uniform, mapping->data.v2i);
        break;
      case MMType_int3:
        shaderSendVec3i(&mat->shader, mapping->uniform, mapping->data.v3i);
        break;
      case MMType_int4:
        shaderSendVec4i(&mat->shader, mapping->uniform, mapping->data.v4i);
        break;
      case MMType_texture: {
        Texture* tex = mapping->data.tex;
        textureBind(*tex, texture_slot);
        Uniform* uniform = shaderGetUniform(&mat->shader, mapping->uniform);
        if (!uniform) continue;
        glUniform1i(uniform->location, texture_slot);
        texture_slot++;
        break;
      }
    }
  }
}

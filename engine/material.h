#ifndef __bse_material__
#define __bse_material__

#include "include.h"
#include "texture.h"
#include "shader.h"
#include "color.h"
#include "renderer.h"

#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/vec4f.h"
#include "math/vec2i.h"
#include "math/vec3i.h"
#include "math/vec4i.h"

typedef enum
{
  MMType_float,
  MMType_float2,
  MMType_float3,
  MMType_float4,
  MMType_int,
  MMType_int2,
  MMType_int3,
  MMType_int4,
  MMType_texture,
} MaterialMapType;

typedef struct
{
  const char* uniform;
  MaterialMapType data_type;
  union {
    float f;
    Vec2f v2f;
    Vec3f v3f;
    Vec4f v4f;
    int i;
    Vec2i v2i;
    Vec3i v3i;
    Vec4i v4i;
    Texture* tex;
  } data;
} MaterialMap;

typedef struct
{
  Shader shader;
  MaterialMap* map;
  int map_count;
  int map_capacity;
} Material;

Material materialDefault(Renderer* renderer);
MaterialMap* materialAddMapping(Material* mat, const char* name);
void materialDestroy(Material* mat);
void materialApply(Material* mat);

#endif

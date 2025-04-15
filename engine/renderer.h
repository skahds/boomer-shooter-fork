#ifndef __bse_renderer__
#define __bse_renderer__

#include "math/mat4.h"
#include "math/vec3f.h"

#include "include.h"
#include "shader.h"
#include "texture.h"

typedef enum
{
  RStateKind_2d,
  RStateKind_3d,
} RStateKind;

typedef struct RendererState
{
  struct RendererState* previous;
  RStateKind kind;
  Mat4 projection;
  Mat4 view;
} RendererState;

typedef struct
{
  struct Context* ctx;
  RendererState* state;
  float fov;
  Shader default_shader;
  Texture default_texture;
} Renderer;

void rendererInit(Renderer* r, struct Context* ctx);
void rendererDestroy(Renderer* r);
void rendererPushNewState(Renderer* r, RStateKind kind);
void rendererPopState(Renderer* r);
void rendererSetViewPosition(Renderer* r, Vec3f pos);

#endif

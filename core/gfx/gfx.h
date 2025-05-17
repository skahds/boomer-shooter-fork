#ifndef __engine_gfx_gfx__
#define __engine_gfx_gfx__

#include "engine.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/mat4.h"
#include "backend.h"

enum GraphicsBackendType
{
  GFX_BACKEND_OPENGL,
};

// might put more stuff here later
struct Renderer
{
  struct GraphicsBackend backend;
};

void InitBackend(struct Engine* engine, enum GraphicsBackendType backend);
void ClearBackground(struct Renderer* renderer, float r, float g, float b);
void AdjustViewport(struct Renderer* r, vec2f_t size);
void SetDepthTest(struct Renderer* r, bool test);

#endif

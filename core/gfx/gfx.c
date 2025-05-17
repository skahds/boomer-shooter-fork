#include "gfx.h"

#ifdef bse_allow_opengl
#include "opengl/gl_gfx.h"
#endif

void InitBackend(struct Engine* engine, enum GraphicsBackendType backend)
{
  switch (backend) {
    case GFX_BACKEND_OPENGL:
    #ifdef bse_allow_opengl
      gl_InitBackend(engine);
    #else
      LogFatal(1, "opengl is not supported on this system");
    #endif
      break;
    default:
      LogFatal(1, "invalid graphics backend");
      break;
  }
}

void ClearBackground(struct Renderer* renderer, float r, float g, float b)
{
  return renderer->backend.clear_background(r, g, b);
}

void AdjustViewport(struct Renderer* r, vec2f_t size)
{
  return r->backend.adjust_viewport(size);
}

void SetDepthTest(struct Renderer* r, bool test)
{
  return r->backend.set_depth_test(test);
}


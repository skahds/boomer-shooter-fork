#ifndef __engine_gfx_opengl_gl_shader__
#define __engine_gfx_opengl_gl_shader__

#include "gfx/shader.h"

struct Shader
{
  uint32_t handle;
  struct ShaderTable uniforms;
  struct ShaderTable attrs;
};

#endif

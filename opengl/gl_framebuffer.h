#ifndef __engine_gfx_opengl_gl_framebuffer__
#define __engine_gfx_opengl_gl_framebuffer__

#include "include.h"
#include "gfx/framebuffer.h"

struct Framebuffer
{
  vec2i_t size;
  uint8_t flags;
  uint32_t fb_handle;
  uint32_t color_handle;
  // will either be a depth buffer, or a depth24-stencil8 buffer
  uint32_t z_mask_handle;

  // to draw the framebuffer as a quad
  struct VertexArray* vao;
  struct BufferObject* vbo;
};

#endif

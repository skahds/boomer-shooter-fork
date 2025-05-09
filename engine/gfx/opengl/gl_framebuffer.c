#include "gl_framebuffer.h"

#include <glad/glad.h>

#include "gfx/gfx_types.h"
#include "gfx/vertex_array.h"
#include "gl_shader.h"
#include "mem.h"

struct Shader* framebuffer_draw_shader = NULL;

struct Framebuffer* FramebufferCreate(Vec2i size, uint8_t flags)
{
  struct Framebuffer* fb = Create(struct Framebuffer);

  fb->size = size;
  fb->flags = flags;
  fb->color_handle = 0;
  fb->z_mask_handle = 0;
  fb->vao = NULL;
  fb->vbo = NULL;

  glGenFramebuffers(1, &fb->fb_handle);

  glBindFramebuffer(GL_FRAMEBUFFER, fb->fb_handle);

  if (flags & FRAMEBUFFER_COLOR_BUF) {
    glGenTextures(1, &fb->color_handle);
    glBindTexture(GL_TEXTURE_2D, fb->color_handle);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      size.x,
      size.y,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D,
      fb->color_handle,
      0
    );

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  if (flags & FRAMEBUFFER_DEPTH_MASK_BUF) {
    glGenTextures(1, &fb->z_mask_handle);
    glBindTexture(GL_TEXTURE_2D, fb->z_mask_handle);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_DEPTH24_STENCIL8,
      size.x,
      size.y,
      0,
      GL_DEPTH_STENCIL,
      GL_UNSIGNED_INT_24_8,
      NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT,
      GL_TEXTURE_2D,
      fb->z_mask_handle,
      0
    );

    glBindTexture(GL_TEXTURE_2D, 0);
  } else if (flags & FRAMEBUFFER_DEPTH_BUF) {
    glGenTextures(1, &fb->z_mask_handle);
    glBindTexture(GL_TEXTURE_2D, fb->z_mask_handle);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_DEPTH_COMPONENT32F,
      size.x,
      size.y,
      0,
      GL_DEPTH_COMPONENT,
      GL_FLOAT,
      NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D,
      fb->z_mask_handle,
      0
    );

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  if (flags & FRAMEBUFFER_DRAWABLE) {
    struct VertexAttrib attribs[] = {
      {TYPE_FLOAT, 2}, // position
      {TYPE_FLOAT, 2}, // UV
    };
    struct VertexFormat format = VertexFormatCreate(attribs, 2);
    fb->vbo = BufferObjectCreate(BUFFER_ARRAY);
    BufferObjectSet(fb->vbo, NULL, 0, DRAW_DYNAMIC);
    BufferObjectBind(fb->vbo);
    fb->vao = VertexArrayCreate(&format);

    if (framebuffer_draw_shader == NULL) {
      framebuffer_draw_shader = ShaderLoadFromFiles(
        "vfbdraw.glsl",
        "ffbdraw.glsl"
      );
    }
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LogWarning("framebuffer %d could not be completed", fb->fb_handle);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fb;
}

void FramebufferDestroy(struct Framebuffer* fb)
{
  if (fb->color_handle != 0) glDeleteTextures(1, &fb->color_handle);
  if (fb->z_mask_handle != 0) glDeleteTextures(1, &fb->z_mask_handle);
  if (fb->vao != NULL) VertexArrayDestroy(fb->vao);
  if (fb->vbo != NULL) BufferObjectDestroy(fb->vbo);
  glDeleteFramebuffers(1, &fb->fb_handle);
  Destroy(fb);
}

void FramebufferBind(struct Framebuffer* fb)
{
  uint32_t handle = 0;
  if (fb != NULL) handle = fb->fb_handle;
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void FramebufferDraw(struct Framebuffer* fb, Vec2i start, Vec2i end)
{
  if (fb->vbo == NULL || fb->vao == NULL) {
    LogWarning(
      "cannot draw framebuffer %d, because it is not drawable",
      fb->fb_handle
    );
    return;
  }

  float vertices[] = {
    start.x, start.y, 0, 0, // tl
    start.x + end.x, start.y, 1, 0, // tr
    start.x, start.y + end.y, 0, 1, // bl
    
    start.x + end.x, start.y, 1, 0, // tr
    start.x + end.x, start.y + end.y, 1, 1, // br
    start.x, start.y + end.y, 0, 1, // bl
  };

  VertexArrayBind(fb->vao);
  BufferObjectSet(fb->vbo, vertices, sizeof(vertices), DRAW_DYNAMIC);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fb->color_handle);

  ShaderBind(framebuffer_draw_shader);
  ShaderSendInt(framebuffer_draw_shader, "tex0", 0);
  VertexArrayDraw(fb->vao, 0, 6, INDEX_TRIANGLES);
}

#include "framebuffer.h"

struct Framebuffer* FramebufferCreate(
  struct Renderer* r,
  struct Vfs* vfs,
  vec2i_t size,
  uint8_t flags)
{
  return r->backend.framebuffer_create(vfs, size, flags);
}

void FramebufferDestroy(struct Renderer* r, struct Framebuffer* fb)
{
  return r->backend.framebuffer_destroy(fb);
}

void FramebufferBind(struct Renderer* r, struct Framebuffer* fb)
{
  return r->backend.framebuffer_bind(fb);
}

void FramebufferResize(
  struct Renderer* r,
  struct Framebuffer* fb,
  vec2i_t size)
{
  return r->backend.framebuffer_resize(fb, size);
}

void FramebufferDraw(
  struct Renderer* r,
  struct Framebuffer* fb,
  vec2i_t start,
  vec2i_t end)
{
  return r->backend.framebuffer_draw(fb, start, end);
}

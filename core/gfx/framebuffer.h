#ifndef __engine_gfx_framebuffer__
#define __engine_gfx_framebuffer__

#include "include.h"
#include "math/vec2i.h"
#include "gfx_types.h"

struct Framebuffer;

struct Framebuffer* FramebufferCreate(vec2i_t size, uint8_t flags);
void FramebufferDestroy(struct Framebuffer* fb);
void FramebufferBind(struct Framebuffer* fb);
void FramebufferResize(struct Framebuffer* fb, vec2i_t size);
void FramebufferDraw(struct Framebuffer* fb, vec2i_t start, vec2i_t end);

#endif

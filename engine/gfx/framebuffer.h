#ifndef __engine_gfx_framebuffer__
#define __engine_gfx_framebuffer__

#include "include.h"
#include "math/vec2i.h"
#include "gfx_types.h"

struct Framebuffer;

struct Framebuffer* FramebufferCreate(Vec2i size, uint8_t flags);
void FramebufferDestroy(struct Framebuffer* fb);
void FramebufferBind(struct Framebuffer* fb);
void FramebufferResize(struct Framebuffer* fb, Vec2i size);
void FramebufferDraw(struct Framebuffer* fb, Vec2i start, Vec2i end);

#endif

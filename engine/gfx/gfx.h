#ifndef __engine_gfx_gfx__
#define __engine_gfx_gfx__

#include "engine.h"
#include "math/vec2f.h"

void InitBackend(struct Engine* engine);
void ClearBackground(float r, float g, float b);
void AdjustViewport(Vec2f size);
void SetDepthTest(bool test);

#endif

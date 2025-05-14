#ifndef __engine_input__
#define __engine_input__

#include "include.h"
#include "key.h"
#include "math/vec2i.h"
#include "engine.h"

bool IsKeyDown(enum Key key);
bool IsMouseDown(int btn);
vec2i_t GetMousePosition(struct Engine* engine);

#endif

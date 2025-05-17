#ifndef __engine_engine__
#define __engine_engine__

#include <lua.h>

#include "include.h"
#include "key.h"
#include "math/vec2i.h"
#include "math/vec2f.h"
#include "timer.h"

struct Engine
{
  struct GLFWwindow* window_handle;
  lua_State* L;
  struct Renderer* renderer;
  int lua_error_handler_index;

  vec2i_t target_screen_size;
  struct Framebuffer* screen;
  vec2i_t screen_size;

  struct Timer timer;
};

void EngineInit(struct Engine* engine, const char* window_title);
void EngineDestroy(struct Engine* engine);
bool EngineIsInit(struct Engine* engine);

void EngineUpdate(struct Engine* engine);
void EngineDraw(struct Engine* engine);

void EngineSwapBuffers(struct Engine* engine);
void EngineClose(struct Engine* engine);
bool EngineIsClosed(struct Engine* engine);

vec2i_t EngineGetWindowSize(struct Engine* engine);
vec2i_t EngineGetScreenSize(struct Engine* engine);

bool IsKeyDown(struct Engine* engine, enum Key key);
bool IsMouseDown(struct Engine* engine, int btn);

vec2f_t GetMousePosition(struct Engine* engine);

#endif

#ifndef __engine_engine__
#define __engine_engine__

#include <lua.h>

#include "include.h"
#include "key.h"
#include "math/vec2i.h"
#include "gfx/framebuffer.h"

struct Engine
{
  struct GLFWwindow* window_handle;
  lua_State* L;
  int lua_error_handler_index;

  Vec2i target_screen_size;
  struct Framebuffer* screen;
  Vec2i screen_size;

  double tick_rate;
  double accum;

  int fps;
  int tps;

  double prev_time;
  double last_fps;
  int frames_rendered;
  int frames_stepped;
};

void EngineInit(struct Engine* engine, const char* window_title);
void EngineDestroy(struct Engine* engine);
bool EngineIsInit(struct Engine* engine);

void EngineUpdate(struct Engine* engine);
void EngineDraw(struct Engine* engine);

void EngineSwapBuffers(struct Engine* engine);
void EngineClose(struct Engine* engine);
bool EngineIsClosed(struct Engine* engine);

Vec2i EngineGetWindowSize(struct Engine* engine);
Vec2i EngineGetScreenSize(struct Engine* engine);

bool IsKeyDown(struct Engine* engine, enum Key key);
bool IsMouseDown(struct Engine* engine, int btn);

#endif

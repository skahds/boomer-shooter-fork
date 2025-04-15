#ifndef __bse_context__
#define __bse_context__

#include <GLFW/glfw3.h>

#include "include.h"
#include "luawrap.h"
#include "renderer.h"
#include "math/vec2i.h"

typedef struct Context
{
  GLFWwindow* window_handle;
  lua_State* L;
  Renderer renderer;
  int error_handler_index;
  Vec2i screen_size;

  double dt;
  int fps;

  double prev_frame_time;
  double last_fps_update;
  int frames_rendered;
} Context;

Context* ctxCreate(const char* title, Vec2i screen_size);
void ctxDestroy(Context* ctx);
bool ctxShouldDoUpdate(const Context* ctx);
void ctxUpdate(Context* ctx);
void ctxDraw(Context* ctx);
void ctxClose(Context* ctx);
// TODO: Update to return an ivec2
int ctxGetWindowWidth(const Context* ctx);
int ctxGetWindowHeight(const Context* ctx);
void ctxGetWindowMousePosition(const Context* ctx, double* x, double* y);
// TODO: Update to return an ivec2
int ctxGetScreenWidth(const Context* ctx);
int ctxGetScreenHeight(const Context* ctx);
void ctxGetScreenMousePosition(const Context* ctx, double* x, double* y);

#endif


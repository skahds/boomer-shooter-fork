#include <glad/glad.h>
#include "context.h"

#include <math.h>

#include "exit_codes.h"
#include "math/vec4f.h"
#include "mesh.h"

typedef struct
{
  float x;
  float y;
  float scale;
} ScreenTransform;

static ScreenTransform getScreenTransform(const Context* ctx) 
{
  float window_width = (float)ctxGetWindowWidth(ctx);
  float window_height = (float)ctxGetWindowHeight(ctx);
  float screen_width = (float)ctxGetScreenWidth(ctx);
  float screen_height = (float)ctxGetScreenHeight(ctx);

  ScreenTransform st;
  st.scale = fmin(window_width / screen_width, window_height / screen_height);
  st.x = (window_width - screen_width * st.scale) / 2;
  st.y = (window_height - screen_height * st.scale) / 2;
  return st;
}

Context* ctxCreate(const char* title, Vec2i screen_size)
{
  assert(screen_size.x > 0);
  assert(screen_size.y > 0);

  Context* ctx = create(Context);

  ctx->L = NULL;
  ctx->error_handler_index = 0;

  ctx->screen_size.x = screen_size.x;
  ctx->screen_size.y = screen_size.y;

  if (glfwInit() < 0) {
    logFatal(could_not_init_glfw, "could not init glfw");
  }
  logDebugInfo("initialized glfw");

#ifdef bse_debug
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

  int wwidth = screen_size.x * 3;
  int wheight = screen_size.y * 3;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  ctx->window_handle = glfwCreateWindow(wwidth, wheight, title, NULL, NULL);
  if (!ctx->window_handle) {
    logFatal(could_not_make_window, "could not create glfw window");
  }
  logDebugInfo("created glfw window - \"%s\" - %dx%d", title, wwidth, wheight);

  glfwSetWindowUserPointer(ctx->window_handle, ctx);
  glfwMakeContextCurrent(ctx->window_handle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    logFatal(could_not_init_glad, "could not init glad");
  }
  logDebugInfo("initialized glad");

  rendererInit(&ctx->renderer, ctx);

  ctx->prev_frame_time = 0;
  ctx->last_fps_update = 0;
  ctx->frames_rendered = 0;

  glfwSwapInterval(0);

  return ctx;
}

void ctxDestroy(Context* ctx)
{
  rendererDestroy(&ctx->renderer);
  glfwDestroyWindow(ctx->window_handle);

  // this segfaults??
  glfwTerminate();

  destroy(ctx);
}

bool ctxShouldDoUpdate(const Context* ctx)
{
  return !glfwWindowShouldClose(ctx->window_handle);
}

void ctxUpdate(Context* ctx)
{
  glfwPollEvents();

  double cur_time = glfwGetTime();
  double dt = cur_time - ctx->prev_frame_time;
  ctx->dt = dt;
  ctx->prev_frame_time = cur_time;

  ctx->frames_rendered++;
  double last_update = cur_time - ctx->last_fps_update;
  if (last_update > 1) {
    ctx->fps = (float)ctx->frames_rendered / last_update;
    ctx->last_fps_update = cur_time;
    ctx->frames_rendered = 0;
  }

  if (ctx->L) {
    lua_getglobal(ctx->L, "update");
    if (!lua_isnil(ctx->L, -1)) {
      if (!lua_isfunction(ctx->L, -1)) {
        logFatal(incorrect_lua_state, "global 'update' should be a function");
      }
      lua_pushnumber(ctx->L, dt); // dt
      lua_pcall(ctx->L, 1, 0, ctx->error_handler_index);
    }
  }
}

void ctxDraw(Context* ctx)
{
  glClearColor(0.2, 0.2, 0.2, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, ctx->screen_size.x * 3, ctx->screen_size.y * 3);

  if (ctx->L) {
    lua_getglobal(ctx->L, "draw");
    if (!lua_isnil(ctx->L, -1)) {
      if (!lua_isfunction(ctx->L, -1)) {
        logFatal(incorrect_lua_state, "global 'draw' should be a function");
      }
      lua_pcall(ctx->L, 0, 0, ctx->error_handler_index);
    }
  }

  glfwSwapBuffers(ctx->window_handle);
}

void ctxClose(Context* ctx)
{
  glfwSetWindowShouldClose(ctx->window_handle, GL_TRUE);
}

int ctxGetWindowWidth(const Context* ctx)
{
  int width;
  glfwGetFramebufferSize(ctx->window_handle, &width, NULL);
  return width;
}

int ctxGetWindowHeight(const Context* ctx)
{
  int height;
  glfwGetFramebufferSize(ctx->window_handle, NULL, &height);
  return height;
}

void ctxGetWindowMousePosition(const Context* ctx, double* x, double* y)
{
  glfwGetCursorPos(ctx->window_handle, x, y);
}

int ctxGetScreenWidth(const Context* ctx)
{
  return ctx->screen_size.x;
}

int ctxGetScreenHeight(const Context* ctx)
{
  return ctx->screen_size.y;
}

void ctxGetScreenMousePosition(const Context* ctx, double* x, double* y)
{
  double mx, my;
  glfwGetCursorPos(ctx->window_handle, &mx, &my);
  ScreenTransform st = getScreenTransform(ctx);

  if (x) {
    mx -= st.x;
    mx = floor(mx / st.scale);
    *x = mx;
  }

  if (y) {
    my -= st.y;
    my = floor(my / st.scale);
    *y = my;
  }
}

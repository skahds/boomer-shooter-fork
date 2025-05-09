#include "engine.h"

#include <GLFW/glfw3.h>

#include "gfx/gfx.h"
#include "math.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  // AdjustViewport((Vec2f){width, height});
  struct Engine* engine = (struct Engine*)glfwGetWindowUserPointer(window);
  Vec2i screen_size = (Vec2i){
    ceil((float)width / engine->screen_scaling),
    ceil((float)height / engine->screen_scaling),
  };
  engine->screen_size = screen_size;
  FramebufferResize(engine->screen, screen_size);
}

void EngineInit(struct Engine* engine, const char* window_title)
{
  LogInfo("initializing engine...");

  if (glfwInit() < 0) {
    LogFatal(1, "could not initialize glfw");
  }

  engine->window_handle =
    glfwCreateWindow(320 * 3, 180 * 3, window_title, NULL, NULL);
  if (engine->window_handle == NULL) {
    LogFatal(1, "could not initialize glfw window");
  }
  LogInfo("created glfw window");

  InitBackend(engine);

  glfwSetWindowUserPointer(engine->window_handle, engine);

  glfwSetFramebufferSizeCallback(
    engine->window_handle, FramebufferSizeCallback);

  engine->L = NULL;
  engine->lua_error_handler_index = 0;

  engine->tick_rate = 30;
  engine->screen_scaling = 4;
  engine->accum = 0;
  engine->fps = 0;
  engine->tps = 0;
  engine->prev_time = 0;
  engine->last_fps = 0;
  engine->frames_rendered = 0;

  Vec2i window_size = EngineGetWindowSize(engine);
  engine->screen_size = (Vec2i){
    ceil((float)window_size.x),
    ceil((float)window_size.y),
  };

  engine->screen = FramebufferCreate(
    engine->screen_size,
    FRAMEBUFFER_COLOR_BUF | FRAMEBUFFER_DEPTH_BUF | FRAMEBUFFER_DRAWABLE
  );
}

void EngineDestroy(struct Engine* engine)
{
  LogInfo("destroying engine...");

  FramebufferDestroy(engine->screen);

  glfwDestroyWindow(engine->window_handle);
  // for some reason this causes a false positive(?) memory leak with asan.
  // but i fairly sure it doesn't actually, but i'm keeping it off for debug
  // builds regardless for my own sanity
#ifndef bse_debug
  glfwTerminate();
#endif

  engine->window_handle = NULL;
}

bool EngineIsInit(struct Engine* engine)
{
  return engine->window_handle != NULL;
}

void EngineUpdate(struct Engine* engine)
{
  glfwPollEvents();

  double current_time = glfwGetTime();
  double dt = current_time - engine->prev_time;
  engine->prev_time = current_time;

  engine->accum += dt;

  while (engine->accum > 1.0 / engine->tick_rate) {
    engine->accum -= 1.0 / engine->tick_rate;
    if (engine->accum < 0) engine->accum = 0;

    engine->frames_stepped++;
    double last_fps_update = current_time - engine->last_fps;
    if (last_fps_update > 1) {
      engine->fps = (double)engine->frames_rendered / last_fps_update;
      engine->tps = (double)engine->frames_stepped / last_fps_update;
      engine->last_fps = current_time;
      engine->frames_rendered = 0;
      engine->frames_stepped = 0;
    }

    if (engine->L != NULL) {
      lua_getglobal(engine->L, "step");
      if (!lua_isnil(engine->L, -1)) {
        if (!lua_isfunction(engine->L, -1)) {
          LogFatal(1, "global 'step' must be a function");
        }
        lua_pcall(engine->L, 0, 0, engine->lua_error_handler_index);
      }
    }
  }
}

void EngineDraw(struct Engine* engine)
{
  // Draw to screen
  FramebufferBind(engine->screen);
  SetDepthTest(true);

  AdjustViewport((Vec2f){engine->screen_size.x, engine->screen_size.y});

  ClearBackground(0.2, 0.2, 0.2);

  if (engine->L != NULL) {
    lua_getglobal(engine->L, "draw");
    if (!lua_isnil(engine->L, -1)) {
      if (!lua_isfunction(engine->L, -1)) {
        LogFatal(1, "global 'draw' must be a function");
      }
      lua_pcall(engine->L, 0, 0, engine->lua_error_handler_index);
    }
  }

  EngineSwapBuffers(engine);

  // Draw screen
  FramebufferBind(NULL);
  Vec2i wsize = EngineGetWindowSize(engine);
  AdjustViewport((Vec2f){wsize.x, wsize.y});

  ClearBackground(0, 0, 0);
  SetDepthTest(false);

  FramebufferDraw(engine->screen, (Vec2i){-1, 1}, (Vec2i){2, -2});

  engine->frames_rendered++;
}

void EngineSwapBuffers(struct Engine* engine)
{
  glfwSwapBuffers(engine->window_handle);
}

void EngineClose(struct Engine* engine)
{
  glfwSetWindowShouldClose(engine->window_handle, GLFW_TRUE);
}

bool EngineIsClosed(struct Engine* engine)
{
  return glfwWindowShouldClose(engine->window_handle);
}

Vec2i EngineGetWindowSize(struct Engine* engine)
{
  Vec2i size;
  glfwGetWindowSize(engine->window_handle, &size.x, &size.y);
  return size;
}

Vec2i EngineGetScreenSize(struct Engine* engine)
{
  return engine->screen_size;
}

bool IsKeyDown(struct Engine* engine, enum Key key)
{
  return glfwGetKey(engine->window_handle, key) == GLFW_PRESS;
}

bool IsMouseDown(struct Engine* engine, int btn)
{
  return glfwGetMouseButton(engine->window_handle, btn - 1) == GLFW_PRESS;
}

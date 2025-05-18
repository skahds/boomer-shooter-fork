#include "engine.h"

#include <GLFW/glfw3.h>

#include "gfx/gfx.h"
#include "gfx/framebuffer.h"
#include "math.h"
#include "mem.h"
#include "wrap/wrap.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  struct Engine* engine = (struct Engine*)glfwGetWindowUserPointer(window);

  float screen_width = engine->target_screen_size.x;
  float screen_height = engine->target_screen_size.y;

  if (width > height) { // the width should change dynamically
    float target_aspect = (float)screen_width / (float)screen_height;
    float aspect = (float)width / (float)height;
    screen_width *= aspect / target_aspect;
  } else { // the height should change dynamically
    float target_aspect = (float)screen_height / (float)screen_width;
    float aspect = (float)height / (float)width;
    screen_height *= aspect / target_aspect;
  }

  vec2i_t screen_size = (vec2i_t){ceil(screen_width), ceil(screen_height)};

  engine->screen_size = screen_size;
  FramebufferResize(engine->renderer, engine->screen, screen_size);
}

void EngineInit(struct Engine* engine, struct EngineConfig conf)
{
  LogInfo("initializing engine...");

  const char* vfs_mnt = "./";
  enum VfsError vfs_err = VfsInit(&engine->vfs, vfs_mnt);
  if (vfs_err) {
    LogFatal(1, "could not mount vfs at '%s'", vfs_mnt);
  }

  if (glfwInit() < 0) {
    LogFatal(1, "could not initialize glfw");
  }

  engine->window_handle = glfwCreateWindow(
    conf.window_size.x, conf.window_size.y, conf.window_title, NULL, NULL);
  if (engine->window_handle == NULL) {
    LogFatal(1, "could not initialize glfw window");
  }
  LogInfo("created glfw window");

  glfwMakeContextCurrent(engine->window_handle);
  glfwSetWindowUserPointer(engine->window_handle, engine);
  glfwSetFramebufferSizeCallback(
    engine->window_handle, FramebufferSizeCallback);
  glfwSwapInterval(conf.vsync);

  engine->renderer = NULL;
  InitBackend(engine, GFX_BACKEND_OPENGL);

  engine->target_screen_size = conf.screen_size;
  engine->screen_size = (vec2i_t){
    ceil((float)conf.window_size.x),
    ceil((float)conf.window_size.y),
  };
  engine->screen = FramebufferCreate(
    engine->renderer,
    &engine->vfs,
    engine->screen_size,
    FRAMEBUFFER_COLOR_BUF | FRAMEBUFFER_DEPTH_BUF | FRAMEBUFFER_DRAWABLE
  );


  engine->timer = TimerCreate();

  // set up game
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  Wrap(L, engine);

  LogInfo("starting game...");
  if (!ProtectedDoFile(L, engine, "game/main.lua")) {
    LogFatal(1, "was not able to run game");
  }
}

void EngineDestroy(struct Engine* engine)
{
  LogInfo("destroying engine...");

  lua_close(engine->L);

  FramebufferDestroy(engine->renderer, engine->screen);

  if (engine->renderer != NULL)
    Destroy(engine->renderer);

  glfwDestroyWindow(engine->window_handle);

  VfsDestroy(&engine->vfs);
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

  TimerStep(&engine->timer);

  while (TimerShouldTick(&engine->timer)) {
    TimerNewTick(&engine->timer);

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
  FramebufferBind(engine->renderer, engine->screen);
  SetDepthTest(engine->renderer, true);

  AdjustViewport(
    engine->renderer,
    (vec2f_t){engine->screen_size.x, engine->screen_size.y}
  );

  ClearBackground(engine->renderer, 0.2, 0.2, 0.2);

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
  FramebufferBind(engine->renderer, NULL);
  vec2i_t wsize = EngineGetWindowSize(engine);
  AdjustViewport(engine->renderer, (vec2f_t){wsize.x, wsize.y});

  ClearBackground(engine->renderer, 0, 0, 0);
  SetDepthTest(engine->renderer, false);

  FramebufferDraw(
    engine->renderer,
    engine->screen,
    (vec2i_t){-1, 1},
    (vec2i_t){2, -2}
  );

  TimerDoneRendering(&engine->timer);
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

vec2i_t EngineGetWindowSize(struct Engine* engine)
{
  vec2i_t size;
  glfwGetWindowSize(engine->window_handle, &size.x, &size.y);
  return size;
}

vec2i_t EngineGetScreenSize(struct Engine* engine)
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

vec2f_t GetMousePosition(struct Engine* engine)
{
  double x, y;
  glfwGetCursorPos(engine->window_handle, &x, &y);
  return (vec2f_t){x, y};
}

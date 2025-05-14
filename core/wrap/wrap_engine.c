#include "wrap.h"

#include <GLFW/glfw3.h>

#include "math/vec2i.h"

static int L_GetTotalTime(lua_State* L)
{
  lua_pushnumber(L, glfwGetTime());
  return 1;
}

static int L_GetFps(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->timer.fps);
  return 1;
}

static int L_GetTps(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->timer.tps);
  return 1;
}

static int L_CloseEngine(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  EngineClose(engine);
  return 0;
}

static int L_GetWindowSize(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  vec2i_t s = EngineGetWindowSize(engine);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_GetScreenSize(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  vec2i_t s = EngineGetScreenSize(engine);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_GetInterpolation(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->timer.accum / (1.0 / engine->timer.tick_rate));
  return 1;
}

static int L_Interpolate(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  lua_pushnumber(
    L,
    (b - a) * (engine->timer.accum / (1.0 / engine->timer.tick_rate)) + a
  );
  return 1;
}

static int L_IsKeyDown(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  int key = luaL_checkinteger(L, 1);
  lua_pushboolean(L, IsKeyDown(engine, (enum Key)key));
  return 1;
}

static int L_IsMouseDown(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  int btn = luaL_checkinteger(L, 1);
  lua_pushboolean(L, IsMouseDown(engine, btn));
  return 1;
}

static int L_GetMousePosition(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  vec2f_t pos = GetMousePosition(engine);
  lua_pushnumber(L, pos.x);
  lua_pushnumber(L, pos.y);
  return 2;
} 

luaL_Reg engine_funcs[] = {
  {"GetTotalTime", L_GetTotalTime},
  {"GetFps", L_GetFps},
  {"GetTps", L_GetTps},
  {"CloseEngine", L_CloseEngine},
  {"GetWindowSize", L_GetWindowSize},
  {"GetScreenSize", L_GetScreenSize},
  {"GetInterpolation", L_GetInterpolation},
  {"Interpolate", L_Interpolate},
  {"IsKeyDown", L_IsKeyDown},
  {"IsMouseDown", L_IsMouseDown},
  {"GetMousePosition", L_GetMousePosition},
  {NULL, NULL},
};

void WrapEngine(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, engine_funcs);
  lua_pop(L, 1);
}

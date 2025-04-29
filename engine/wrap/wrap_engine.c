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
  lua_pushnumber(L, engine->fps);
  return 1;
}

static int L_GetTps(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->tps);
  return 1;
}

static int L_GetWindowSize(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  Vec2i s = EngineGetWindowSize(engine);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_GetInterpolation(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->accum / (1.0 / engine->tick_rate));
  return 1;
}

static int L_Interpolate(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  lua_pushnumber(L, (b - a) * (engine->accum / (1.0 / engine->tick_rate)) + a);
  return 1;
}

luaL_Reg engine_funcs[] = {
  {"GetTotalTime", L_GetTotalTime},
  {"GetFps", L_GetFps},
  {"GetTps", L_GetTps},
  {"GetWindowSize", L_GetWindowSize},
  {"GetInterpolation", L_GetInterpolation},
  {"Interpolate", L_Interpolate},
  {NULL, NULL},
};

void WrapEngine(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, engine_funcs);
  lua_pop(L, 1);
}

#include "include.h"
#include "exit_codes.h"
#include "context.h"

static int L_errorHandler(lua_State* L)
{
  lua_getglobal(L, "debug");
  if (!lua_istable(L, -1)) {
    logFatal(
      lua_vm_error,
      "%s\nDebug library has been removed. Cannot provide stack trace.",
      lua_tostring(L, 1));
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_iscfunction(L, -1)) {
    logFatal(
      lua_vm_error,
      "%s\n'debug.traceback' has been removed. Cannot provide stack trace.",
      lua_tostring(L, 1));
  }

  lua_pushvalue(L, -3);
  lua_pushnumber(L, 0);
  lua_call(L, 2, 1);

  if (lua_isstring(L, -1)) {
    lua_close(L);
    logFatal(lua_vm_error, "%s", lua_tostring(L, -1));
  }
  lua_close(L);
  logFatal(lua_vm_error, "%s", lua_tostring(L, -3));
  return 0;
}

static int L_getTotalTime(lua_State* L)
{
  lua_pushnumber(L, glfwGetTime());
  return 1;
}
static int L_getDelta(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  lua_pushnumber(L, ctx->dt);
  return 1;
}

static int L_getFps(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  lua_pushinteger(L, ctx->fps);
  return 1;
}

luaL_Reg L_time[] = {
  {"getTotalTime", L_getTotalTime},
  {"getDelta", L_getDelta},
  {"getFps", L_getFps},
  {NULL, NULL}
};

static int L_getWindowSize(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  lua_pushinteger(L, ctxGetWindowWidth(ctx));
  lua_pushinteger(L, ctxGetWindowHeight(ctx));
  return 2;
}

static int L_closeWindow(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  ctxClose(ctx);
  return 0;
}

static int L_getWindowMousePosition(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  double x, y;
  ctxGetWindowMousePosition(ctx, &x, &y);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

luaL_Reg L_window[] = {
  {"getSize", L_getWindowSize},
  {"getMousePosition", L_getWindowMousePosition},
  {"close", L_closeWindow},
  {NULL, NULL}
};

static int L_getScreenSize(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  lua_pushinteger(L, ctxGetScreenWidth(ctx));
  lua_pushinteger(L, ctxGetScreenHeight(ctx));
  return 2;
}

static int L_getScreenMousePosition(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  double x, y;
  ctxGetScreenMousePosition(ctx, &x, &y);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

luaL_Reg L_screen[] = {
  {"getSize", L_getScreenSize},
  {"getMousePosition", L_getScreenMousePosition},
  {NULL, NULL}
};

void wrapContext(lua_State* L, Context* ctx)
{
  ctx->L = L;

  lua_pushcfunction(L, L_errorHandler);
  ctx->error_handler_index = lua_gettop(L);

  luaL_register(L, "time", L_time);
  luaL_register(L, "window", L_window);
  luaL_register(L, "screen", L_screen);

  lua_pushlightuserdata(L, ctx);
  lua_setfield(L, LUA_REGISTRYINDEX, "bse_context");
}

void protectedDoFile(lua_State* L, const char* file)
{
  if (luaL_dofile(L, file) != LUA_OK) {
    L_errorHandler(L);
  }
}

Context* luaGetContext(lua_State* L)
{
  lua_getfield(L, LUA_REGISTRYINDEX, "bse_context");
  return (Context*)lua_touserdata(L, -1);
}

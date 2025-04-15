#include "luawrap.h"

#include "include.h"
#include "log.h"

static void catArgs(lua_State* L, int start)
{
  int argc = lua_gettop(L);

  for (int i = start; i <= argc; i++) {
    lua_getglobal(L, "tostring");
    lua_pushvalue(L, i);
    lua_call(L, 1, 1);

    if (i != argc) {
      lua_pushstring(L, " ");
    }
  }
  lua_concat(L, (argc - start + 1) * 2 - 1);
}

static int L_logInfo(lua_State* L)
{
  catArgs(L, 1);
  logInfo("%s", lua_tostring(L, -1));
  return 0;
}

static int L_logWarning(lua_State* L)
{
  catArgs(L, 1);
  logWarning("%s", lua_tostring(L, -1));
  return 0;
}

static int L_logError(lua_State* L)
{
  catArgs(L, 1);
  logError("%s", lua_tostring(L, -1));
  return 0;
}

static int L_logFatal(lua_State* L)
{
  catArgs(L, 2);
  logFatal(lua_tointeger(L, 1), "%s", lua_tostring(L, -1));
  return 0;
}

luaL_Reg L_log[] = {
  {"info", L_logInfo},
  {"warning", L_logWarning},
  {"error", L_logError},
  {"fatal", L_logFatal},
  {NULL, NULL},
};

void wrapLog(lua_State* L)
{
  luaL_register(L, "log", L_log);
}

#include "wrap.h"

#include "log.h"

static void CatArgs(lua_State* L, int start)
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

static int L_LogInfo(lua_State* L)
{
  CatArgs(L, 1);
  LogInfo("%s", lua_tostring(L, -1));
  return 0;
}

static int L_LogWarning(lua_State* L)
{
  CatArgs(L, 1);
  LogWarning("%s", lua_tostring(L, -1));
  return 0;
}

static int L_LogError(lua_State* L)
{
  CatArgs(L, 1);
  LogError("%s", lua_tostring(L, -1));
  return 0;
}

static int L_LogFatal(lua_State* L)
{
  CatArgs(L, 2);
  LogFatal(luaL_checkinteger(L, 1), "%s", lua_tostring(L, -1));
  return 0;
}

luaL_Reg log_funcs[] = {
  {"LogInfo", L_LogInfo},
  {"LogWarning", L_LogWarning},
  {"LogError", L_LogError},
  {"LogFatal", L_LogFatal},
  {NULL, NULL},
};

void WrapLog(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, log_funcs);
  lua_pop(L, 1);
}

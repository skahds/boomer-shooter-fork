#include "luawrap.h"

static void clearGlobal(lua_State* L, const char* global)
{
  lua_pushnil(L);
  lua_setglobal(L, global);
}

lua_State* initLua()
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  clearGlobal(L, "print"); // use `log`
  
  return L;
}



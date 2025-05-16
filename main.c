#include "include.h"

#include "engine.h"
#include "wrap/wrap.h"

int main()
{
  LogInfo("os: %s", bse_os_str);

  struct Engine engine;
  EngineInit(&engine, "Chime");

  // TODO: move this into core/engine.c
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  Wrap(L, &engine);

  ProtectedDoFile(L, &engine, "game/main.lua");

  while (!EngineIsClosed(&engine))
  {
    EngineUpdate(&engine);
    EngineDraw(&engine);
  }

  lua_close(L);

  EngineDestroy(&engine);
}

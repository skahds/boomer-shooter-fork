#include "include.h"

#include "engine.h"
#include "wrap/wrap.h"

int main()
{
  struct Engine engine;
  EngineInit(&engine, "Boomer Shooter");

  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  Wrap(L, &engine);

  ProtectedDoFile(L, &engine, "game/main.lua");

  while (!EngineIsClosed(&engine))
  {
    if (IsKeyDown(&engine, KEY_ESCAPE)) {
      EngineClose(&engine);
    }

    EngineUpdate(&engine);
    EngineDraw(&engine);
  }

  lua_close(L);

  EngineDestroy(&engine);
}

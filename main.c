#include "include.h"

#include "engine.h"

int main()
{
  LogInfo("os: %s", bse_os_str);

  struct Engine engine;
  EngineInit(&engine);

  while (!EngineIsClosed(&engine))
  {
    EngineUpdate(&engine);
    EngineDraw(&engine);
  }

  EngineDestroy(&engine);
}

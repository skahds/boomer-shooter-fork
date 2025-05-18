#include "include.h"

#include "engine.h"

int main()
{
  LogInfo("os: %s", bse_os_str);

  struct EngineConfig conf = (struct EngineConfig){
    .window_title = "DEMONCHIME",
    .window_size = (vec2i_t){320 * 3, 180 * 3},
    .screen_size = (vec2i_t){320, 180},
    .vsync = 0,
  };

  struct Engine engine;
  EngineInit(&engine, conf);

  while (!EngineIsClosed(&engine))
  {
    EngineUpdate(&engine);
    EngineDraw(&engine);
  }

  EngineDestroy(&engine);
}

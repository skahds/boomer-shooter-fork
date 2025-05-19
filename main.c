#include "include.h"

#include "engine.h"

int main(int argc, const char* args[])
{
  LogInfo("os: %s", bse_os_str);

  const char* mount_path = "./";
  if (argc > 1) mount_path = args[1];

  struct EngineConfig conf = (struct EngineConfig){
    .mount_path = mount_path,
    .window_title = "DEMONCHIME",
    .window_size = (vec2i_t){320 * 3, 180 * 3},
    .screen_size = (vec2i_t){320, 180},
    .vsync = 0,
  };

  struct Engine engine;
  EngineInit(&engine, conf);

  VfsMount(&engine.vfs, "dat.zip");

  EngineInitLua(&engine);

  while (!EngineIsClosed(&engine))
  {
    EngineUpdate(&engine);
    EngineDraw(&engine);
  }

  EngineDestroy(&engine);
}

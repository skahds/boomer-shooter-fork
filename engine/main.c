#include "include.h"

#include "engine.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "math/transform.h"
#include "wrap/wrap.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SimpleVertex
{
  float x;
  float y;
  float z;
  float u;
  float v;
  float r;
  float g;
  float b;
  float a;
};

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

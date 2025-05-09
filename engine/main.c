#include "include.h"

#include "engine.h"
#include "wrap/wrap.h"
#include "gfx/framebuffer.h"
#include "gfx/gfx.h"

int main()
{
  struct Engine engine;
  EngineInit(&engine, "Boomer Shooter");

  struct Framebuffer* fb = FramebufferCreate(
    (Vec2i){320, 180},
    FRAMEBUFFER_COLOR_BUF | FRAMEBUFFER_DEPTH_BUF | FRAMEBUFFER_DRAWABLE
  );

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

    FramebufferBind(fb);
    ClearBackground(0.2, 0.2, 0.2);
    SetDepthTest(true);
    EngineDraw(&engine, (Vec2i){320, 180});

    FramebufferBind(NULL);
    Vec2i wsize = EngineGetWindowSize(&engine);
    AdjustViewport((Vec2f){wsize.x, wsize.y});

    ClearBackground(0, 0, 0);
    SetDepthTest(false);

    FramebufferDraw(fb, (Vec2i){-1, 1}, (Vec2i){2, -2});
  }

  lua_close(L);

  FramebufferDestroy(fb);
  EngineDestroy(&engine);
}

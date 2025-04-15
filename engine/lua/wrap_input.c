#include "luawrap.h"

#include <GLFW/glfw3.h>

#include "context.h"
#include "keys.h"

const char* key_str_map[Key_count] = {
#define KEY_DEF(name, _) #name,
#include "key_def.h"
#undef KEY_DEF
};

Key key_index_map[Key_count] = {
#define KEY_DEF(name, _) Key_##name,
#include "key_def.h"
#undef KEY_DEF
};

static int L_isKeyDown(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  return glfwGetKey(ctx->window_handle, lua_tointeger(L, 1)) == GLFW_PRESS;
}

static int L_isMouseDown(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  int button = lua_tointeger(L, 1);
  button--; // In the Lua api, it should start at 1, but here it starts at 0
  int state = glfwGetMouseButton(ctx->window_handle, button);
  return state == GLFW_PRESS;
}

luaL_Reg L_input[] = {
  {"isKeyDown", L_isKeyDown},
  {"isMouseDown", L_isMouseDown},
  {NULL, NULL}
};

void wrapInput(lua_State* L)
{
  lua_newtable(L);

  for (int i = 0; i < Key_count; i++) {
    // Map the string to the key index
    lua_pushinteger(L, key_index_map[i]);
    lua_setfield(L, -2, key_str_map[i]);
    // Map the key index to the string
    lua_pushinteger(L, key_index_map[i]);
    lua_pushstring(L, key_str_map[i]);
    lua_settable(L, -3);
  }

  lua_setglobal(L, "Key");

  const char* button_names[] = {
    "left",
    "right",
    "middle",
    "b4",
    "b5",
    "b6",
    "b7",
    "b8",
  };

  lua_newtable(L);

  for (int i = 0; i < 8; i++) {
    // Same as the keys
    lua_pushinteger(L, i + 1);
    lua_setfield(L, -2, button_names[i]);

    lua_pushinteger(L, i + 1);
    lua_pushstring(L, button_names[i]);
    lua_settable(L, -3);
  }

  lua_setglobal(L, "MouseButton");

  luaL_register(L, "input", L_input);
}

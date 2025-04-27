#include "wrap.h"

#include "texture.h"
#include "mem.h"

static int L_LoadTexture(lua_State* L)
{
  const char* path = luaL_checkstring(L, 1);
  struct Texture* tex = Create(struct Texture);
  *tex = TextureLoad(path);
  CreateLuaData(L, tex, TEXTURE_MT_NAME, LUA_TYPE_TEXTURE);
  return 1;
}

luaL_Reg texture_funcs[] = {
  {"LoadTexture", L_LoadTexture},
  {NULL, NULL},
};

static int L_TextureMt_GetSize(lua_State* L)
{
  struct Texture* tex = (struct Texture*)ReadLuaData(L, 1, LUA_TYPE_TEXTURE);
  lua_pushinteger(L, tex->width);
  lua_pushinteger(L, tex->height);
  return 2;
}

static int L_TextureMt_Bind(lua_State* L)
{
  struct Texture* tex = (struct Texture*)ReadLuaData(L, 1, LUA_TYPE_TEXTURE);
  TextureBind(tex, luaL_checkinteger(L, 2));
  return 2;
}

static int L_TextureMt__index(lua_State* L)
{
  luaL_getmetatable(L, TEXTURE_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_TextureMt__gc(lua_State* L)
{
  struct Texture* tex = (struct Texture*)ReadLuaData(L, 1, LUA_TYPE_TEXTURE);
  TextureDestroy(tex);
  Destroy(tex);
  return 0;
}

luaL_Reg texture_mt[] = {
  {"GetSize", L_TextureMt_GetSize},
  {"Bind", L_TextureMt_Bind},
  {"__index", L_TextureMt__index},
  {"__gc", L_TextureMt__gc},
  {NULL, NULL},
};

void WrapTexture(lua_State* L)
{
  lua_getglobal(L, "bse");
  RegisterFunctions(L, texture_funcs);

  luaL_newmetatable(L, TEXTURE_MT_NAME);
  RegisterFunctions(L, texture_mt);

  lua_pop(L, 2);
}

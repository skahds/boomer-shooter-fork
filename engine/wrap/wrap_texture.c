#include "wrap.h"

#include "gfx/texture.h"
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
  lua_pushinteger(L, tex->size.x);
  lua_pushinteger(L, tex->size.y);
  return 2;
}

static int L_TextureMt_GenerateMipmaps(lua_State* L)
{
  struct Texture* tex = (struct Texture*)ReadLuaData(L, 1, LUA_TYPE_TEXTURE);
  TextureGenerateMipmaps(tex);
  return 0;
}

static int L_TextureMt_SetFilter(lua_State* L)
{
  struct Texture* tex = (struct Texture*)ReadLuaData(L, 1, LUA_TYPE_TEXTURE);
  enum TextureFilter min = luaL_checkinteger(L, 2);
  enum TextureFilter mag = luaL_checkinteger(L, 3);
  TextureSetFilter(tex, min, mag);
  return 0;
}

static int L_TextureMt_SetWrap(lua_State* L)
{
  struct Texture* tex = (struct Texture*)ReadLuaData(L, 1, LUA_TYPE_TEXTURE);
  enum TextureWrap x = luaL_checkinteger(L, 2);
  enum TextureWrap y = luaL_checkinteger(L, 3);
  TextureSetWrap(tex, x, y);
  return 0;
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
  {"GenerateMipmaps", L_TextureMt_GenerateMipmaps},
  {"SetFilter", L_TextureMt_SetFilter},
  {"SetWrap", L_TextureMt_SetWrap},
  {"Bind", L_TextureMt_Bind},
  {"__index", L_TextureMt__index},
  {"__gc", L_TextureMt__gc},
  {NULL, NULL},
};

void WrapTexture(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, texture_funcs);

  luaL_newmetatable(L, TEXTURE_MT_NAME);
  RegisterFunctions(L, texture_mt);

  lua_pop(L, 2);
}

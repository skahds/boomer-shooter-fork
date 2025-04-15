#include "luawrap.h"

#include "texture.h"

#define expected_texture_errmsg "expected type 'texture'"

static int L_textureLoad(lua_State* L)
{
  Texture* texture = create(Texture);

  const char* path = luaL_checkstring(L, 1);

  *texture = textureLoad(path);

  LuaData* data = (LuaData*)lua_newuserdata(L, sizeof(LuaData));
  data->type = LuaDataType_texture;
  data->ptr = texture;

  luaL_getmetatable(L, "texturemt");
  lua_setmetatable(L, -2);

  return 1;
}

luaL_Reg texture[] = {
  {"load", L_textureLoad},
  {NULL, NULL},
};

static int L_textureMtGetSize(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_texture) {
    luaL_error(L, expected_texture_errmsg);
  }

  Texture* texture = (Texture*)data->ptr;

  lua_pushnumber(L, texture->width);
  lua_pushnumber(L, texture->height);
  return 2;
}

static int L_textureMt__index(lua_State* L)
{
  luaL_getmetatable(L, "texturemt");
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_textureMt__gc(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_texture) {
    luaL_error(L, expected_texture_errmsg);
  }

  textureDestroy(*((Texture*)data->ptr));
  destroy(data->ptr);
  return 0;
}

luaL_Reg texture_mt[] = {
  {"getSize", L_textureMtGetSize},
  {"__index", L_textureMt__index},
  {"__gc", L_textureMt__gc},
  {NULL, NULL},
};

void wrapTexture(lua_State* L)
{
  luaL_register(L, "texture", texture);

  luaL_newmetatable(L, "texturemt");
  for (luaL_Reg* f = texture_mt; f->name != NULL; f++) {
    lua_pushcfunction(L, f->func);
    lua_setfield(L, -2, f->name);
  }
  lua_pop(L, 1);
}

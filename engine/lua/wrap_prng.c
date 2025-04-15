#include "luawrap.h"

#include "include.h"
#include "prng.h"

#define expected_prng_errmsg "expected type 'prng'"

static int L_prngCreate(lua_State* L)
{
  Prng* p = create(Prng);
  *p = prngCreate(luaL_optinteger(L, 1, 0));

  LuaData* data = (LuaData*)lua_newuserdata(L, sizeof(LuaData));
  data->type = LuaDataType_prng;
  data->ptr = p;

  luaL_getmetatable(L, "prngmt");
  lua_setmetatable(L, -2);

  return 1;
}

static int L_prngMtNextInt(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_prng) {
    luaL_error(L, expected_prng_errmsg);
  }
  Prng* p = (Prng*)data->ptr;
  lua_pushinteger(L, prngGetInt(p));
  return 1;
}

static int L_prngMtNext(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_prng) {
    luaL_error(L, expected_prng_errmsg);
  }
  Prng* p = (Prng*)data->ptr;
  lua_pushnumber(L, prngGetDouble(p));
  return 1;
}

static int L_prngMtRange(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_prng) {
    luaL_error(L, expected_prng_errmsg);
  }
  Prng* p = (Prng*)data->ptr;
  double d = prngGetDoubleRange(p, lua_tonumber(L, 2), lua_tonumber(L, 3));
  lua_pushnumber(L, d);
  return 1;
}

static int L_prngMtRangeInt(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_prng) {
    luaL_error(L, expected_prng_errmsg);
  }
  Prng* p = (Prng*)data->ptr;
  int i = prngGetIntRange(p, lua_tointeger(L, 2), lua_tointeger(L, 3));
  lua_pushinteger(L, i);
  return 1;
}

static int L_prngMt__index(lua_State* L)
{
  luaL_getmetatable(L, "prngmt");
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_prngMt__gc(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_prng) {
    luaL_error(L, expected_prng_errmsg);
  }
  destroy(data->ptr);
  return 0;
}

luaL_Reg prng_mt[] = {
  {"nextInt", L_prngMtNextInt},
  {"next", L_prngMtNext},
  {"range", L_prngMtRange},
  {"rangeInt", L_prngMtRangeInt},
  {"__index", L_prngMt__index},
  {"__gc", L_prngMt__gc},
  {NULL, NULL},
};

luaL_Reg prng[] = {
  {"create", L_prngCreate},
  {NULL, NULL},
};

void wrapPrng(lua_State* L)
{
  luaL_register(L, "prng", prng);

  luaL_newmetatable(L, "prngmt");
  for (luaL_Reg* f = prng_mt; f->name != NULL; f++) {
    lua_pushcfunction(L, f->func);
    lua_setfield(L, -2, f->name);
  }
  lua_pop(L, 1);
}

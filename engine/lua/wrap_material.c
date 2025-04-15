#include "luawrap.h"

#include "include.h"
#include "material.h"
#include "context.h"

#define expected_material_errmsg "expected type 'material'"

static int L_materialDefault(lua_State* L)
{
  Context* ctx = luaGetContext(L);
  Material* mat = create(Material);
  *mat = materialDefault(&ctx->renderer);

  LuaData* data = (LuaData*)lua_newuserdata(L, sizeof(LuaData));
  data->type = LuaDataType_material;
  data->ptr = mat;

  luaL_getmetatable(L, "materialmt");
  lua_setmetatable(L, -2);

  return 1;
}

static int L_materialMtAddTexture(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_material) {
    luaL_error(L, expected_material_errmsg);
  }
  Material* mat = (Material*)data->ptr;

  const char* uniform = luaL_checkstring(L, 2);

  LuaData* tex_data = (LuaData*)lua_touserdata(L, 3);
  if (tex_data == NULL || tex_data->type != LuaDataType_texture) {
    luaL_error(L, "expected type 'texture' for argument #2");
  }

  MaterialMap* map = materialAddMapping(mat, uniform);
  map->data_type = MMType_texture;
  map->data.tex = tex_data->ptr;

  return 0;
}

static int L_materialMtAddVec4f(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_material) {
    luaL_error(L, expected_material_errmsg);
  }
  Material* mat = (Material*)data->ptr;

  const char* uniform = luaL_checkstring(L, 2);

  float x = luaL_checknumber(L, 3);
  float y = luaL_checknumber(L, 4);
  float z = luaL_checknumber(L, 5);
  float w = luaL_checknumber(L, 6);

  MaterialMap* map = materialAddMapping(mat, uniform);
  map->data_type = MMType_float4;
  map->data.v4f = (Vec4f){x, y, z, w};

  return 0;
}

static int L_materialMt__index(lua_State* L)
{
  luaL_getmetatable(L, "materialmt");
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_materialMt__gc(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_material) {
    luaL_error(L, expected_material_errmsg);
  }

  Material* mat = (Material*)data->ptr;
  materialDestroy(mat);
  destroy(data->ptr);
  return 0;
}

luaL_Reg material_mt[] = {
  {"addVec4f", L_materialMtAddVec4f},
  {"addTexture", L_materialMtAddTexture},
  {"__index", L_materialMt__index},
  {"__gc", L_materialMt__gc},
  {NULL, NULL},
};

luaL_Reg material[] = {
  {"default", L_materialDefault},
  {NULL, NULL},
};

void wrapMaterial(lua_State* L)
{
  luaL_register(L, "material", material);

  luaL_newmetatable(L, "materialmt");
  for (luaL_Reg* f = material_mt; f->name != NULL; f++) {
    lua_pushcfunction(L, f->func);
    lua_setfield(L, -2, f->name);
  }
  lua_pop(L, 1);
}

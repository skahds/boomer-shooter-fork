#include "wrap.h"

#include "mesh.h"
#include "mem.h"

static int L_CreateMeshFormat(lua_State* L)
{
  struct MeshFormat* fmt = Create(struct MeshFormat);
  *fmt = MeshFormatCreate();
  CreateLuaData(L, fmt, MESH_FORMAT_MT_NAME, LUA_TYPE_MESH_FORMAT);
  return 1;
}

luaL_Reg mesh_format_funcs[] = {
  {"CreateMeshFormat", L_CreateMeshFormat},
  {NULL, NULL},
};

static int L_MeshFormatMt_AddAttr(lua_State* L)
{
  struct MeshFormat* fmt = 
    (struct MeshFormat*)ReadLuaData(L, 1, LUA_TYPE_MESH_FORMAT);

  size_t type_name_len;
  const char* type_name = luaL_checklstring(L, 2, &type_name_len);

  enum DataType type = StringToDataType(type_name, type_name_len);
  int count = luaL_checkinteger(L, 3);

  MeshFormatAddAttr(fmt, type, count);
  return 0;
}

static int L_MeshFormatMt__index(lua_State* L)
{
  luaL_getmetatable(L, MESH_FORMAT_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_MeshFormatMt__gc(lua_State* L)
{
  struct MeshFormat* fmt =
    (struct MeshFormat*)ReadLuaData(L, 1, LUA_TYPE_MESH_FORMAT);
  MeshFormatDestroy(fmt);
  Destroy(fmt);
  return 0;
}

luaL_Reg mesh_format_mt[] = {
  {"AddAttr", L_MeshFormatMt_AddAttr},
  {"__index", L_MeshFormatMt__index},
  {"__gc", L_MeshFormatMt__gc},
  {NULL, NULL},
};

void WrapMeshFormat(lua_State* L)
{
  lua_getglobal(L, "bse");
  RegisterFunctions(L, mesh_format_funcs);

  luaL_newmetatable(L, MESH_FORMAT_MT_NAME);
  RegisterFunctions(L, mesh_format_mt);

  lua_pop(L, 2);
}

#include "wrap.h"

#include "mesh.h"
#include "mem.h"

static int L_CreateMesh(lua_State* L)
{
  struct MeshFormat* fmt = 
    (struct MeshFormat*)ReadLuaData(L, 1, LUA_TYPE_MESH_FORMAT);

  struct Mesh* mesh = Create(struct Mesh);
  *mesh = MeshCreate(fmt);
  CreateLuaData(L, mesh, MESH_MT_NAME, LUA_TYPE_MESH);
  return 1;
}

luaL_Reg mesh_funcs[] = {
  {"CreateMesh", L_CreateMesh},
  {NULL, NULL},
};

static int L_MeshMt_SetVertices(lua_State* L)
{
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  luaL_checktype(L, 2, LUA_TTABLE);

  struct MeshFormat* fmt = mesh->format;

  int vertex_count = lua_objlen(L, 2);

  mesh->vertices = CreateVoidArray(fmt->stride, vertex_count);
  mesh->vertex_count = vertex_count;

  for (int vertex_i = 0; vertex_i < vertex_count; vertex_i++) {
    lua_pushinteger(L, vertex_i + 1);
    lua_gettable(L, 2);
    if (lua_type(L, -1) != LUA_TTABLE) {
      Destroy(mesh->vertices);
      luaL_error(L, "vertices must be tables");
    }

    size_t voffset = vertex_i * fmt->stride;
    int offset = 0;

    for (uint16_t attr_i = 0; attr_i < fmt->count; attr_i++) {
      struct VertAttr* attr = &fmt->attrs[attr_i];
      size_t type_size = GetAttrSize(*attr);

      for (int i = 0; i < attr->count; i++) {
        lua_pushinteger(L, offset + 1);
        lua_gettable(L, -2);
        if (lua_type(L, -1) != LUA_TNUMBER) {
          Destroy(mesh->vertices);
          luaL_error(
            L, "currently only numbers are supported values in vertices");
        }

        switch (attr->type) {
          case TYPE_FLOAT: {
            float f = lua_tonumber(L, -1);
            lua_pop(L, 1);
            float* a = mesh->vertices + voffset;
            *a = f;
            break;
          }
          case TYPE_INT: {
            int f = lua_tointeger(L, -1);
            lua_pop(L, 1);
            int* a = mesh->vertices + voffset;
            *a = f;
            break;
          }
        }

        voffset += type_size;
        offset++;
      }
    }
  }

  return 0;
}

static int L_MeshMt_Finalize(lua_State* L)
{
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  MeshFinalize(mesh, lua_toboolean(L, 2));
  return 0;
}

static int L_MeshMt_Draw(lua_State* L)
{
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  MeshDraw(mesh);
  return 0;
}

static int L_MeshMt__index(lua_State* L)
{
  luaL_getmetatable(L, MESH_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_MeshMt__gc(lua_State* L)
{
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  if (mesh->vertex_count != 0) {
    Destroy(mesh->vertices);
    mesh->vertex_count = 0;
  }
  if (mesh->index_count != 0) {
    Destroy(mesh->indices);
    mesh->index_count = 0;
  }
  MeshDestroy(mesh);
  Destroy(mesh);
  return 0;
}

luaL_Reg mesh_mt[] = {
  {"SetVertices", L_MeshMt_SetVertices},
  {"Finalize", L_MeshMt_Finalize},
  {"Draw", L_MeshMt_Draw},
  {"__index", L_MeshMt__index},
  {"__gc", L_MeshMt__gc},
  {NULL, NULL},
};

void WrapMesh(lua_State* L)
{
  lua_getglobal(L, "bse");
  RegisterFunctions(L, mesh_funcs);

  luaL_newmetatable(L, MESH_MT_NAME);
  RegisterFunctions(L, mesh_mt);

  lua_pop(L, 2);
}

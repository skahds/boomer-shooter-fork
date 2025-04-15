#include "luawrap.h"

#include <string.h>

#include "include.h"
#include "mesh.h"
#include "context.h"

#define expected_mesh_errmsg "expected type 'mesh'"

static int L_meshCreate(lua_State* L)
{
  Mesh* mesh = create(Mesh);

  LuaData* mat_data = (LuaData*)lua_touserdata(L, 1);
  if (mat_data == NULL || mat_data->type != LuaDataType_material) {
    luaL_error(L, "expected type 'material' for argument #1");
  }
  Material* mat = mat_data->ptr;

  *mesh = meshCreate(*mat);

  LuaData* data = (LuaData*)lua_newuserdata(L, sizeof(LuaData));
  data->type = LuaDataType_mesh;
  data->ptr = mesh;

  luaL_getmetatable(L, "meshmt");
  lua_setmetatable(L, -2);

  return 1;
}

static int L_meshMtSetVertexFormat(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_mesh) {
    luaL_error(L, expected_mesh_errmsg);
  }

  luaL_checktype(L, 2, LUA_TTABLE);

  Mesh* mesh = (Mesh*)data->ptr;

  int attr_count = lua_objlen(L, 2);

  VertexFormat format;
  format.attr_count = attr_count;
  format.attrs = createArr(VertexAttribute, attr_count);

  size_t vertex_size = 0;

  for (int i = 0; i < attr_count; i++) {
    VertexAttribute* attr = &format.attrs[i];

    lua_pushinteger(L, i + 1);
    lua_gettable(L, 2);
    if (lua_type(L, -1) != LUA_TTABLE) {
      return luaL_error(L, "vertex format must be a table");
    }

    lua_getfield(L, -1, "type");
    if (lua_type(L, -1) != LUA_TSTRING) {
      return luaL_error(L, "vertex attribute 'type' must be a string "
                        "of either 'float' or 'int'");
    }

    size_t stype_len;
    const char* stype = lua_tolstring(L, -1, &stype_len);
    lua_pop(L, 1);

    lua_getfield(L, -1, "components");
    if (lua_type(L, -1) != LUA_TNUMBER) {
      return luaL_error(L, "vertex attribute 'type' must be a number");
    }

    int components = lua_tointeger(L, -1);
    lua_pop(L, 1);

    attr->components = components;

    if (stype_len == 5 && memcmp(stype, "float", stype_len) == 0) {
      attr->type = AttrType_float;
      attr->size = sizeof(float) * components;
    }
    if (stype_len == 3 && memcmp(stype, "int", stype_len) == 0) {
      attr->type = AttrType_int;
      attr->size = sizeof(int) * components;
    }

    vertex_size += attr->size;
  }

  format.vertex_size = vertex_size;

  mesh->format = format;
  return 0;
}

static int L_meshMtSetIndices(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_mesh) {
    luaL_error(L, expected_mesh_errmsg);
  }

  luaL_checktype(L, 2, LUA_TTABLE);

  Mesh* mesh = (Mesh*)data->ptr;

  int indices_count = lua_objlen(L, 2);

  mesh->indices_count = indices_count;
  mesh->indices = createArr(uint16_t, indices_count);

  for (int i = 0; i < indices_count; i++) {
    lua_pushinteger(L, i + 1);
    lua_gettable(L, 2);
    if (lua_type(L, -1) != LUA_TNUMBER) {
      destroy(mesh->indices);
      return luaL_error(L, "index must be a number");
    }

    int iindex = lua_tointeger(L, -1);
    lua_pop(L, 1);

    if (iindex > UINT16_MAX) {
      destroy(mesh->indices);
      return luaL_error(
        L, "index is too large. must be smaller than %d", UINT16_MAX);
    }

    uint16_t index = (uint16_t)iindex;
    mesh->indices[i] = index;
  }

  return 0;
}

static int L_meshMtSetVertices(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_mesh) {
    luaL_error(L, expected_mesh_errmsg);
  }

  luaL_checktype(L, 2, LUA_TTABLE);

  Mesh* mesh = (Mesh*)data->ptr;

  int vertices_count = lua_objlen(L, 2);
  VertexFormat format = mesh->format;

  mesh->vertices_count = vertices_count;
  mesh->vertices = createSized(format.vertex_size * vertices_count);

  for (int i = 0; i < vertices_count; i++) {
    lua_pushinteger(L, i + 1);
    lua_gettable(L, 2);
    if (lua_type(L, -1) != LUA_TTABLE) {
      destroy(mesh->vertices);
      return luaL_error(L, "vertex must be a table");
    }

    size_t vertex_offset = i * format.vertex_size;
    int vi = 0;

    for (int attr_i = 0; attr_i < format.attr_count; attr_i++) {
      
      VertexAttribute* attr = &format.attrs[attr_i];
      size_t component_size;
      switch (attr->type) {
        case AttrType_float: component_size = sizeof(float); break;
        case AttrType_int: component_size = sizeof(int); break;
      }
      
      for (int component = 0; component < attr->components; component++) {
        lua_pushinteger(L, vi + 1);
        lua_gettable(L, -2);
        if (lua_type(L, -1) != LUA_TNUMBER) {
          destroy(mesh->vertices);
          return luaL_error(L, "vertex attribute component must be a number");
        }

        switch (attr->type) {
          case AttrType_float: {
            float f = lua_tonumber(L, -1);
            lua_pop(L, 1);
            float* a = mesh->vertices + vertex_offset;
            *a = f;
            break;
          }
          case AttrType_int: {
            int i = lua_tointeger(L, -1);
            lua_pop(L, 1);
            int* a = mesh->vertices + vertex_offset;
            *a = i;
            break;
          }
        }

        vertex_offset += component_size;

        vi++;
      }
    }
  }

  return 0;
}

static int L_meshMtFinalize(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_mesh) {
    luaL_error(L, expected_mesh_errmsg);
  }

  Mesh* mesh = (Mesh*)data->ptr;

  meshFinalize(mesh, lua_toboolean(L, 2));

  return 0;
}

static int L_meshMtDraw(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_mesh) {
    luaL_error(L, expected_mesh_errmsg);
  }

  Context* ctx = luaGetContext(L);

  float x = luaL_optnumber(L, 2, 0);
  float y = luaL_optnumber(L, 3, 0);
  float z = luaL_optnumber(L, 4, 0);

  float rx = luaL_optnumber(L, 5, 0);
  float ry = luaL_optnumber(L, 6, 0);
  float rz = luaL_optnumber(L, 7, 0);

  Mesh* mesh = (Mesh*)data->ptr;

  meshDraw(&ctx->renderer, mesh, (Vec3f){x, y, z}, (Vec3f){rx, ry, rz});

  return 0;
}

static int L_meshMt__index(lua_State* L)
{
  luaL_getmetatable(L, "meshmt");
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_meshMt__gc(lua_State* L)
{
  LuaData* data = (LuaData*)lua_touserdata(L, 1);
  if (data == NULL || data->type != LuaDataType_mesh) {
    luaL_error(L, expected_mesh_errmsg);
  }

  Mesh* mesh = (Mesh*)data->ptr;

  if (mesh->vertices != NULL) {
    destroy(mesh->vertices);
  }
  if (mesh->indices != NULL) {
    destroy(mesh->indices);
  }
  if (mesh->format.attrs != simple_vertex_format.attrs) {
    destroy(mesh->format.attrs);
  }

  meshDestroy(mesh);
  destroy(data->ptr);
  return 0;
}

luaL_Reg mesh_mt[] = {
  {"setVertexFormat", L_meshMtSetVertexFormat},
  {"setVertices", L_meshMtSetVertices},
  {"setIndices", L_meshMtSetIndices},
  {"finalize", L_meshMtFinalize},
  {"draw", L_meshMtDraw},
  {"__index", L_meshMt__index},
  {"__gc", L_meshMt__gc},
  {NULL, NULL},
};

luaL_Reg mesh[] = {
  {"create", L_meshCreate},
  {NULL, NULL},
};

void wrapMesh(lua_State* L)
{
  luaL_register(L, "mesh", mesh);

  luaL_newmetatable(L, "meshmt");
  for (luaL_Reg* f = mesh_mt; f->name != NULL; f++) {
    lua_pushcfunction(L, f->func);
    lua_setfield(L, -2, f->name);
  }
  lua_pop(L, 1);
}

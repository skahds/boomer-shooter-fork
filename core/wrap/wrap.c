#include "wrap.h"

static int L_ErrorHandler(lua_State* L)
{
  lua_getglobal(L, "debug");
  if (!lua_istable(L, -1)) {
    LogFatal(
      1, "%s\ndebug library has been removed; cannot provide stack trace",
      lua_tostring(L, 1));
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_iscfunction(L, -1)) {
    LogFatal(
      1, "%s\n'debug.traceback' has been removed; cannot provide stack trace",
      lua_tostring(L, 1));
  }

  lua_pushvalue(L, 1);
  lua_pushnumber(L, 0);
  lua_call(L, 2, 1);

  if (lua_isstring(L, -1)) {
    LogError("%s", lua_tostring(L, -1));
  } else {
    LogError("%s\n(stack trace unavailable)", lua_tostring(L, 1));
  }
  lua_close(L);
  exit(1);
  return 0;
}

static const char* DataTypeToString(enum LuaDataType type)
{
  switch (type) {
    case LUA_TYPE_INVALID: return "Invalid";
    case LUA_TYPE_ENGINE: return "Engine";
    case LUA_TYPE_IMAGE: return "Image";
    case LUA_TYPE_TEXTURE: return "Texture";
    case LUA_TYPE_MESH: return "Mesh";
    case LUA_TYPE_VERTEX_FORMAT: return "VertexFormat";
    case LUA_TYPE_SHADER: return "Shader";
    case LUA_TYPE_MAT4: return "Mat4";
    case LUA_TYPE_PRNG: return "Prng";
  }
  return "Invalid";
}

static void TypeError(lua_State* L, int index, enum LuaDataType expected)
{
  const char* type_str = DataTypeToString(expected);
  luaL_error(L, "expected type '%s' for argument #%d", type_str, index);
}

void CreateLuaData(
  lua_State* L,
  void* data,
  const char* mt,
  enum LuaDataType type)
{
  struct LuaData* ld =
    (struct LuaData*)lua_newuserdata(L, sizeof(struct LuaData));
  ld->type = type;
  ld->ptr = data;

  luaL_getmetatable(L, mt);
  lua_setmetatable(L, -2);
}

void* ReadLuaData(lua_State* L, int index, enum LuaDataType expected)
{
  if (!lua_isuserdata(L, index)) {
    TypeError(L, index, expected);
  }

  struct LuaData* data = (struct LuaData*)lua_touserdata(L, index);

  if (data->type != expected) {
    TypeError(L, index, expected);
  }

  if (data->ptr == NULL) {
    luaL_error(L, "cannot use freed userdata");
  }

  return data->ptr;
}

void RegisterFunctions(lua_State* L, const luaL_Reg* funcs)
{
  for (const luaL_Reg* func = funcs; func->name != NULL; func++) {
    lua_pushcfunction(L, func->func);
    lua_setfield(L, -2, func->name);
  }
}

void ProtectedDoFile(lua_State* L, struct Engine* engine, const char* file)
{
  if (luaL_dofile(L, file) != LUA_OK) {
    lua_pushcfunction(L, L_ErrorHandler);
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);
  }
}

void Wrap(lua_State* L, struct Engine* engine)
{
  engine->L = L;

  lua_pushcfunction(L, L_ErrorHandler);
  engine->lua_error_handler_index = lua_gettop(L);

  lua_newtable(L);
  lua_setglobal(L, CORE_NAME);

  WrapEngine(L);
  WrapEnums(L);
  WrapTexture(L);
  WrapLog(L);
  WrapShader(L);
  WrapVertexFormat(L);
  WrapMesh(L);
  WrapMat4(L);
  WrapPrng(L);

  luaL_newmetatable(L, ENGINE_MT_NAME);

  CreateLuaData(L, engine, ENGINE_MT_NAME, LUA_TYPE_ENGINE);
  lua_setfield(L, LUA_REGISTRYINDEX, "__BSE_ENGINE__");
}

struct Engine* GetEngine(lua_State* L)
{
  lua_getfield(L, LUA_REGISTRYINDEX, "__BSE_ENGINE__");
  struct Engine* engine = (struct Engine*)ReadLuaData(L, -1, LUA_TYPE_ENGINE);
  lua_pop(L, 1);
  return engine;
}

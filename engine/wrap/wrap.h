#ifndef __engine_wrap__
#define __engine_wrap__

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "include.h"
#include "engine.h"
#include "math/mat4.h"

enum LuaDataType
{
  LUA_TYPE_INVALID = 0,
  LUA_TYPE_ENGINE,
  LUA_TYPE_IMAGE,
  LUA_TYPE_TEXTURE,
  LUA_TYPE_MESH,
  LUA_TYPE_MESH_FORMAT,
  LUA_TYPE_SHADER,
  LUA_TYPE_MAT4,
  LUA_TYPE_PRNG,
};

#define ENGINE_MT_NAME "__ENGINE_MT__"
#define IMAGE_MT_NAME "__IMAGE_MT__"
#define TEXTURE_MT_NAME "__TEXTURE_MT__"
#define MESH_MT_NAME "__MESH_MT__"
#define MESH_FORMAT_MT_NAME "__MESH_FORMAT_MT__"
#define SHADER_MT_NAME "__SHADER_MT__"
#define MAT4_MT_NAME "__MAT4_MT__"
#define PRNG_MT_NAME "__PRNG_MT__"

struct LuaMat4
{
  Mat4 m;
};

struct LuaData
{
  enum LuaDataType type;
  void* ptr;
};

void CreateLuaData(
  lua_State* L, void* data, const char* mt, enum LuaDataType type);
void* ReadLuaData(lua_State* L, int index, enum LuaDataType expected);
struct Engine* GetEngine(lua_State* L);
void RegisterFunctions(lua_State* L, const luaL_Reg* funcs);
void ProtectedDoFile(lua_State* L, struct Engine* engine, const char* file);

void Wrap(lua_State* L, struct Engine* engine);
void WrapEngine(lua_State* L);
void WrapTexture(lua_State* L);
void WrapLog(lua_State* L);
void WrapShader(lua_State* L);
void WrapMeshFormat(lua_State* L);
void WrapMesh(lua_State* L);
void WrapMat4(lua_State* L);
void WrapPrng(lua_State* L);

#endif

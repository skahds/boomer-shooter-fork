#ifndef __bse_luawrap__
#define __bse_luawrap__

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "include.h"

struct Context;

typedef enum
{
  LuaDataType_prng,
  LuaDataType_image,
  LuaDataType_texture,
  LuaDataType_mesh,
  LuaDataType_material,
} LuaDataType;

typedef struct
{
  LuaDataType type;
  void* ptr;
} LuaData;

lua_State* initLua();

void wrapLog(lua_State* L);
void wrapInput(lua_State* L);
void wrapContext(lua_State* L, struct Context* ctx);
void wrapPrng(lua_State* L);
void wrapMesh(lua_State* L);
void wrapMaterial(lua_State* L);
void wrapTexture(lua_State* L);

struct Context* luaGetContext(lua_State* L);
void protectedDoFile(lua_State* L, const char* file);

#endif

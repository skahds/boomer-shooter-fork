#include "wrap.h"

#include "gfx/shader.h"
#include "mem.h"

static int L_LoadShader(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  const char* vert_path = luaL_checkstring(L, 1);
  const char* frag_path = luaL_checkstring(L, 2);
  struct Shader* shader = ShaderLoadFromFiles(
    engine->renderer,
    engine->vfs,
    vert_path,
    frag_path
  );
  CreateLuaData(L, shader, SHADER_MT_NAME, LUA_TYPE_SHADER);
  return 1;
}

static int L_LoadShaderFromSource(lua_State* L)
{
  struct Renderer* r = GetEngine(L)->renderer;
  const char* vert_src = luaL_checkstring(L, 1);
  const char* frag_src = luaL_checkstring(L, 2);
  struct Shader* shader = ShaderLoadFromSource(r, vert_src, frag_src);
  CreateLuaData(L, shader, SHADER_MT_NAME, LUA_TYPE_SHADER);
  return 1;
}

luaL_Reg shader_funcs[] = {
  {"LoadShader", L_LoadShader},
  {"LoadShaderFromSource", L_LoadShaderFromSource},
  {NULL, NULL},
};

static int L_ShaderMt_SendF(lua_State* L)
{
  int arg_count = lua_gettop(L) - 2;
  if (arg_count > 4) {
    luaL_error(L, "cannot send more than 4 floats to shader");
  }

  struct Shader* shader = (struct Shader*)ReadLuaData(L, 1, LUA_TYPE_SHADER);
  const char* uniform = luaL_checkstring(L, 2);

  float args[4];
  for (int i = 0; i < arg_count; i++) {
    args[i] = luaL_checknumber(L, i + 3);
  }

  struct Renderer* r = GetEngine(L)->renderer;
  switch (arg_count) {
    case 1: ShaderSendFloat(r, shader, uniform, args[0]); break;
    case 2: ShaderSendVec2f(r, shader, uniform, *((vec2f_t*)&args)); break;
    case 3: ShaderSendVec3f(r, shader, uniform, *((vec3f_t*)&args)); break;
    case 4: ShaderSendVec4f(r, shader, uniform, *((vec4f_t*)&args)); break;
  }

  return 0;
}

static int L_ShaderMt_SendI(lua_State* L)
{
  int arg_count = lua_gettop(L) - 2;
  if (arg_count > 4) {
    luaL_error(L, "cannot send more than 4 floats to shader");
  }

  struct Shader* shader = (struct Shader*)ReadLuaData(L, 1, LUA_TYPE_SHADER);
  const char* uniform = luaL_checkstring(L, 2);

  int args[4];
  for (int i = 0; i < arg_count; i++) {
    args[i] = luaL_checkinteger(L, i + 3);
  }

  struct Renderer* r = GetEngine(L)->renderer;
  switch (arg_count) {
    case 1: ShaderSendInt(r, shader, uniform, args[0]); break;
    case 2: ShaderSendVec2i(r, shader, uniform, *((vec2i_t*)&args)); break;
    case 3: ShaderSendVec3i(r, shader, uniform, *((vec3i_t*)&args)); break;
    case 4: ShaderSendVec4i(r, shader, uniform, *((vec4i_t*)&args)); break;
  }

  return 0;
}

static int L_ShaderMt_SendMat4(lua_State* L)
{
  struct Shader* shader = (struct Shader*)ReadLuaData(L, 1, LUA_TYPE_SHADER);
  const char* uniform = luaL_checkstring(L, 2);
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 3, LUA_TYPE_MAT4);
  
  struct Renderer* r = GetEngine(L)->renderer;
  ShaderSendMat4(r, shader, uniform, m->m);

  return 0;
}

static int L_ShaderMt_Bind(lua_State* L)
{
  struct Renderer* r = GetEngine(L)->renderer;
  struct Shader* shader = (struct Shader*)ReadLuaData(L, 1, LUA_TYPE_SHADER);
  ShaderBind(r, shader);
  return 0;
}

static int L_ShaderMt__index(lua_State* L)
{
  luaL_getmetatable(L, SHADER_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_ShaderMt__gc(lua_State* L)
{
  struct Renderer* r = GetEngine(L)->renderer;
  struct Shader* shader = (struct Shader*)ReadLuaData(L, 1, LUA_TYPE_SHADER);
  ShaderDestroy(r, shader);
  return 0;
}

luaL_Reg shader_mt[] = {
  {"SendF", L_ShaderMt_SendF},
  {"SendI", L_ShaderMt_SendI},
  {"SendMat4", L_ShaderMt_SendMat4},
  {"Bind", L_ShaderMt_Bind},
  {"__index", L_ShaderMt__index},
  {"__gc", L_ShaderMt__gc},
  {NULL, NULL},
};

void WrapShader(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, shader_funcs);

  luaL_newmetatable(L, SHADER_MT_NAME);
  RegisterFunctions(L, shader_mt);

  lua_pop(L, 2);
}

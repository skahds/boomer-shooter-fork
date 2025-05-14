#include "wrap.h"

#include "gfx/gfx_types.h"
#include "key.h"

struct LuaEnum
{
  const char* name;
  int value;
};

#define IMAGE_FORMAT_NAME "img_format"
#define TEXTURE_FILTER_NAME "tex_filter"
#define TEXTURE_WRAP_NAME "tex_wrap"
#define KEY_NAME "key"

struct LuaEnum image_format[] = {
  {"R8", IMAGE_FORMAT_R8},
  {"RA8", IMAGE_FORMAT_RA8},
  {"RGB8", IMAGE_FORMAT_RGB8},
  {"RGBA8", IMAGE_FORMAT_RGBA8},
  {NULL, 0},
};

struct LuaEnum texture_filter[] = {
  {"NEAREST", TEXTURE_FILTER_NEAREST},
  {"NEAREST_MIPMAP", TEXTURE_FILTER_NEAREST_MIPMAP},
  {"NEAREST_MIPMAP_NEAREST", TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST},
  {"LINEAR", TEXTURE_FILTER_LINEAR},
  {"LINEAR_MIPMAP", TEXTURE_FILTER_LINEAR_MIPMAP},
  {"LINEAR_MIPMAP_NEAREST", TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST},
  {NULL, 0},
};

struct LuaEnum texture_wrap[] = {
  {"REPEAT", TEXTURE_WRAP_REPEAT},
  {"MIRRORED_REPEAT", TEXTURE_WRAP_MIRRORED_REPEAT},
  {"CLAMP_EDGE", TEXTURE_WRAP_CLAMP_EDGE},
  {"CLAMP_BORDER", TEXTURE_WRAP_CLAMP_BORDER},
  {NULL, 0},
};

struct LuaEnum key_wrap[] = {
#define KEY_DEF(name, _) {#name, KEY_##name},
#include "key_def.h"
#undef KEY_DEF
  {NULL, 0},
};

static void RegisterEnum(
  lua_State* L,
  const char* name,
  struct LuaEnum* enooms)
{
  lua_newtable(L);

  for (struct LuaEnum* enoom = enooms; enoom->name != NULL; enoom++) {
    lua_pushinteger(L, enoom->value);
    lua_setfield(L, -2, enoom->name);
  }

  lua_setfield(L, -2, name);
}

void WrapEnums(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);

  RegisterEnum(L, IMAGE_FORMAT_NAME, image_format);
  RegisterEnum(L, TEXTURE_FILTER_NAME, texture_filter);
  RegisterEnum(L, TEXTURE_WRAP_NAME, texture_wrap);
  RegisterEnum(L, KEY_NAME, key_wrap);

  lua_pop(L, 1);
}

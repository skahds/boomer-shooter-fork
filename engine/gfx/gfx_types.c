#include "gfx_types.h"

#include <string.h>

struct DataTypeName
{
  const char* name;
  size_t len;
  enum GfxDataType type;
};

struct DataTypeName type_names[] = {
  {"half", 4, TYPE_HALF},
  {"float", 5, TYPE_FLOAT},
  {"double", 6, TYPE_DOUBLE},
  {"uchar", 5, TYPE_UCHAR},
  {"char", 4, TYPE_CHAR},
  {"ushort", 6, TYPE_USHORT},
  {"short", 5, TYPE_SHORT},
  {"uint", 4, TYPE_UINT},
  {"int", 3, TYPE_INT},
  {NULL, 0, 0},
};

size_t GetGfxDataTypeSize(enum GfxDataType type)
{
  switch (type) {
    case TYPE_UNKNOWN: return sizeof(signed char);
    case TYPE_HALF: return sizeof(float) / 2;
    case TYPE_FLOAT: return sizeof(float);
    case TYPE_DOUBLE: return sizeof(double);
    case TYPE_UCHAR: return sizeof(unsigned char);
    case TYPE_CHAR: return sizeof(signed char);
    case TYPE_USHORT: return sizeof(unsigned short);
    case TYPE_SHORT: return sizeof(signed short);
    case TYPE_UINT: return sizeof(unsigned int);
    case TYPE_INT: return sizeof(signed int);
  }
  return 0; // unreachable
}

enum GfxDataType StringToDataType(const char* str, size_t len)
{
  for (struct DataTypeName* type = type_names; type->name != NULL; type++) {
    if (type->len == len &&
        memcmp(type->name, str, len) == 0) {
      return type->type;
    }
  }
  return TYPE_UNKNOWN;
}

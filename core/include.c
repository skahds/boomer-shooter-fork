#include "include.h"
#include "mem.h"

#include <stdarg.h>
#include <string.h>

char* Concat(const char* lhs, ...)
{
  const char* str;

  va_list args;
  va_start(args, lhs);

  size_t len = strlen(lhs);

  while ((str = va_arg(args, const char*))) {
    len += strlen(str);
  }

  va_end(args);

  char* cat = CreateArray(char, len + 1);

  va_start(args, lhs);

  strcpy(cat, lhs);
  while ((str = va_arg(args, const char*))) {
    strcat(cat, str);
  }
  
  va_end(args);

  return cat;
}


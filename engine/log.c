#include "log.h"

#include "include.h"

#include <stdarg.h>
#include <stdio.h>

static void logMsg(
  FILE* file,
  const char* restrict tag,
  const char* restrict fmt,
  va_list args)
{
  if (fmt == NULL) return;
  if (tag != NULL) fputs(tag, file);
  vfprintf(file, fmt, args);
  fputc('\n', file);
}

void __logInfo(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  logMsg(stdout, log_info_tag, fmt, args);
  va_end(args);
}

void logWarning(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  logMsg(stderr, log_warning_tag, fmt, args);
  va_end(args);
}

void logError(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  logMsg(stderr, log_error_tag, fmt, args);
  va_end(args);
}

void logFatal(int exit_code, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  logMsg(stderr, log_fatal_tag, fmt, args);
  va_end(args);
  exit(exit_code);
}

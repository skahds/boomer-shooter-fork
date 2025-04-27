#ifndef __engine_log__
#define __engine_log__

#include <stdio.h>

// __VA_OPT__ is not portable in C99, might be worth making these inline 
// functions?

// `logDebug()` should be a no-op in release
#ifdef bse_debug
# define stringify2(x) #x
# define stringify(x) stringify2(x)
# define LogDebug(fmt, ...) \
  __Log(stderr, "[debug] [" __FILE__ ":" stringify(__LINE__) "] ", \
    fmt __VA_OPT__(,) __VA_ARGS__)
#else
# define logDebug(fmt, ...)
#endif

#define LogInfo(fmt, ...) \
  __Log(stdout, "[info] ", fmt __VA_OPT__(,) __VA_ARGS__)
#define LogWarning(fmt, ...) \
  __Log(stderr, "[warning] ", fmt __VA_OPT__(,) __VA_ARGS__)
#define LogError(fmt, ...) \
  __Log(stderr, "[error] ", fmt __VA_OPT__(,) __VA_ARGS__)
#define LogFatal(c, fmt, ...) \
  (__Log(stderr, "[error] ", fmt __VA_OPT__(,) __VA_ARGS__), exit(1))

void __Log(FILE* file, const char* tag, const char* fmt, ...);

#endif

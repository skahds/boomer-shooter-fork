#ifndef __engine_log__
#define __engine_log__

#include <stdio.h>

// __VA_OPT__ is not portable in C99, might be worth making these inline 
// functions?

// `logDebug()` should be a no-op in release

#define TEXT_NORMAL "\033[0m"
#define TEXT_BOLD_GREEN "\033[32;1m"
#define TEXT_BOLD_YELLOW "\033[33;1m"
#define TEXT_BOLD_RED "\033[31;1m"
#define TEXT_DARK_GRAY "\033[90m"

#ifdef bse_debug
# define stringify2(x) #x
# define stringify(x) stringify2(x)
# define LogDebug(fmt, ...) \
  __Log(stderr, \
    "[" TEXT_BOLD_GREEN "debug" TEXT_NORMAL "] [" TEXT_DARK_GRAY \
    __FILE__ ":" stringify(__LINE__) TEXT_NORMAL "] ", \
    fmt __VA_OPT__(,) __VA_ARGS__)
#else
# define logDebug(fmt, ...)
#endif

#define LogInfo(fmt, ...) \
  __Log(stdout, \
    "[" TEXT_DARK_GRAY "info" TEXT_NORMAL "] ", \
    fmt __VA_OPT__(,) __VA_ARGS__)

#define LogWarning(fmt, ...) \
  __Log(stderr, \
    "[" TEXT_BOLD_YELLOW "warning" TEXT_NORMAL "] ", \
    fmt __VA_OPT__(,) __VA_ARGS__)

#define LogError(fmt, ...) \
  __Log(stderr, \
    "[" TEXT_BOLD_RED "error" TEXT_NORMAL "] ", \
    fmt __VA_OPT__(,) __VA_ARGS__)

#define LogFatal(c, fmt, ...) \
  (__Log( \
         stderr, \
         "[" TEXT_BOLD_RED "error" TEXT_NORMAL "] ", \
         fmt __VA_OPT__(,) __VA_ARGS__), exit(1))

void __Log(FILE* file, const char* tag, const char* fmt, ...);

#endif

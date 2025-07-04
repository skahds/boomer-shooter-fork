#ifndef __engine_log__
#define __engine_log__

#include <stdio.h>

// FIXME: __VA_OPT__ is not portable in C99, might be worth making these inline 
// functions?

#ifdef bse_linux
#define TEXT_NORMAL "\033[0m"
#define TEXT_BOLD_GREEN "\033[32;1m"
#define TEXT_BOLD_YELLOW "\033[33;1m"
#define TEXT_BOLD_RED "\033[31;1m"
#define TEXT_DARK_GRAY "\033[90m"
#else
#define TEXT_NORMAL
#define TEXT_BOLD_GREEN
#define TEXT_BOLD_YELLOW
#define TEXT_BOLD_RED
#define TEXT_DARK_GRAY
#endif

// `logDebug()` should be a no-op in release
#ifdef bse_debug
# define stringify2(x) #x
# define stringify(x) stringify2(x)
# define LogDebug(...) \
  __Log(stderr, \
    "[" TEXT_BOLD_GREEN "debug" TEXT_NORMAL "] [" TEXT_DARK_GRAY \
    __FILE__ ":" stringify(__LINE__) TEXT_NORMAL "] ", \
    __VA_ARGS__)
#else
# define LogDebug(...)
#endif

#define LogInfo(...) \
  __Log(stdout, \
    "[" TEXT_DARK_GRAY "info" TEXT_NORMAL "] ", \
    __VA_ARGS__)

#define LogWarning(...) \
  __Log(stderr, \
    "[" TEXT_BOLD_YELLOW "warning" TEXT_NORMAL "] ", \
    __VA_ARGS__)

#define LogError(...) \
  __Log(stderr, \
    "[" TEXT_BOLD_RED "error" TEXT_NORMAL "] ", \
    __VA_ARGS__)

#define LogFatal(c, ...) \
  (__Log( \
         stderr, \
         "[" TEXT_BOLD_RED "error" TEXT_NORMAL "] ", \
         __VA_ARGS__), exit(c))

void __Log(FILE* file, const char* tag, const char* fmt, ...);

#endif

#ifndef __bse_log__
#define __bse_log__

#ifndef log_info_tag
# define log_info_tag "[info] "
#endif
#ifndef log_error_tag
# define log_error_tag "[error] "
#endif
#ifndef log_warning_tag
# define log_warning_tag "[warning] "
#endif
#ifndef log_fatal_tag
# define log_fatal_tag "[fatal] "
#endif

#ifdef bse_debug
# define logDebugInfo(...) __logInfo(__VA_ARGS__)
#else
# define logDebugInfo(...)
#endif

#define logInfo(...) __logInfo(__VA_ARGS__)

// info and warning do nothing in release builds
void __logInfo(const char* fmt, ...);
void logWarning(const char* fmt, ...);
void logError(const char* fmt, ...);

// will close the program
void logFatal(int exit_code, const char* fmt, ...);

#endif

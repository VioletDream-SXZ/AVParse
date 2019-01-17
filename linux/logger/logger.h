#ifndef GY_LOGGER_H
#define GY_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define GY_INFO_INFO     "[GY::INF]"
#define GY_ERROR_INFO    "[GY::ERROR]"
#define GY_DEBUG_INFO    "[GY::DEBUG]"
#define GY_WARNING_INFO  "[GY::WARNING]"

#define GY_INFO     0
#define GY_ERROR    1
#define GY_DEBUG    2
#define GY_WARNING  3

void gy_log_internal(const char* file, const int line, const int level, const char* psz_fmt, const char* args);
void gy_log_info(const char* file, const int line, const char* psz_fmt, ...);
void gy_log_error(const char* file, const int line, const char* psz_fmt, ...);
void gy_log_debug(const char* file, const int line, const char* psz_fmt, ...);
void gy_log_warning(const char* file, const int line, const char* psz_fmt, ...);

bool setLogOutputFp(FILE* fp);
bool setLogOutputFile(const char* str);

#endif // GY_LOGGER_H
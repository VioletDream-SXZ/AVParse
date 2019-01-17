#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

const int MAX_BUF_SIZE = 1024 * 1024;
static FILE* outFp = stdout;

static void printVaListData(const char* file, const int line, const char* level_info, const char* psz_fmt, va_list args)
{
  static char buff[MAX_BUF_SIZE];
  static int  number = 0;
  
  memset(buff, 0, sizeof(buff));

  time_t tt = time(NULL);
   tm*   tm = localtime(&tt);
  number += sprintf(buff, "[%04d-%02d-%02d %02d:%02d:%02d]", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
  number += sprintf(buff + number, "%s[%020s: %04d]: ", level_info, file, line);
  vsnprintf(buff + number, MAX_BUF_SIZE - number - 1, psz_fmt, args);

  fprintf(outFp, "%s", buff);
}

void gy_log_internal(const char* file, const int line, const int level, const char* psz_fmt, ...)
{
  va_list arg;
  va_start(arg, psz_fmt);
  
  if(level == GY_INFO)
    printVaListData(file, line, GY_INFO_INFO, psz_fmt, arg);
  else if(level == GY_ERROR)
    printVaListData(file, line, GY_ERROR_INFO, psz_fmt, arg);
  else if(level == GY_DEBUG)
    printVaListData(file, line, GY_DEBUG_INFO, psz_fmt, arg);
  else if(level == GY_WARNING)
    printVaListData(file, line, GY_WARNING_INFO, psz_fmt, arg);
  else
    printVaListData(file, line, GY_INFO_INFO, psz_fmt, arg);
  
  va_end(arg);
}

void gy_log_info(const char* file, const int line, const char* psz_fmt, ...)
{
  va_list arg;
  va_start(arg, psz_fmt);
  printVaListData(file, line, GY_INFO_INFO, psz_fmt, arg);
  va_end(arg);
}

void gy_log_error(const char* file, const int line, const char* psz_fmt, ...)
{
  va_list arg;
  va_start(arg, psz_fmt);
  printVaListData(file, line, GY_ERROR_INFO, psz_fmt, arg);
  va_end(arg);
}

void gy_log_debug(const char* file, const int line, const char* psz_fmt, ...)
{
  va_list arg;
  va_start(arg, psz_fmt);
  printVaListData(file, line, GY_DEBUG_INFO, psz_fmt, arg);
  va_end(arg);
}

void gy_log_warning(const char* file, const int line, const char* psz_fmt, ...)
{
  va_list arg;
  va_start(arg, psz_fmt);
  printVaListData(file, line, GY_WARNING_INFO, psz_fmt, arg);
  va_end(arg);
}

bool setLogOutputFp(FILE* fp)
{
  if(fp == NULL)
    return false;
  
  if(outFp != stdout)
    fclose(outFp);
  
  outFp = fp;
  return true;
}

bool setLogOutputFile(const char* str)
{
  FILE* fp = fopen(str, "a+");
  if(fp == NULL)
    return false;
  
  if(outFp != stdout)
    fclose(outFp);
  
  outFp = fp;
  return true;
}
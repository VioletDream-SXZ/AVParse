#include "logger.h"

#include <stdio.h>

static FILE* outFp = stdout;

static void printVaListData(const char* file, const int line, const char* level_info, const char* psz_fmt, va_list args)
{
  static char buff[1024 * 1024];
  static int  number = 0;
  
  memset(buff, 0, sizeof(buff));
  char* point;
  int   value, len;
  
  number = 0;
  
  while(psz_fmt != NULL && (*psz_fmt) != '\0')
  {
    while(psz_fmt != NULL && (*psz_fmt) != 0 && (*psz_fmt) != '%')
    {
      buff[number++] = *psz_fmt;
      psz_fmt++;
    }
    
    if(*psz_fmt == 0 || psz_fmt == NULL)
      break;
    
    psz_fmt++;
    
    switch(*psz_fmt)
    {
      case 's':
        point = va_arg(args, char*);
        strcpy(buff + number, point);
        number += strlen(point);
        break;
      case 'd':
        value = va_arg(args, int);
        len   = sprintf(buff+number, "%d", value);
        number += len;
        break;
      default:
        buff[number++] = '%';
        break;
    }
    
    psz_fmt++;
  }
  
  fprintf(outFp, "%s %s-%d: %s", level_info, file, line, buff);
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

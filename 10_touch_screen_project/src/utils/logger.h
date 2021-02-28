#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdio.h>

#define COLOR_READ "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLACK "\033[0m"

#define LOG_ERROR(fmt, args...) \
  do\
  {\
    fprintf(stderr, COLOR_READ"[%s %d %s]: ", __FILE__, __LINE__, __FUNCTION__);\
    fprintf(stderr, COLOR_READ#fmt COLOR_BLACK "\n", ##args);\
  }while(0)

#define LOG_WARN(fmt, args...) \
  do\
  {\
    fprintf(stderr, COLOR_YELLOW"[%s %d %s]: ", __FILE__, __LINE__, __FUNCTION__);\
    fprintf(stderr, COLOR_YELLOW#fmt COLOR_BLACK "\n", ##args);\
  }while(0)

#define LOG_INFO(fmt, args...) \
  do\
  {\
    fprintf(stdout, "[%s %d %s]: ", __FILE__, __LINE__, __FUNCTION__);\
    fprintf(stdout, fmt "\n", ##args);\
  }while(0)

#endif /* _LOGGER_H */

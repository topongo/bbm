#include "debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

void debug(const char *fmt, ...) {
#if defined(DEBUG)
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "==DBG=> ");
  vfprintf(stderr, fmt, args);
  va_end(args);
#endif
}


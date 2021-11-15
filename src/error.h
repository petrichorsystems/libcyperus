/*
all of the following functions are copied from the following textbook:

Advanced Programming in the UNIX Environment
Second Edition
  W. Richard Stevens
  Stephen A. Rago
ISBN: 0-201-43307-9

there may be changes to both function names and anything related
to improving compatibility with cyperus' programming architecture.

*/

#ifndef LIBCYPERUS_ERRORD_H_
#define LIBCYPERUS_ERRORD_H_

#include <errno.h>
#include <stdarg.h>

static void doit(int, int, const char *, va_list ap);

void error_ret(const char *fmt, ...);
void error_sys(const char *fmt, ...);
void error_exit(int error, const char *fmt, ...);
void error_dump(const char *fmt, ...);
void error_msg(const char *fmt, ...);
void error_quit(const char *fmt, ...);

#endif


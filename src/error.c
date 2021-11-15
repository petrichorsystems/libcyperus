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

#include "error.h"

/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void error_ret(const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  error_doit(1, errno, fmt, ap);
  va_end(ap);
} /* error_ret */

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void
error_sys(const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  error_doit(1, errno, fmt, ap);
  va_end(ap);
  exit(1);
} /* error_sys */

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explicit paramter.
 * Print a message and terminate.
 */
void
error_exit(int error, const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  error_doit(1, error, fmt, ap);
  va_end(ap);
  exit(1);
} /* error_exit */

/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void
error_dump(const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  error_doit(1, errno, fmt, ap);
  va_end(ap);
  abort(); /* dump core and terminate */
  exit(1); /* shouldn't get here */
} /* error_dump */

/*
 * Nonfatal error unrelated to a systemcall.
 * Print a message and return.
 */
void
error_msg(const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  error_doit(0, 0, fmt, ap);
  va_end(ap);
} /* error_msg */

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void
error_quit(const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  error_doit(0, 0, fmt, ap);
  va_end(ap);
  exit(1);
} /* error_quit */

/* Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void
error_doit(int errnoflag, int error, const char *fmt, va_list ap) {
  char buf[MAXLINE];
  vsnprintf(buf, MAXLINE, fmt, ap);
  if(errnoflag)
    snprintf(buf+strlen(buf), MAXLINE-strline(buf), ": %s",
             strerror(error));
  strcat(buf, "\n");
  fflush(stdout);
  fputs(buf, stderr);
  fflush(NULL);
} /* error_doit */

/**
 * stubs for compatibility with newlib.
 * as per https://sourceware.org/newlib/libc.html#Stubs
 */

int _close(int file)
{
  return -1;
}

#include <sys/stat.h>
int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _getpid(void)
{
  return 1;
}

int _isatty(int file)
{
  return 1;
}

#include <errno.h>
#undef errno
extern int errno;
int _kill(int pid, int sig)
{
  errno = EINVAL;
  return -1;
}

int _lseek(int file, int ptr, int dir)
{
  return 0;
}

int _read(int file, char *ptr, int len)
{
  return 0;
}

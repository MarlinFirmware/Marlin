
#ifndef MYGETLINE_H
#define MYGETLINE_H 1

//#include "config.h"

#if !HAVE_GETLINE
  #include <stdio.h>
  ssize_t getdelim (char **lineptr, size_t *n, int delimiter, FILE *fp);
  #define getline(lineptr, n, stream) getdelim (lineptr, n, '\n', stream)
#endif

#endif // MYGETLINE_H


/**
 * getline.c --- Based on...
 *
 * getdelim.c --- Implementation of replacement getdelim function.
 * Copyright (C) 1994, 1996, 1997, 1998, 2001, 2003, 2005 Free
 * Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/* Ported from glibc by Simon Josefsson. */

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#if !HAVE_GETLINE

//#include "getdelim.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#ifndef SIZE_MAX
  #define SIZE_MAX ((size_t) -1)
#endif
#ifndef SSIZE_MAX
  #define SSIZE_MAX ((ssize_t) (SIZE_MAX / 2))
#endif
#if !HAVE_FLOCKFILE
  #undef flockfile
  #define flockfile(x) ((void)0)
#endif
#if !HAVE_FUNLOCKFILE
  #undef funlockfile
  #define funlockfile(x) ((void)0)
#endif

/* Read up to (and including) a DELIMITER from FP into *LINEPTR (and
   NUL-terminate it).  *LINEPTR is a pointer returned from malloc (or
   NULL), pointing to *N characters of space.  It is realloc'ed as
   necessary.  Returns the number of characters read (not including
   the null terminator), or -1 on error or EOF.  */

ssize_t
getdelim (char **lineptr, size_t *n, int delimiter, FILE *fp) {
  ssize_t result;
  size_t cur_len = 0;

  if (lineptr == NULL || n == NULL || fp == NULL) {
    errno = EINVAL;
    return -1;
  }

  flockfile (fp);

  if (*lineptr == NULL || *n == 0) {
    *n = 120;
    *lineptr = (char *) malloc(*n);
    if (*lineptr == NULL) {
      result = -1;
      goto unlock_return;
    }
  }

  for (;;) {
    int i;

    i = getc(fp);
    if (i == EOF) {
      result = -1;
      break;
    }

    /* Make enough space for len+1 (for final NUL) bytes.  */
    if (cur_len + 1 >= *n) {
      size_t needed_max =
        SSIZE_MAX < SIZE_MAX ? (size_t) SSIZE_MAX + 1 : SIZE_MAX;
      size_t needed = 2 * *n + 1;   /* Be generous. */
      char *new_lineptr;

      if (needed_max < needed)
        needed = needed_max;
      if (cur_len + 1 >= needed) {
        result = -1;
        goto unlock_return;
      }

      new_lineptr = (char *) realloc (*lineptr, needed);
      if (new_lineptr == NULL) {
        result = -1;
        goto unlock_return;
      }

      *lineptr = new_lineptr;
      *n = needed;
    }

    (*lineptr)[cur_len] = i;
    cur_len++;

    if (i == delimiter) break;
  }
  (*lineptr)[cur_len] = '\0';
  result = cur_len ? (int) cur_len : (int) result;

  unlock_return:
  funlockfile(fp);
  return result;
}

#endif

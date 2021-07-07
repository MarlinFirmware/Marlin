/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file wirish/syscalls.c
 * @brief newlib stubs
 *
 * Low level system routines used by newlib for basic I/O and memory
 * allocation. You can override most of these.
 */

#include <libmaple/libmaple.h>

#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>

/* If CONFIG_HEAP_START (or CONFIG_HEAP_END) isn't defined, then
 * assume _lm_heap_start (resp. _lm_heap_end) is appropriately set by
 * the linker */
#ifndef CONFIG_HEAP_START
extern char _lm_heap_start;
#define CONFIG_HEAP_START               ((void *)&_lm_heap_start)
#endif
#ifndef CONFIG_HEAP_END
extern char _lm_heap_end;
#define CONFIG_HEAP_END                 ((void *)&_lm_heap_end)
#endif

/*
 * _sbrk -- Increment the program break.
 *
 * Get incr bytes more RAM (for use by the heap).  malloc() and
 * friends call this function behind the scenes.
 */
void *_sbrk(int incr) {
    static void * pbreak = NULL; /* current program break */
    void * ret;

    if (pbreak == NULL) {
        pbreak = CONFIG_HEAP_START;
    }

    if ((CONFIG_HEAP_END - pbreak < incr) ||
        (pbreak - CONFIG_HEAP_START < -incr)) {
        errno = ENOMEM;
        return (void *)-1;
    }

    ret = pbreak;
    pbreak += incr;
    return ret;
}

__weak int _open(const char *path, int flags, ...) {
    return 1;
}

__weak int _close(int fd) {
    return 0;
}

__weak int _fstat(int fd, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

__weak int _isatty(int fd) {
    return 1;
}

__weak int isatty(int fd) {
    return 1;
}

__weak int _lseek(int fd, off_t pos, int whence) {
    return -1;
}

__weak unsigned char getch(void) {
    return 0;
}


__weak int _read(int fd, char *buf, size_t cnt) {
    *buf = getch();

    return 1;
}

__weak void putch(unsigned char c) {
}

__weak void cgets(char *s, int bufsize) {
    char *p;
    int c;
    int i;

    for (i = 0; i < bufsize; i++) {
        *(s+i) = 0;
    }
//    memset(s, 0, bufsize);

    p = s;

    for (p = s; p < s + bufsize-1;) {
        c = getch();
        switch (c) {
        case '\r' :
        case '\n' :
            putch('\r');
            putch('\n');
            *p = '\n';
            return;

        case '\b' :
            if (p > s) {
                *p-- = 0;
                putch('\b');
                putch(' ');
                putch('\b');
            }
            break;

        default :
            putch(c);
            *p++ = c;
            break;
        }
    }
    return;
}

__weak int _write(int fd, const char *buf, size_t cnt) {
    int i;

    for (i = 0; i < cnt; i++)
        putch(buf[i]);

    return cnt;
}

/* Override fgets() in newlib with a version that does line editing */
__weak char *fgets(char *s, int bufsize, void *f) {
    cgets(s, bufsize);
    return s;
}

__weak void _exit(int exitcode) {
    while (1)
        ;
}

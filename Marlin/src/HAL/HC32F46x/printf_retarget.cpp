#ifdef REDIRECT_PRINTF_TO_SERIAL
#include <stdio.h>
#include <stdarg.h>
#include "../../inc/MarlinConfig.h"

// do printf retargeting using stdio hooks (_write or fputc)
// #define PRINTF_STDIO_RETARGET

// target for printf retargeting
#ifndef PRINTF_TARGET
#define PRINTF_TARGET MYSERIAL1
#endif

#ifdef PRINTF_STDIO_RETARGET
#if defined(__GNUC__) && !defined(__CC_ARM)
extern "C" int _write(int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        PRINTF_TARGET.write(pBuffer[i]);
    }

    return size;
}
#else
extern "C" int32_t fputc(int32_t ch, FILE *f)
{
    PRINTF_TARGET.write(pBuffer[i]);
    return ch;
}
#endif
#else
extern "C" int printf(const char *fmt, ...)
{
    va_list argv;
    va_start(argv, fmt);

// format using vsprintf
#define PRINTF_BUFFER_SIZE 512
    char buffer[PRINTF_BUFFER_SIZE];
    int len = vsnprintf(buffer, PRINTF_BUFFER_SIZE, fmt, argv);

    // print to target
    PRINTF_TARGET.print(buffer);

    va_end(argv);
    return len;
}
#endif
#endif

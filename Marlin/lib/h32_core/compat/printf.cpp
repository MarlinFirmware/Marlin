/**
 * write printf to Serial
 */
#include <stdio.h>
#include <stdarg.h>
#include "../src/inc/MarlinConfig.h"

#ifdef REDIRECT_PRINTF_TO_SERIAL

#ifndef PRINTF_TARGET
  #define PRINTF_TARGET MYSERIAL1
#endif

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
#ifdef REDIRECT_PRINTF_TO_SERIAL
#include "inc/MarlinConfig.h"

#ifndef PRINTF_TARGET
#define PRINTF_TARGET MYSERIAL1
#endif

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
#endif

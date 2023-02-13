
#ifndef _WIRISH_WIRISH_TIME_H_
#define _WIRISH_WIRISH_TIME_H_

#include "libmaple_types.h"

void delay(unsigned long ms);
void delayMicroseconds(uint32 us);

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    void yield(void);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif

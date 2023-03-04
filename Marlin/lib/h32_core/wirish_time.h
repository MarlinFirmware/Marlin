#pragma once
#include "libmaple_types.h"
#include "hdsc/common/hc32_ddl.h"

#define delay(ms) Ddl_Delay1ms(ms)
#define delayMicroseconds(us) Ddl_Delay1us(us)

#ifdef __cplusplus
extern "C"
{
#endif

    static void __empty() {}
    void yield() __attribute__((weak, alias("__empty")));

#ifdef __cplusplus
}
#endif

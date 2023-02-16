
#include "wirish_time.h"
#include "libmaple_types.h"
#include "hc32f46x_utility.h"

void delay(unsigned long ms)
{
    volatile uint32_t i = 0ul;
    uint32_t u32Cyc = 0ul;

    u32Cyc = SystemCoreClock;
    u32Cyc = u32Cyc / 10000ul;
    while (ms-- > 0ul)
    {
        yield();
        i = u32Cyc;
        while (i-- > 0ul)
        {
            ;
        }
    }
}

void delayMicroseconds(uint32 us)
{
    Ddl_Delay1us(us);
}

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

static void __empty() {}

#pragma GCC diagnostic pop

void yield(void) {} //__attribute__ ((weak, alias("__empty")));

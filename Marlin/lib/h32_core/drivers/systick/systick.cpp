#include "systick.h"
#include "../../hdsc/common/hc32_ddl.h"

volatile uint32_t uptime = 0;

extern "C" void SysTick_IrqHandler(void)
{
    uptime++;
}

void systick_init()
{
    stc_clk_freq_t clkFreq;
    CLK_GetClockFreq(&clkFreq);
    SysTick_Config(clkFreq.sysclkFreq / TICKS_PER_SECOND);
}

uint32_t millis()
{
    return uptime;
}

uint32_t micros()
{
    //TODO: stub implementation
    DDL_ASSERT(0);
    return uptime * 1000;
}

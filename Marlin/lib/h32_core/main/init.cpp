#include "init.h"
#include "systick.h"
#include "../drivers/adc/adc.h"
#include "../interrupts.h"
#include "../hdsc/common/hc32_ddl.h"

uint32_t CPU_FREQ = 0;

inline void sysclock_init(void)
{
    // setup divisors for the different bus clocks
    stc_clk_sysclk_cfg_t sysClkConf = {
        .enHclkDiv = ClkSysclkDiv1,
        .enExclkDiv = ClkSysclkDiv2,
        .enPclk0Div = ClkSysclkDiv1,
        .enPclk1Div = ClkSysclkDiv2,
        .enPclk2Div = ClkSysclkDiv4,
        .enPclk3Div = ClkSysclkDiv4,
        .enPclk4Div = ClkSysclkDiv2,
    };
    CLK_SysClkConfig(&sysClkConf);

    // configure and enable XTAL clock source
    stc_clk_xtal_cfg_t xtalConf = {
        .enFastStartup = Enable,
        .enMode = ClkXtalModeOsc,
        .enDrv = ClkXtalLowDrv,
    };
    CLK_XtalConfig(&xtalConf);
    CLK_XtalCmd(Enable);

    // configure PLL using XTAL clock as source
    stc_clk_mpll_cfg_t pllConf = {
        .PllpDiv = 4u,
        .PllqDiv = 4u,
        .PllrDiv = 4u,
        .plln = 50u,
        .pllmDiv = 1u,
    };
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&pllConf);

    // enable MPLL and wait until ready
    CLK_MpllCmd(Enable);
    while (CLK_GetFlagStatus(ClkFlagMPLLRdy) != Set)
        ;

    // switch the system clock to MPLL
    CLK_SetSysClkSource(CLKSysSrcMPLL);

    // read system clock
    stc_clk_freq_t clkFreq;
    CLK_GetClockFreq(&clkFreq);
    CPU_FREQ = clkFreq.pclk1Freq;
}

inline void flash_init()
{
    // set flash latency and cache
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_5);
    EFM_InstructionCacheCmd(Enable);
    EFM_Lock();
}

void board_init()
{
#if defined(__CC_ARM) && defined(__TARGET_FPU_VFP)
    SCB->CPACR |= 0x00F00000;
#endif
    // setup VTO register
    SCB->VTOR = ((uint32_t)APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk);

    // init system
    flash_init();
    sysclock_init();
    systick_init();
    interrupts_init();
    adc_init();
}

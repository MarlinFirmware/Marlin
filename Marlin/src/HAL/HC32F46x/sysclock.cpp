/**
 * HC32f460 system clock configuration
 */
#include <core_hooks.h>
#include <drivers/sysclock/sysclock_util.h>

// get BOARD_XTAL_FREQUENCY from configuration / pins
#include "../../inc/MarlinConfig.h"

void core_hook_sysclock_init()
{
    // set wait cycles, as we are about to switch to 200 MHz HCLK
    sysclock_configure_flash_wait_cycles();
    sysclock_configure_sram_wait_cycles();

    // configure MPLLp to 200 MHz output, with different settings depending on XTAL availability
#if BOARD_XTAL_FREQUENCY == 8000000 // 8 MHz XTAL
    // - M = 1   => 8 MHz / 1 = 8 MHz
    // - N = 50  => 8 MHz * 50 = 400 MHz
    // - P = 2   => 400 MHz / 2 = 200 MHz (sysclk)
    // - Q,R = 4 => 400 MHz / 4 = 100 MHz (dont care)
    stc_clk_mpll_cfg_t pllConf = {
        .PllpDiv = 2u, // P
        .PllqDiv = 4u, // Q
        .PllrDiv = 4u, // R
        .plln = 50u,   // N
        .pllmDiv = 1u, // M
    };
    sysclock_configure_xtal();
    sysclock_configure_mpll(ClkPllSrcXTAL, &pllConf);

#elif BOARD_XTAL_FREQUENCY == 16000000 // 16 MHz XTAL
    // - M = 1   => 16 MHz / 1 = 16 MHz
    // - N = 50  => 16 MHz * 25 = 400 MHz
    // - P = 2   => 400 MHz / 2 = 200 MHz (sysclk)
    // - Q,R = 4 => 400 MHz / 4 = 100 MHz (dont care)
    stc_clk_mpll_cfg_t pllConf = {
        .PllpDiv = 2u, // P
        .PllqDiv = 4u, // Q
        .PllrDiv = 4u, // R
        .plln = 50u,   // N
        .pllmDiv = 1u, // M
    };
    sysclock_configure_xtal();
    sysclock_configure_mpll(ClkPllSrcXTAL, &pllConf);

#warning "16 MHz XTAL is not tested yet, please check clock dump on startup and report any issues"

#else // HRC (16 MHz)
    // - M = 1   => 16 MHz / 1 = 16 MHz
    // - N = 25  => 16 MHz * 25 = 400 MHz
    // - P = 2   => 400 MHz / 2 = 200 MHz (sysclk)
    // - Q,R = 4 => 400 MHz / 4 = 100 MHz (dont care)
    stc_clk_mpll_cfg_t pllConf = {
        .PllpDiv = 2u, // P
        .PllqDiv = 4u, // Q
        .PllrDiv = 4u, // R
        .plln = 25u,   // N
        .pllmDiv = 1u, // M
    };
    sysclock_configure_hrc();
    sysclock_configure_mpll(ClkPllSrcHRC, &pllConf);

    // HRC could have been configured by ICG to 20 MHz
    // TODO: handle this gracefully
    if (1UL != (HRC_FREQ_MON() & 1UL))
    {
        panic("HRC is not 16 MHz");
    }

#if defined(BOARD_XTAL_FREQUENCY)
#warning "no valid XTAL frequency defined, falling back to HRC. please submit a PR or issue to add support for your XTAL frequency"
#endif
#endif

    // setup clock divisors for sysclk = 200 MHz:
    // note: PCLK1 is used for step+temp timers, and need to be kept at 50 MHz (until there is a better solution)
    stc_clk_sysclk_cfg_t sysClkConf = {
        .enHclkDiv = ClkSysclkDiv1,  // HCLK  = 200 MHz (CPU)
        .enExclkDiv = ClkSysclkDiv2, // EXCLK = 100 MHz (SDIO)
        .enPclk0Div = ClkSysclkDiv1, // PCLK0 = 200 MHz (Timer6 (not used))
        .enPclk1Div = ClkSysclkDiv4, // PCLK1 = 50 MHz (USART, SPI, I2S, Timer0 (step+temp), TimerA (Servo))
        .enPclk2Div = ClkSysclkDiv4, // PCLK2 = 50 MHz (ADC)
        .enPclk3Div = ClkSysclkDiv4, // PCLK3 = 50 MHz (I2C, WDT)
        .enPclk4Div = ClkSysclkDiv2, // PCLK4 = 100 MHz (ADC ctl)
    };
    sysclock_set_clock_dividers(&sysClkConf);

    // set power mode
#define POWER_MODE_SYSTEM_CLOCK 200000000 // 200 MHz
    power_mode_update_pre(POWER_MODE_SYSTEM_CLOCK);

    // switch to MPLL as sysclk source
    CLK_SetSysClkSource(CLKSysSrcMPLL);

    // set power mode
    power_mode_update_post(POWER_MODE_SYSTEM_CLOCK);
}

/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * HC32f460 system clock configuration
 */

#ifdef ARDUINO_ARCH_HC32

// Get BOARD_XTAL_FREQUENCY from configuration / pins
#include "../../inc/MarlinConfig.h"

#include <core_hooks.h>
#include <drivers/sysclock/sysclock_util.h>

void core_hook_sysclock_init() {
  // Set wait cycles, as we are about to switch to 200 MHz HCLK
  sysclock_configure_flash_wait_cycles();
  sysclock_configure_sram_wait_cycles();

  // Configure MPLLp to 200 MHz output, with different settings depending on XTAL availability
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

  #warning "HC32F460 with 16 MHz XTAL has not been tested."

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
    // TODO: handle gracefully if HRC is not 16 MHz
    if (1UL != (HRC_FREQ_MON() & 1UL)) {
      panic("HRC is not 16 MHz");
    }

    #ifdef BOARD_XTAL_FREQUENCY
      #warning "No valid XTAL frequency defined, falling back to HRC."
    #endif
  #endif

  // Setup clock divisors for sysclk = 200 MHz:
  // Note: PCLK1 is used for step+temp timers, and need to be kept at 50 MHz (until there is a better solution)
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

  // Set power mode
  #define POWER_MODE_SYSTEM_CLOCK 200000000 // 200 MHz
  power_mode_update_pre(POWER_MODE_SYSTEM_CLOCK);

  // Switch to MPLL as sysclk source
  CLK_SetSysClkSource(CLKSysSrcMPLL);

  // Set power mode
  power_mode_update_post(POWER_MODE_SYSTEM_CLOCK);
  #undef POWER_MODE_SYSTEM_CLOCK
}

#endif // ARDUINO_ARCH_HC32

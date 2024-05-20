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

#include "../../inc/MarlinConfig.h"
#include "sysclock.h"

#include <core_hooks.h>
#include <drivers/sysclock/sysclock_util.h>

/**
 * @brief Constexpr for loop helper.
 * @tparam Start The start value.
 * @tparam End The end value.
 * @tparam Inc The increment value.
 * @tparam F The function to call.
 * 
 * @note Taken from https://artificial-mind.net/blog/2020/10/31/constexpr-for 
 */
template <auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F&& f)
{
    if constexpr (Start < End)
    {
        f(std::integral_constant<decltype(Start), Start>());
        constexpr_for<Start + Inc, End, Inc>(f);
    }
}

/***
 * @brief Automatically calculate M, N, P values for the MPLL to reach a target frequency.
 * @param input_frequency The input frequency.
 * @param target_frequency The target frequency.
 * @return The MPLL configuration structure. Q and R are not set.
 * 
 * @note
 * Simplified MPLL block diagram, with intermediary clocks (1) = VCO_in, (2) = VCO_out:
 * 
 *  INPUT -> [/ M] -(1)-> [* N] -(2)-|-> [/ P] -> MPLL-P
 */
constexpr stc_clk_mpll_cfg_t get_mpll_config(const uint32_t input_frequency, const uint32_t target_frequency)
{
  uint32_t best_M = 0, 
      best_N = 0, 
      best_P = 0;

  // FIXME don't consider M in auto-config, as otherwise we get 
  //       "expression not folded to a constant due to excessive constexpr function call complexity"
  const int M = 1;

  // input clock divider: M in [1, 24]
  //constexpr_for<1, 24, 1>([&input_frequency, &target_frequency, &best_M, &best_N, &best_P] (int M) {
    // VCO multiplier: N in [20, 480]
    constexpr_for<20, 480, 1>([&input_frequency, &target_frequency, &best_M, &best_N, &best_P, &M] (int N) {
      // output "P" divider: P in [2, 16]
      constexpr_for<2, 16, 1>([&input_frequency, &target_frequency, &best_M, &best_N, &best_P, &M, &N] (int P) {
        // calculate actual frequencies given M, N, P
        const double vco_in_frequency = static_cast<double>(input_frequency) / static_cast<double>(M);
        const double vco_out_frequency = vco_in_frequency * static_cast<double>(N);
        const double pll_out_frequency = vco_out_frequency / static_cast<double>(P);

        // 1 <= VCO_in <= 25 MHz
        if (vco_in_frequency < 1e6 || vco_in_frequency > 25e6) {
          return;
        }

        // 240 <= VCO_out <= 480 MHz
        if (vco_out_frequency < 240e6 || vco_out_frequency > 480e6) {
          return;
        }

        // check if the calculate frequency matches the target
        // only if no previous match has been found
        if (pll_out_frequency == target_frequency && best_M == 0) {
          best_M = M;
          best_N = N;
          best_P = P;
        }
      });
    });
  //});

  return {
    .PllpDiv = best_P,
    .PllqDiv = best_P, // don't care for Q and R
    .PllrDiv = best_P, // "
    .plln = best_N,
    .pllmDiv = best_M,
  };
}

/**
 * @brief Get the division factor required to get the target frequency from the input frequency.
 * @tparam input_freq The input frequency.
 * @tparam target_freq The target frequency.
 * @return The division factor. 
 */
template <uint32_t input_freq, uint32_t target_freq>
constexpr en_clk_sysclk_div_factor_t get_division_factor()
{
  // Calculate the divider to get the target frequency
  constexpr float fdivider = static_cast<float>(input_freq) / static_cast<float>(target_freq);
  constexpr int divider = static_cast<int>(fdivider);

  // divider must be an integer
  static_assert(fdivider == divider, "Target frequency not achievable, divider must be an integer");

  // divider must be between 1 and 64 (enum range), and must be a power of 2
  static_assert(divider >= 1 && divider <= 64, "Invalid divider, out of range");
  static_assert((divider & (divider - 1)) == 0, "Invalid divider, not a power of 2");

  // return the divider
  switch (divider)
  {
    case 1:  return ClkSysclkDiv1;
    case 2:  return ClkSysclkDiv2;
    case 4:  return ClkSysclkDiv4;
    case 8:  return ClkSysclkDiv8;
    case 16: return ClkSysclkDiv16;
    case 32: return ClkSysclkDiv32;
    case 64: return ClkSysclkDiv64;
  }
}

/**
 * @brief Validate the runtime clocks match the expected values. 
 */
void validate_system_clocks()
{
  #define CLOCK_ASSERT(expected, actual)                  \
    if (expected != actual) {                             \
      SERIAL_ECHOPGM(                                     \
        "Clock Mismatch for " #expected ": "              \
        "expected ", expected,                            \
        ", got ", actual                                  \
        );                                                \
      CORE_ASSERT_FAIL("Clock Mismatch: " #expected);     \
    }

  update_system_clock_frequencies();

  CLOCK_ASSERT(F_SYSTEM_CLOCK, SYSTEM_CLOCK_FREQUENCIES.system);
  CLOCK_ASSERT(F_HCLK, SYSTEM_CLOCK_FREQUENCIES.hclk);
  CLOCK_ASSERT(F_EXCLK, SYSTEM_CLOCK_FREQUENCIES.exclk);
  CLOCK_ASSERT(F_PCLK0, SYSTEM_CLOCK_FREQUENCIES.pclk0);
  CLOCK_ASSERT(F_PCLK1, SYSTEM_CLOCK_FREQUENCIES.pclk1);
  CLOCK_ASSERT(F_PCLK2, SYSTEM_CLOCK_FREQUENCIES.pclk2);
  CLOCK_ASSERT(F_PCLK3, SYSTEM_CLOCK_FREQUENCIES.pclk3);
  CLOCK_ASSERT(F_PCLK4, SYSTEM_CLOCK_FREQUENCIES.pclk4);
}

/**
 * @brief Configure HC32 system clocks.
 *
 * This function is called by the Arduino core early in the startup process, before setup() is called.
 * It is used to configure the system clocks to the desired state.
 *
 * See https://github.com/MarlinFirmware/Marlin/pull/27099 for more information.
 */
void core_hook_sysclock_init() {
  // Set wait cycles, as we are about to switch to 200 MHz HCLK
  sysclock_configure_flash_wait_cycles();
  sysclock_configure_sram_wait_cycles();

  // Select MPLL input frequency based on clock availability
  #if BOARD_XTAL_FREQUENCY == 8000000 || BOARD_XTAL_FREQUENCY == 16000000 // 8 MHz or 16 MHz XTAL
    constexpr uint32_t mpll_input_clock = BOARD_XTAL_FREQUENCY;

    sysclock_configure_xtal();
    
    #if BOARD_XTAL_FREQUENCY == 16000000
      #warning "HC32F460 with 16 MHz XTAL has not been tested."
    #endif
  #else // HRC (16 MHz)
    constexpr uint32_t mpll_input_clock = 16000000;

    sysclock_configure_hrc();

    // HRC could have been configured by ICG to 20 MHz
    // TODO: handle gracefully if HRC is not 16 MHz
    if (1UL != (HRC_FREQ_MON() & 1UL)) {
      panic("HRC is not 16 MHz");
    }

    #if defined(BOARD_XTAL_FREQUENCY)
      #warning "No valid XTAL frequency defined, falling back to HRC."
    #endif
  #endif

  // Automagically calculate MPLL configuration
  constexpr stc_clk_mpll_cfg_t pllConf = get_mpll_config(mpll_input_clock, F_SYSTEM_CLOCK);
  static_assert(pllConf.pllmDiv != 0 && pllConf.plln != 0 && pllConf.PllpDiv != 0, "MPLL auto-configuration failed");
  sysclock_configure_mpll(ClkPllSrcXTAL, &pllConf);

  // Setup clock divisors
  constexpr stc_clk_sysclk_cfg_t sysClkConf = {
      .enHclkDiv  = get_division_factor<F_SYSTEM_CLOCK, F_HCLK>(),
      .enExclkDiv = get_division_factor<F_SYSTEM_CLOCK, F_EXCLK>(),
      .enPclk0Div = get_division_factor<F_SYSTEM_CLOCK, F_PCLK0>(),
      .enPclk1Div = get_division_factor<F_SYSTEM_CLOCK, F_PCLK1>(),
      .enPclk2Div = get_division_factor<F_SYSTEM_CLOCK, F_PCLK2>(),
      .enPclk3Div = get_division_factor<F_SYSTEM_CLOCK, F_PCLK3>(),
      .enPclk4Div = get_division_factor<F_SYSTEM_CLOCK, F_PCLK4>(),
  };
  sysclock_set_clock_dividers(&sysClkConf);

  // Set power mode, before switch
  power_mode_update_pre(F_SYSTEM_CLOCK);

  // Switch to MPLL-P as system clock source
  CLK_SetSysClkSource(CLKSysSrcMPLL);

  // Set power mode, after switch
  power_mode_update_post(F_SYSTEM_CLOCK);

  // Verify clocks match expected values (at runtime)
  #if ENABLED(MARLIN_DEV_MODE) || ENABLED(ALWAYS_VALIDATE_CLOCKS)
    validate_system_clocks();
  #endif

  // Verify clock configuration (at compile time)
  #if ARDUINO_CORE_VERSION_INT >= GET_VERSION_INT(1, 2, 0)
    assert_mpll_config_valid<
      mpll_input_clock,
      pllConf.pllmDiv,  
      pllConf.plln,
      pllConf.PllpDiv,
      pllConf.PllqDiv,
      pllConf.PllrDiv
    >();
    
    assert_system_clocks_valid<
      F_SYSTEM_CLOCK,
      sysClkConf.enHclkDiv,
      sysClkConf.enPclk0Div,
      sysClkConf.enPclk1Div,
      sysClkConf.enPclk2Div,
      sysClkConf.enPclk3Div,
      sysClkConf.enPclk4Div,
      sysClkConf.enExclkDiv
    >();

    static_assert(get_mpll_output_clock(
      mpll_input_clock,
      pllConf.pllmDiv,
      pllConf.plln,
      pllConf.PllpDiv
    ) == F_SYSTEM_CLOCK, "actual MPLL output clock does not match F_SYSTEM_CLOCK");
  #endif
}

#endif // ARDUINO_ARCH_HC32

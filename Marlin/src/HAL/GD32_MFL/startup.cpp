/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../platforms.h"

#ifdef ARDUINO_ARCH_MFL

#include <FMC.hpp>
#include <CORTEX.hpp>
#include <RCU.hpp>
#include <PMU.hpp>
#include <AFIO.hpp>
#include <CEE.hpp>

#include "TickTime.h"

/**
 * System clock and flash wait states.
 *
 * The core's STARTUP::startup_init() raises SYSCLK with no timeout on any of
 * the oscillator waits and without ever programming FMC_WS. On this board it
 * never returns, and the bootloader's wait-state count is left behind, so the
 * first instruction fetch after the switch would fault anyway.
 *
 * Do the ramp here instead, with bounded waits, the wait states programmed
 * first, and a fallback to the 8 MHz RC oscillator if the crystal or the PLL
 * does not come up.
 *
 * 120 MHz (8 MHz HXTAL / 2 x 30) is the core's F303 target; the stock Anycubic
 * firmware runs at 72 MHz (8 MHz HXTAL / 2 x 18).
 */
#define GD32_CLOCK_MHZ 120

// FWDGT_CTRL: writing the reload value keeps the bootloader's watchdog fed
constexpr uint32_t fwdgt_ctrl_addr = 0x40003000UL;
constexpr uint32_t fwdgt_reload = 0xAAAAU;

static auto waitStable(rcu::OSCI_Select osc, uint32_t tries) -> bool {
  auto &rcu = rcu::RCU::get_instance();
  while (tries--) {
    *reinterpret_cast<volatile uint32_t*>(fwdgt_ctrl_addr) = fwdgt_reload;
    if (rcu.is_osci_stable(osc)) return true;
  }
  return false;
}

static inline void setWaitStates(uint32_t ws) {
  auto &fmc = fmc::FMC::get_instance();
  volatile uint32_t *reg = fmc.reg_address(fmc::FMC_Regs::WS);
  *reg = (*reg & ~0x7U) | (ws & 0x7U);
}

void init() {
  cortex::CORTEX::get_instance().set_nvic_priority_group(cortex::Priority_Group::PRIO_GROUP_PRE4SUB0);

  auto &rcu = rcu::RCU::get_instance();
  auto &pmu_inst = pmu::PMU::get_instance();

  #if GD32_CLOCK_MHZ <= 8
    setWaitStates(0);
  #else
    // 72 MHz needs 2 wait states, 120 MHz needs 3
    setWaitStates(GD32_CLOCK_MHZ > 90 ? 3 : 2);

    /**
     * The 1.2 V domain needs the higher LDO setting before the PLL starts to be
     * able to run above ~100 MHz.
     */
    pmu_inst.set_ldo_output(pmu::Output_Voltage::LDO_VOLTAGE_HIGH);

    rcu.set_osci_enable(rcu::OSCI_Select::HXTAL, true);
    if (waitStable(rcu::OSCI_Select::HXTAL, 500000U)) {
      rcu.set_ahb_prescaler(rcu::AHB_Prescaler::CKSYS_DIV1);
      rcu.set_apb2_prescaler(rcu::APB_Prescaler::CKAHB_DIV1);
      rcu.set_apb1_prescaler(rcu::APB_Prescaler::CKAHB_DIV2);

      // PLL clock = HXTAL / 2 * MUL
      rcu.set_pll_presel(rcu::PLL_Presel::PLLPRESRC_HXTAL);
      rcu.set_predv0_config(true);
      #if GD32_CLOCK_MHZ > 90
        rcu.set_pll_config(rcu::PLL_Source::PLLSRC_HXTAL_IRC48M, rcu::PLLMF_Select::PLL_MUL30);
      #else
        rcu.set_pll_config(rcu::PLL_Source::PLLSRC_HXTAL_IRC48M, rcu::PLLMF_Select::PLL_MUL18);
      #endif

      rcu.set_osci_enable(rcu::OSCI_Select::PLL_CK, true);
      if (waitStable(rcu::OSCI_Select::PLL_CK, 500000U)) {
        // High-drive output buffers are required for the faster edges
        pmu_inst.set_high_driver_enable(true);
        pmu_inst.high_driver_switch(true);

        rcu.set_system_source(rcu::System_Clock_Source::SOURCE_PLL);
        for (uint32_t n = 200000U; n; --n)
          if (rcu.get_system_source() == rcu::System_Clock_Source::SOURCE_PLL) break;
      }
    }
  #endif

  // Keep the ADC clock at or below 14 MHz (12 MHz here)
  rcu.set_adc_prescaler(GD32_CLOCK_MHZ > 90 ? rcu::ADC_Prescaler::CKAHB_DIV10 : rcu::ADC_Prescaler::CKAHB_DIV6);

  #ifndef DISABLE_CEE_ENHANCE
    cee::CEE::get_instance().set_enhanced_mode_enable(true);
  #endif

  // GPIO compensation cell, needed for reliable I/O at high clock rates
  {
    auto &afio = gpio::AFIO::get_instance();
    afio.set_compensation(true);
    for (uint32_t n = 100000U; n; --n)
      if (afio.get_compensation()) break;
  }

  // Records the clock actually reached, so baud rates and delays stay correct
  // even if we fell back to the RC oscillator.
  rcu.update_system_clock();

  tickInit(0x00U);
}

#endif // ARDUINO_ARCH_MFL

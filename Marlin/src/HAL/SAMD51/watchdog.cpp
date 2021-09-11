/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
#ifdef __SAMD51__

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

#include "watchdog.h"

#define WDT_TIMEOUT_REG TERN(WATCHDOG_DURATION_8S, WDT_CONFIG_PER_CYC8192, WDT_CONFIG_PER_CYC4096) // 4 or 8 second timeout

void watchdog_init() {
  // The low-power oscillator used by the WDT runs at 32,768 Hz with
  // a 1:32 prescale, thus 1024 Hz, though probably not super precise.

  // Setup WDT clocks
  MCLK->APBAMASK.bit.OSC32KCTRL_ = true;
  MCLK->APBAMASK.bit.WDT_ = true;
  OSC32KCTRL->OSCULP32K.bit.EN1K = true;      // Enable out 1K (this is what WDT uses)

  WDT->CTRLA.bit.ENABLE = false;              // Disable watchdog for config
  SYNC(WDT->SYNCBUSY.bit.ENABLE);

  WDT->INTENCLR.reg = WDT_INTENCLR_EW;        // Disable early warning interrupt
  WDT->CONFIG.reg = WDT_TIMEOUT_REG;          // Set a 4s or 8s period for chip reset

  HAL_watchdog_refresh();

  WDT->CTRLA.reg = WDT_CTRLA_ENABLE;          // Start watchdog now in normal mode
  SYNC(WDT->SYNCBUSY.bit.ENABLE);
}

#endif // USE_WATCHDOG

#endif // __SAMD51__

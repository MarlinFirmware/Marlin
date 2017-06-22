/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef TARGET_LPC1768

#include "../../../MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

  #include "watchdog.h"

  void watchdog_init(void) {
    LPC_WDT->WDCLKSEL = 0x1;                    // Set CLK src to PCLK
    LPC_WDT->WDTC = 4 * (SystemCoreClock / 16); // 4 second timeout (16 divisor = WD has a fixed /4 prescaler, PCLK default is /4)
    LPC_WDT->WDMOD = 0x3;                       // Enabled and Reset
    LPC_WDT->WDFEED = 0xAA;                     // Initial feed to start
    LPC_WDT->WDFEED = 0x55;
  }

  void HAL_clear_reset_source(void) {
    LPC_WDT->WDMOD &= ~(1 << 2);
  }

  uint8_t HAL_get_reset_source (void) {
    if((LPC_WDT->WDMOD >> 2) & 1) return RST_WATCHDOG;
    return RST_POWER_ON;
  }

#endif // USE_WATCHDOG

#endif

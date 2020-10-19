/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * HAL Watchdog for Teensy 4.0 (IMXRT1062DVL6A) / 4.1 (IMXRT1062DVJ6A)
 */

#ifdef __IMXRT1062__

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

#include "watchdog.h"

// 4 seconds timeout
#define WDTO 4 //seconds

uint8_t timeoutval = (WDTO - 0.5f) / 0.5f;

void watchdog_init() {

  CCM_CCGR3 |= CCM_CCGR3_WDOG1(3);  // enable WDOG1 clocks
  WDOG1_WMCR = 0;                   // disable power down PDE
  WDOG1_WCR |= WDOG_WCR_SRS | WDOG_WCR_WT(timeoutval);
  WDOG1_WCR |= WDOG_WCR_WDE | WDOG_WCR_WDT | WDOG_WCR_SRE;

}

void HAL_watchdog_refresh() {
  // Watchdog refresh sequence
  WDOG1_WSR = 0x5555;
  WDOG1_WSR = 0xAAAA;
}

#endif // USE_WATCHDOG
#endif // __IMXRT1062__

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

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

#include "lpc17xx_wdt.h"
#include "watchdog.h"

void watchdog_init(void) {
  WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);
  WDT_Start(WDT_TIMEOUT);
}

void HAL_clear_reset_source(void) {
  WDT_ClrTimeOutFlag();
}

uint8_t HAL_get_reset_source(void) {
  if (TEST(WDT_ReadTimeOutFlag(), 0)) return RST_WATCHDOG;
  return RST_POWER_ON;
}

void watchdog_reset() {
  WDT_Feed();
  #if PIN_EXISTS(LED)
    TOGGLE(LED_PIN);  // heart beat indicator
  #endif
}

#else
  void HAL_clear_reset_source(void) {}
  uint8_t HAL_get_reset_source(void) { return RST_POWER_ON; }
#endif // USE_WATCHDOG

#endif // TARGET_LPC1768

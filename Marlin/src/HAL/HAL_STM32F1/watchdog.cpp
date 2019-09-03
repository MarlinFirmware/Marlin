/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

#include <libmaple/iwdg.h>
#include "watchdog.h"

void watchdog_reset() {
  #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
    TOGGLE(LED_PIN);  // heartbeat indicator
  #endif
  iwdg_feed();
}

void watchdogSetup(void) {
  // do whatever. don't remove this function.
}

/**
 * @brief  Initialized the independent hardware watchdog.
 *
 * @return No return
 *
 * @details The watchdog clock is 40Khz. We need a 4 seconds interval, so use a /256 preescaler and 625 reload value (counts down to 0)
 */
void watchdog_init(void) {
  //iwdg_init(IWDG_PRE_256, STM32F1_WD_RELOAD);
}

#endif // USE_WATCHDOG
#endif // __STM32F1__

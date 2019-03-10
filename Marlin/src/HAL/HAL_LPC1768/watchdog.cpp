/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  #if ENABLED(WATCHDOG_RESET_MANUAL)
    // We enable the watchdog timer, but only for the interrupt.

    // Configure WDT to only trigger an interrupt
    // Disable WDT interrupt (just in case, to avoid triggering it!)
    NVIC_DisableIRQ(WDT_IRQn);

    // We NEED memory barriers to ensure Interrupts are actually disabled!
    // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
    __DSB();
    __ISB();

    // Configure WDT to only trigger an interrupt
    // Initialize WDT with the given parameters
    WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_INT_ONLY);

    // Configure and enable WDT interrupt.
    NVIC_ClearPendingIRQ(WDT_IRQn);
    NVIC_SetPriority(WDT_IRQn, 0); // Use highest priority, so we detect all kinds of lockups
    NVIC_EnableIRQ(WDT_IRQn);
  #else
    WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);
  #endif
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
  #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
    TOGGLE(LED_PIN);  // heartbeat indicator
  #endif
}

#else

  void HAL_clear_reset_source(void) {}
  uint8_t HAL_get_reset_source(void) { return RST_POWER_ON; }

#endif // USE_WATCHDOG

#endif // TARGET_LPC1768

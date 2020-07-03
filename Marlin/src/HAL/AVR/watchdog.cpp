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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifdef __AVR__

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

#include "watchdog.h"

#include "../../MarlinCore.h"

// Initialize watchdog with 8s timeout, if possible. Otherwise, make it 4s.
void watchdog_init() {
  #if ENABLED(WATCHDOG_DURATION_8S) && defined(WDTO_8S)
    #define WDTO_NS WDTO_8S
  #else
    #define WDTO_NS WDTO_4S
  #endif
  #if ENABLED(WATCHDOG_RESET_MANUAL)
    // Enable the watchdog timer, but only for the interrupt.
    // Take care, as this requires the correct order of operation, with interrupts disabled.
    // See the datasheet of any AVR chip for details.
    wdt_reset();
    cli();
    _WD_CONTROL_REG = _BV(_WD_CHANGE_BIT) | _BV(WDE);
    _WD_CONTROL_REG = _BV(WDIE) | (WDTO_NS & 0x07) | ((WDTO_NS & 0x08) << 2); // WDTO_NS directly does not work. bit 0-2 are consecutive in the register but the highest value bit is at bit 5
                                                                              // So worked for up to WDTO_2S
    sei();
    wdt_reset();
  #else
    wdt_enable(WDTO_NS); // The function handles the upper bit correct.
  #endif
  //delay(10000); // test it!
}

//===========================================================================
//=================================== ISR ===================================
//===========================================================================

// Watchdog timer interrupt, called if main program blocks >4sec and manual reset is enabled.
#if ENABLED(WATCHDOG_RESET_MANUAL)
  ISR(WDT_vect) {
    sei();  // With the interrupt driven serial we need to allow interrupts.
    SERIAL_ERROR_MSG(STR_WATCHDOG_FIRED);
    minkill();  // interrupt-safe final kill and infinite loop
  }
#endif

#endif // USE_WATCHDOG
#endif // __AVR__

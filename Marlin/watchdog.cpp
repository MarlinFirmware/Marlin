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

#include "Marlin.h"

#if ENABLED(USE_WATCHDOG)

#include "watchdog.h"

// Initialize watchdog with a 4 sec interrupt time
void watchdog_init() {
  #if ENABLED(WATCHDOG_RESET_MANUAL)
    // We enable the watchdog timer, but only for the interrupt.
    // Take care, as this requires the correct order of operation, with interrupts disabled. See the datasheet of any AVR chip for details.
    wdt_reset();
    _WD_CONTROL_REG = _BV(_WD_CHANGE_BIT) | _BV(WDE);
    _WD_CONTROL_REG = _BV(WDIE) | WDTO_4S;
  #else
    wdt_enable(WDTO_4S);
  #endif
}

//===========================================================================
//=================================== ISR ===================================
//===========================================================================

// Watchdog timer interrupt, called if main program blocks >4sec and manual reset is enabled.
#if ENABLED(WATCHDOG_RESET_MANUAL)
  ISR(WDT_vect) {
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM("Something is wrong, please turn off the printer.");
    kill(PSTR("ERR:Please Reset")); //kill blocks //16 characters so it fits on a 16x2 display
    while (1); //wait for user or serial reset
  }
#endif //WATCHDOG_RESET_MANUAL

#endif //USE_WATCHDOG

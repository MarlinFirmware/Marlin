/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Low level pin manipulation routines - used by all the drivers.
 *
 * These are based on the LPC1768 pinMode, digitalRead & digitalWrite routines.
 *
 * Couldn't just call exact copies because the overhead killed the LCD update speed
 * With an intermediate level the softspi was running in the 10-20kHz range which
 * resulted in using about about 25% of the CPU's time.
 */

#ifdef __PLAT_NATIVE_SIM__

#include "../fastio.h"
#include "LCD_pin_routines.h"

#ifdef __cplusplus
  extern "C" {
#endif

void u8g_SetPinOutput(uint8_t internal_pin_number) { SET_DIR_OUTPUT(internal_pin_number); }
void u8g_SetPinInput(uint8_t internal_pin_number) { SET_DIR_INPUT(internal_pin_number); }
void u8g_SetPinLevel(uint8_t pin, uint8_t pin_status) { WRITE_PIN(pin, pin_status); }
uint8_t u8g_GetPinLevel(uint8_t pin) { return READ_PIN(pin); }
void usleep(uint64_t microsec) { assert(false); /* why we here? */ }

#ifdef __cplusplus
  }
#endif

#endif // __PLAT_NATIVE_SIM__

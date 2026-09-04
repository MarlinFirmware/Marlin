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
 * Low level pin manipulation routines - used by all the drivers.
 */

#ifdef TARGET_LPC5528

#include "../../../inc/MarlinConfigPre.h"
#include "../../shared/Marduino.h"

#ifdef __cplusplus
  extern "C" {
#endif

void pinMode_LCD(uint8_t pin, uint8_t mode) {
  pinMode(pin, mode);
}

void u8g_SetPinOutput(uint8_t internal_pin_number) {
  pinMode_LCD(internal_pin_number, OUTPUT);
}

void u8g_SetPinInput(uint8_t internal_pin_number) {
  pinMode_LCD(internal_pin_number, INPUT);
}

void u8g_SetPinLevel(uint8_t pin, uint8_t pin_status) {
  digitalWrite(pin, pin_status);
}

uint8_t u8g_GetPinLevel(uint8_t pin) {
  return digitalRead(pin);
}

#ifdef __cplusplus
  }
#endif

#endif // TARGET_LPC5528
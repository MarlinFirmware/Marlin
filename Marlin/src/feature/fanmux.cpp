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

/**
 * feature/pause.cpp - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

#include "../inc/MarlinConfig.h"

#if HAS_FANMUX

#include "fanmux.h"

void fanmux_switch(const uint8_t e) {
  WRITE(FANMUX0_PIN, TEST(e, 0) ? HIGH : LOW);
  #if PIN_EXISTS(FANMUX1)
    WRITE(FANMUX1_PIN, TEST(e, 1) ? HIGH : LOW);
    #if PIN_EXISTS(FANMUX2)
      WRITE(FANMUX2_PIN, TEST(e, 2) ? HIGH : LOW);
    #endif
  #endif
}

void fanmux_init(void) {
  SET_OUTPUT(FANMUX0_PIN);
  #if PIN_EXISTS(FANMUX1)
    SET_OUTPUT(FANMUX1_PIN);
    #if PIN_EXISTS(FANMUX2)
      SET_OUTPUT(FANMUX2_PIN);
    #endif
  #endif
  fanmux_switch(0);
}

#endif // HAS_FANMUX

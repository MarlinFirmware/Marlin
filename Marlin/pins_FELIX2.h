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

/**
 * FELIXprinters v2.0/3.0 (RAMPS v1.4) pin assignments
 */

#include "pins_RAMPS_14_EFB.h"

#undef HEATER_1_PIN
#define HEATER_1_PIN        7 // EXTRUDER 2

#undef SDPOWER
#define SDPOWER             1

#define PS_ON_PIN          12

#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

  #define BLEN_C 2
  #define BLEN_B 1
  #define BLEN_A 0
  #define SD_DETECT_PIN 6

#endif // NEWPANEL && ULTRA_LCD

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

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Felix 2.0+ supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "Felix 2.0+"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN 7

#include "pins_RAMPS.h"

//
// Misc. Functions
//
#undef SDPOWER
#define SDPOWER             1

#define PS_ON_PIN          12

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

  #define SD_DETECT_PIN 6

#endif // NEWPANEL && ULTRA_LCD

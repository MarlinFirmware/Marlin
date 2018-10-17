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

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "Ender-4 supports only 1 hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_NAME "Ender-4"

#include "pins_RAMPS.h"

// The board only has one controllable fan connector, the others are just plain 12V connectors
// in the default configuration, this is used to control the brightness of the LED band
// hotend and controller fan are therefore always-on
#define ENDER4_FAN_PIN RAMPS_D9_PIN
#undef FAN_PIN

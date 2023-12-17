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
#pragma once

/**
 * VORON Design v2 pin assignments
 * See https://github.com/mzbotreprap/VORON/blob/master/Firmware/Marlin/pins_RAMPS_VORON.h
 * ATmega2560
 */

#define BOARD_INFO_NAME "VORON Design v2"

//
// Heaters / Fans
//
#define MOSFET_C_PIN                          11
#define FAN0_PIN                               5  // Using the pin for the controller fan since controller fan is always on.
#define CONTROLLER_FAN_PIN                     8

//
// Auto fans
//
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                      6  // Servo pin 6 for E3D Fan
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN                      6  // Servo pin 6 for E3D Fan (same pin for both extruders since it's the same fan)
#endif

#include "pins_RAMPS.h"

//
// LCD / Controller
//
#undef BEEPER_PIN

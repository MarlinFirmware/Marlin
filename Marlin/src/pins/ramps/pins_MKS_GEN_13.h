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
 * Arduino Mega with RAMPS v1.4 adjusted pin assignments
 * Schematic (1.4): https://github.com/makerbase-mks/MKS-GEN/blob/master/hardware/MKS%20GEN%20V1.4_004/MKS%20GEN%20V1.4_004%20SCH.pdf
 * ATmega2560, ATmega1280
 *
 *  MKS GEN v1.3  (Extruder, Fan, Bed)
 *  MKS GEN v1.3  (Extruder, Extruder, Fan, Bed)
 *  MKS GEN v1.4  (Extruder, Fan, Bed)
 *  MKS GEN v1.4  (Extruder, Extruder, Fan, Bed)
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS GEN 1.3/1.4 supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "MKS GEN >= v1.3"

//
// Heaters / Fans
//
#define MOSFET_B_PIN                           7
#define FAN0_PIN                               9

//
// PSU / SERVO
//
// If PSU_CONTROL is specified, always hijack Servo 3
//
#if ENABLED(PSU_CONTROL)
  #define SERVO3_PIN                          -1
  #define PS_ON_PIN                            4
#endif

#include "pins_RAMPS.h"

#undef EXP2_08_PIN
#define EXP2_08_PIN                           -1  // RESET

//
// LCD / Controller
//

#if ANY(VIKI2, miniVIKI)
  // LCD pins defined in Marlin/src/pins/lcd/VIKI2.h
#endif

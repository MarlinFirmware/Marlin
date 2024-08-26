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
 * Velleman K8400 (Vertex)
 * 3DRAG clone
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMPS/Velleman%20K8400/k8400-schema-electronique.jpg
 * Origin: https://filimprimante3d.fr/documents/k8400-schema-electronique.jpg
 * ATmega2560, ATmega1280
 *
 * K8400 has some minor differences over a normal 3Drag:
 *  - No X/Y max endstops
 *  - Second extruder step pin has moved
 *  - No power supply control
 *  - Second heater has moved pin
 */

#define BOARD_INFO_NAME         "K8400"
#define DEFAULT_MACHINE_NAME    "Vertex"

//
// Steppers
//
#if HAS_CUTTER
  #define Z_STEP_PIN                          32
#endif

#define E1_STEP_PIN                           32

//
// Limit Switches
//
#define X_STOP_PIN                             3
#define Y_STOP_PIN                            14

//
// Fans
//
#define FAN0_PIN                               8

#if ANY(BLTOUCH, TOUCH_MI_PROBE)
  #define INVERTED_PROBE_STATE
#endif

#include "pins_3DRAG.h" // ... RAMPS

//
// Heaters
//
#undef HEATER_1_PIN
#define HEATER_1_PIN                          11

//
// Misc. Functions
//
#undef PS_ON_PIN
#undef KILL_PIN
#undef SD_DETECT_PIN

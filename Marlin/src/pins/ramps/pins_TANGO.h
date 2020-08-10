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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * BIQU Tango pin assignments
 */

#define BOARD_INFO_NAME "Tango"

#define FAN_PIN                               8
#define FAN1_PIN                             -1

#define ORIG_E0_AUTO_FAN_PIN                  7

#ifndef TEMP_0_PIN
  #if TEMP_SENSOR_0 == -1
    #define TEMP_0_PIN                       10   // Analog Input (connector *K1* on Tango thermocouple ADD ON is used)
  #else
    #define TEMP_0_PIN                       15   // Analog Input (default connector for thermistor *T0* on rumba board is used)
  #endif
#endif

#ifndef TEMP_1_PIN
  #if TEMP_SENSOR_1 == -1
    #define TEMP_1_PIN                        9   // Analog Input (connector *K2* on Tango thermocouple ADD ON is used)
  #else
    #define TEMP_1_PIN                       14   // Analog Input (default connector for thermistor *T1* on rumba board is used)
  #endif
#endif

#include "pins_RUMBA.h"

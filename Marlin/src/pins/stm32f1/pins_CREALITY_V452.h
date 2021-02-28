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
 * Creality v4.5.2 (STM32F103RET6) board pin assignments
 */

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "Creality v4.5.2 only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_NAME "Creality v4.5.2"

#define HEATER_0_PIN                        PA1   // HEATER1
#define HEATER_BED_PIN                      PA2   // HOT BED
#define FAN_PIN                             PA0   // FAN
#define PROBE_ACTIVATION_SWITCH_PIN         PC6   // Optoswitch to Enable Z Probe

#include "pins_CREALITY_V45x.h"

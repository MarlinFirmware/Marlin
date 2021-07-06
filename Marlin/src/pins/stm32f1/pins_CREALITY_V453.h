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
 * Creality v4.5.3 (STM32F103RET6) board pin assignments
 */

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "Creality v4.5.3 only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_NAME "Creality v4.5.3"

#define HEATER_0_PIN                        PB14  // HEATER1
#define HEATER_BED_PIN                      PB13  // HOT BED
#define FAN_PIN                             PB15  // FAN
#define PROBE_ACTIVATION_SWITCH_PIN         PB2   // Optoswitch to Enable Z Probe

#include "pins_CREALITY_V45x.h"

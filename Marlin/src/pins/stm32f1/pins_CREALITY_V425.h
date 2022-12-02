/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * CREALITY 4.2.5 (STM32F103RE / STM32F103RC) board pin assignments
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Creality v4.2.5 only supports 1 hotend / E stepper."
#endif

#define BOARD_INFO_NAME "Creality V4.2.5"
#define DEFAULT_MACHINE_NAME "CR200B"

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  #undef NO_EEPROM_SELECTED
#elif DISABLED(IIC_BL24CXX_EEPROM)
  #define SDCARD_EEPROM_EMULATION                 // SD EEPROM until all EEPROM is BL24CXX
#endif

//
// Servos
//
#define SERVO0_PIN                          PB1   // BLTouch OUT

//
// Limit Switches
//
#define X_STOP_PIN                          PA3
#define Y_STOP_PIN                          PA4
#define Z_STOP_PIN                          PA5

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB0   // BLTouch IN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA6   // "Pulled-high"
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA0   // HEATER1
#define HEATER_BED_PIN                      PA1   // HOT BED
#define FAN_PIN                             PA2   // FAN

#include "pins_CREALITY_V4.h"

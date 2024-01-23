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
 * Creality V24S1_301 (STM32F103RE / STM32F103RC) board pin assignments as found on Ender-3 S1.
 * Also supports the STM32F4 version of the board with identical pin mapping.
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Creality v24S1 only supports 1 hotend / E stepper."
  #define E_ERROR 1
#endif

#if ALL(BLTOUCH, Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
  #error "Disable Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN when using BLTOUCH with Creality V24S1-301."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "Creality V24S1-301"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender-3 S1"
#endif

//
// Servos
//
#if !defined(SERVO0_PIN) && !defined(HAS_PIN_27_BOARD)
  #define SERVO0_PIN                        PC13  // BLTouch OUT
#endif

//
// Limit Switches
//
#define Z_STOP_PIN                          PA15

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PC14  // BLTouch IN
#endif

//
// Probe enable
//
#if ENABLED(PROBE_ENABLE_DISABLE) && !defined(PROBE_ENABLE_PIN)
  #define PROBE_ENABLE_PIN            SERVO0_PIN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15  // "Pulled-high"
#endif

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PA7   // HOT BED
#define FAN1_PIN                            PC0   // extruder fan

//
// SD Card
//
#define ONBOARD_SPI_DEVICE                     1
#define ONBOARD_SD_CS_PIN                   PA4   // SDSS

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER
  //#define HEATER_0_PIN                    -1
  //#define HEATER_BED_PIN                  -1
  #define FAN0_PIN                          -1
  #define SPINDLE_LASER_PWM_PIN             PC0   // Bed FET
  #define SPINDLE_LASER_ENA_PIN             PC0   // FET 1
  #define SPINDLE_DIR_PIN                   PC0   // FET 4
  //#define LASER_SOFT_PWM_PIN              PC0
#endif

#include "pins_CREALITY_V4.h"

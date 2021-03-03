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

// reference https://gitee.com/markyue/pandapi_wiki/raw/master/imges/doc/Schematic_PandaPiV2.8.pdf

#define BOARD_INFO_NAME "PANDAPI_V28"

// Release PB3/PB4 (TMC_SW Pins) from JTAG pins
#define DISABLE_JTAG

//
// Servos
//
#define SERVO0_PIN                          PA11   // SERVOS

//
// Limit Switches
//
#define X_STOP_PIN                          PA3   // "X-STOP"
#define Y_STOP_PIN                          PA4   // "Y-STOP"
#define Z_STOP_PIN                          PA1  // "Z-STOP"

//
// Z Probe must be this pin
//
//#define Z_MIN_PROBE_PIN                     PC14  // "PROBE"
//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PIN                  PC14  // "PROBE"
#endif
//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC2   // "E0-STOP"
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PB9
#define X_STEP_PIN                          PB2
#define X_DIR_PIN                           PA7


#define Y_ENABLE_PIN                        PB9
#define Y_STEP_PIN                          PA6
#define Y_DIR_PIN                           PA5


#define Z_ENABLE_PIN                        PB9
#define Z_STEP_PIN                          PB3
#define Z_DIR_PIN                           PA15


#define E0_ENABLE_PIN                       PB9
#define E0_STEP_PIN                         PB15
#define E0_DIR_PIN                          PB14





//
// Temperature Sensors
//
#define TEMP_0_PIN                          PB0   // Analog Input "TH0"
#define TEMP_BED_PIN                        PB1   // Analog Input "TB0"

//
// Heaters / Fans
//
#define FAN_SOFT_PWM
#define HEATER_0_PIN                        PB12   // "HE"
#define HEATER_BED_PIN                      PB13   // "HB"
#define FAN_PIN                            PA8// PB5   // "FAN0"
#define FAN1_PIN            PA9
#define FAN2_PIN            PA10
#define FAN3_PIN              PA12 // LED


#define BEEPER_PIN       PA0

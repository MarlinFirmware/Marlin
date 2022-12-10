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
 * CartesioV11 pin assignments
 * ATmega2560, ATmega1280
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME "CN Controls V11"

//
// Limit Switches
//
#define X_STOP_PIN                            PinL6
#define Y_STOP_PIN                            PinL4
#define Z_STOP_PIN                            PinL7

//
// Steppers
//
#define X_STEP_PIN                            PinC3
#define X_DIR_PIN                             PinC1
#define X_ENABLE_PIN                          PinC2

#define Y_STEP_PIN                            PinC0
#define Y_DIR_PIN                             PinG2
#define Y_ENABLE_PIN                          PinD7

#define Z_STEP_PIN                            PinG1
#define Z_DIR_PIN                             PinL1
#define Z_ENABLE_PIN                          PinG0

#define E0_STEP_PIN                           PinA7
#define E0_DIR_PIN                            PinA6
#define E0_ENABLE_PIN                         PinE5

#define E1_STEP_PIN                           PinF7
#define E1_DIR_PIN                            PinK0
#define E1_ENABLE_PIN                         PinF6

#define E2_STEP_PIN                           PinJ0
#define E2_DIR_PIN                            PinJ1
#define E2_ENABLE_PIN                         PinH1

#define E3_STEP_PIN                           PinL5
#define E3_DIR_PIN                            PinL0
#define E3_ENABLE_PIN                         PinL2

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE0  // Analog Input
#define TEMP_1_PIN                            PinE5  // Analog Input.  3 for tool2 -> 2 for chambertemp
#define TEMP_2_PIN                            PinE4  // Analog Input.  9 for tool3 -> 2 for chambertemp
#define TEMP_3_PIN                            PinB5  // Analog Input. 11 for tool4 -> 2 for chambertemp
#define TEMP_BED_PIN                          PinE1  // Analog Input

#ifndef TEMP_CHAMBER_PIN
  //#define TEMP_CHAMBER_PIN                  PinE4  // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE3
#define HEATER_1_PIN                          PinF4
#define HEATER_2_PIN                          PinK2
#define HEATER_3_PIN                          PinL3
#define HEATER_BED_PIN                        PinE4

#ifndef FAN_PIN
  //#define FAN_PIN                           PinH4  // common PWM pin for all tools
#endif

//
// Auto fans
//
#define AUTO_FAN_PIN                          PinH4
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinB7

// Tools

//#define TOOL_0_PIN                          PinG5
//#define TOOL_1_PIN                          PinF5
//#define TOOL_2_PIN                          PinH5
//#define TOOL_3_PIN                          PinC7
//#define TOOL_PWM_PIN                        PinH4  // common PWM pin for all tools

// Common I/O

//#define FIL_RUNOUT_PIN                      -1
//#define PWM_1_PIN                           PinB5
//#define PWM_2_PIN                           PinB4
//#define SPARE_IO                            PinB6

//
// LCD / Controller
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN                          PinH3

  #define BTN_EN1                             PinA1
  #define BTN_EN2                             PinA3
  #define BTN_ENC                             PinA5

  #if HAS_MARLINUI_U8GLIB
    #define DOGLCD_A0                         PinA4
    #define DOGLCD_CS                         PinA2
    #define DOGLCD_MOSI                       -1  // Prevent auto-define by Conditionals_post.h
    #define DOGLCD_SCK                        -1
  #endif
#endif

// Hardware buttons for manual movement of XYZ
#define SHIFT_OUT_PIN                         PinD2
#define SHIFT_LD_PIN                          PinD3
#define SHIFT_CLK_PIN                         PinH0

//#define UI1                                 PinC6
//#define UI2                                 PinA0

#define STAT_LED_BLUE_PIN                     -1
#define STAT_LED_RED_PIN                      PinC6

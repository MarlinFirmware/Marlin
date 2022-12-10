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
 * CartesioV12 pin assignments
 * ATmega2560, ATmega1280
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME "CN Controls V12"

//
// Limit Switches
//
#define X_STOP_PIN                            PinD2
#define Y_STOP_PIN                            PinA0
#define Z_STOP_PIN                            PinA1

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA5
#define X_ENABLE_PIN                          PinA4

#define Y_STEP_PIN                            PinA6
#define Y_DIR_PIN                             PinC7
#define Y_ENABLE_PIN                          PinA7

#define Z_STEP_PIN                            PinC6
#define Z_DIR_PIN                             PinC4
#define Z_ENABLE_PIN                          PinC5

#define E0_STEP_PIN                           PinF3
#define E0_DIR_PIN                            PinF1
#define E0_ENABLE_PIN                         PinF4

#define E1_STEP_PIN                           PinF7
#define E1_DIR_PIN                            PinK0
#define E1_ENABLE_PIN                         PinF6

#define E2_STEP_PIN                           PinL3
#define E2_DIR_PIN                            PinK4
#define E2_ENABLE_PIN                         PinL5

#define E3_STEP_PIN                           PinL4
#define E3_DIR_PIN                            PinK7
#define E3_ENABLE_PIN                         PinL2

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE0  // Analog Input
#define TEMP_1_PIN                            PinH6  // Analog Input.  9 for tool2 -> 13 for chambertemp
#define TEMP_2_PIN                            PinB7  // Analog Input. 10 for tool3 -> 13 for chambertemp
#define TEMP_3_PIN                            PiNB5  // Analog Input. 11 for tool4 -> 13 for chambertemp
#define TEMP_BED_PIN                          PinJ1  // Analog Input

#ifndef TEMP_CHAMBER_PIN
  //#define TEMP_CHAMBER_PIN                  PinB7  // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB5
#define HEATER_1_PIN                          PinH6
#define HEATER_2_PIN                          PinH3
#define HEATER_3_PIN                          PinE5
#define HEATER_BED_PIN                        PinA2

#ifndef FAN_PIN
  #define FAN_PIN                             PinE3  // 5 is PWMtool3 -> 7 is common PWM pin for all tools
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
#define SD_DETECT_PIN                         PinJ0

// Tools

//#define TOOL_0_PIN                          PinF2
//#define TOOL_0_PWM_PIN                      PinB4  // red warning led at dual extruder
//#define TOOL_1_PIN                          PinF5
//#define TOOL_1_PWM_PIN                      PinH5  // lights at dual extruder
//#define TOOL_2_PIN                          PinG5
//#define TOOL_2_PWM_PIN                      PinE3
//#define TOOL_3_PIN                          PinJ1
//#define TOOL_3_PWM_PIN                      PinE4

// Common I/O

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinD3
#endif
//#define PWM_1_PIN                           PinB6
//#define PWM_2_PIN                           PinB7
//#define SPARE_IO                            PinH0

//
// LCD / Controller
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN                          PinH1

  #define BTN_EN1                             PinC1
  #define BTN_EN2                             PinC3
  #define BTN_ENC                             PinD7

  #if HAS_MARLINUI_U8GLIB
    #define DOGLCD_A0                         PinG2
    #define DOGLCD_CS                         PinC2
    #define DOGLCD_MOSI                       PinL1
    #define DOGLCD_SCK                        PinL0
  #endif
#endif

// Hardware buttons for manual movement of XYZ
#define SHIFT_OUT_PIN                         PinL7
#define SHIFT_LD_PIN                          PinG0
#define SHIFT_CLK_PIN                         PinG1

//#define UI1                                 PinL6
//#define UI2                                 PinC0

#define STAT_LED_BLUE_PIN                     -1
#define STAT_LED_RED_PIN                      PinB4  // TOOL_0_PWM_PIN

#define LCD_SCREEN_ROTATE                    180  // 0, 90, 180, 270

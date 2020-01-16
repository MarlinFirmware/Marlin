/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * CartesioV12 pin assignments
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME "CN Controls V12"

//
// Limit Switches
//
#define X_STOP_PIN         19
#define Y_STOP_PIN         22
#define Z_STOP_PIN         23

//
// Steppers
//
#define X_STEP_PIN         25
#define X_DIR_PIN          27
#define X_ENABLE_PIN       26

#define Y_STEP_PIN         28
#define Y_DIR_PIN          30
#define Y_ENABLE_PIN       29

#define Z_STEP_PIN         31
#define Z_DIR_PIN          33
#define Z_ENABLE_PIN       32

#define E0_STEP_PIN        57
#define E0_DIR_PIN         55
#define E0_ENABLE_PIN      58

#define E1_STEP_PIN        61
#define E1_DIR_PIN         62
#define E1_ENABLE_PIN      60

#define E2_STEP_PIN        46
#define E2_DIR_PIN         66
#define E2_ENABLE_PIN      44

#define E3_STEP_PIN        45
#define E3_DIR_PIN         69
#define E3_ENABLE_PIN      47

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
#define TEMP_1_PIN          9   // Analog Input.  9 for tool2 -> 13 for chambertemp
#define TEMP_2_PIN         13   // Analog Input. 10 for tool3 -> 13 for chambertemp
#define TEMP_3_PIN         11   // Analog Input. 11 for tool4 -> 13 for chambertemp
#define TEMP_BED_PIN       14   // Analog Input

#ifndef TEMP_CHAMBER_PIN
  //#define TEMP_CHAMBER_PIN 13 // Analog Input
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       11
#define HEATER_1_PIN        9
#define HEATER_2_PIN        6
#define HEATER_3_PIN        3
#define HEATER_BED_PIN     24

#ifndef FAN_PIN
  #define FAN_PIN           5   // 5 is PWMtool3 -> 7 is common PWM pin for all tools
#endif

#define ORIG_E0_AUTO_FAN_PIN 7
#define ORIG_E1_AUTO_FAN_PIN 7
#define ORIG_E2_AUTO_FAN_PIN 7
#define ORIG_E3_AUTO_FAN_PIN 7

//
// Misc. Functions
//
#define SDSS               53
#define SD_DETECT_PIN      15

// Tools

//#define TOOL_0_PIN         56
//#define TOOL_0_PWM_PIN     10   // red warning led at dual extruder
//#define TOOL_1_PIN         59
//#define TOOL_1_PWM_PIN      8   // lights at dual extruder
//#define TOOL_2_PIN          4
//#define TOOL_2_PWM_PIN      5
//#define TOOL_3_PIN         14
//#define TOOL_3_PWM_PIN      2

// Common I/O

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   18
#endif
//#define PWM_1_PIN          12
//#define PWM_2_PIN          13
//#define SPARE_IO           17

//
// LCD / Controller
//
#define BEEPER_PIN         16

// Pins for DOGM SPI LCD Support
#define DOGLCD_A0          39
#define DOGLCD_CS          35
#define DOGLCD_MOSI        48
#define DOGLCD_SCK         49
#define LCD_SCREEN_ROT_180

// The encoder and click button
#define BTN_EN1            36
#define BTN_EN2            34
#define BTN_ENC            38

// Hardware buttons for manual movement of XYZ
#define SHIFT_OUT          42
#define SHIFT_LD           41
#define SHIFT_CLK          40

//#define UI1              43
//#define UI2              37

#define STAT_LED_BLUE_PIN  -1
#define STAT_LED_RED_PIN   10   // TOOL_0_PWM_PIN

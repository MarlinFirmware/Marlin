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
 * Formbot T-Rex 3 pin assignments
 * ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Formbot supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME      "Formbot"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// Servos
//
#define SERVO0_PIN                            11
#define SERVO1_PIN                            -1  // was 6
#define SERVO2_PIN                            -1
#define SERVO3_PIN                            -1

//
// Limit Switches
//
#define X_MIN_PIN                              3
#ifndef X_MAX_PIN
  #define X_MAX_PIN                            2
#endif
#define Y_MIN_PIN                             14
#define Y_MAX_PIN                             15
#define Z_MIN_PIN                             18
#define Z_MAX_PIN                             19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     32
#endif

//
// Steppers
//
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38
#ifndef X_CS_PIN
  #define X_CS_PIN                            53
#endif

#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            49
#endif

#define Z_STEP_PIN                            46
#define Z_DIR_PIN                             48
#define Z_ENABLE_PIN                          62
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            40
#endif

#define E0_STEP_PIN                           26
#define E0_DIR_PIN                            28
#define E0_ENABLE_PIN                         24
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           42
#endif

#define E1_STEP_PIN                           36
#define E1_DIR_PIN                            34
#define E1_ENABLE_PIN                         30
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           44
#endif

#if HAS_X2_STEPPER
  #define X2_STEP_PIN                         42
  #define X2_DIR_PIN                          43
  #define X2_ENABLE_PIN                       44
#else
  #define E2_STEP_PIN                         42
  #define E2_DIR_PIN                          43
  #define E2_ENABLE_PIN                       44
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            13  // Analog Input
#define TEMP_1_PIN                            15  // Analog Input
#define TEMP_BED_PIN                          14  // Analog Input

// SPI for MAX Thermocouple
#if !HAS_MEDIA
  #define TEMP_0_CS_PIN                       66  // Don't use 53 if using Display/SD card
#else
  #define TEMP_0_CS_PIN                       66  // Don't use 49 (SD_DETECT_PIN)
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          10
#define HEATER_1_PIN                           7
#define HEATER_BED_PIN                         8

#define FAN0_PIN                               9
#define FAN1_PIN                              12

#define FIL_RUNOUT_PIN                        22
#define FIL_RUNOUT2_PIN                       21

//
// Misc. Functions
//
#define SD_SS_PIN                             53

#ifndef LED_PIN
  #define LED_PIN                             13
#endif

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                       5
#endif

#define SPINDLE_LASER_PWM_PIN                 -1  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                  4  // Pullup!

// Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
#define FILWIDTH_PIN                           5  // Analog Input

//
// LCD / Controller
//
// Formbot only supports REPRAP_DISCOUNT_SMART_CONTROLLER
//
#if IS_RRD_SC

  // Define de-facto EXP1/EXP2 ports matching this board's physical wiring.
  // The default passthrough in lcd/adapters.h maps LCD1/LCD2 -> EXP1/EXP2, so
  // any display's pins come from Marlin/src/pins/lcd/RRD_SC.h or WIRED.h.
  #define EXP1_01_PIN                         37  // BEEPER
  #define EXP1_02_PIN                         35  // BTN_ENC
  #define EXP1_03_PIN                         17  // LCD_EN
  #define EXP1_04_PIN                         16  // LCD_RS
  #define EXP1_05_PIN                         23  // LCD_D4
  #define EXP1_06_PIN                         25  // LCD_D5
  #define EXP1_07_PIN                         27  // LCD_D6
  #define EXP1_08_PIN                         29  // LCD_D7

  #define EXP2_01_PIN                         -1  // not connected
  #define EXP2_02_PIN                         -1  // not connected
  #define EXP2_03_PIN                         31  // BTN_EN1
  #define EXP2_04_PIN                         -1  // not connected
  #define EXP2_05_PIN                         33  // BTN_EN2
  #define EXP2_06_PIN                         -1  // not connected
  #define EXP2_07_PIN                         49  // SD_DETECT
  #define EXP2_08_PIN                         41  // KILL

  #define SD_DETECT_PIN              EXP2_07_PIN

  #ifndef KILL_PIN
    #define KILL_PIN                 EXP2_08_PIN
  #endif
  #ifndef BEEPER_PIN
    #define BEEPER_PIN               EXP1_01_PIN
  #endif
#endif

#if HAS_MARLINUI_U8GLIB
  #define BOARD_ST7920_DELAY_1               125
  #define BOARD_ST7920_DELAY_2               125
  #define BOARD_ST7920_DELAY_3               125
#endif

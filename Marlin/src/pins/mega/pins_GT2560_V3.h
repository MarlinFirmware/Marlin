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
 * Geeetech GT2560 3.0/3.1 pin assignments
 * ATmega2560
 *
 * Also GT2560 RevB and GT2560 4.0/4.1
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "GT2560 3.x"
#endif

//
// Servos
//
#define SERVO0_PIN                            PinB5  // 13 untested  3Dtouch

//
// Limit Switches
//
#ifndef X_STOP_PIN
  #ifndef X_MIN_PIN
    #define X_MIN_PIN                         PinA2
  #endif
  #ifndef X_MAX_PIN
    #define X_MAX_PIN                         PinA0
  #endif
#endif
#ifndef Y_STOP_PIN
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                         PinA6
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                         PinA4
  #endif
#endif
#ifndef Z_STOP_PIN
  #ifndef Z_MIN_PIN
    #define Z_MIN_PIN                         PinC7
  #endif
  #ifndef Z_MAX_PIN
    #define Z_MAX_PIN                         PinC5
  #endif
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC5
#endif

//
// Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinK4
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                     PinK5
#endif
#ifndef FIL_RUNOUT3_PIN
  #define FIL_RUNOUT3_PIN                     PinF0
#endif

//
// Power Recovery
//
#define POWER_LOSS_PIN                        PinK7  // Pin to detect power loss
#define POWER_LOSS_STATE                      LOW

//
// Steppers
//
#define X_STEP_PIN                            PinC0
#define X_DIR_PIN                             PinG2
#define X_ENABLE_PIN                          PinC2

#define Y_STEP_PIN                            PinC6
#define Y_DIR_PIN                             PinC4
#define Y_ENABLE_PIN                          PinA7

#define Z_STEP_PIN                            PinA3
#define Z_DIR_PIN                             PinA1
#define Z_ENABLE_PIN                          PinA5

#define E0_STEP_PIN                           PinL3
#define E0_DIR_PIN                            PinL5
#define E0_ENABLE_PIN                         PinB6

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

#define E2_STEP_PIN                           PinL6
#define E2_DIR_PIN                            PinL4
#define E2_ENABLE_PIN                         PinG0

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB5  // Analog Input
#define TEMP_1_PIN                            PiNH6  // Analog Input
#define TEMP_2_PIN                            PinH5  // Analog Input
#define TEMP_BED_PIN                          PinB4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB4
#define HEATER_1_PIN                          PinE5
#define HEATER_2_PIN                          PinE4
#define HEATER_BED_PIN                        PinG5
#define FAN_PIN                               PinH6
#define FAN1_PIN                              PinH5
#define FAN2_PIN                              PinH4

//
// Misc. Functions
//
#define SD_DETECT_PIN                         PinD7
#define SDSS                                  PinB0
#define LED_PIN                               PinB7  // Use 6 (case light) for external LED. 13 is internal (yellow) LED.
#define PS_ON_PIN                             PinB6

#if NUM_RUNOUT_SENSORS < 3
  #define SUICIDE_PIN                         PinF0  // This pin must be enabled at boot to keep power flowing
#endif

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinH3  // 21
#endif

//
// LCD Controller
//
#define BEEPER_PIN                            PinD3

#if ENABLED(YHCB2004)
  #ifndef YHCB2004_CLK
    #define YHCB2004_CLK                      PinE3
    #define DIO52                   YHCB2004_CLK
  #endif
  #ifndef YHCB2004_MOSI
    #define YHCB2004_MOSI                     PinD0
    #define DIO50                  YHCB2004_MOSI
  #endif
  #ifndef YHCB2004_MISO
    #define YHCB2004_MISO                     PinC1
    #define DIO51                  YHCB2004_MISO
  #endif
#elif HAS_WIRED_LCD
  #ifndef LCD_PINS_RS
    #define LCD_PINS_RS                       PinD1
  #endif
  #ifndef LCD_PINS_ENABLE
    #define LCD_PINS_ENABLE                   PinH0
  #endif
  #ifndef LCD_PINS_D4
    #define LCD_PINS_D4                       PinH1
  #endif
  #ifndef LCD_PINS_D5
    #define LCD_PINS_D5                       PinD0
  #endif
  #ifndef LCD_PINS_D6
    #define LCD_PINS_D6                       PinE3
  #endif
  #ifndef LCD_PINS_D7
    #define LCD_PINS_D7                       PinC1
  #endif
#endif

#if ENABLED(YHCB2004)
  #ifndef BTN_EN1
    #define BTN_EN1                           PinH1
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2                           PinH0
  #endif
  #ifndef BTN_ENC
    #define BTN_ENC                           PinD2
  #endif
#elif IS_NEWPANEL
  #ifndef BTN_EN1
    #define BTN_EN1                           PinL7
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2                           PinG1
  #endif
  #ifndef BTN_ENC
    #define BTN_ENC                           PinD3
  #endif
#endif

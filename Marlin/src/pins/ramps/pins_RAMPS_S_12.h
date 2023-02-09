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
 * Arduino Mega with RAMPS-S v1.2 by Sakul.cz pin assignments
 * Written by Michal Rábek <rabek33@gmail.com>
 * ATmega2560
 *
 * Applies to the following boards:
 *
 *  BOARD_RAMPS_S_12_EEFB  Ramps S 1.2 (Hotend0, Hotend1, Fan, Bed)
 *  BOARD_RAMPS_S_12_EEEB  Ramps S 1.2 (Hotend0, Hotend1, Hotend2, Bed)
 *  BOARD_RAMPS_S_12_EFFB  Ramps S 1.2 (Hotend, Fan0, Fan1, Bed)
 *
 * Other pins_MYBOARD.h files may override these defaults
 */

#include "env_validate.h"

// Custom flags and defines for the build
//#define BOARD_CUSTOM_BUILD_FLAGS -D__FOO__

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "RAMPS S 1.2"
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                          PinB4
#endif
#ifndef SERVO1_PIN
  #define SERVO1_PIN                          PinB5
#endif
#ifndef SERVO2_PIN
  #define SERVO2_PIN                          PinB6
#endif
#ifndef SERVO3_PIN
  #define SERVO3_PIN                          PinL5
#endif

//
// Limit Switches
//
#ifndef X_STOP_PIN
  #ifndef X_MIN_PIN
    #define X_MIN_PIN                         PinC0
  #endif
  #ifndef X_MAX_PIN
    #define X_MAX_PIN                         PinC1
  #endif
#endif
#ifndef Y_STOP_PIN
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                         PinC2
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                         PinC3
  #endif
#endif
#ifndef Z_STOP_PIN
  #ifndef Z_MIN_PIN
    #define Z_MIN_PIN                         PinC4
  #endif
  #ifndef Z_MAX_PIN
    #define Z_MAX_PIN                         PinC5
  #endif
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinE3
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinL5  // RAMPS_S S3 on the servos connector
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinH0
#define X_DIR_PIN                             PinH1
#define X_ENABLE_PIN                          PinL1

#define Y_STEP_PIN                            PinF0
#define Y_DIR_PIN                             PinL2
#define Y_ENABLE_PIN                          PinF1

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                          PinF3
#endif
#define Z_DIR_PIN                             PinF2
#define Z_ENABLE_PIN                          PinK0

#define E0_STEP_PIN                           PinA1
#define E0_DIR_PIN                            PinA0
#define E0_ENABLE_PIN                         PinA2

#define E1_STEP_PIN                           PinA4
#define E1_DIR_PIN                            PinA3
#define E1_ENABLE_PIN                         PinA5

#define E2_STEP_PIN                           PinA7
#define E2_DIR_PIN                            PinA6
#define E2_ENABLE_PIN                         PinG2

//
// Temperature Sensors
//
#ifndef TEMP_0_PIN
  #define TEMP_0_PIN                          PinJ0  // Analog Input
#endif
#ifndef TEMP_1_PIN
  #define TEMP_1_PIN                          PinJ1  // Analog Input
#endif
#ifndef TEMP_2_PIN
  #define TEMP_2_PIN                          PinB7  // Analog Input
#endif
#ifndef TEMP_3_PIN
  #define TEMP_3_PIN                          PinB6  // Analog Input
#endif
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN                        PinB5  // Analog Input
#endif

//
// Heaters / Fans
//
#ifndef MOSFET_D_PIN
  #define MOSFET_D_PIN                        -1
#endif
#ifndef RAMPS_S_HE_0
  #define RAMPS_S_HE_0                        PinE4
#endif
#ifndef RAMPS_S_HE_1
  #define RAMPS_S_HE_1                        PinE5
#endif
#ifndef RAMPS_S_HE_2
  #define RAMPS_S_HE_2                        PinH3
#endif

#define HEATER_BED_PIN                        PinH6

#define HEATER_0_PIN                RAMPS_S_HE_0

#if MB(RAMPS_S_12_EEFB)                           // Hotend0, Hotend1, Fan, Bed
  #define HEATER_1_PIN              RAMPS_S_HE_1
  #define FAN_PIN                   RAMPS_S_HE_2
#elif MB(RAMPS_S_12_EEEB)                         // Hotend0, Hotend1, Hotend2, Bed
  #define HEATER_1_PIN              RAMPS_S_HE_1
  #define HEATER_2_PIN              RAMPS_S_HE_2
#elif MB(RAMPS_S_12_EFFB)                         // Hotend, Fan0, Fan1, Bed
  #define FAN_PIN                   RAMPS_S_HE_1
  #define FAN1_PIN                  RAMPS_S_HE_2
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7

#ifndef KILL_PIN
  #define KILL_PIN                            PinL3
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        PinF6  // Analog Input on EXTEND
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                           PinB6  // RAMPS_S S2 on the servos connector
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !defined(CASE_LIGHT_PIN) && !defined(SPINDLE_LASER_ENA_PIN)
  #if NUM_SERVOS <= 1                             // Prefer the servo connector
    #define CASE_LIGHT_PIN                    PinB6  // Hardware PWM (RAMPS_S S1 on the servos connector)
  #elif HAS_FREE_AUX2_PINS
    #define CASE_LIGHT_PIN                    PinL5  // Hardware PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER && !defined(SPINDLE_LASER_ENA_PIN)
  #define SPINDLE_LASER_ENA_PIN               PinG5  // Pullup or pulldown!
  #define SPINDLE_LASER_PWM_PIN               PinH3  // Hardware PWM
  #define SPINDLE_DIR_PIN                     PinE3
#endif

//
// TMC SPI
//
#ifndef TMC_SW_MOSI
  #define TMC_SW_MOSI                         PinB2
#endif
#ifndef TMC_SW_MISO
  #define TMC_SW_MISO                         PinB3
#endif
#ifndef TMC_SW_SCK
  #define TMC_SW_SCK                          PinB0
#endif

//
// Průša i3 MK2 Multiplexer Support
//
#if HAS_PRUSA_MMU1
  #ifndef E_MUX0_PIN
    #define E_MUX0_PIN                        PinA7  // E2_STEP_PIN
  #endif
  #ifndef E_MUX1_PIN
    #define E_MUX1_PIN                        PinA6  // E2_DIR_PIN
  #endif
  #ifndef E_MUX2_PIN
    #define E_MUX2_PIN                        PinG2  // E2_ENABLE_PIN
  #endif
#endif

//////////////////////////
// LCDs and Controllers //
//////////////////////////

//
// LCD Display output pins
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN                          PinL4
  #define LCD_PINS_RS                         PinD2
  #define LCD_PINS_ENABLE                     PinL0
  #define LCD_PINS_D4                         PinD3
  #define LCD_PINS_D5                         PinC7
  #define LCD_PINS_D6                         PinG0
  #define LCD_PINS_D7                         PinC6
  #ifndef SD_DETECT_PIN
    #define SD_DETECT_PIN                     PinD7
  #endif

  #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define BTN_ENC_EN               LCD_PINS_D7  // Detect the presence of the encoder
  #endif

#endif

//
// LCD Display input pins
//
#if IS_NEWPANEL
  #define BTN_EN1                             PinG1
  #define BTN_EN2                             PinL7
  #define BTN_ENC                             PinL6
#endif

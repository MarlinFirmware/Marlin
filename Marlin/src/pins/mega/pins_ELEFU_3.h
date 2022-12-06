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
 * Elefu RA Board Pin Assignments
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Elefu Ra v3"

//
// Limit Switches
//
#define X_MIN_PIN                             PinC2
#define X_MAX_PIN                             PinC3
#define Y_MIN_PIN                             PinC4
#define Y_MAX_PIN                             PinC5
#define Z_MIN_PIN                             PinC6
#define Z_MAX_PIN                             PinC7

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC7
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinL0
#define X_DIR_PIN                             PinB7
#define X_ENABLE_PIN                          PinL1

#define Y_STEP_PIN                            PinB5
#define Y_DIR_PIN                             PinH6
#define Y_ENABLE_PIN                          PinB6

#define Z_STEP_PIN                            PinH4
#define Z_DIR_PIN                             PinH3
#define Z_ENABLE_PIN                          PinH5

#define E0_STEP_PIN                           PinG1
#define E0_DIR_PIN                            PinG0
#define E0_ENABLE_PIN                         PinC0

#define E1_STEP_PIN                           PinD3
#define E1_DIR_PIN                            PinD2
#define E1_ENABLE_PIN                         PinD7

#define E2_STEP_PIN                           PinL6
#define E2_DIR_PIN                            PinL2
#define E2_ENABLE_PIN                         PinL7

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE5  // Analog Input
#define TEMP_1_PIN                            PinE4  // Analog Input
#define TEMP_2_PIN                            PinE1  // Analog Input
#define TEMP_BED_PIN                          PinE0  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinL4  // 12V PWM1
#define HEATER_1_PIN                          PinL3  // 12V PWM2
#define HEATER_2_PIN                          PinH0  // 12V PWM3
#define HEATER_BED_PIN                        PinL5  // DOUBLE 12V PWM

#ifndef FAN_PIN
  #define FAN_PIN                             PinH1  // 5V PWM
#endif

//
// Misc. Functions
//
#define PS_ON_PIN                             PinB4  // Set to -1 if using a manual switch on the PWRSW Connector
#define SLEEP_WAKE_PIN                        PinA4  // This feature still needs work
#define PHOTOGRAPH_PIN                        PinA7

//
// LCD / Controller
//
#define BEEPER_PIN                            PinC1

#if ENABLED(RA_CONTROL_PANEL)

  #define SDSS                                PinB0
  #define SD_DETECT_PIN                       PinA6

  #define BTN_EN1                             PinJ1
  #define BTN_EN2                             PinG2
  #define BTN_ENC                             PinJ0

#endif // RA_CONTROL_PANEL

#if ENABLED(RA_DISCO)
  // variables for which pins the TLC5947 is using
  #define TLC_CLOCK_PIN                       PinA3
  #define TLC_BLANK_PIN                       PinA1
  #define TLC_XLAT_PIN                        PinA0
  #define TLC_DATA_PIN                        PinA2

  // We also need to define pin to port number mapping for the 2560 to match the pins listed above.
  // If you change the TLC pins, update this as well per the 2560 datasheet! This currently only works with the RA Board.
  #define TLC_CLOCK_BIT                        3
  #define TLC_CLOCK_PORT    &PORTA

  #define TLC_BLANK_BIT                        1
  #define TLC_BLANK_PORT    &PORTA

  #define TLC_DATA_BIT                         2
  #define TLC_DATA_PORT     &PORTA

  #define TLC_XLAT_BIT                         0
  #define TLC_XLAT_PORT     &PORTA

  // Change this to match your situation. Lots of TLCs takes up the arduino SRAM very quickly, so be careful
  // Leave it at at least 1 if you have enabled RA_LIGHTING
  // The number of TLC5947 boards chained together for use with the animation, additional ones will repeat the animation on them, but are not individually addressable and mimic those before them. You can leave the default at 2 even if you only have 1 TLC5947 module.
  #define NUM_TLCS                             2

  // These TRANS_ARRAY values let you change the order the LEDs on the lighting modules will animate for chase functions.
  // Modify them according to your specific situation.
  // NOTE: the array should be 8 long for every TLC you have. These defaults assume (2) TLCs.
  #define TRANS_ARRAY { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8 }    // forward
  //#define TRANS_ARRAY { 7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15 }  // backward
#endif // RA_DISCO

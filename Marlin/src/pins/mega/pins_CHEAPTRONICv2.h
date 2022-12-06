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
 * Cheaptronic v2.0 pin assignments
 * Built and sold by Michal Dyntar - RRO
 *          www.reprapobchod.cz
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Cheaptronic v2.0"

//
// Limit Switches
//
#define X_MIN_PIN                             PinC7
#define X_MAX_PIN                             PinC6
#define Y_MIN_PIN                             PinC5
#define Y_MAX_PIN                             PinC4
#define Z_MIN_PIN                             PinC3
#define Z_MAX_PIN                             PinC2

//
// Steppers
//
#define X_STEP_PIN                            PinH0
#define X_DIR_PIN                             PinH1
#define X_ENABLE_PIN                          PinL1

#define Y_STEP_PIN                            PinF0
#define Y_DIR_PIN                             PinL2
#define Y_ENABLE_PIN                          PinF1

#define Z_STEP_PIN                            PinF3
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
// Temperature sensors
//
#define TEMP_0_PIN                            PinJ0
#define TEMP_1_PIN                            PinB7
#define TEMP_2_PIN                            PinJ1
#define TEMP_3_PIN                            PinB5  // should be used for chamber temperature control
#define TEMP_BED_PIN                          PinB6

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH3
#define HEATER_1_PIN                          PinH4
#define HEATER_2_PIN                          PinH5
#define HEATER_BED_PIN                        PinH6
#ifndef FAN_PIN
  #define FAN_PIN                             PinE5
#endif
#define FAN2_PIN                              PinF4  // additional fan or light control output

//
// Other board specific pins
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinC0  // board input labeled as F-DET
#endif
#define Z_MIN_PROBE_PIN                       PinC1  // additional external board input labeled as E-SENS (should be used for Z-probe)
#define LED_PIN                               PinB7
#define SPINDLE_ENABLE_PIN                    PinG5  // additional PWM pin 1 at JP1 connector - should be used for laser control too
#define EXT_2                                 PinE3  // additional PWM pin 2 at JP1 connector
#define EXT_3                                 PinE4  // additional PWM pin 3 at JP1 connector
#define PS_ON_PIN                             PinL4
#define KILL_PIN                              PinL3

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        PinB5  // shared with TEMP_3 analog input
#endif

//
// LCD / Controller
//
#define LCD_PINS_RS                           PinD2
#define LCD_PINS_ENABLE                       PinL7
#define LCD_PINS_D4                           PinD3
#define LCD_PINS_D5                           PinD7
#define LCD_PINS_D6                           PinG0
#define LCD_PINS_D7                           PinG1

#if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
  #define BTN_ENC_EN                 LCD_PINS_D7  // Detect the presence of the encoder
#endif

//
// Beeper, SD Card, Encoder
//
#define BEEPER_PIN                            PinL5

#if ENABLED(SDSUPPORT)
  #define SDSS                                PinB0
  #define SD_DETECT_PIN                       PinL0
#endif

#if IS_NEWPANEL
  #define BTN_EN1                             PinB5
  #define BTN_EN2                             PinB6
  #define BTN_ENC                             PinL6
#endif

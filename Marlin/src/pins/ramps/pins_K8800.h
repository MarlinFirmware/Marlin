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
 * Velleman K8800 (Vertex)
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMPS/Velleman%20K8800/K8800-schematic-V1.4.pdf
 * Origin: https://www.velleman.eu/downloads/files/vertex-delta/schematics/K8800-schematic-V1.4.pdf
 * ATmega2560, ATmega1280
 */

#include "env_validate.h"

#define BOARD_INFO_NAME      "Velleman K8800"
#define DEFAULT_MACHINE_NAME "Vertex Delta"

#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Limit Switches
//
#define X_STOP_PIN                            PinE5
#define Y_STOP_PIN                            PinJ1
#define Z_STOP_PIN                            PinK4

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinK6
#endif

#define FIL_RUNOUT_PIN                        PinK7  // PK7

//
// Steppers
//
#define X_STEP_PIN                            PinF0
#define X_DIR_PIN                             PinF1
#define X_ENABLE_PIN                          PinD7

#define Y_STEP_PIN                            PinF6
#define Y_DIR_PIN                             PinF7
#define Y_ENABLE_PIN                          PinF2

#define Z_STEP_PIN                            PinL3
#define Z_DIR_PIN                             PinL1
#define Z_ENABLE_PIN                          PinK1

#define E0_STEP_PIN                           PinA4
#define E0_DIR_PIN                            PinA6
#define E0_ENABLE_PIN                         PinA2

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB7

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB4
#define FAN_PIN                               PinH5
#define CONTROLLER_FAN_PIN                    PinH6

//
// Misc. Functions
//
#define KILL_PIN                              PinD1  // PD1
#define CASE_LIGHT_PIN                        PinH4

//
// SD Card
//
#define SDSS                                  PinA3
#define SD_DETECT_PIN                         PinD0  // PD0

//
// LCD / Controller
//
#define BEEPER_PIN                            PinH3

#if HAS_WIRED_LCD

  #define LCD_SDSS                            PinB0

  #define DOGLCD_CS                           PinA7
  #define DOGLCD_A0                           PinA5

  #define LCD_PINS_RS                         PinA5
  #define LCD_PINS_ENABLE                     PinA7
  #define LCD_PINS_D4                         PinC0
  #define LCD_PINS_D5                         PinC2
  #define LCD_PINS_D6                         PinC4
  #define LCD_PINS_D7                         PinC6

  //#define LCD_SCREEN_ROTATE                180  // 0, 90, 180, 270

  #if IS_NEWPANEL
    #define BTN_EN1                           PinH0
    #define BTN_EN2                           PinH1
    #define BTN_ENC                           PinA1
  #endif

#endif // HAS_WIRED_LCD

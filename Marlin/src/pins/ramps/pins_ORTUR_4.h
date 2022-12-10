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
 * Ortur 4 Arduino Mega based on RAMPS v1.4 pin assignments
 * ATmega2560
 */

#define BOARD_INFO_NAME "Ortur 4.3"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// Servos
//
#define SERVO0_PIN                            PinA7

//
// Limit Switches
//
#define X_MAX_PIN                             PinD3
#define Z_MIN_PIN                             PinK1

#define Z_MIN_PROBE_PIN                       PinE4
#define FIL_RUNOUT_PIN                        PinF5

//
// Steppers
//
#define E0_STEP_PIN                           PinC1
#define E0_DIR_PIN                            PinC3
#define E0_ENABLE_PIN                         PinC7
#define E0_CS_PIN                             PinL5

#define E1_STEP_PIN                           PinA4
#define E1_DIR_PIN                            PinA6
#define E1_ENABLE_PIN                         PinA2
#define E1_CS_PIN                             PinL7

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinJ0  // Analog Input
#define TEMP_1_PIN                            PinB7  // Analog Input

#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                     PinF5
  #define X_SERIAL_RX_PIN                     PinK1

  #define Y_SERIAL_TX_PIN                     PinK2
  #define Y_SERIAL_RX_PIN                     PinG1

  #define Z_SERIAL_TX_PIN                     PinL5
  #define Z_SERIAL_RX_PIN                     PinL7

  #define E0_SERIAL_TX_PIN                    PinK4
  #define E0_SERIAL_RX_PIN                    PinK3
#endif

//
// LCD / Controller
//
#if IS_RRD_FG_SC
  #define BEEPER_PIN                          PinC2

  #define LCD_PINS_RS                         PinA5
  #define LCD_PINS_ENABLE                     PinA1
  #define LCD_PINS_D4                         PinC0

  #define LCD_SDSS                            PinB0
  #define SD_DETECT_PIN                       PinL0

  #define BTN_EN1                             PinA7
  #define BTN_EN2                             PinA3
  #define BTN_ENC                             PinH1

  #define LCD_PINS_DEFINED

#endif

#include "pins_RAMPS.h"

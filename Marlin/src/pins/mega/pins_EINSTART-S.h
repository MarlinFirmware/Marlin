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
 * Einstart-S pin assignments
 * ATmega2560, ATmega1280
 * PCB Silkscreen: 3DPrinterCon_v3.5
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME "Einstart-S"

//
// Limit Switches
//
#define X_STOP_PIN                            PinL5
#define Y_STOP_PIN                            PinL6
#define Z_STOP_PIN                            PinL7

//
// Steppers
//
#define X_STEP_PIN                            PinJ5
#define X_DIR_PIN                             PinJ4
#define X_ENABLE_PIN                          PinJ3

#define Y_STEP_PIN                            PinC6
#define Y_DIR_PIN                             PinC5
#define Y_ENABLE_PIN                          PinJ2

#define Z_STEP_PIN                            PinC3
#define Z_DIR_PIN                             PinC2
#define Z_ENABLE_PIN                          PinC4

#define E0_STEP_PIN                           PinC1
#define E0_DIR_PIN                            PinC0
#define E0_ENABLE_PIN                         PinC7

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE0  // Analog Input
#define TEMP_BED_PIN                          PinE1  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinD6
#define HEATER_BED_PIN                        PinD7

#define FAN_PIN                               PinD5

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinG5

//////////////////////////
// LCDs and Controllers //
//////////////////////////

//
// LCD Display output pins
//

// Requires #define U8GLIB_SH1106_EINSTART in Configuration.h
// u8glib constructor
// U8GLIB_SH1106_128X64 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, LCD_PINS_DC, LCD_PINS_RS);

#define LCD_PINS_DC                           PinE2
#define LCD_PINS_RS                           PinE6
// DOGM SPI LCD Support
#define DOGLCD_CS                             PinE5
#define DOGLCD_MOSI                           PinE4
#define DOGLCD_SCK                            PinE3
#define DOGLCD_A0                             PinE4

//
// LCD Display input pins
//
#define BTN_UP                                PinA3
#define BTN_DOWN                              PinA4
#define BTN_LEFT                              PinA5
#define BTN_RIGHT                             PinA6

// 'OK' button
#define BTN_ENC                               PinA7

// Set Kill to right arrow, same as RIGID_PANEL
#define KILL_PIN                              PinA6

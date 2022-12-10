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

// ATmega2561

#if NOT_TARGET(__AVR_ATmega1281__, __AVR_ATmega2561__)
  #error "Oops! Select 'Silvergate' in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME "Silver Gate"

#define X_STEP_PIN                            PinA5
#define X_DIR_PIN                             PinA6
#define X_ENABLE_PIN                          PinA4
#define X_MIN_PIN                             PinC1
#define X_MAX_PIN                             PinC4

#define Y_STEP_PIN                            PinA2
#define Y_DIR_PIN                             PinA3
#define Y_ENABLE_PIN                          PinA1
#define Y_MIN_PIN                             PinC2
#define Y_MAX_PIN                             PinC5

#define Z_STEP_PIN                            PinE2
#define Z_DIR_PIN                             PinA0
#define Z_ENABLE_PIN                          PinE6
#define Z_MIN_PIN                             PinC3
#define Z_MAX_PIN                             PinC6

#define E0_STEP_PIN                           PinG2
#define E0_DIR_PIN                            PinC7
#define E0_ENABLE_PIN                         PinA7

#define SDSS                                  PinB0

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinC4  // X_MAX unless overridden
#endif

#ifndef FAN_PIN
  #define FAN_PIN                             PinE3
#endif

#define HEATER_0_PIN                          PinB5

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     PinE5
#endif

#define CONTROLLER_FAN_PIN                    PinE4

#define TEMP_0_PIN                            PinB5  // Analog Input

#define HEATER_BED_PIN                        PinB6
#define TEMP_BED_PIN                          PinB4

#if HAS_WIRED_LCD
  #if IS_U8GLIB_ST7920                      // SPI GLCD 12864 ST7920
    #define LCD_PINS_RS                       PinC0
    #define LCD_PINS_ENABLE                   PinD3
    #define LCD_PINS_D4                       PinG0
    #define BEEPER_PIN                        PinG4
    #define BTN_EN1                           PinD2
    #define BTN_EN2                           PinD5
    #define BTN_ENC                           PinD7
    #define LCD_BACKLIGHT_PIN                 PinB4
    #if ENABLED(SILVER_GATE_GLCD_CONTROLLER)
      #define KILL_PIN                        PinD4
      #define HOME_PIN                        PinG3
    #endif
    #define BOARD_ST7920_DELAY_1               0
    #define BOARD_ST7920_DELAY_2             250
    #define BOARD_ST7920_DELAY_3               0
  #endif
#endif

#define SD_DETECT_PIN                         PinE7

#define STAT_LED_RED_PIN                      PinD6
#define STAT_LED_BLUE_PIN                     PinG1

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinF5
#endif

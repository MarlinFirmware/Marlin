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
 * Arduino Mega with RAMPS v1.4Plus, also known as 3DYMY version, pin assignments
 * The differences to the RAMPS v1.4 are:
 *  - Swap heater E0 with E1
 *  - Swap pins 8 and 10. Bed/Fan/Hotend as labeled on the board are on pins 8/9/10.
 *
 * LCD adapter
 *         ------                     ------
 *    D37 |10  9 | D35    (MISO) D50 |10  9 | D52 (SCK)
 *    D31 | 8  7 | D41           D29 | 8  7 | D53
 *    D33   6  5 | D23           D25   6  5 | D51 (MOSI)
 *    D42 | 4  3 | D44           D49 | 4  3 | D27
 *    GND | 2  1 | 5V            GND | 2  1 | 5V
 *         ------                     ------
 *          EXP1                       EXP2
 *
 * Applies to the following boards:
 *
 *  RAMPS_PLUS_EFB (Extruder, Fan, Bed)
 *  RAMPS_PLUS_EEB (Extruder, Extruder, Bed)
 *  RAMPS_PLUS_EFF (Extruder, Fan, Fan)
 *  RAMPS_PLUS_EEF (Extruder, Extruder, Fan)
 *  RAMPS_PLUS_SF  (Spindle, Controller Fan)
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "RAMPS 1.4 Plus"

#define MOSFET_A_PIN                           8
#define MOSFET_C_PIN                          10

//
// Steppers
//
#define X_CS_PIN                              -1
#define Y_CS_PIN                              -1
#define Z_CS_PIN                              -1

// Swap E0 / E1 on 3DYMY
#define E0_STEP_PIN                           36
#define E0_DIR_PIN                            34
#define E0_ENABLE_PIN                         30
#define E0_CS_PIN                             -1

#define E1_STEP_PIN                           26
#define E1_DIR_PIN                            28
#define E1_ENABLE_PIN                         24
#define E1_CS_PIN                             -1

#include "pins_RAMPS.h"

#if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
  #undef LCD_PINS_RS
  #define LCD_PINS_RS                         41
  #undef LCD_PINS_ENABLE
  #define LCD_PINS_ENABLE                     31
  #undef LCD_PINS_D4
  #define LCD_PINS_D4                         33
  #undef LCD_PINS_D5
  #define LCD_PINS_D5                         23
  #undef LCD_PINS_D6
  #define LCD_PINS_D6                         42
  #undef LCD_PINS_D7
  #define LCD_PINS_D7                         44
  #undef BTN_EN1
  #define BTN_EN1                             25
  #undef BTN_EN2
  #define BTN_EN2                             29
  #undef KILL_PIN
  #define KILL_PIN                            27
#elif IS_ULTRA_LCD && NONE(REPRAPWORLD_GRAPHICAL_LCD, CR10_STOCKDISPLAY) && !BOTH(IS_NEWPANEL, PANEL_ONE)
  #if DISABLED(MKS_12864OLED) || ENABLED(MKS_12864OLED_SSD1306)
    #undef LCD_PINS_RS
    #define LCD_PINS_RS                       42  // 3DYMY boards pin 16 -> 42
    #undef LCD_PINS_ENABLE
    #define LCD_PINS_ENABLE                   44  // 3DYMY boards pin 17 -> 44
  #endif
  #undef LCD_PINS_D7
  #define LCD_PINS_D7                         53  // 3DYMY boards pin 29 -> 53
#endif

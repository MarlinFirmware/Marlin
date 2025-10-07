/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * pins/lcd/WIRED.h
 * Fallback for generic wired displays (HAS_WIRED_LCD).
 *
 * Defaults are wrapped in #ifndef so boards with non-standard wiring can
 * pre-define overrides that win, and boards that already define the pins
 * (e.g. before pins_lcd.h is reached) don't trigger a double-define.
 */

// HAS_WIRED_LCD
//   HAS_DISPLAY
//     HAS_STATUS_MESSAGE

#ifndef BEEPER_PIN
  #define BEEPER_PIN        LCD1_01_PIN
#endif

#ifndef BTN_ENC
  #define BTN_ENC           LCD1_02_PIN
#endif
#ifndef BTN_EN1
  #define BTN_EN1           LCD2_03_PIN
#endif
#ifndef BTN_EN2
  #define BTN_EN2           LCD2_05_PIN
#endif

#ifndef LCD_PINS_RS
  #define LCD_PINS_RS       LCD1_04_PIN
#endif
#ifndef LCD_PINS_EN
  #define LCD_PINS_EN       LCD1_03_PIN
#endif
#ifndef LCD_PINS_D4
  #define LCD_PINS_D4       LCD1_05_PIN
#endif

#if IS_ULTIPANEL
  #ifndef LCD_PINS_D5
    #define LCD_PINS_D5     LCD1_06_PIN
  #endif
  #ifndef LCD_PINS_D6
    #define LCD_PINS_D6     LCD1_07_PIN
  #endif
  #ifndef LCD_PINS_D7
    #define LCD_PINS_D7     LCD1_08_PIN
  #endif
#endif

#ifndef LCD_SDSS_PIN
  #define LCD_SDSS_PIN      LCD2_04_PIN
#endif

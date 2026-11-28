/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * pins/lcd/FYSETC_242_OLED_12864.h
 */

// IS_RRD_SC
// U8GLIB_SH1106
//   HAS_U8GLIB_I2C_OLED
//     HAS_WIRED_LCD
//       HAS_DISPLAY
//         HAS_STATUS_MESSAGE
//     DOGLCD
//       HAS_MARLINUI_U8GLIB > (128x64)
//   IS_ULTIPANEL
//     HAS_WIRED_LCD
//       HAS_DISPLAY
//         HAS_STATUS_MESSAGE
//     IS_NEWPANEL
//     HAS_MARLINUI_MENU
//       HAS_MANUAL_MOVE_MENU

#ifndef BEEPER_PIN
  #define BEEPER_PIN                 LCD2_03_PIN
#endif

#ifndef BTN_ENC
  #define BTN_ENC                    LCD1_02_PIN
#endif
#ifndef BTN_EN1
  #define BTN_EN1                    LCD1_01_PIN
#endif
#ifndef BTN_EN2
  #define BTN_EN2                    LCD1_08_PIN
#endif

#ifndef LCD_PINS_DC
  #define LCD_PINS_DC                LCD1_06_PIN
#endif
#ifndef LCD_PINS_RS
  #define LCD_PINS_RS                LCD2_05_PIN
#endif

#ifndef DOGLCD_SCK
  #define DOGLCD_SCK                 LCD1_03_PIN
#endif
#ifndef DOGLCD_CS
  #define DOGLCD_CS                  LCD1_04_PIN
#endif
#ifndef DOGLCD_MOSI
  #define DOGLCD_MOSI                LCD1_05_PIN
#endif

#ifndef DOGLCD_A0
  #define DOGLCD_A0                  LCD_PINS_DC
#endif

#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN               LCD1_07_PIN  // The panel's LEDs (not the board's header)
#endif

#ifndef KILL_PIN
  #define KILL_PIN                          -1    // NC
#endif

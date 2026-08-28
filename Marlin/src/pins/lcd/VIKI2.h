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
 */

#pragma once

/**
 * pins/lcd/VIKI2.h - LCD_I2C_VIKI / miniVIKI
 *
 * VIKI2 is a 128x64 DOGm-based panel with 5 buttons and status LEDs.
 * Despite being grouped with I2C controllers in Conditionals, many boards
 * route it over SPI-style pins (DOGLCD_CS/A0) and define explicit EXP
 * tokens for it. This header therefore supplies default mappings for both
 * the SPI transaction pins and the button / SD / LED pins.
 *
 * Boards with non-standard wiring can pre-define any of these pins before
 * including `pins_lcd.h`; the `#ifndef` guards make the board's override win.
 *
 * Connector convention:
 *   LCD1 = EXP1 or first 10-pin LCD connector
 *   LCD2 = EXP2 or second 10-pin connector
 *
 * Common VIKI2 pin assignments seen across boards:
 *   LCD1_01 BEEPER / LCD1_02 BTN_ENC / LCD1_06 DOGLCD_CS / LCD1_07 DOGLCD_A0
 *   LCD2_03 BTN_EN1 / LCD2_05 BTN_EN2 / LCD2_07 SD_DETECT / LCD2_08 KILL
 *
 * Boards that need board-unique pins simply pre-define the affected macros
 * before `pins_lcd.h` is reached, and the guards below skip the defaults.
 */

// Output / control pins

#ifndef DOGLCD_CS
  #define DOGLCD_CS       LCD1_06_PIN
#endif
#ifndef DOGLCD_A0
  #define DOGLCD_A0       LCD1_07_PIN
#endif

// Buttons and encoder

#ifndef BEEPER_PIN
  #define BEEPER_PIN      LCD1_01_PIN
#endif
#ifndef BTN_ENC
  #define BTN_ENC         LCD1_02_PIN
#endif
#ifndef BTN_EN1
  #define BTN_EN1         LCD2_03_PIN
#endif
#ifndef BTN_EN2
  #define BTN_EN2         LCD2_05_PIN
#endif

// SD and kill

#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN   LCD2_07_PIN
#endif
#ifndef KILL_PIN
  #define KILL_PIN        LCD2_08_PIN
#endif

// Board-specific LED and rotation defaults; guarded so unusual boards keep
// their custom values. Many boards route these through an adapter or raw port
// pin, so do NOT force a value that would collide with a real override.

#ifndef LCD_SCREEN_ROTATE
  #define LCD_SCREEN_ROTATE 0
#endif

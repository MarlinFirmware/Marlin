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
 * ZONESTAR_LCD (ANET A8 2004 LCD with ADC keypad)
 *
 * Standard EXP1 wiring used by BTT SKR E3 / E3 DIP / E3 RRF / Manta E3 / FLY RRF E3 boards.
 * Note: This LCD operates at 5V. On 3.3V boards the connector pins must be level-shifted.
 *       Do NOT connect directly to 5V — CONNECTING TO 5V WILL DAMAGE THE BOARD!
 *
 * ADC_KEYPAD_PIN is board-specific (servo pin repurposed) and must be defined per-board.
 */
#if ENABLED(ZONESTAR_LCD)

  #ifndef LCD_PINS_RS
    #define LCD_PINS_RS                LCD1_06_PIN
  #endif
  #ifndef LCD_PINS_EN
    #define LCD_PINS_EN                LCD1_02_PIN
  #endif
  #ifndef LCD_PINS_D4
    #define LCD_PINS_D4                LCD1_07_PIN
  #endif
  #ifndef LCD_PINS_D5
    #define LCD_PINS_D5                LCD1_05_PIN
  #endif
  #ifndef LCD_PINS_D6
    #define LCD_PINS_D6                LCD1_03_PIN
  #endif
  #ifndef LCD_PINS_D7
    #define LCD_PINS_D7                LCD1_01_PIN
  #endif

#endif

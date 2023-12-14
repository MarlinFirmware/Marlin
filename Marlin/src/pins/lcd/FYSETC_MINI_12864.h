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
 * pins/lcd/FYSETC_MINI_12864.h
 * See https://wiki.fysetc.com/Mini12864_Panel
 */

/**
 * NOTES:
 *
 * All these are the same with different backlight options.
 *
 * FYSETC_MINI_12864_X_X    // Type C/D/E/F. No tunable RGB Backlight by default
 * FYSETC_MINI_12864_1_2    // Type C/D/E/F. Simple RGB Backlight (always on)
 * FYSETC_MINI_12864_2_0    // Type A/B. Discreet RGB Backlight
 * FYSETC_MINI_12864_2_1    // Type A/B. NeoPixel RGB Backlight
 * FYSETC_GENERIC_12864_1_1 // Larger display with basic ON/OFF backlight
 *
 * NOTE: MKS_MINI_12864_V3 and BTT_MINI_12864_V1 are the same as FYSETC_MINI_12864_2_1 with plugs rotated 180 degrees
 *
 * Custom adapters are required for these boards:
 *   pins_BTT_SKR_E3_DIP.h               ADAPTER_CUSTOM_E3_DIP_TFT
 *   pins_BTT_SKR_MINI_E3_common.h       ADAPTER_CUSTOM_MINI_E3_TFT
 *   pins_CREALITY_V4.h                  ADAPTER_CUSTOM_CREALITY_V4_DEBUG_HEADER
 *   pins_BTT_SKR_MINI_E3_V3_0.h         ADAPTER_CUSTOM_E3_V3_NEO
 *                                       ADAPTER_SKR_MINI_SCREEN
 */

// U8GLIB_MINI12864_2X_HAL : u8g_dev_uc1701_mini12864_HAL_sw_spi, u8g_dev_uc1701_mini12864_HAL_hw_spi

// FYSETC_MINI_12864_X_X, FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1, FYSETC_GENERIC_12864_1_1
//   FYSETC_MINI_12864
//   DOGLCD
//     HAS_MARLINUI_U8GLIB
//   IS_ULTIPANEL
//     HAS_WIRED_LCD
//       HAS_DISPLAY
//         HAS_STATUS_MESSAGE
//     IS_NEWPANEL
//     HAS_MARLINUI_MENU
//       HAS_MANUAL_MOVE_MENU
//   LED_COLORS_REDUCE_GREEN

/**
 *   FYSETC Mini 12864 https://wiki.fysetc.com/docs/Mini12864Panel
 *
 *           ------                      ------
 *   BEEPER | 1  2 | ENC           MISO | 1  2 | SCK
 *   LCD_CS | 3  4 | LCD_RS         EN1 | 3  4 | SDSS
 *  LCD_RST | 5  6   LED_R (DIN)    EN2 | 5  6   MOSI
 *    LED_G | 7  8 | LED_B       SD_DET | 7  8 | RESET
 *      GND | 9 10 | 5V             GND | 9 10 | KILL
 *           ------                      ------
 *            EXP1                        EXP2
 */

#define BEEPER_PIN        LCD1_01_PIN

#define BTN_ENC           LCD1_02_PIN
#define BTN_EN1           LCD2_03_PIN
#define BTN_EN2           LCD2_05_PIN

#define DOGLCD_CS         LCD1_03_PIN
#define DOGLCD_A0         LCD1_04_PIN

#define DOGLCD_MISO       LCD2_01_PIN
#define DOGLCD_SCK        LCD2_02_PIN
#define DOGLCD_MOSI       LCD2_06_PIN

#define LCD_RESET_PIN     LCD1_05_PIN   // Must be HIGH or open for LCD to operate
#define KILL_PIN          LCD2_08_PIN

#if ENABLED(FYSETC_MINI_12864_2_1)
  #define LCD_NEOPIXEL_PIN LCD1_06_PIN
#elif ANY(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
  #ifndef RGB_LED_R_PIN
    #define RGB_LED_R_PIN LCD1_06_PIN
  #endif
  #ifndef RGB_LED_G_PIN
    #define RGB_LED_G_PIN LCD1_07_PIN
  #endif
  #ifndef RGB_LED_B_PIN
    #define RGB_LED_B_PIN LCD1_08_PIN
  #endif
#elif ENABLED(FYSETC_GENERIC_12864_1_1)
  #define LCD_BACKLIGHT_PIN LCD1_07_PIN
#endif

#if HAS_MEDIA
  #ifndef SDSS
    #define SDSS          LCD2_04_PIN
  #endif
  #ifndef SD_DETECT_PIN
    #define SD_DETECT_PIN LCD2_07_PIN
  #endif
#endif

// Default LCD appearance
#define LCD_CONTRAST_INIT 255

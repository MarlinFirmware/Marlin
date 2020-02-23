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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * lcd/dogm/ultralcd_DOGM.h
 */

#include "../../inc/MarlinConfigPre.h"

#include <U8glib.h>
#include "HAL_LCD_class_defines.h"

//#define ALTERNATIVE_LCD

#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
  #define U8G_CLASS U8GLIB_ST7920_128X64_4X
  #if DISABLED(SDSUPPORT) && (LCD_PINS_D4 == SCK_PIN) && (LCD_PINS_ENABLE == MOSI_PIN)
    #define U8G_PARAM LCD_PINS_RS
  #else
    #define U8G_PARAM LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS
  #endif

#elif ENABLED(U8GLIB_ST7920)

  // RepRap Discount Full Graphics Smart Controller

  #if DISABLED(SDSUPPORT) && (LCD_PINS_D4 == SCK_PIN) && (LCD_PINS_ENABLE == MOSI_PIN)
    #define U8G_CLASS U8GLIB_ST7920_128X64_4X_HAL               // 2 stripes, HW SPI (Shared with SD card. Non-standard LCD adapter on AVR.)
    #define U8G_PARAM LCD_PINS_RS
  #else
    #if ENABLED(ALTERNATIVE_LCD)
      #define U8G_CLASS U8GLIB_ST7920_128X64_4X                 // 2 stripes, SW SPI (Original u8glib device)
    #else
      #define U8G_CLASS U8GLIB_ST7920_128X64_RRD                // Adjust stripes with PAGE_HEIGHT in ultralcd_st7920_u8glib_rrd.h
    #endif
    #define U8G_PARAM LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS // AVR version ignores these pin settings
                                                                // HAL version uses these pin settings
  #endif

#elif ENABLED(CARTESIO_UI)

  // CartesioUI LCD

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_DOGM128_2X                         // 4 stripes
    #define FORCE_SOFT_SPI                                      // SW-SPI
  #else
    #define U8G_CLASS U8GLIB_DOGM128_2X                         // 4 stripes (HW-SPI)
  #endif

#elif ENABLED(U8GLIB_LM6059_AF)

  // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_LM6059                             // 8 stripes (HW-SPI)
  #else
    #define U8G_CLASS U8GLIB_LM6059_2X                          // 4 stripes (HW-SPI)
  #endif

#elif ENABLED(U8GLIB_ST7565_64128N)

  // MaKrPanel, Mini Viki, Viki 2.0, AZSMZ 12864 ST7565 controller

  #define SMART_RAMPS MB(RAMPS_SMART_EFB, RAMPS_SMART_EEB, RAMPS_SMART_EFF, RAMPS_SMART_EEF, RAMPS_SMART_SF)
  #define U8G_CLASS U8GLIB_64128N_2X_HAL                        // 4 stripes (HW-SPI)
  #if SMART_RAMPS || DOGLCD_SCK != SCK_PIN || DOGLCD_MOSI != MOSI_PIN
    #define FORCE_SOFT_SPI                                      // SW-SPI
  #endif

#elif ENABLED(MKS_12864OLED_SSD1306)

  // MKS 128x64 (SSD1306) OLED I2C LCD

  #define FORCE_SOFT_SPI                                        // SW-SPI

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_SSD1306_128X64_2X                  // 4 stripes
  #else
    #define U8G_CLASS U8GLIB_SSD1306_128X64                     // 8 stripes
  #endif

#elif ENABLED(U8GLIB_SSD1306)

  // Generic SSD1306 OLED I2C LCD

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_SSD1306_128X64_2X_I2C_2_WIRE       // 4 stripes
  #else
    #define U8G_CLASS U8GLIB_SSD1306_128X64_2X                  // 4 stripes
  #endif
  #define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)

#elif ENABLED(MKS_12864OLED)

  // MKS 128x64 (SH1106) OLED I2C LCD

  #define FORCE_SOFT_SPI                                        // SW-SPI

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_SH1106_128X64_2X                   // 4 stripes
  #else
    #define U8G_CLASS U8GLIB_SH1106_128X64                      // 8 stripes
  #endif

#elif ENABLED(U8GLIB_SH1106)

  // Generic SH1106 OLED I2C LCD

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_SH1106_128X64_2X_I2C_2_WIRE        // 4 stripes
  #else
    #define U8G_CLASS U8GLIB_SH1106_128X64_2X                   // 4 stripes
  #endif
  #define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)       // I2C

#elif ENABLED(U8GLIB_SSD1309)

  // Generic support for SSD1309 OLED I2C LCDs

  #define U8G_CLASS U8GLIB_SSD1309_128X64
  #define U8G_PARAM (U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST)       // I2C

#elif ENABLED(FYSETC_MINI_12864)

  // The FYSETC Mini 12864 display

  #define U8G_CLASS U8GLIB_MINI12864_2X_HAL                     // 4 stripes

#elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

  // The MKS_MINI_12864 V1/V2 aren't exact copies of the MiniPanel.
  // Panel management is in u8g_dev_uc1701_mini12864_HAL.cpp with
  // extra delays added to remove glitches seen with fast MCUs.

  #define U8G_CLASS U8GLIB_MINI12864_2X_HAL                     // 8 stripes (HW-SPI)

#elif ENABLED(MINIPANEL)

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_MINI12864
  #else
    #define U8G_CLASS U8GLIB_MINI12864_2X                       // 8 stripes (HW-SPI)
  #endif

#elif ENABLED(U8GLIB_SH1106_EINSTART)

  // Connected via motherboard header

  #define U8G_CLASS U8GLIB_SH1106_128X64
  #define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, LCD_PINS_DC, LCD_PINS_RS

#elif ENABLED(FSMC_GRAPHICAL_TFT)

  // Unspecified 320x240 TFT pre-initialized by built-in bootloader
  #if (FSMC_UPSCALE == 3)
    #define U8G_CLASS U8GLIB_TFT_480X320_UPSCALE_FROM_128X64
  #else
    #define U8G_CLASS U8GLIB_TFT_320X240_UPSCALE_FROM_128X64
  #endif
  #define U8G_PARAM FSMC_CS_PIN, FSMC_RS_PIN

#else

  #if ENABLED(ALTERNATIVE_LCD)
    #define U8G_CLASS U8GLIB_DOGM128                            // 8 stripes (HW-SPI)
  #else
    #define U8G_CLASS U8GLIB_DOGM128_2X                         // 4 stripes (HW-SPI)
  #endif

#endif

// Use HW-SPI if no other option is specified
#ifndef U8G_PARAM
  #if ENABLED(FORCE_SOFT_SPI)
    #define U8G_PARAM DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0 // SW-SPI
  #else
    #define U8G_PARAM DOGLCD_CS, DOGLCD_A0                      // HW-SPI
  #endif
#endif

// LCD_FULL_PIXEL_WIDTH =
// LCD_PIXEL_OFFSET_X + (LCD_PIXEL_WIDTH * 2) + LCD_PIXEL_OFFSET_X
#if ENABLED(FSMC_GRAPHICAL_TFT)
  //@ 2.8" TFT
  // #define LCD_FULL_PIXEL_WIDTH  320
  // #define LCD_PIXEL_OFFSET_X    32
  // #define LCD_FULL_PIXEL_HEIGHT 240
  // #define LCD_PIXEL_OFFSET_Y    32
  //@ 3.5" TFT
  #define LCD_FULL_PIXEL_WIDTH  480
  #define LCD_PIXEL_OFFSET_X    48
  #define LCD_FULL_PIXEL_HEIGHT 320
  #define LCD_PIXEL_OFFSET_Y    32 // to leave at least 60px for UI
  #if (FSMC_UPSCALE == 3)
    extern uint16_t ui_color;
    extern uint16_t bg_color;
    extern void switchColorPreset(uint8_t preset);
  #endif
#endif

// For selective rendering within a Y range
#define PAGE_OVER(ya)         ((ya) <= u8g.getU8g()->current_page.y1) // Does the current page follow a region top?
#define PAGE_UNDER(yb)        ((yb) >= u8g.getU8g()->current_page.y0) // Does the current page precede a region bottom?
#define PAGE_CONTAINS(ya, yb) ((yb) >= u8g.getU8g()->current_page.y0 && (ya) <= u8g.getU8g()->current_page.y1) // Do two vertical regions overlap?

// Only Western languages support big / small fonts
#if DISABLED(DISPLAY_CHARSET_ISO10646_1)
  #undef USE_BIG_EDIT_FONT
  #undef USE_SMALL_INFOFONT
#endif

#define MENU_FONT_NAME    ISO10646_1_5x7
#define MENU_FONT_WIDTH    6
#define MENU_FONT_ASCENT  10
#define MENU_FONT_DESCENT  2
#define MENU_FONT_HEIGHT  (MENU_FONT_ASCENT + MENU_FONT_DESCENT)

#if ENABLED(USE_BIG_EDIT_FONT)
  #define EDIT_FONT_NAME    u8g_font_9x18
  #define EDIT_FONT_WIDTH    9
  #define EDIT_FONT_ASCENT  10
  #define EDIT_FONT_DESCENT  3
#else
  #define EDIT_FONT_NAME    MENU_FONT_NAME
  #define EDIT_FONT_WIDTH   MENU_FONT_WIDTH
  #define EDIT_FONT_ASCENT  MENU_FONT_ASCENT
  #define EDIT_FONT_DESCENT MENU_FONT_DESCENT
#endif
#define EDIT_FONT_HEIGHT (EDIT_FONT_ASCENT + EDIT_FONT_DESCENT)

// Get the Ascent, Descent, and total Height for the Info Screen font
#if ENABLED(USE_SMALL_INFOFONT)
  extern const u8g_fntpgm_uint8_t u8g_font_6x9[];
  #define INFO_FONT_ASCENT 7
#else
  #define INFO_FONT_ASCENT 8
#endif
#define INFO_FONT_DESCENT 2
#define INFO_FONT_HEIGHT (INFO_FONT_ASCENT + INFO_FONT_DESCENT)
#define INFO_FONT_WIDTH   6

extern U8G_CLASS u8g;

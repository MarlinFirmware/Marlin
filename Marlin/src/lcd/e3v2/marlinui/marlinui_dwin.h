/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/e3v2/marlinui/lcdprint_dwin.h
 */

#include "../../../inc/MarlinConfigPre.h"
#include "dwin_lcd.h"

typedef uint16_t dwin_coord_t;  // Screen can be pretty big
typedef uint16_t lcd_uint_t;
typedef int16_t lcd_int_t;

typedef struct {
  uint8_t index, width, height;
  uint16_t fg, bg;
  bool solid;
} dwin_font_t;

extern dwin_font_t dwin_font;

// Only Western languages support big / small fonts
//#if DISABLED(DISPLAY_CHARSET_ISO10646_1)
//  #undef USE_BIG_EDIT_FONT
//  #undef USE_SMALL_INFOFONT
//#endif

#if ENABLED(USE_BIG_EDIT_FONT)
  #define DWIN_FONT_EDIT font10x20
#else
  #define DWIN_FONT_EDIT font8x16
#endif

#define DWIN_FONT_INFO font8x16

#if DWIN_FONT_MENU == font6x12
  #define MENU_FONT_WIDTH    6
  #define MENU_FONT_ASCENT  10
  #define MENU_FONT_DESCENT  2
#elif DWIN_FONT_MENU == font8x16
  #define MENU_FONT_WIDTH    8
  #define MENU_FONT_ASCENT  13
  #define MENU_FONT_DESCENT  3
#elif DWIN_FONT_MENU == font10x20
  #define MENU_FONT_WIDTH   10
  #define MENU_FONT_ASCENT  16
  #define MENU_FONT_DESCENT  4
#endif
#define MENU_FONT_HEIGHT (MENU_FONT_ASCENT + MENU_FONT_DESCENT)

#define EXTRA_ROW_HEIGHT 8
#define MENU_LINE_HEIGHT (MENU_FONT_HEIGHT + EXTRA_ROW_HEIGHT)

#if DWIN_FONT_EDIT == font6x12
  #define EDIT_FONT_WIDTH    6
  #define EDIT_FONT_ASCENT  10
  #define EDIT_FONT_DESCENT  2
#elif DWIN_FONT_EDIT == font8x16
  #define EDIT_FONT_WIDTH    8
  #define EDIT_FONT_ASCENT  13
  #define EDIT_FONT_DESCENT  3
#elif DWIN_FONT_EDIT == font10x20
  #define EDIT_FONT_WIDTH   10
  #define EDIT_FONT_ASCENT  16
  #define EDIT_FONT_DESCENT  4
#endif
#define EDIT_FONT_HEIGHT (EDIT_FONT_ASCENT + EDIT_FONT_DESCENT)

#if DWIN_FONT_INFO == font6x12
  #define INFO_FONT_WIDTH    6
  #define INFO_FONT_ASCENT  10
  #define INFO_FONT_DESCENT  2
#elif DWIN_FONT_INFO == font8x16
  #define INFO_FONT_WIDTH    8
  #define INFO_FONT_ASCENT  13
  #define INFO_FONT_DESCENT  3
#elif DWIN_FONT_INFO == font10x20
  #define INFO_FONT_WIDTH   10
  #define INFO_FONT_ASCENT  16
  #define INFO_FONT_DESCENT  4
#endif
#define INFO_FONT_HEIGHT (INFO_FONT_ASCENT + INFO_FONT_DESCENT)

#if DWIN_FONT_STAT == font6x12
  #define STAT_FONT_WIDTH    6
  #define STAT_FONT_ASCENT  10
  #define STAT_FONT_DESCENT  2
#elif DWIN_FONT_STAT == font8x16
  #define STAT_FONT_WIDTH    8
  #define STAT_FONT_ASCENT  13
  #define STAT_FONT_DESCENT  3
#elif DWIN_FONT_STAT == font10x20
  #define STAT_FONT_WIDTH   10
  #define STAT_FONT_ASCENT  16
  #define STAT_FONT_DESCENT  4
#elif DWIN_FONT_STAT == font12x24
  #define STAT_FONT_WIDTH   12
  #define STAT_FONT_ASCENT  19
  #define STAT_FONT_DESCENT  5
#elif DWIN_FONT_STAT == font14x28
  #define STAT_FONT_WIDTH   14
  #define STAT_FONT_ASCENT  22
  #define STAT_FONT_DESCENT  6
#elif DWIN_FONT_STAT == font16x32
  #define STAT_FONT_WIDTH   16
  #define STAT_FONT_ASCENT  26
  #define STAT_FONT_DESCENT  6
#elif DWIN_FONT_STAT == font20x40
  #define STAT_FONT_WIDTH   20
  #define STAT_FONT_ASCENT  32
  #define STAT_FONT_DESCENT  8
#elif DWIN_FONT_STAT == font24x48
  #define STAT_FONT_WIDTH   24
  #define STAT_FONT_ASCENT  38
  #define STAT_FONT_DESCENT 10
#elif DWIN_FONT_STAT == font28x56
  #define STAT_FONT_WIDTH   28
  #define STAT_FONT_ASCENT  44
  #define STAT_FONT_DESCENT 12
#elif DWIN_FONT_STAT == font32x64
  #define STAT_FONT_WIDTH   32
  #define STAT_FONT_ASCENT  50
  #define STAT_FONT_DESCENT 14
#endif
#define STAT_FONT_HEIGHT (STAT_FONT_ASCENT + STAT_FONT_DESCENT)

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

#define RED(color)            ((color >> 8) & 0xF8)
#define GREEN(color)          ((color >> 3) & 0xFC)
#define BLUE(color)           ((color << 3) & 0xF8)
#define RGB(red, green, blue) (((red << 8) & 0xF800) | ((green << 3) & 0x07E0) | ((blue >> 3) & 0x001F))
#define COLOR(color)          RGB(((color >> 16) & 0xFF), ((color >> 8) & 0xFF), (color & 0xFF))
#define HALF(color)           RGB(RED(color) >> 1, GREEN(color) >> 1, BLUE(color) >> 1)

// see https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html

#define COLOR_BLACK           0x0000  // #000000
#define COLOR_WHITE           0xFFFF  // #FFFFFF
#define COLOR_SILVER          0xC618  // #C0C0C0
#define COLOR_GREY            0x7BEF  // #808080
#define COLOR_DARKGREY        0x4208  // #404040
#define COLOR_DARKGREY2       0x39E7  // #303030
#define COLOR_DARK            0x0003  // Some dark color

#define COLOR_RED             0xF800  // #FF0000
#define COLOR_SCARLET         0xF904  // #FF2020
#define COLOR_LIME            0x7E00  // #00FF00
#define COLOR_BLUE            0x001F  // #0000FF
#define COLOR_LIGHT_BLUE      0x061F  // #00C3FF
#define COLOR_YELLOW          0xFFE0  // #FFFF00
#define COLOR_MAGENTA         0xF81F  // #FF00FF
#define COLOR_FUCHSIA         0xF81F  // #FF00FF
#define COLOR_CYAN            0x07FF  // #00FFFF
#define COLOR_AQUA            0x07FF  // #00FFFF
#define COLOR_DODGER_BLUE     0x041F  // #0080FF
#define COLOR_VIVID_VIOLET    0x7933 // #772399

#define COLOR_DARK_PURPLE     0x9930  // #992380

#define COLOR_MAROON          0x7800  // #800000
#define COLOR_GREEN           0x03E0  // #008000
#define COLOR_NAVY            0x000F  // #000080
#define COLOR_OLIVE           0x8400  // #808000
#define COLOR_PURPLE          0x8010  // #800080
#define COLOR_TEAL            0x0410  // #008080

#define COLOR_ORANGE          0xFC00  // #FF7F00
#define COLOR_VIVID_GREEN     0x7FE0  // #7FFF00
#define COLOR_DARK_ORANGE     0xFC40  // #FF8C00
#define COLOR_CORAL_RED       0xF9E7  // #FF3F3F

#define COLOR_DARK_PURPLE     0x9930  // #992380

#ifndef COLOR_BACKGROUND
  #define COLOR_BACKGROUND    0x20AC  // #1E156E
#endif
#define COLOR_SELECTION_BG    0x9930  // #992380
#define COLOR_WEBSITE_URL     0x03B7

#define COLOR_INACTIVE          COLOR_GREY
#define COLOR_COLD              COLOR_AQUA
#define COLOR_HOTEND            COLOR_SCARLET
#define COLOR_HEATED_BED        COLOR_DARK_ORANGE
#define COLOR_CHAMBER           COLOR_DARK_ORANGE
#define COLOR_FAN               COLOR_AQUA

#define COLOR_AXIS_HOMED        COLOR_WHITE
#define COLOR_AXIS_NOT_HOMED    COLOR_YELLOW

#define COLOR_RATE_100          COLOR_VIVID_GREEN
#define COLOR_RATE_ALTERED      COLOR_YELLOW

#define COLOR_PRINT_TIME        COLOR_AQUA

#define COLOR_PROGRESS_FRAME    COLOR_WHITE
#define COLOR_PROGRESS_BAR      COLOR_BLUE
#define COLOR_PROGRESS_BG       COLOR_BLACK

#define COLOR_STATUS_MESSAGE    COLOR_YELLOW

#define COLOR_CONTROL_ENABLED   COLOR_WHITE
#define COLOR_CONTROL_DISABLED  COLOR_GREY
#define COLOR_CONTROL_CANCEL    COLOR_SCARLET
#define COLOR_CONTROL_CONFIRM   COLOR_VIVID_GREEN
#define COLOR_BUSY              COLOR_SILVER

#define COLOR_MENU_TEXT         COLOR_YELLOW
#define COLOR_MENU_VALUE        COLOR_WHITE

#define COLOR_SLIDER            COLOR_WHITE
#define COLOR_SLIDER_INACTIVE   COLOR_GREY

#define COLOR_UBL               COLOR_WHITE

#define COLOR_TOUCH_CALIBRATION COLOR_WHITE

#define COLOR_KILL_SCREEN_BG    COLOR_MAROON
#define COLOR_KILL_SCREEN_TEXT  COLOR_WHITE

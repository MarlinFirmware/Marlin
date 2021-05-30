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

#include "../../inc/MarlinConfigPre.h"

#define RED(color)            ((color >> 8) & 0xF8)
#define GREEN(color)          ((color >> 3) & 0xFC)
#define BLUE(color)           ((color << 3) & 0xF8)
#define RGB(red, green, blue) (((red << 8) & 0xF800) | ((green << 3) & 0x07E0) | ((blue >> 3) & 0x001F))
#define COLOR(color)          RGB(((color >> 16) & 0xFF), ((color >> 8) & 0xFF), (color & 0xFF))
#define HALF(color)           RGB(RED(color) >> 1, GREEN(color) >> 1, BLUE(color) >> 1)

// see https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html

#define COLOR_DARKGREY    0x4208  //#404040 DEFAULT

#define C_BLACK           0x0000  //#000000 DEFAULT
#define C_DARK_PLUS_GREY  0x3186  //#323232
#define C_DARK_GREY       0x7BEF  //#808080 DEFAULT
#define C_GREY            0x7C0F  //#828282
#define C_MEDIUM_GREY     0xBDD7  //#BEBEBE
#define C_SILVER          0xC618  //#C0C0C0 DEFAULT
#define C_LIGHT_GREY      0xCE79  //#D0D0D0
#define C_WHITE           0xFFFF  //#FFFFFF DEFAULT

#define C_MARRON          0x7800  //#800000 DEFAULT
#define C_DARK_PLUS_RED   0x9945  //#A02C2D
#define C_DARK_RED        0xE121  //#FF280A
#define C_SCARLET         0xF904  //#FF2020 DEFAULT
#define C_RED             0xF800  //#FF0000 DEFAULT
#define C_CORAL_RED       0xF9E7  //#FF3F3F DEFAULT

#define C_LIGHT_PINK      0xFC13  //#FF82A0
#define C_DARK_PURPLE     0x9930  //#A02787 DEFAULT
#define C_PURPLE          0xE12D  //#323232

#define C_DARK_BLUE       0x20AC  //#1E156E DEFAULT
#define C_BLUE            0x001F  //#0000FF DEFAULT
#define C_LIGHT_BLUE      0x03B7  //#0078BE DEFAULT
#define C_LIGHT_BLUE2     0x061F  //#00C3FF DEFAULT
#define C_PASTEL_BLUE     0x6E1A  //#6EC3DC
#define C_AQUA            0x07FF  //#00FFFF DEFAULT

#define C_DARK_GREEN      0x0560  //#00AF00
#define C_PASTEL_GREEN    0x6706  //#64E632
#define C_GREEN           0x07E0  //#00FF00
#define C_LIGHT_GREEN     0x7FE0  //#82FF00 DEFAULT

#define C_GOLD            0xFE40  //#FFCD00
#define C_YELLOW          0xFFE0  //#FFFF00 DEFAULT
#define C_LIGHT_YELLOW    0xE7EA  //#EBFF55

#define C_ORANGE          0xFBE0  //#FF8000
#define C_LIGHT_ORANGE    0xFC40  //#FF8C00 DEFAULT

//Main Color
#ifndef MAIN_COLOR
  #define MAIN_COLOR    C_WHITE
#endif

//Shadow Color
#ifndef COLOR_SHADOW
  #if ENABLED(MARLIN_UI)
    #define COLOR_SHADOW    C_BLACK
  #elif ENABLED(DARK_UI)
    #define COLOR_SHADOW    C_BLACK
  #elif ENABLED(WHITE_UI)
    #define COLOR_SHADOW    C_WHITE
  #endif
#endif

//Marlin BG Color
#ifndef COLOR_BACKGROUND
  #if ENABLED(MARLIN_UI)
    #define COLOR_BACKGROUND    C_DARK_BLUE
  #elif ENABLED(DARK_UI)
    #define COLOR_BACKGROUND    C_DARK_PLUS_GREY
  #elif ENABLED(WHITE_UI)
    #define COLOR_BACKGROUND    C_WHITE
  #endif
#endif

//Hover Color
#ifndef COLOR_SELECTION_BG 
  #if ENABLED(MARLIN_UI)
    #define COLOR_SELECTION_BG    C_DARK_PURPLE
  #elif ENABLED(DARK_UI)
    #define COLOR_SELECTION_BG    C_DARK_GREY
  #elif ENABLED(WHITE_UI)
    #define COLOR_SELECTION_BG    C_SILVER
  #endif
#endif

//Link Color
#ifndef COLOR_WEBSITE_URL
  #if ENABLED(MARLIN_UI)
    #define COLOR_WEBSITE_URL    C_LIGHT_BLUE
  #elif ENABLED(DARK_UI)
    #define COLOR_WEBSITE_URL    C_PASTEL_GREEN
  #elif ENABLED(WHITE_UI)
    #define COLOR_WEBSITE_URL    C_PASTEL_GREEN
  #endif
#endif

//Off icons Color
#ifndef COLOR_INACTIVE
  #if ENABLED(White_UI)
    #define COLOR_INACTIVE          C_DARK_GREY
  #else
    #define COLOR_INACTIVE          C_LIGHT_GREY
  #endif
#endif

//Cold Icons Color
#ifndef COLOR_COLD
  #if ENABLED(MARLIN_UI)
    #define COLOR_COLD    C_AQUA
  #elif ENABLED(DARK_UI)
    #define COLOR_COLD    C_PASTEL_BLUE
  #elif ENABLED(WHITE_UI)
    #define COLOR_COLD    C_BLUE
  #endif
#endif

//Heated Hotend Color
#ifndef COLOR_HOTEND
  #if ENABLED(MARLIN_UI)
    #define COLOR_HOTEND    C_SCARLET
  #elif ENABLED(DARK_UI)
    #define COLOR_HOTEND    C_DARK_RED 
  #elif ENABLED(WHITE_UI)
    #define COLOR_HOTEND    C_RED
  #endif
#endif

//Heated BED Color
#ifndef COLOR_HEATED_BED
  #if ENABLED(MARLIN_UI)
    #define COLOR_HEATED_BED    C_LIGHT_ORANGE
  #elif ENABLED(DARK_UI)
    #define COLOR_HEATED_BED    C_ORANGE
  #elif ENABLED(WHITE_UI)
    #define COLOR_HEATED_BED    C_ORANGE
  #endif
#endif

//Chamber Color
#ifndef COLOR_CHAMBER
  #if ENABLED(MARLIN_UI)
    #define COLOR_CHAMBER    C_LIGHT_ORANGE
  #elif ENABLED(DARK_UI)
    #define COLOR_CHAMBER    C_ORANGE
  #elif ENABLED(WHITE_UI)
    #define COLOR_CHAMBER    C_ORANGE
  #endif
#endif

//Fan Color
#ifndef COLOR_FAN
  #if ENABLED(MARLIN_UI)
    #define COLOR_FAN    C_AQUA
  #elif ENABLED(DARK_UI)
    #define COLOR_FAN    C_PASTEL_BLUE
  #elif ENABLED(WHITE_UI)
    #define COLOR_FAN    C_BLUE
  #endif
#endif

//Home Position Color
#ifndef COLOR_AXIS_HOMED
  #if ENABLED(WHITE_UI)
    #define COLOR_AXIS_HOMED    C_DARK_GREY
  #else
    #define COLOR_AXIS_HOMED    C_WHITE
  #endif
#endif
#ifndef COLOR_AXIS_NOT_HOMED
  #if ENABLED(MARLIN_UI)
    #define COLOR_AXIS_NOT_HOMED    C_YELLOW
  #elif ENABLED(DARK_UI)
    #define COLOR_AXIS_NOT_HOMED    C_LIGHT_PINK
  #elif ENABLED(WHITE_UI)
    #define COLOR_AXIS_NOT_HOMED    C_LIGHT_PINK
  #endif
#endif

//Velocity and Flow Color
#ifndef COLOR_RATE_100
  #if ENABLED(MARLIN_UI)
    #define COLOR_RATE_100    C_LIGHT_GREEN
  #elif ENABLED(DARK_UI)
    #define COLOR_RATE_100    C_PASTEL_GREEN
  #elif ENABLED(WHITE_UI)
    #define COLOR_RATE_100    C_DARK_GREEN
  #endif
#endif
#ifndef COLOR_RATE_ALTERED
  #if ENABLED(MARLIN_UI)
    #define COLOR_RATE_ALTERED    C_YELLOW
  #elif ENABLED(DARK_UI)
    #define COLOR_RATE_ALTERED    C_LIGHT_YELLOW
  #elif ENABLED(WHITE_UI)
    #define COLOR_RATE_ALTERED    C_GOLD
  #endif
#endif

//Time Color
#ifndef COLOR_PRINT_TIME
  #if ENABLED(MARLIN_UI)
    #define COLOR_PRINT_TIME    C_AQUA
  #elif ENABLED(DARK_UI)
    #define COLOR_PRINT_TIME    C_PASTEL_BLUE
  #elif ENABLED(WHITE_UI)
    #define COLOR_PRINT_TIME    C_BLUE
  #endif
#endif

// Bar Time Color
#ifndef COLOR_PROGRESS_FRAME
  #if ENABLED(MARLIN_UI)
    #define COLOR_PROGRESS_FRAME    C_WHITE
  #elif ENABLED(DARK_UI)
    #define COLOR_PROGRESS_FRAME    COLOR_BACKGROUND
  #elif ENABLED(WHITE_UI)
    #define COLOR_PROGRESS_FRAME    COLOR_BACKGROUND
  #endif
#endif
#ifndef COLOR_PROGRESS_BAR
  #if ENABLED(MARLIN_UI)
    #define COLOR_PROGRESS_BAR    C_BLUE
  #elif ENABLED(DARK_UI)
    #define COLOR_PROGRESS_BAR    C_PASTEL_BLUE
  #elif ENABLED(WHITE_UI)
    #define COLOR_PROGRESS_BAR    C_BLUE
  #endif
#endif
#ifndef COLOR_PROGRESS_BG
  #if ENABLED(MARLIN_UI)
    #define COLOR_PROGRESS_BG    C_BLACK
  #elif ENABLED(DARK_UI)
    #define COLOR_PROGRESS_BG    C_DARK_GREY
  #elif ENABLED(WHITE_UI)
    #define COLOR_PROGRESS_BG    C_LIGHT_GREY
  #endif
#endif

//Message Color
#ifndef COLOR_STATUS_MESSAGE
  #if ENABLED(MARLIN_UI)
    #define COLOR_STATUS_MESSAGE    C_YELLOW
  #elif ENABLED(DARK_UI)
    #define COLOR_STATUS_MESSAGE    C_LIGHT_PINK
  #elif ENABLED(WHITE_UI)
    #define COLOR_STATUS_MESSAGE    C_LIGHT_PINK
  #endif
#endif

//Controls Color
#ifndef COLOR_CONTROL_ENABLED
  #if ENABLED(WHITE_UI)
    #define COLOR_CONTROL_ENABLED    C_DARK_GREY
  #else
    #define COLOR_CONTROL_ENABLED    C_WHITE
  #endif
#endif
#ifndef COLOR_CONTROL_DISABLED
  #if ENABLED(WHITE_UI)
    #define COLOR_CONTROL_DISABLED          C_MEDIUM_GREY
  #else
    #define COLOR_CONTROL_DISABLED          C_DARK_GREY
  #endif
#endif
#ifndef COLOR_CONTROL_CANCEL
  #if ENABLED(MARLIN_UI)
    #define COLOR_CONTROL_CANCEL    C_SCARLET
  #elif ENABLED(DARK_UI)
    #define COLOR_CONTROL_CANCEL    C_DARK_RED 
  #elif ENABLED(WHITE_UI)
    #define COLOR_CONTROL_CANCEL    C_RED
  #endif
#endif
#ifndef COLOR_CONTROL_CONFIRM
  #if ENABLED(MARLIN_UI)
    #define COLOR_CONTROL_CONFIRM    C_LIGHT_GREEN
  #elif ENABLED(DARK_UI)
    #define COLOR_CONTROL_CONFIRM    C_PASTEL_GREEN
  #elif ENABLED(WHITE_UI)
    #define COLOR_CONTROL_CONFIRM    C_GREEN
  #endif
#endif
#ifndef COLOR_BUSY
  #if ENABLED(WHITE_UI)
    #define COLOR_BUSY              C_SILVER
  #else
    #define COLOR_BUSY              C_SILVER
  #endif
#endif

//Text Menu Color
#ifndef COLOR_MENU_TEXT
  #if ENABLED(MARLIN_UI) 
    #define COLOR_MENU_TEXT    C_YELLOW
  #elif ENABLED(DARK_UI)
    #define COLOR_MENU_TEXT    C_LIGHT_PINK
  #elif ENABLED(WHITE_UI)
    #define COLOR_MENU_TEXT    C_LIGHT_PINK
  #endif
 #endif
#ifndef COLOR_MENU_VALUE
  #if ENABLED(WHITE_UI)
    #define COLOR_MENU_VALUE    C_DARK_GREY
  #else
    #define COLOR_MENU_VALUE    C_WHITE
  #endif
#endif

//Slider Color
#ifndef COLOR_SLIDER
  #if ENABLED(WHITE_UI)
    #define COLOR_SLIDER    C_DARK_GREY
  #else
    #define COLOR_SLIDER    C_WHITE
  #endif
#endif
#ifndef COLOR_SLIDER_INACTIVE
  #if ENABLED(WHITE_UI)
    #define COLOR_SLIDER_INACTIVE    C_LIGHT_GREY
  #else
    #define COLOR_SLIDER_INACTIVE    C_DARK_GREY
  #endif
#endif

//Color UBL
#ifndef COLOR_UBL
  #if ENABLED(WHITE_UI)
    #define COLOR_UBL    C_DARK_GREY
  #else
    #define COLOR_UBL    C_WHITE
  #endif
#endif

//Calibration Color
#ifndef COLOR_TOUCH_CALIBRATION
  #if ENABLED(WHITE_UI)
    #define COLOR_TOUCH_CALIBRATION    C_DARK_GREY
  #else
    #define COLOR_TOUCH_CALIBRATION    C_WHITE
  #endif
#endif

//Error Screen Color
#ifndef COLOR_KILL_SCREEN_BG
  #if ENABLED(MARLIN_UI)
    #define COLOR_KILL_SCREEN_BG          C_MARRON
  #else
    #define COLOR_KILL_SCREEN_BG          C_DARK_PLUS_RED
  #endif
#endif
#ifndef COLOR_KILL_SCREEN_TEXT
  #define COLOR_KILL_SCREEN_TEXT  C_WHITE
#endif

//Submenu axis color
#ifndef E_BTN_COLOR
  #if ENABLED(MARLIN_UI)
    #define E_BTN_COLOR    C_YELLOW
  #elif ENABLED(DARK_UI)
    #define E_BTN_COLOR    C_LIGHT_PINK
  #elif ENABLED(WHITE_UI)
    #define E_BTN_COLOR    C_LIGHT_PINK
  #endif
#endif
#ifndef X_BTN_COLOR
  #if ENABLED(MARLIN_UI)
    #define X_BTN_COLOR    C_CORAL_RED
  #elif ENABLED(DARK_UI)
    #define X_BTN_COLOR    C_DARK_RED
  #elif ENABLED(WHITE_UI)
    #define X_BTN_COLOR    C_RED
  #endif
#endif
#ifndef Y_BTN_COLOR
  #if ENABLED(MARLIN_UI)
    #define Y_BTN_COLOR    C_LIGHT_GREEN
  #elif ENABLED(DARK_UI)
    #define Y_BTN_COLOR    C_PASTEL_GREEN
  #elif ENABLED(WHITE_UI)
    #define Y_BTN_COLOR    C_DARK_GREEN
  #endif
#endif
#ifndef Z_BTN_COLOR
  #if ENABLED(MARLIN_UI)
    #define Z_BTN_COLOR    C_LIGHT_BLUE2
  #elif ENABLED(DARK_UI)
    #define Z_BTN_COLOR    C_PASTEL_BLUE
  #elif ENABLED(WHITE_UI)
    #define Z_BTN_COLOR    C_BLUE
  #endif
#endif
#ifndef OFF_BTN_COLOR
  #if ENABLED(WHITE_UI)
    #define OFF_BTN_COLOR    C_DARK_GREY
  #else
    #define OFF_BTN_COLOR    C_WHITE
  #endif
#endif
#ifndef AXIS_MENU_MESSAGE_COLOR
  #if ENABLED(MARLIN_UI)
    #define AXIS_MENU_MESSAGE_COLOR    C_YELLOW
  #elif ENABLED(DARK_UI)
    #define AXIS_MENU_MESSAGE_COLOR    C_LIGHT_YELLOW
  #elif ENABLED(WHITE_UI)
    #define AXIS_MENU_MESSAGE_COLOR    C_GOLD
  #endif
#endif
#ifndef COLOR_SHADOW_AXIS
  #if ENABLED(WHITE_UI)
    #define COLOR_SHADOW_AXIS    COLOR_BACKGROUND
  #else
    #define COLOR_SHADOW_AXIS    COLOR_DARKGREY
  #endif
#endif
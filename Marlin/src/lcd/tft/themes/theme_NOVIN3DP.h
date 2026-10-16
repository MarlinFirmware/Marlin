/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 Novin3dp
 *
 * This file is part of Marlin and is distributed under the GNU GPL v3 or later.
 * See https://www.gnu.org/licenses/ for license details.
 */
#pragma once

/**
 * Novin3dp Cosmic Cyan theme for MKS Robin Nano V3.x + MKS TS35.
 * Uses a static navy background and cyan accents; no full-screen bitmap is added.
 * Select with: #define TFT_THEME NOVIN3DP
 */
#define MOVE_AXIS_SCREEN

#ifndef COLOR_BACKGROUND
  #define COLOR_BACKGROUND        0x0865  // #08142B deep space navy
#endif
#ifndef COLOR_SELECTION_BG
  #define COLOR_SELECTION_BG      0x115A  // #102B50 selected row
#endif
#ifndef COLOR_WEBSITE_URL
  #define COLOR_WEBSITE_URL       0x577F  // #55E8FF cyan
#endif
#ifndef COLOR_INACTIVE
  #define COLOR_INACTIVE          0x630C  // subdued grey
#endif
#ifndef COLOR_COLD
  #define COLOR_COLD              0x577F
#endif
#ifndef COLOR_HOTEND
  #define COLOR_HOTEND             0xF986  // red
#endif
#ifndef COLOR_HEATED_BED
  #define COLOR_HEATED_BED         0xFD20  // amber
#endif
#ifndef COLOR_CHAMBER
  #define COLOR_CHAMBER            0xFD20
#endif
#ifndef COLOR_COOLER
  #define COLOR_COOLER             0x577F
#endif
#ifndef COLOR_FAN
  #define COLOR_FAN                0x577F
#endif
#ifndef COLOR_AXIS_HOMED
  #define COLOR_AXIS_HOMED         0xFFFF
#endif
#ifndef COLOR_AXIS_NOT_HOMED
  #define COLOR_AXIS_NOT_HOMED     0xFD20
#endif
#ifndef COLOR_RATE_100
  #define COLOR_RATE_100           0x07EF  // mint
#endif
#ifndef COLOR_RATE_ALTERED
  #define COLOR_RATE_ALTERED       0xFD20
#endif
#ifndef COLOR_PRINT_TIME
  #define COLOR_PRINT_TIME         0x577F
#endif
#ifndef COLOR_PROGRESS_FRAME
  #define COLOR_PROGRESS_FRAME     0x577F
#endif
#ifndef COLOR_PROGRESS_BAR
  #define COLOR_PROGRESS_BAR       0x577F
#endif
#ifndef COLOR_PROGRESS_BG
  #define COLOR_PROGRESS_BG        0x0865
#endif
#ifndef COLOR_STATUS_MESSAGE
  #define COLOR_STATUS_MESSAGE     0xD7FF  // pale cyan
#endif
#ifndef COLOR_CONTROL_ENABLED
  #define COLOR_CONTROL_ENABLED    0xFFFF
#endif
#ifndef COLOR_CONTROL_DISABLED
  #define COLOR_CONTROL_DISABLED   0x630C
#endif
#ifndef COLOR_CONTROL_CANCEL
  #define COLOR_CONTROL_CANCEL     0xF986
#endif
#ifndef COLOR_CONTROL_CONFIRM
  #define COLOR_CONTROL_CONFIRM    0x07EF
#endif
#ifndef COLOR_BUSY
  #define COLOR_BUSY               0xBDF7
#endif
#ifndef COLOR_MENU_TEXT
  #define COLOR_MENU_TEXT          0x577F
#endif
#ifndef COLOR_MENU_STATIC_TEXT
  #define COLOR_MENU_STATIC_TEXT   0xBDF7
#endif
#ifndef COLOR_MENU_INVERT_TEXT
  #define COLOR_MENU_INVERT_TEXT   0xFFFF
#endif
#ifndef COLOR_MENU_BACK_TEXT
  #define COLOR_MENU_BACK_TEXT     0x577F
#endif
#ifndef COLOR_MENU_EDIT_TEXT
  #define COLOR_MENU_EDIT_TEXT     0x07EF
#endif
#ifndef COLOR_MENU_VALUE
  #define COLOR_MENU_VALUE         0xFFFF
#endif
#ifndef COLOR_SLIDER
  #define COLOR_SLIDER             0x577F
#endif
#ifndef COLOR_SLIDER_INACTIVE
  #define COLOR_SLIDER_INACTIVE    0x630C
#endif
#ifndef COLOR_KEYPAD_BG
  #define COLOR_KEYPAD_BG          0x115A
#endif
#ifndef COLOR_UBL
  #define COLOR_UBL                0x577F
#endif
#ifndef COLOR_TOUCH_CALIBRATION
  #define COLOR_TOUCH_CALIBRATION  0xFFFF
#endif
#ifndef COLOR_KILL_SCREEN_BG
  #define COLOR_KILL_SCREEN_BG     0x7800
#endif
#ifndef COLOR_KILL_SCREEN_TEXT
  #define COLOR_KILL_SCREEN_TEXT   0xFFFF
#endif
#ifndef E_BTN_COLOR
  #define E_BTN_COLOR              0x577F
#endif
#ifndef X_BTN_COLOR
  #define X_BTN_COLOR              0xF986
#endif
#ifndef Y_BTN_COLOR
  #define Y_BTN_COLOR              0x07EF
#endif
#ifndef Z_BTN_COLOR
  #define Z_BTN_COLOR              0x577F
#endif

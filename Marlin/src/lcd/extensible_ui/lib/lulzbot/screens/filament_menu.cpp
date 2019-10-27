/*********************
 * filament_menu.cpp *
 *********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && ANY(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void FilamentMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_large)
    #ifdef TOUCH_UI_PORTRAIT
      #define GRID_ROWS 9
      #define GRID_COLS 2
         .text  ( BTN_POS(1,1),      BTN_SIZE(2,1), GET_TEXT_F(FILAMENT))
         .font(font_medium).colors(normal_btn)
      #if ENABLED(FILAMENT_RUNOUT_SENSOR)
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(2).button( BTN_POS(1,2),  BTN_SIZE(2,1), GET_TEXT_F(RUNOUT_SENSOR))
      #if ENABLED(LIN_ADVANCE)
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(3).button( BTN_POS(1,3),  BTN_SIZE(2,1), GET_TEXT_F(LINEAR_ADVANCE))
      .colors(action_btn)
      .tag(1) .button( BTN_POS(1,9), BTN_SIZE(2,1), GET_TEXT_F(BACK));
      #undef GRID_COLS
      #undef GRID_ROWS
    #else
      #define GRID_ROWS 6
      #define GRID_COLS 3
         .text  ( BTN_POS(1,1),      BTN_SIZE(3,1), GET_TEXT_F(FILAMENT))
         .font(font_medium).colors(normal_btn)
      #if ENABLED(FILAMENT_RUNOUT_SENSOR)
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(2).button( BTN_POS(1,2),  BTN_SIZE(3,1), GET_TEXT_F(RUNOUT_SENSOR))
      #if ENABLED(LIN_ADVANCE)
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(3).button( BTN_POS(1,3),  BTN_SIZE(3,1), GET_TEXT_F(LINEAR_ADVANCE))
      .colors(action_btn)
      .tag(1) .button( BTN_POS(1,6), BTN_SIZE(3,1), GET_TEXT_F(BACK));
    #endif
  }
}

bool FilamentMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS();                   break;
    #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    case 2: GOTO_SCREEN(FilamentRunoutScreen); break;
    #endif
    #if ENABLED(LIN_ADVANCE)
    case 3: GOTO_SCREEN(LinearAdvanceScreen);  break;
    #endif
    default: return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI

/*********************
 * bio_tune_menu.cpp *
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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE) && defined(TOUCH_UI_LULZBOT_BIO)

#include "screens.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void TuneMenu::onRedraw(draw_mode_t what) {
  #define GRID_ROWS 8
  #define GRID_COLS 2

  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(font_large)
       .text( BTN_POS(1,1), BTN_SIZE(2,1), GET_TEXT_F(MSG_PRINT_MENU));
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(font_medium)
       .enabled( isPrinting()).tag(2).button( BTN_POS(1,2), BTN_SIZE(2,1), GET_TEXT_F(MSG_PRINT_SPEED))
                              .tag(3).button( BTN_POS(1,3), BTN_SIZE(2,1), GET_TEXT_F(MSG_BED_TEMPERATURE))
       .enabled(TERN_(BABYSTEPPING, true))
                              .tag(4).button( BTN_POS(1,4), BTN_SIZE(2,1), GET_TEXT_F(MSG_NUDGE_NOZZLE))
       .enabled(!isPrinting()).tag(5).button( BTN_POS(1,5), BTN_SIZE(2,1), GET_TEXT_F(MSG_MOVE_TO_HOME))
       .enabled(!isPrinting()).tag(6).button( BTN_POS(1,6), BTN_SIZE(2,1), GET_TEXT_F(MSG_RAISE_PLUNGER))
       .enabled(!isPrinting()).tag(7).button( BTN_POS(1,7), BTN_SIZE(2,1), GET_TEXT_F(MSG_RELEASE_XY_AXIS))
       .colors(action_btn)    .tag(1).button( BTN_POS(1,8), BTN_SIZE(2,1), GET_TEXT_F(MSG_BACK));
  }
  #undef GRID_COLS
  #undef GRID_ROWS
}

bool TuneMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS();                                     break;
    case 2: GOTO_SCREEN(FeedratePercentScreen);                  break;
    case 3: GOTO_SCREEN(TemperatureScreen);                      break;
    case 4: GOTO_SCREEN(NudgeNozzleScreen);                      break;
    case 5: GOTO_SCREEN(BioConfirmHomeXYZ);                      break;
    case 6: SpinnerDialogBox::enqueueAndWait_P(F("G0 E0 F120")); break;
    case 7: StatusScreen::unlockMotors();                        break;
    default:
      return false;
  }
  return true;
}

#endif // TOUCH_UI_FTDI_EVE

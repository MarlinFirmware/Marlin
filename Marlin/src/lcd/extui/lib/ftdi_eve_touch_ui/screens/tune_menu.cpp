/*******************
 * tune_menu.cpp *
 *******************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE) && !defined(TOUCH_UI_LULZBOT_BIO)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void TuneMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true));
  }

  #ifdef TOUCH_UI_PORTRAIT
    #define GRID_ROWS 9
    #define GRID_COLS 2
    #define TEMPERATURE_POS BTN_POS(1,1), BTN_SIZE(2,1)
    #define FIL_CHANGE_POS  BTN_POS(1,2), BTN_SIZE(2,1)
    #define FILAMENT_POS    BTN_POS(1,3), BTN_SIZE(2,1)
    #define NUDGE_NOZ_POS   BTN_POS(1,4), BTN_SIZE(2,1)
    #define SPEED_POS       BTN_POS(1,5), BTN_SIZE(2,1)
    #define PAUSE_POS       BTN_POS(1,6), BTN_SIZE(2,1)
    #define STOP_POS        BTN_POS(1,7), BTN_SIZE(2,1)
    #define CASE_LIGHT_POS  BTN_POS(1,8), BTN_SIZE(2,1)
    #define BACK_POS        BTN_POS(1,9), BTN_SIZE(2,1)
  #else
    #define GRID_ROWS 5
    #define GRID_COLS 2
    #define TEMPERATURE_POS BTN_POS(1,1), BTN_SIZE(1,1)
    #define NUDGE_NOZ_POS   BTN_POS(2,1), BTN_SIZE(1,1)
    #define FIL_CHANGE_POS  BTN_POS(1,2), BTN_SIZE(1,1)
    #define SPEED_POS       BTN_POS(2,2), BTN_SIZE(1,1)
    #define PAUSE_POS       BTN_POS(1,3), BTN_SIZE(1,1)
    #define STOP_POS        BTN_POS(2,3), BTN_SIZE(1,1)
    #define FILAMENT_POS    BTN_POS(1,4), BTN_SIZE(1,1)
    #define CASE_LIGHT_POS  BTN_POS(2,4), BTN_SIZE(1,1)
    #define BACK_POS        BTN_POS(1,5), BTN_SIZE(2,1)
  #endif

  if (what & FOREGROUND) {
    using namespace ExtUI;

    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(2).button( TEMPERATURE_POS, GET_TEXT_F(MSG_TEMPERATURE))
       .enabled(!isPrinting() || isPrintingFromMediaPaused())
       .tag(3).button( FIL_CHANGE_POS,  GET_TEXT_F(MSG_FILAMENTCHANGE))
       .enabled(EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR))
       .tag(9).button( FILAMENT_POS, GET_TEXT_F(MSG_FILAMENT))
       .enabled(EITHER(HAS_BED_PROBE, BABYSTEPPING))
       .tag(4).button( NUDGE_NOZ_POS, GET_TEXT_F(TERN(BABYSTEPPING, MSG_NUDGE_NOZZLE, MSG_ZPROBE_ZOFFSET)))
       .tag(5).button( SPEED_POS, GET_TEXT_F(MSG_PRINT_SPEED))
       .tag(isPrintingFromMediaPaused() ? 7 : 6)
       .enabled(TERN0(SDSUPPORT, isPrintingFromMedia()))
       .button( PAUSE_POS, isPrintingFromMediaPaused() ? GET_TEXT_F(MSG_RESUME_PRINT) : GET_TEXT_F(MSG_PAUSE_PRINT))
       .enabled(TERN0(SDSUPPORT, isPrintingFromMedia()))
       .tag(8).button( STOP_POS, GET_TEXT_F(MSG_STOP_PRINT))
       .enabled(ENABLED(CASE_LIGHT_ENABLE))
       .tag(10).button( CASE_LIGHT_POS, GET_TEXT_F(MSG_CASE_LIGHT))
       .tag(1).colors(action_btn)
             .button( BACK_POS, GET_TEXT_F(MSG_BACK));
  }
  #undef GRID_COLS
  #undef GRID_ROWS
}

bool TuneMenu::onTouchEnd(uint8_t tag) {
  using namespace Theme;
  using namespace ExtUI;
  switch (tag) {
    case 1:  GOTO_PREVIOUS();                    break;
    case 2:  GOTO_SCREEN(TemperatureScreen);     break;
    case 3:  GOTO_SCREEN(ChangeFilamentScreen);  break;
    case 4:
      #if ENABLED(BABYSTEPPING)
        GOTO_SCREEN(NudgeNozzleScreen);
      #elif HAS_BED_PROBE
        GOTO_SCREEN(ZOffsetScreen);
      #endif
      break;
    case 5:  GOTO_SCREEN(FeedratePercentScreen);     break;
    case 6:  sound.play(twinkle, PLAY_ASYNCHRONOUS); ExtUI::pausePrint();  GOTO_SCREEN(StatusScreen); break;
    case 7:  sound.play(twinkle, PLAY_ASYNCHRONOUS); ExtUI::resumePrint(); GOTO_SCREEN(StatusScreen); break;
    case 8:
      GOTO_SCREEN(ConfirmAbortPrintDialogBox);
      current_screen.forget();
      PUSH_SCREEN(StatusScreen);
      break;
    #if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
    case 9:  GOTO_SCREEN(FilamentMenu); break;
    #endif
    #if ENABLED(CASE_LIGHT_ENABLE)
    case 10: GOTO_SCREEN(CaseLightScreen); break;
    #endif
    default:
      return false;
  }
  return true;
}

#endif // TOUCH_UI_FTDI_EVE

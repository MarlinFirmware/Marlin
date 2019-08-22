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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && !defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void TuneMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .font(font_medium);
  }

  #ifdef TOUCH_UI_PORTRAIT
    #define GRID_ROWS 8
    #define GRID_COLS 2
  #else
    #define GRID_ROWS 4
    #define GRID_COLS 2
  #endif

  if (what & FOREGROUND) {
    using namespace ExtUI;

    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(font_medium)
    #ifdef TOUCH_UI_PORTRAIT
       .tag(2).enabled(1)      .button( BTN_POS(1,1), BTN_SIZE(2,1), F("Temperature"))
       .tag(3).enabled(!isPrinting()).button( BTN_POS(1,2), BTN_SIZE(2,1), F("Change Filament"))
       #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
          .enabled(1)
        #else
          .enabled(0)
        #endif
       .tag(9).button( BTN_POS(1,3), BTN_SIZE(2,1), F("Filament Options"))
      #if ENABLED(BABYSTEPPING)
       .tag(4).enabled(1)      .button( BTN_POS(1,4), BTN_SIZE(2,1), F("Nudge Nozzle"))
      #else
        #if HAS_BED_PROBE
          .enabled(1)
        #else
          .enabled(0)
        #endif
       .tag(4)                 .button( BTN_POS(1,4), BTN_SIZE(2,1), F("Adjust Z-Offset"))
      #endif
       .tag(5).enabled(1)      .button( BTN_POS(1,5), BTN_SIZE(2,1), F("Print Speed"))
       .tag(isPrintingFromMediaPaused() ? 7 : 6)
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
        .button( BTN_POS(1,6), BTN_SIZE(2,1), isPrintingFromMediaPaused() ? F("Resume Print") : F("Pause Print"))
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
      .tag(8)             .button( BTN_POS(1,7), BTN_SIZE(2,1), F("Cancel Print"))
      .tag(1).colors(action_btn)
                          .button( BTN_POS(1,8), BTN_SIZE(2,1), F("Back"));
    #else // TOUCH_UI_PORTRAIT
       .tag(2).enabled(1) .button( BTN_POS(1,1), BTN_SIZE(1,1), F("Temperature"))
       .tag(3).enabled(!isPrinting()).button( BTN_POS(1,2), BTN_SIZE(1,1), F("Change Filament"))
      #if ENABLED(BABYSTEPPING)
       .enabled(1)
      #else
       .enabled(0)
      #endif
        #if ENABLED(BABYSTEPPING)
          .tag(4)         .button( BTN_POS(2,1), BTN_SIZE(1,1), F("Nudge Nozzle"))
        #else
          #if HAS_BED_PROBE
            .enabled(1)
          #else
            .enabled(0)
          #endif
          .tag(4)         .button( BTN_POS(1,4), BTN_SIZE(2,1), F("Adjust Z-Offset"))
        #endif
       .tag(5).enabled(1) .button( BTN_POS(2,2), BTN_SIZE(1,1), F("Print Speed"))
       .tag(isPrintingFromMediaPaused() ? 7 : 6)
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
                          .button( BTN_POS(1,3), BTN_SIZE(1,1), isPrintingFromMediaPaused() ? F("Resume Print") : F("Pause Print"))
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
       .tag(8).           button( BTN_POS(2,3), BTN_SIZE(1,1), F("Cancel Print"))
       #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
          .enabled(1)
        #else
          .enabled(0)
        #endif
       .tag(9).button( BTN_POS(1,4), BTN_SIZE(1,1), F("Filament Options"))
       .tag(1).colors(action_btn) .button( BTN_POS(2,4), BTN_SIZE(1,1), F("Back"));
    #endif
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
      #else
        #if HAS_BED_PROBE
          GOTO_SCREEN(ZOffsetScreen);
        #endif
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
    #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
    case 9:  GOTO_SCREEN(FilamentMenu); break;
    #endif
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI

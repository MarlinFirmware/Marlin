/*****************************************
 * cocoa_press/advance_settings_menu.cpp *
 *****************************************/

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
#include "../screens.h"

#ifdef COCOA_ADVANCED_SETTINGS_MENU

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define GRID_COLS 3
#define GRID_ROWS 4
#define STEPS_PER_MM_POS        BTN_POS(1,1), BTN_SIZE(1,1)
#define TMC_CURRENT_POS         BTN_POS(2,1), BTN_SIZE(1,1)
#define LIN_ADVANCE_POS         BTN_POS(3,1), BTN_SIZE(1,1)
#define VELOCITY_POS            BTN_POS(1,2), BTN_SIZE(1,1)
#define ACCELERATION_POS        BTN_POS(2,2), BTN_SIZE(1,1)
#define JERK_POS                BTN_POS(3,2), BTN_SIZE(1,1)
#define DISPLAY_POS             BTN_POS(1,3), BTN_SIZE(1,1)
#define INTERFACE_POS           BTN_POS(2,3), BTN_SIZE(1,1)
#define ENDSTOPS_POS            BTN_POS(3,3), BTN_SIZE(1,1)
#define RESTORE_DEFAULTS_POS    BTN_POS(1,4), BTN_SIZE(2,1)
#define BACK_POS                BTN_POS(3,4), BTN_SIZE(1,1)

void AdvancedSettingsMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true));
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
      .font(Theme::font_medium)
      .tag(2) .button(STEPS_PER_MM_POS,       GET_TEXT_F(MSG_STEPS_PER_MM))
              .enabled(ENABLED(HAS_TRINAMIC_CONFIG))
      .tag(3) .button(TMC_CURRENT_POS,        GET_TEXT_F(MSG_TMC_CURRENT))
              .enabled(ENABLED(LIN_ADVANCE))
      .tag(4) .button(LIN_ADVANCE_POS,        GET_TEXT_F(MSG_LINEAR_ADVANCE))
      .tag(5) .button(VELOCITY_POS,           GET_TEXT_F(MSG_MAX_SPEED_NO_UNITS))
      .tag(6) .button(ACCELERATION_POS,       GET_TEXT_F(MSG_ACCELERATION))
      .tag(7) .button(JERK_POS,               GET_TEXT_F(TERN(HAS_JUNCTION_DEVIATION, MSG_JUNCTION_DEVIATION, MSG_JERK)))
      .tag(8) .button(ENDSTOPS_POS,           GET_TEXT_F(MSG_LCD_ENDSTOPS))
      .tag(9) .button(INTERFACE_POS,          GET_TEXT_F(MSG_INTERFACE))
      .tag(10).button(DISPLAY_POS,            GET_TEXT_F(MSG_DISPLAY_MENU))
      .tag(11).button(RESTORE_DEFAULTS_POS,   GET_TEXT_F(MSG_RESTORE_DEFAULTS))
              .colors(action_btn)
      .tag(1).button(BACK_POS,                GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

bool AdvancedSettingsMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1: SaveSettingsDialogBox::promptToSaveSettings(); break;
    case  2: GOTO_SCREEN(StepsScreen); break;
    #if HAS_TRINAMIC_CONFIG
    case  3: GOTO_SCREEN(StepperCurrentScreen); break;
    #endif
    #if ENABLED(LIN_ADVANCE)
    case  4: GOTO_SCREEN(LinearAdvanceScreen);  break;
    #endif
    case  5: GOTO_SCREEN(MaxVelocityScreen); break;
    case  6: GOTO_SCREEN(DefaultAccelerationScreen);  break;
    case  7: GOTO_SCREEN(TERN(HAS_JUNCTION_DEVIATION, JunctionDeviationScreen, JerkScreen)); break;
    case  8: GOTO_SCREEN(EndstopStatesScreen); break;
    case  9: GOTO_SCREEN(InterfaceSettingsScreen); LockScreen::check_passcode(); break;
    case 10: GOTO_SCREEN(DisplayTuningScreen); break;
    case 11: GOTO_SCREEN(RestoreFailsafeDialogBox); LockScreen::check_passcode(); break;
    default: return false;
  }
  return true;
}
#endif // COCOA_ADVANCED_SETTINGS_MENU

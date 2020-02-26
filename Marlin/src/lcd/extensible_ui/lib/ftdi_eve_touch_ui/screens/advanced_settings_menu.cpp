/*****************************
 * advance_settings_menu.cpp *
 *****************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE) && !defined(TOUCH_UI_LULZBOT_BIO)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

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
    #ifdef TOUCH_UI_PORTRAIT
      #define GRID_ROWS 10
      #define GRID_COLS 2
      .enabled(
        #if HAS_BED_PROBE
          1
        #endif
      )
      .tag(2) .button( BTN_POS(1,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_ZPROBE_ZOFFSET))
      .enabled(
        #if HAS_CASE_LIGHT
          1
        #endif
      )
      .tag(16).button( BTN_POS(1,6),  BTN_SIZE(1,1), GET_TEXT_F(MSG_CASE_LIGHT))
      .tag(3) .button( BTN_POS(2,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_STEPS_PER_MM))
      .enabled(
        #if HAS_TRINAMIC
          1
        #endif
      )
      .tag(13).button( BTN_POS(1,4), BTN_SIZE(1,1), GET_TEXT_F(MSG_TMC_CURRENT))
      .enabled(
        #if HAS_TRINAMIC
          1
        #endif
      )
      .tag(14).button( BTN_POS(1,7), BTN_SIZE(2,1), GET_TEXT_F(MSG_TMC_HOMING_THRS))
      .enabled(
        #if HOTENDS > 1
          1
        #endif
      )
      .tag(4) .button( BTN_POS(1,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_OFFSETS_MENU))
      .enabled(
        #if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
          1
        #endif
      )
      .tag(11).button( BTN_POS(1,3), BTN_SIZE(1,1), GET_TEXT_F(MSG_FILAMENT))
      .tag(12).button( BTN_POS(1,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_LCD_ENDSTOPS))
      .tag(15).button( BTN_POS(2,6), BTN_SIZE(1,1), GET_TEXT_F(MSG_DISPLAY_MENU))
      .tag(9) .button( BTN_POS(1,8), BTN_SIZE(2,1), GET_TEXT_F(MSG_INTERFACE_SETTINGS))
      .tag(10).button( BTN_POS(1,9), BTN_SIZE(2,1), GET_TEXT_F(MSG_RESTORE_DEFAULTS))
      .tag(5) .button( BTN_POS(2,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_VELOCITY))
      .tag(6) .button( BTN_POS(2,3), BTN_SIZE(1,1), GET_TEXT_F(MSG_ACCELERATION))
      #if DISABLED(CLASSIC_JERK)
      .tag(7) .button( BTN_POS(2,4), BTN_SIZE(1,1), GET_TEXT_F(MSG_JUNCTION_DEVIATION))
      #else
      .tag(7) .button( BTN_POS(2,4), BTN_SIZE(1,1), GET_TEXT_F(MSG_JERK))
      #endif
      .enabled(
        #if ENABLED(BACKLASH_GCODE)
          1
        #endif
      )
      .tag(8).button( BTN_POS(2,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_BACKLASH))
      .colors(action_btn)
      .tag(1) .button( BTN_POS(1,10), BTN_SIZE(2,1), GET_TEXT_F(MSG_BACK));
      #undef GRID_COLS
      #undef GRID_ROWS
    #else
      #define GRID_ROWS 6
      #define GRID_COLS 3
      .enabled(
        #if HAS_BED_PROBE
          1
        #endif
      )
      .tag(2) .button( BTN_POS(1,1),  BTN_SIZE(1,1), GET_TEXT_F(MSG_ZPROBE_ZOFFSET))
      .enabled(
        #if HAS_CASE_LIGHT
          1
        #endif
      )
      .tag(16).button( BTN_POS(1,4),  BTN_SIZE(1,1), GET_TEXT_F(MSG_CASE_LIGHT))
      .enabled(1)
      .tag(3) .button( BTN_POS(2,1),  BTN_SIZE(1,1), GET_TEXT_F(MSG_STEPS_PER_MM))
      .enabled(
        #if HAS_TRINAMIC
          1
        #endif
      )
      .tag(13).button( BTN_POS(3,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_TMC_CURRENT))
      .enabled(
        #if HAS_TRINAMIC
          1
        #endif
      )
      .tag(14).button( BTN_POS(3,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_TMC_HOMING_THRS))
      .enabled(
        #if ENABLED(BACKLASH_GCODE)
          1
        #endif
      )
      .tag(8).button( BTN_POS(3,3),  BTN_SIZE(1,1), GET_TEXT_F(MSG_BACKLASH))
      .enabled(
        #if HOTENDS > 1
          1
        #endif
      )
      .tag(4) .button( BTN_POS(1,2),  BTN_SIZE(1,1), GET_TEXT_F(MSG_OFFSETS_MENU))
      .tag(12).button( BTN_POS(3,4),  BTN_SIZE(1,1), GET_TEXT_F(MSG_LCD_ENDSTOPS))
      .tag(5) .button( BTN_POS(2,2),  BTN_SIZE(1,1), GET_TEXT_F(MSG_VELOCITY))
      .tag(6) .button( BTN_POS(2,3),  BTN_SIZE(1,1), GET_TEXT_F(MSG_ACCELERATION))
      #if DISABLED(CLASSIC_JERK)
      .tag(7) .button( BTN_POS(2,4),  BTN_SIZE(1,1), GET_TEXT_F(MSG_JUNCTION_DEVIATION))
      #else
      .tag(7) .button( BTN_POS(2,4),  BTN_SIZE(1,1), GET_TEXT_F(MSG_JERK))
      #endif
      .tag(11).button( BTN_POS(1,3),  BTN_SIZE(1,1), GET_TEXT_F(MSG_FILAMENT))
      .tag(15).button( BTN_POS(3,5),  BTN_SIZE(1,1), GET_TEXT_F(MSG_DISPLAY_MENU))
      .tag(9) .button( BTN_POS(1,5),  BTN_SIZE(2,1), GET_TEXT_F(MSG_INTERFACE_SETTINGS))
      .tag(10).button( BTN_POS(1,6),  BTN_SIZE(2,1), GET_TEXT_F(MSG_RESTORE_DEFAULTS))
      .colors(action_btn)
      .tag(1) .button( BTN_POS(3,6),  BTN_SIZE(1,1), GET_TEXT_F(MSG_BACK));
    #endif
  }
}

bool AdvancedSettingsMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: SaveSettingsDialogBox::promptToSaveSettings(); break;
    #if HAS_BED_PROBE
    case 2:  GOTO_SCREEN(ZOffsetScreen);              break;
    #endif
    case 3:  GOTO_SCREEN(StepsScreen);                break;
    #if HOTENDS > 1
    case 4:  GOTO_SCREEN(NozzleOffsetScreen);         break;
    #endif
    case 5:  GOTO_SCREEN(MaxVelocityScreen);          break;
    case 6:  GOTO_SCREEN(DefaultAccelerationScreen);  break;
    case 7:
      #if DISABLED(CLASSIC_JERK)
        GOTO_SCREEN(JunctionDeviationScreen);
      #else
        GOTO_SCREEN(JerkScreen);
      #endif
      break;
    #if ENABLED(BACKLASH_GCODE)
    case 8:  GOTO_SCREEN(BacklashCompensationScreen); break;
    #endif
    case 9:  GOTO_SCREEN(InterfaceSettingsScreen);  LockScreen::check_passcode(); break;
    case 10: GOTO_SCREEN(RestoreFailsafeDialogBox); LockScreen::check_passcode(); break;
    #if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
    case 11: GOTO_SCREEN(FilamentMenu); break;
    #endif
    case 12: GOTO_SCREEN(EndstopStatesScreen); break;
    #if HAS_TRINAMIC
    case 13: GOTO_SCREEN(StepperCurrentScreen); break;
    case 14: GOTO_SCREEN(StepperBumpSensitivityScreen); break;
    #endif
    case 15: GOTO_SCREEN(DisplayTuningScreen); break;
    #if HAS_CASE_LIGHT
    case 16: GOTO_SCREEN(CaseLightScreen); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // TOUCH_UI_FTDI_EVE

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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE) && NONE(TOUCH_UI_LULZBOT_BIO, TOUCH_UI_COCOA_PRESS)

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

    #ifdef TOUCH_UI_PORTRAIT
      #if EITHER(HAS_MULTI_HOTEND, SENSORLESS_HOMING)
        #define GRID_ROWS 9
      #else
        #define GRID_ROWS 8
      #endif
      #define GRID_COLS 2
      #define RESTORE_DEFAULTS_POS    BTN_POS(1,1), BTN_SIZE(2,1)
      #define DISPLAY_POS             BTN_POS(1,2), BTN_SIZE(1,1)
      #define INTERFACE_POS           BTN_POS(2,2), BTN_SIZE(1,1)
      #define ZPROBE_ZOFFSET_POS      BTN_POS(1,3), BTN_SIZE(1,1)
      #define STEPS_PER_MM_POS        BTN_POS(2,3), BTN_SIZE(1,1)
      #define FILAMENT_POS            BTN_POS(1,4), BTN_SIZE(1,1)
      #define VELOCITY_POS            BTN_POS(2,4), BTN_SIZE(1,1)
      #define TMC_CURRENT_POS         BTN_POS(1,5), BTN_SIZE(1,1)
      #define ACCELERATION_POS        BTN_POS(2,5), BTN_SIZE(1,1)
      #define ENDSTOPS_POS            BTN_POS(1,6), BTN_SIZE(1,1)
      #define JERK_POS                BTN_POS(2,6), BTN_SIZE(1,1)
      #define CASE_LIGHT_POS          BTN_POS(1,7), BTN_SIZE(1,1)
      #define BACKLASH_POS            BTN_POS(2,7), BTN_SIZE(1,1)
      #define OFFSETS_POS             BTN_POS(1,8), BTN_SIZE(1,1)
      #define TMC_HOMING_THRS_POS     BTN_POS(2,8), BTN_SIZE(1,1)
      #if EITHER(HAS_MULTI_HOTEND, SENSORLESS_HOMING)
        #define BACK_POS              BTN_POS(1,9), BTN_SIZE(2,1)
      #else
        #define BACK_POS              BTN_POS(1,8), BTN_SIZE(2,1)
      #endif
    #else
      #define GRID_ROWS 6
      #define GRID_COLS 3
      #define ZPROBE_ZOFFSET_POS      BTN_POS(1,1), BTN_SIZE(1,1)
      #define CASE_LIGHT_POS          BTN_POS(1,4), BTN_SIZE(1,1)
      #define STEPS_PER_MM_POS        BTN_POS(2,1), BTN_SIZE(1,1)
      #define TMC_CURRENT_POS         BTN_POS(3,1), BTN_SIZE(1,1)
      #define TMC_HOMING_THRS_POS     BTN_POS(3,2), BTN_SIZE(1,1)
      #define BACKLASH_POS            BTN_POS(3,3), BTN_SIZE(1,1)
      #define FILAMENT_POS            BTN_POS(1,3), BTN_SIZE(1,1)
      #define ENDSTOPS_POS            BTN_POS(3,4), BTN_SIZE(1,1)
      #define DISPLAY_POS             BTN_POS(3,5), BTN_SIZE(1,1)
      #define INTERFACE_POS           BTN_POS(1,5), BTN_SIZE(2,1)
      #define RESTORE_DEFAULTS_POS    BTN_POS(1,6), BTN_SIZE(2,1)
      #define VELOCITY_POS            BTN_POS(2,2), BTN_SIZE(1,1)
      #define ACCELERATION_POS        BTN_POS(2,3), BTN_SIZE(1,1)
      #define JERK_POS                BTN_POS(2,4), BTN_SIZE(1,1)
      #define OFFSETS_POS             BTN_POS(1,2), BTN_SIZE(1,1)
      #define BACK_POS                BTN_POS(3,6), BTN_SIZE(1,1)
    #endif

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
      .enabled(ENABLED(HAS_BED_PROBE))
      .tag(2) .button(ZPROBE_ZOFFSET_POS,     GET_TEXT_F(MSG_ZPROBE_ZOFFSET))
      .enabled(ENABLED(CASE_LIGHT_ENABLE))
      .tag(16).button(CASE_LIGHT_POS,         GET_TEXT_F(MSG_CASE_LIGHT))
      .tag(3) .button(STEPS_PER_MM_POS,       GET_TEXT_F(MSG_STEPS_PER_MM))
      .enabled(ENABLED(HAS_TRINAMIC_CONFIG))
      .tag(13).button(TMC_CURRENT_POS,        GET_TEXT_F(MSG_TMC_CURRENT))
      .enabled(ENABLED(SENSORLESS_HOMING))
      .tag(14).button(TMC_HOMING_THRS_POS,    GET_TEXT_F(MSG_TMC_HOMING_THRS))
      .enabled(ENABLED(HAS_MULTI_HOTEND))
      .tag(4) .button(OFFSETS_POS,            GET_TEXT_F(MSG_OFFSETS_MENU))
      .enabled(EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR))
      .tag(11).button(FILAMENT_POS,           GET_TEXT_F(MSG_FILAMENT))
      .tag(12).button(ENDSTOPS_POS,           GET_TEXT_F(MSG_LCD_ENDSTOPS))
      .tag(15).button(DISPLAY_POS,            GET_TEXT_F(MSG_DISPLAY_MENU))
      .tag(9) .button(INTERFACE_POS,          GET_TEXT_F(MSG_INTERFACE))
      .tag(10).button(RESTORE_DEFAULTS_POS,   GET_TEXT_F(MSG_RESTORE_DEFAULTS))
      .tag(5) .button(VELOCITY_POS,           GET_TEXT_F(MSG_VELOCITY))
      .tag(6) .button(ACCELERATION_POS,       GET_TEXT_F(MSG_ACCELERATION))
      .tag(7) .button(JERK_POS,               GET_TEXT_F(TERN(HAS_JUNCTION_DEVIATION, MSG_JUNCTION_DEVIATION, MSG_JERK)))
      .enabled(ENABLED(BACKLASH_GCODE))
      .tag(8).button(BACKLASH_POS,            GET_TEXT_F(MSG_BACKLASH))
      .colors(action_btn)
      .tag(1).button(BACK_POS,                GET_TEXT_F(MSG_BACK));
  }
}

bool AdvancedSettingsMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1: SaveSettingsDialogBox::promptToSaveSettings(); break;
    #if HAS_BED_PROBE
    case  2: GOTO_SCREEN(ZOffsetScreen);              break;
    #endif
    case  3: GOTO_SCREEN(StepsScreen);                break;
    #if ENABLED(HAS_MULTI_HOTEND)
    case  4: GOTO_SCREEN(NozzleOffsetScreen);         break;
    #endif
    case  5: GOTO_SCREEN(MaxVelocityScreen);          break;
    case  6: GOTO_SCREEN(DefaultAccelerationScreen);  break;
    case  7: GOTO_SCREEN(TERN(HAS_JUNCTION_DEVIATION, JunctionDeviationScreen, JerkScreen)); break;
    #if ENABLED(BACKLASH_GCODE)
    case  8: GOTO_SCREEN(BacklashCompensationScreen); break;
    #endif
    case  9: GOTO_SCREEN(InterfaceSettingsScreen);  LockScreen::check_passcode(); break;
    case 10: GOTO_SCREEN(RestoreFailsafeDialogBox); LockScreen::check_passcode(); break;
    #if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
    case 11: GOTO_SCREEN(FilamentMenu); break;
    #endif
    case 12: GOTO_SCREEN(EndstopStatesScreen); break;
    #if HAS_TRINAMIC_CONFIG
    case 13: GOTO_SCREEN(StepperCurrentScreen); break;
    #endif
    #if ENABLED(SENSORLESS_HOMING)
    case 14: GOTO_SCREEN(StepperBumpSensitivityScreen); break;
    #endif
    case 15: GOTO_SCREEN(DisplayTuningScreen); break;
    #if ENABLED(CASE_LIGHT_ENABLE)
    case 16: GOTO_SCREEN(CaseLightScreen); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // TOUCH_UI_FTDI_EVE && !TOUCH_UI_LULZBOT_BIO

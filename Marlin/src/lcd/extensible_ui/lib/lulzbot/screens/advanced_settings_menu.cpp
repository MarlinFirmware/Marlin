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

#if ENABLED(LULZBOT_TOUCH_UI) && !defined(LULZBOT_USE_BIOPRINTER_UI)

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
      #define GRID_ROWS 9
      #define GRID_COLS 2
      #if HAS_BED_PROBE
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(2) .button( BTN_POS(1,1), BTN_SIZE(1,1), F("Z Offset "))
      .enabled(1)
      .tag(3) .button( BTN_POS(2,1), BTN_SIZE(1,1), F("Steps/mm"))
      #if HAS_TRINAMIC
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(13).button( BTN_POS(1,5), BTN_SIZE(1,1), F("Motor mA"))
      #if HAS_TRINAMIC
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(14).button( BTN_POS(1,4), BTN_SIZE(1,1), F("Bump Sense"))
      #if HOTENDS > 1
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(4) .button( BTN_POS(1,2), BTN_SIZE(1,1), F("Nozzle Offset"))
      #if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(11).button( BTN_POS(1,3), BTN_SIZE(1,1), F("Filament"))
      .tag(12).button( BTN_POS(1,6), BTN_SIZE(1,1), F("Endstops"))
      .tag(15).button( BTN_POS(2,6), BTN_SIZE(1,1), F("Display"))
      .tag(9) .button( BTN_POS(1,7), BTN_SIZE(2,1), F("Interface Settings"))
      .tag(10).button( BTN_POS(1,8), BTN_SIZE(2,1), F("Restore Factory Defaults"))
      .tag(5) .button( BTN_POS(2,2), BTN_SIZE(1,1), F("Velocity "))
      .tag(6) .button( BTN_POS(2,3), BTN_SIZE(1,1), F("Acceleration"))
      #if ENABLED(JUNCTION_DEVIATION)
      .tag(7) .button( BTN_POS(2,4), BTN_SIZE(1,1), F("Junc Dev"))
      #else
      .tag(7) .button( BTN_POS(2,4), BTN_SIZE(1,1), F("Jerk"))
      #endif
      #if ENABLED(BACKLASH_GCODE)
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(8).button( BTN_POS(2,5), BTN_SIZE(1,1), F("Backlash"))
      .colors(action_btn)
      .tag(1) .button( BTN_POS(1,9), BTN_SIZE(2,1), F("Back"));
      #undef GRID_COLS
      #undef GRID_ROWS
    #else
      #define GRID_ROWS 6
      #define GRID_COLS 3
      #if HAS_BED_PROBE
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(2) .button( BTN_POS(1,1),  BTN_SIZE(1,2), F("Z Offset "))
      .enabled(1)
      .tag(3) .button( BTN_POS(2,1),  BTN_SIZE(1,1), F("Steps/mm"))
      #if HAS_TRINAMIC
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(13).button( BTN_POS(3,1), BTN_SIZE(1,1), F("Motor mA"))
      #if HAS_TRINAMIC
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(14).button( BTN_POS(3,2), BTN_SIZE(1,1), F("Bump Sense"))
      #if ENABLED(BACKLASH_GCODE)
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(8).button( BTN_POS(3,3),  BTN_SIZE(1,1), F("Backlash"))
      #if HOTENDS > 1
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(4) .button( BTN_POS(1,3),  BTN_SIZE(1,1), F("Nozzle Offsets"))
      .tag(12).button( BTN_POS(3,4),  BTN_SIZE(1,1), F("Endstops"))
      .tag(5) .button( BTN_POS(2,2),  BTN_SIZE(1,1), F("Velocity "))
      .tag(6) .button( BTN_POS(2,3),  BTN_SIZE(1,1), F("Acceleration"))
      #if ENABLED(JUNCTION_DEVIATION)
      .tag(7) .button( BTN_POS(2,4),  BTN_SIZE(1,1), F("Junc Dev"))
      #else
      .tag(7) .button( BTN_POS(2,4),  BTN_SIZE(1,1), F("Jerk"))
      #endif
      .tag(11).button( BTN_POS(1,4),  BTN_SIZE(1,1), F("Filament"))
      .tag(15).button( BTN_POS(3,5),  BTN_SIZE(1,1), F("Display"))
      .tag(9) .button( BTN_POS(1,5),  BTN_SIZE(2,1), F("Interface Settings"))
      .tag(10).button( BTN_POS(1,6),  BTN_SIZE(2,1), F("Restore Defaults"))
      .colors(action_btn)
      .tag(1) .button( BTN_POS(3,6),  BTN_SIZE(1,1), F("Back"));
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
      #if ENABLED(JUNCTION_DEVIATION)
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
    default: return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI

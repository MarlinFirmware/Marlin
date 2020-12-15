/*****************
 * language_en.h *
 *****************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
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

#pragma once

#include "language.h"

#if ENABLED(TOUCH_UI_UTF8_COPYRIGHT)
  #define COPYRIGHT_SIGN    u8"©"
#else
  #define COPYRIGHT_SIGN    u8"(c)"
#endif

#if ENABLED(TOUCH_UI_UTF8_WESTERN_CHARSET)
  #define DEGREE_SIGN       u8"°"
#else
  #define DEGREE_SIGN       u8" "
#endif

namespace Language_en {
  PROGMEM Language_Str MSG_BUTTON_OKAY              = u8"Okay";
  PROGMEM Language_Str MSG_BUTTON_MENU              = u8"Menu";
  PROGMEM Language_Str MSG_BUTTON_MEDIA             = u8"Media";
  PROGMEM Language_Str MSG_BUTTON_OPEN              = u8"Open";
  PROGMEM Language_Str MSG_CLEAN_NOZZLE             = u8"Clean Nozzle";
  PROGMEM Language_Str MSG_VMAX_X                   = u8"Vmax X";
  PROGMEM Language_Str MSG_VMAX_Y                   = u8"Vmax Y";
  PROGMEM Language_Str MSG_VMAX_Z                   = u8"Vmax Z";
  PROGMEM Language_Str MSG_ACCEL_PRINTING           = u8"Printing";
  PROGMEM Language_Str MSG_ACCEL_TRAVEL             = u8"Travel";
  PROGMEM Language_Str MSG_ACCEL_RETRACT            = u8"Retraction";
  PROGMEM Language_Str MSG_AMAX_X                   = u8"Amax X";
  PROGMEM Language_Str MSG_AMAX_Y                   = u8"Amax Y";
  PROGMEM Language_Str MSG_AMAX_Z                   = u8"Amax Z";
  PROGMEM Language_Str MSG_AXIS_X                   = u8"X";
  PROGMEM Language_Str MSG_AXIS_X2                  = u8"X2";
  PROGMEM Language_Str MSG_AXIS_Y                   = u8"Y";
  PROGMEM Language_Str MSG_AXIS_Y2                  = u8"Y2";
  PROGMEM Language_Str MSG_AXIS_Z                   = u8"Z";
  PROGMEM Language_Str MSG_AXIS_Z2                  = u8"Z2";
  PROGMEM Language_Str MSG_AXIS_E                   = u8"E";
  PROGMEM Language_Str MSG_AXIS_E1                  = u8"E1";
  PROGMEM Language_Str MSG_AXIS_E2                  = u8"E2";
  PROGMEM Language_Str MSG_AXIS_E3                  = u8"E3";
  PROGMEM Language_Str MSG_AXIS_E4                  = u8"E4";
  PROGMEM Language_Str MSG_AXIS_ALL                 = u8"All";
  PROGMEM Language_Str MSG_HOME                     = u8"Home";
  PROGMEM Language_Str MSG_PRINT_STARTING           = u8"Print starting";
  PROGMEM Language_Str MSG_PRINT_FINISHED           = u8"Print finished";
  PROGMEM Language_Str MSG_PRINT_ERROR              = u8"Print error";
  PROGMEM Language_Str MSG_ABOUT_TOUCH_PANEL_1      = u8"Color Touch Panel";
  PROGMEM Language_Str MSG_ABOUT_TOUCH_PANEL_2      = WEBSITE_URL;
  PROGMEM Language_Str MSG_LICENSE                  = u8"This program is free software: you can redistribute it and/or modify it under the terms of "
                                                        "the GNU General Public License as published by the Free Software Foundation, either version 3 "
                                                        "of the License, or (at your option) any later version.\n\nTo view a copy of the GNU General "
                                                        "Public License, go to the following location: https://www.gnu.org/licenses.";
  PROGMEM Language_Str MSG_RUNOUT_1                 = u8"Runout 1";
  PROGMEM Language_Str MSG_RUNOUT_2                 = u8"Runout 2";
  PROGMEM Language_Str MSG_DISPLAY_MENU             = u8"Display";
  PROGMEM Language_Str MSG_INTERFACE                = u8"Interface";
  PROGMEM Language_Str MSG_MEASURE_AUTOMATICALLY    = u8"Measure automatically";
  PROGMEM Language_Str MSG_H_OFFSET                 = u8"H Offset";
  PROGMEM Language_Str MSG_V_OFFSET                 = u8"V Offset";
  PROGMEM Language_Str MSG_TOUCH_SCREEN             = u8"Touch Screen";
  PROGMEM Language_Str MSG_CALIBRATE                = u8"Calibrate";
  PROGMEM Language_Str MSG_UNITS_MILLIAMP           = u8"mA";
  PROGMEM Language_Str MSG_UNITS_MM                 = u8"mm";
  PROGMEM Language_Str MSG_UNITS_MM_S               = u8"mm/s";
  PROGMEM Language_Str MSG_UNITS_MM_S2              = u8"mm/s" SUPERSCRIPT_TWO;
  PROGMEM Language_Str MSG_UNITS_STEP_MM            = u8"st/mm";
  PROGMEM Language_Str MSG_UNITS_PERCENT            = u8"%";
  PROGMEM Language_Str MSG_UNITS_C                  = DEGREE_SIGN u8"C";
  PROGMEM Language_Str MSG_IDLE                     = u8"idle";
  PROGMEM Language_Str MSG_SET_MAXIMUM              = u8"Set Maximum";
  PROGMEM Language_Str MSG_PRINT_SPEED              = u8"Print Speed";
  PROGMEM Language_Str MSG_LINEAR_ADVANCE           = u8"Linear Advance";
  PROGMEM Language_Str MSG_LINEAR_ADVANCE_K         = u8"K";
  PROGMEM Language_Str MSG_LINEAR_ADVANCE_K1        = u8"K E1";
  PROGMEM Language_Str MSG_LINEAR_ADVANCE_K2        = u8"K E2";
  PROGMEM Language_Str MSG_LINEAR_ADVANCE_K3        = u8"K E3";
  PROGMEM Language_Str MSG_LINEAR_ADVANCE_K4        = u8"K E4";
  PROGMEM Language_Str MSG_NUDGE_NOZZLE             = u8"Nudge Nozzle";
  PROGMEM Language_Str MSG_ADJUST_BOTH_NOZZLES      = u8"Adjust Both Nozzles";
  PROGMEM Language_Str MSG_SHOW_OFFSETS             = u8"Show Offsets";
  PROGMEM Language_Str MSG_INCREMENT                = u8"Increment";
  PROGMEM Language_Str MSG_ERASE_FLASH_WARNING      = u8"Are you sure? SPI flash will be erased.";
  PROGMEM Language_Str MSG_ERASING                  = u8"Erasing...";
  PROGMEM Language_Str MSG_ERASED                   = u8"SPI flash erased";
  PROGMEM Language_Str MSG_CALIBRATION_WARNING      = u8"For best results, unload the filament and clean the hotend prior to starting calibration. Continue?";
  PROGMEM Language_Str MSG_START_PRINT_CONFIRMATION = u8"Start printing %s?";
  PROGMEM Language_Str MSG_ABORT_WARNING            = u8"Are you sure you want to cancel the print?";
  PROGMEM Language_Str MSG_EXTRUDER_SELECTION       = u8"Extruder Selection";
  PROGMEM Language_Str MSG_CURRENT_TEMPERATURE      = u8"Current Temp";
  PROGMEM Language_Str MSG_REMOVAL_TEMPERATURE      = u8"Removal Temp";
  PROGMEM Language_Str MSG_CAUTION                  = u8"Caution:";
  PROGMEM Language_Str MSG_HOT                      = u8"Hot!";
  PROGMEM Language_Str MSG_UNLOAD_FILAMENT          = u8"Unload/Retract";
  PROGMEM Language_Str MSG_LOAD_FILAMENT            = u8"Load/Extrude";
  PROGMEM Language_Str MSG_MOMENTARY                = u8"Momentary";
  PROGMEM Language_Str MSG_CONTINUOUS               = u8"Continuous";
  PROGMEM Language_Str MSG_PLEASE_WAIT              = u8"Please wait...";
  PROGMEM Language_Str MSG_PRINT_MENU               = u8"Print Menu";
  PROGMEM Language_Str MSG_FINE_MOTION              = u8"Fine motion";
  PROGMEM Language_Str MSG_ENABLE_MEDIA             = u8"Enable Media";
  PROGMEM Language_Str MSG_INSERT_MEDIA             = u8"Insert Media...";
  PROGMEM Language_Str MSG_LCD_BRIGHTNESS           = u8"LCD brightness";
  PROGMEM Language_Str MSG_SOUND_VOLUME             = u8"Sound volume";
  PROGMEM Language_Str MSG_SCREEN_LOCK              = u8"Screen lock";
  PROGMEM Language_Str MSG_BOOT_SCREEN              = u8"Boot screen";
  PROGMEM Language_Str MSG_SOUNDS                   = u8"Sounds";
  PROGMEM Language_Str MSG_CLICK_SOUNDS             = u8"Click sounds";
  PROGMEM Language_Str MSG_EEPROM_RESTORED          = u8"Settings restored from backup";
  PROGMEM Language_Str MSG_EEPROM_RESET             = u8"Settings restored to default";
  PROGMEM Language_Str MSG_EEPROM_SAVED             = u8"Settings saved!";
  PROGMEM Language_Str MSG_EEPROM_SAVE_PROMPT       = u8"Do you wish to save these settings as defaults?";
  PROGMEM Language_Str MSG_EEPROM_RESET_WARNING     = u8"Are you sure? Customizations will be lost.";

  PROGMEM Language_Str MSG_PASSCODE_REJECTED        = u8"Wrong passcode!";
  PROGMEM Language_Str MSG_PASSCODE_ACCEPTED        = u8"Passcode accepted!";
  PROGMEM Language_Str MSG_PASSCODE_SELECT          = u8"Select Passcode:";
  PROGMEM Language_Str MSG_PASSCODE_REQUEST         = u8"Enter Passcode:";

  PROGMEM Language_Str MSG_TOUCH_CALIBRATION_START  = u8"Release to begin screen calibration";
  PROGMEM Language_Str MSG_TOUCH_CALIBRATION_PROMPT = u8"Touch the dots to calibrate";
  PROGMEM Language_Str MSG_AUTOLEVEL_X_AXIS         = u8"Level X Axis";
  PROGMEM Language_Str MSG_BED_MAPPING_DONE         = u8"Bed mapping finished";
  PROGMEM Language_Str MSG_BED_MAPPING_INCOMPLETE   = u8"Not all points probed";
  PROGMEM Language_Str MSG_LEVELING                 = u8"Leveling";
  PROGMEM Language_Str MSG_SHOW_MESH                = u8"Show Bed Mesh";

  #ifdef TOUCH_UI_LULZBOT_BIO
    PROGMEM Language_Str MSG_MOVE_TO_HOME           = u8"Move to Home";
    PROGMEM Language_Str MSG_RAISE_PLUNGER          = u8"Raise Plunger";
    PROGMEM Language_Str MSG_RELEASE_XY_AXIS        = u8"Release X and Y Axis";
    PROGMEM Language_Str MSG_BED_TEMPERATURE        = u8"Bed Temperature";
    PROGMEM Language_Str MSG_HOME_XYZ_WARNING       = u8"About to move to home position. Ensure the top and the bed of the printer are clear.\n\nContinue?";
    PROGMEM Language_Str MSG_HOME_E_WARNING         = u8"About to re-home plunger and auto-level. Remove syringe prior to proceeding.\n\nContinue?";
  #endif

  #ifdef TOUCH_UI_COCOA_PRESS
    PROGMEM Language_Str MSG_BODY                   = u8"Body";
    PROGMEM Language_Str MSG_INTERNAL               = u8"Internal";
    PROGMEM Language_Str MSG_EXTERNAL               = u8"External";
    PROGMEM Language_Str MSG_CHOCOLATE              = u8"Chocolate";
    PROGMEM Language_Str MSG_UNLOAD_CARTRIDGE       = u8"Unload Cartridge";
    PROGMEM Language_Str MSG_LOAD_CHOCOLATE         = u8"Load Chocolate";
    PROGMEM Language_Str MSG_CARTRIDGE_IN           = u8"Cartridge In";
    PROGMEM Language_Str MSG_CARTRIDGE_OUT          = u8"Cartridge Out";
    PROGMEM Language_Str MSG_PREHEAT_CHOCOLATE      = u8"Preheat Chocolate";
    PROGMEM Language_Str MSG_PREHEAT_FINISHED       = u8"Preheat finished";
    PROGMEM Language_Str MSG_PREHEAT                = u8"Preheat";
    PROGMEM Language_Str MSG_BUTTON_PAUSE           = u8"Pause";
    PROGMEM Language_Str MSG_BUTTON_RESUME          = u8"Resume";
    PROGMEM Language_Str MSG_ELAPSED_PRINT          = u8"Elapsed Print";
    PROGMEM Language_Str MSG_XYZ_MOVE               = u8"XYZ Move";
    PROGMEM Language_Str MSG_E_MOVE                 = u8"Extrusion Move";
  #endif
}; // namespace Language_en

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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

#include "language.h"

namespace Language_en {
  PROGMEM Language_Str YES                      = u8"Yes";
  PROGMEM Language_Str NO                       = u8"No";
  PROGMEM Language_Str BACK                     = u8"Back";
  PROGMEM Language_Str OKAY                     = u8"Okay";
  PROGMEM Language_Str MENU                     = u8"Menu";
  PROGMEM Language_Str MEDIA                    = u8"Media";

  PROGMEM Language_Str AUTO_HOME                = u8"Auto Home";
  PROGMEM Language_Str CLEAN_NOZZLE             = u8"Clean Nozzle";
  PROGMEM Language_Str MOVE_AXIS                = u8"Move Axis";
  PROGMEM Language_Str MOTORS_OFF               = u8"Motors Off";
  PROGMEM Language_Str TEMPERATURE              = u8"Temperature";
  PROGMEM Language_Str CHANGE_FILAMENT          = u8"Change Filament";
  PROGMEM Language_Str ADVANCED_SETTINGS        = u8"Advanced Settings";
  PROGMEM Language_Str ABOUT_PRINTER            = u8"About Printer";
  PROGMEM Language_Str PRINTER_STATISTICS       = u8"Printer Statistics";

  PROGMEM Language_Str ZPROBE_ZOFFSET           = u8"Z Offset";
  PROGMEM Language_Str STEPS_PER_MM             = u8"Steps/mm";
  PROGMEM Language_Str TOOL_OFFSETS             = u8"Tool Offsets";
  PROGMEM Language_Str VELOCITY                 = u8"Velocity";
  PROGMEM Language_Str VMAX_X                   = u8"Vmax X";
  PROGMEM Language_Str VMAX_Y                   = u8"Vmax Y";
  PROGMEM Language_Str VMAX_Z                   = u8"Vmax Z";
  PROGMEM Language_Str VMAX_E1                  = u8"Vmax E1";
  PROGMEM Language_Str VMAX_E2                  = u8"Vmax E2";
  PROGMEM Language_Str VMAX_E3                  = u8"Vmax E3";
  PROGMEM Language_Str VMAX_E4                  = u8"Vmax E4";
  PROGMEM Language_Str ACCELERATION             = u8"Acceleration";
  PROGMEM Language_Str ACCEL_PRINTING           = u8"Printing";
  PROGMEM Language_Str ACCEL_TRAVEL             = u8"Travel";
  PROGMEM Language_Str ACCEL_RETRACT            = u8"Retraction";
  PROGMEM Language_Str AMAX_X                   = u8"Amax X";
  PROGMEM Language_Str AMAX_Y                   = u8"Amax Y";
  PROGMEM Language_Str AMAX_Z                   = u8"Amax Z";
  PROGMEM Language_Str AMAX_E1                  = u8"Amax E1";
  PROGMEM Language_Str AMAX_E2                  = u8"Amax E2";
  PROGMEM Language_Str AMAX_E3                  = u8"Amax E3";
  PROGMEM Language_Str AMAX_E4                  = u8"Amax E4";
  PROGMEM Language_Str JERK                     = u8"Jerk";
  PROGMEM Language_Str JUNC_DEVIATION           = u8"Junc Dev";
  PROGMEM Language_Str BACKLASH                 = u8"Backlash";
  PROGMEM Language_Str SMOOTHING                = u8"Smoothing";
  PROGMEM Language_Str CORRECTION               = u8"Correction";
  PROGMEM Language_Str MOTOR_CURRENT            = u8"Currents";
  PROGMEM Language_Str FILAMENT                 = u8"Filament";
  PROGMEM Language_Str ENDSTOPS                 = u8"Endstops";
  PROGMEM Language_Str SOFT_ENDSTOPS            = u8"Soft Endstops";
  PROGMEM Language_Str RESTORE_DEFAULTS         = u8"Restore Defaults";

  PROGMEM Language_Str HOTEND                   = u8"Hot End";
  PROGMEM Language_Str HOTEND1                  = u8"Hot End 1";
  PROGMEM Language_Str HOTEND2                  = u8"Hot End 2";
  PROGMEM Language_Str HOTEND3                  = u8"Hot End 3";
  PROGMEM Language_Str HOTEND4                  = u8"Hot End 4";
  PROGMEM Language_Str BED                      = u8"Bed";
  PROGMEM Language_Str AXIS_X                   = u8"X";
  PROGMEM Language_Str AXIS_Y                   = u8"Y";
  PROGMEM Language_Str AXIS_Z                   = u8"Z";
  PROGMEM Language_Str AXIS_E                   = u8"E";
  PROGMEM Language_Str AXIS_E1                  = u8"E1";
  PROGMEM Language_Str AXIS_E2                  = u8"E2";
  PROGMEM Language_Str AXIS_E3                  = u8"E3";
  PROGMEM Language_Str AXIS_E4                  = u8"E4";
  PROGMEM Language_Str AXIS_ALL                 = u8"All";
  PROGMEM Language_Str HOME                     = u8"Home";

  PROGMEM Language_Str FAN_SPEED                = u8"Fan Speed";
  PROGMEM Language_Str RUNOUT_SENSOR            = u8"Runout Sensor";

  PROGMEM Language_Str OPEN_DIR                 = u8"Open";
  PROGMEM Language_Str PRINT_FILE               = u8"Print";

  PROGMEM Language_Str RESUME_PRINT             = u8"Resume Print";
  PROGMEM Language_Str PAUSE_PRINT              = u8"Pause Print";
  PROGMEM Language_Str STOP_PRINT               = u8"Stop Print";

  PROGMEM Language_Str PRINT_STARTING           = u8"Print starting";
  PROGMEM Language_Str PRINT_FINISHED           = u8"Print finished";
  PROGMEM Language_Str PRINT_ERROR              = u8"Print error";

  PROGMEM Language_Str INFO_PRINT_COUNT         = u8"Print Count";
  PROGMEM Language_Str INFO_COMPLETED_PRINTS    = u8"Total Prints";
  PROGMEM Language_Str INFO_PRINT_TIME          = u8"Total Print Time";
  PROGMEM Language_Str INFO_PRINT_LONGEST       = u8"Longest Print";
  PROGMEM Language_Str INFO_PRINT_FILAMENT      = u8"Filament Used";

  PROGMEM Language_Str PRINTER_HALTED           = u8"PRINTER HALTED";
  PROGMEM Language_Str PLEASE_RESET             = u8"Please reset";

  PROGMEM Language_Str COLOR_TOUCH_PANEL        = u8"Color Touch Panel";
  #if ENABLED(TOUCH_UI_UTF8_COPYRIGHT)
    PROGMEM Language_Str ABOUT_ALEPH_OBJECTS    = u8"© 2019 Aleph Objects, Inc.\n\nwww.lulzbot.com";
  #else
    PROGMEM Language_Str ABOUT_ALEPH_OBJECTS    = u8"(C) 2019 Aleph Objects, Inc.\n\nwww.lulzbot.com";
  #endif

  PROGMEM Language_Str FIRMWARE_FOR_TOOLHEAD    = u8"Firmware for toolhead:\n%s\n\n";

  PROGMEM Language_Str HOME_SENSE               = u8"Home Sense";
  PROGMEM Language_Str X_MAX                    = u8"X Max";
  PROGMEM Language_Str X_MIN                    = u8"X Min";
  PROGMEM Language_Str Y_MAX                    = u8"Y Max";
  PROGMEM Language_Str Y_MIN                    = u8"Y Min";
  PROGMEM Language_Str Z_MAX                    = u8"Z Max";
  PROGMEM Language_Str Z_MIN                    = u8"Z Min";
  PROGMEM Language_Str Z_PROBE                  = u8"Z Probe";
  PROGMEM Language_Str RUNOUT_1                 = u8"Runout 1";
  PROGMEM Language_Str RUNOUT_2                 = u8"Runout 2";
  PROGMEM Language_Str DISPLAY_MENU             = u8"Display";
  PROGMEM Language_Str INTERFACE_SETTINGS       = u8"Interface Settings";
  PROGMEM Language_Str MEASURE_AUTOMATICALLY    = u8"Measure automatically";
  PROGMEM Language_Str H_OFFSET                 = u8"H Offset";
  PROGMEM Language_Str V_OFFSET                 = u8"V Offset";
  PROGMEM Language_Str TOUCH_SCREEN             = u8"Touch Screen";
  PROGMEM Language_Str CALIBRATE                = u8"Calibrate";

  PROGMEM Language_Str UNITS_MILLIAMP           = u8"mA";
  PROGMEM Language_Str UNITS_MM                 = u8"mm";
  PROGMEM Language_Str UNITS_MM_S               = u8"mm/s";
  #if ENABLED(TOUCH_UI_UTF8_WESTERN_CHARSET)
    PROGMEM Language_Str UNITS_MM_S2            = u8"mm/s²";
  #else
    PROGMEM Language_Str UNITS_MM_S2            = u8"mm/s^2";
  #endif
  PROGMEM Language_Str UNITS_STEP_MM            = u8"st/mm";
  PROGMEM Language_Str UNITS_PERCENT            = u8"%";
  #if ENABLED(TOUCH_UI_UTF8_WESTERN_CHARSET)
    PROGMEM Language_Str UNITS_C                = u8"°C";
  #else
    PROGMEM Language_Str UNITS_C                = u8" C";
  #endif
  PROGMEM Language_Str MATERIAL_PLA             = u8"PLA";
  PROGMEM Language_Str MATERIAL_ABS             = u8"ABS";
  PROGMEM Language_Str MATERIAL_HIGH_TEMP       = u8"High";
  PROGMEM Language_Str TEMP_IDLE                = u8"idle";

  PROGMEM Language_Str PRINTING                 = u8"Printing";
  PROGMEM Language_Str SET_MAXIMUM              = u8"Set Maximum";
  PROGMEM Language_Str DETECTION_THRESHOLD      = u8"Detection Threshold";
  PROGMEM Language_Str DISTANCE                 = u8"Distance";
  PROGMEM Language_Str COOLDOWN                 = u8"Cooldown (All Off)";
  PROGMEM Language_Str PRINT_SPEED              = u8"Print Speed";
  PROGMEM Language_Str SPEED                    = u8"Speed";
  PROGMEM Language_Str LINEAR_ADVANCE           = u8"Linear Advance";
  PROGMEM Language_Str LINEAR_ADVANCE_K         = u8"K";
  PROGMEM Language_Str LINEAR_ADVANCE_K1        = u8"K E1";
  PROGMEM Language_Str LINEAR_ADVANCE_K2        = u8"K E2";
  PROGMEM Language_Str LINEAR_ADVANCE_K3        = u8"K E3";
  PROGMEM Language_Str LINEAR_ADVANCE_K4        = u8"K E4";
  PROGMEM Language_Str NUDGE_NOZZLE             = u8"Nudge Nozzle";
  PROGMEM Language_Str ADJUST_BOTH_NOZZLES      = u8"Adjust Both Nozzles";
  PROGMEM Language_Str SHOW_OFFSETS             = u8"Show Offsets";
  PROGMEM Language_Str INCREMENT                = u8"Increment";
  PROGMEM Language_Str ERASE_FLASH_WARNING      = u8"Are you sure? SPI flash will be erased.";
  PROGMEM Language_Str ERASING                  = u8"Erasing...";
  PROGMEM Language_Str ERASED                   = u8"SPI flash erased";
  PROGMEM Language_Str CALIBRATION_WARNING      = u8"For best results, unload the filament and clean the hotend prior to starting calibration. Continue?";
  PROGMEM Language_Str START_PRINT_CONFIRMATION = u8"Start printing %s?";
  PROGMEM Language_Str ABORT_WARNING            = u8"Are you sure you want to cancel the print?";
  PROGMEM Language_Str EXTRUDER_SELECTION       = u8"Extruder Selection";
  PROGMEM Language_Str CURRENT_TEMPERATURE      = u8"Current Temp";
  PROGMEM Language_Str REMOVAL_TEMPERATURE      = u8"Removal Temp";
  PROGMEM Language_Str HEATING                  = u8"Heating";
  PROGMEM Language_Str CAUTION                  = u8"Caution:";
  PROGMEM Language_Str HOT                      = u8"Hot!";
  PROGMEM Language_Str UNLOAD_FILAMENT          = u8"Unload";
  PROGMEM Language_Str LOAD_FILAMENT            = u8"Load/Extruder";
  PROGMEM Language_Str MOMENTARY                = u8"Momentary";
  PROGMEM Language_Str CONTINUOUS               = u8"Continuous";
  PROGMEM Language_Str PLEASE_WAIT              = u8"Please wait...";
  PROGMEM Language_Str PRINT_MENU               = u8"Print Menu";
  PROGMEM Language_Str FINE_MOTION              = u8"Fine motion";
  PROGMEM Language_Str ENABLE_MEDIA             = u8"Enable Media";
  PROGMEM Language_Str INSERT_MEDIA             = u8"Insert Media...";
  PROGMEM Language_Str LCD_BRIGHTNESS           = u8"LCD brightness";
  PROGMEM Language_Str SOUND_VOLUME             = u8"Sound volume";
  PROGMEM Language_Str SCREEN_LOCK              = u8"Screen lock";
  PROGMEM Language_Str BOOT_SCREEN              = u8"Boot screen";
  PROGMEM Language_Str INTERFACE_SOUNDS         = u8"Interface Sounds";
  PROGMEM Language_Str CLICK_SOUNDS             = u8"Click sounds";
  PROGMEM Language_Str EEPROM_RESTORED          = u8"Settings restored from backup";
  PROGMEM Language_Str EEPROM_RESET             = u8"Settings restored to default";
  PROGMEM Language_Str EEPROM_SAVED             = u8"Settings saved!";
  PROGMEM Language_Str EEPROM_SAVE_PROMPT       = u8"Do you wish to save these settings as defaults?";
  PROGMEM Language_Str EEPROM_RESET_WARNING     = u8"Are you sure? Customizations will be lost.";

  PROGMEM Language_Str PASSCODE_REJECTED        = u8"Wrong passcode!";
  PROGMEM Language_Str PASSCODE_ACCEPTED        = u8"Passcode accepted!";
  PROGMEM Language_Str PASSCODE_SELECT          = u8"Select Passcode:";
  PROGMEM Language_Str PASSCODE_REQUEST         = u8"Enter Passcode:";

  PROGMEM Language_Str TOUCH_CALIBRATION_START  = u8"Release to begin screen calibration";
  PROGMEM Language_Str TOUCH_CALIBRATION_PROMPT = u8"Touch the dots to calibrate";

  #ifdef LULZBOT_USE_BIOPRINTER_UI
    PROGMEM Language_Str MAIN_MENU              = u8"Main Menu";
    PROGMEM Language_Str MOVE_TO_HOME           = u8"Move To Home";
    PROGMEM Language_Str RAISE_PLUNGER          = u8"Raise Plunger";
    PROGMEM Language_Str RELEASE_XY_AXIS        = u8"Release X and Y Axis";
    PROGMEM Language_Str AUTOLEVEL_X_AXIS       = u8"Auto-level X Axis";
    PROGMEM Language_Str BED_TEMPERATURE        = u8"Bed Temperature";
    PROGMEM Language_Str HOME_XYZ_WARNING       = u8"About to move to home position. Ensure the top and the bed of the printer are clear.\n\nContinue?";
    PROGMEM Language_Str HOME_E_WARNING         = u8"About to re-home plunger and auto-level. Remove syringe prior to proceeding.\n\nContinue?";
  #endif
}; // namespace Language_en

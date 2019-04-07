/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * English
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define en 1234
#if LCD_LANGUAGE == en
  #define NOT_EXTENDED_ISO10646_1_5X7
#endif
#undef en

#ifndef THIS_LANGUAGES_SPECIAL_SYMBOLS
  #define THIS_LANGUAGES_SPECIAL_SYMBOLS      _UxGT("³")
#endif

#ifdef NOT_EXTENDED_ISO10646_1_5X7
  #define MSG_CUBED                           _UxGT("^3")
#else
  #define MSG_CUBED                           _UxGT("³")
#endif

#ifndef CHARSIZE
  #define CHARSIZE 1
#endif

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" Ready.")
#endif
#ifndef MSG_YES
  #define MSG_YES                             _UxGT("YES")
#endif
#ifndef MSG_NO
  #define MSG_NO                              _UxGT("NO")
#endif
#ifndef MSG_BACK
  #define MSG_BACK                            _UxGT("Back")
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     _UxGT("Card inserted")
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      _UxGT("Card removed")
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#endif
#ifndef MSG_LCD_SOFT_ENDSTOPS
  #define MSG_LCD_SOFT_ENDSTOPS               _UxGT("Soft Endstops")
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            _UxGT("Main")
#endif
#ifndef MSG_ADVANCED_SETTINGS
  #define MSG_ADVANCED_SETTINGS               _UxGT("Advanced Settings")
#endif
#ifndef MSG_CONFIGURATION
  #define MSG_CONFIGURATION                   _UxGT("Configuration")
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       _UxGT("Autostart")
#endif
#ifndef MSG_DISABLE_STEPPERS
  #define MSG_DISABLE_STEPPERS                _UxGT("Disable steppers")
#endif
#ifndef MSG_DEBUG_MENU
  #define MSG_DEBUG_MENU                      _UxGT("Debug Menu")
#endif
#ifndef MSG_PROGRESS_BAR_TEST
  #define MSG_PROGRESS_BAR_TEST               _UxGT("Progress Bar Test")
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME                       _UxGT("Auto home")
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X                     _UxGT("Home X")
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y                     _UxGT("Home Y")
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z                     _UxGT("Home Z")
#endif
#ifndef MSG_AUTO_Z_ALIGN
  #define MSG_AUTO_Z_ALIGN                    _UxGT("Auto Z-Align")
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Next Point")
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#endif
#ifndef MSG_Z_FADE_HEIGHT
  #define MSG_Z_FADE_HEIGHT                   _UxGT("Fade Height")
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS                _UxGT("Set home offsets")
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN                      _UxGT("Set origin")
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1                       _UxGT("Preheat " PREHEAT_1_LABEL)
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" All")
#endif
#ifndef MSG_PREHEAT_1_END
  #define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" End")
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Bed")
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" conf")
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2                       _UxGT("Preheat " PREHEAT_2_LABEL)
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" All")
#endif
#ifndef MSG_PREHEAT_2_END
  #define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" End")
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Bed")
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" conf")
#endif
#ifndef MSG_PREHEAT_CUSTOM
  #define MSG_PREHEAT_CUSTOM                  _UxGT("Preheat Custom")
#endif
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        _UxGT("Cooldown")
#endif
#ifndef MSG_LASER_MENU
  #define MSG_LASER_MENU                      _UxGT("Laser Control")
#endif
#ifndef MSG_LASER_OFF
  #define MSG_LASER_OFF                       _UxGT("Laser Off")
#endif
#ifndef MSG_LASER_ON
  #define MSG_LASER_ON                        _UxGT("Laser On")
#endif
#ifndef MSG_LASER_POWER
  #define MSG_LASER_POWER                     _UxGT("Laser power")
#endif
#ifndef MSG_SPINDLE_REVERSE
  #define MSG_SPINDLE_REVERSE                 _UxGT("Spindle Reverse")
#endif
#ifndef MSG_SWITCH_PS_ON
  #define MSG_SWITCH_PS_ON                    _UxGT("Switch power on")
#endif
#ifndef MSG_SWITCH_PS_OFF
  #define MSG_SWITCH_PS_OFF                   _UxGT("Switch power off")
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE                         _UxGT("Extrude")
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT                         _UxGT("Retract")
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS                       _UxGT("Move axis")
#endif
#ifndef MSG_BED_LEVELING
  #define MSG_BED_LEVELING                    _UxGT("Bed Leveling")
#endif
#ifndef MSG_LEVEL_BED
  #define MSG_LEVEL_BED                       _UxGT("Level bed")
#endif
#ifndef MSG_LEVEL_CORNERS
  #define MSG_LEVEL_CORNERS                   _UxGT("Level corners")
#endif
#ifndef MSG_NEXT_CORNER
  #define MSG_NEXT_CORNER                     _UxGT("Next corner")
#endif
#ifndef MSG_EDITING_STOPPED
  #define MSG_EDITING_STOPPED                 _UxGT("Mesh Editing Stopped")
#endif
#ifndef MSG_MESH_X
  #define MSG_MESH_X                          _UxGT("Index X")
#endif
#ifndef MSG_MESH_Y
  #define MSG_MESH_Y                          _UxGT("Index Y")
#endif
#ifndef MSG_MESH_EDIT_Z
  #define MSG_MESH_EDIT_Z                     _UxGT("Z Value")
#endif
#ifndef MSG_USER_MENU
  #define MSG_USER_MENU                       _UxGT("Custom Commands")
#endif
#ifndef MSG_UBL_DOING_G29
  #define MSG_UBL_DOING_G29                   _UxGT("Doing G29")
#endif
#ifndef MSG_UBL_UNHOMED
  #define MSG_UBL_UNHOMED                     _UxGT("Home XYZ first")
#endif
#ifndef MSG_UBL_TOOLS
  #define MSG_UBL_TOOLS                       _UxGT("UBL Tools")
#endif
#ifndef MSG_UBL_LEVEL_BED
  #define MSG_UBL_LEVEL_BED                   _UxGT("Unified Bed Leveling")
#endif
#ifndef MSG_IDEX_MENU
  #define MSG_IDEX_MENU                       _UxGT("IDEX Mode")
#endif
#ifndef MSG_IDEX_MODE_AUTOPARK
  #define MSG_IDEX_MODE_AUTOPARK              _UxGT("Auto-Park")
#endif
#ifndef MSG_IDEX_MODE_DUPLICATE
  #define MSG_IDEX_MODE_DUPLICATE             _UxGT("Duplication")
#endif
#ifndef MSG_IDEX_MODE_MIRRORED_COPY
  #define MSG_IDEX_MODE_MIRRORED_COPY         _UxGT("Mirrored copy")
#endif
#ifndef MSG_IDEX_MODE_FULL_CTRL
  #define MSG_IDEX_MODE_FULL_CTRL             _UxGT("Full control")
#endif
#ifndef MSG_IDEX_X_OFFSET
  #define MSG_IDEX_X_OFFSET                   _UxGT("2nd nozzle X")
#endif
#ifndef MSG_IDEX_Y_OFFSET
  #define MSG_IDEX_Y_OFFSET                   _UxGT("2nd nozzle Y")
#endif
#ifndef MSG_IDEX_Z_OFFSET
  #define MSG_IDEX_Z_OFFSET                   _UxGT("2nd nozzle Z")
#endif
#ifndef MSG_IDEX_SAVE_OFFSETS
  #define MSG_IDEX_SAVE_OFFSETS               _UxGT("Save Offsets")
#endif
#ifndef MSG_UBL_MANUAL_MESH
  #define MSG_UBL_MANUAL_MESH                 _UxGT("Manually Build Mesh")
#endif
#ifndef MSG_UBL_BC_INSERT
  #define MSG_UBL_BC_INSERT                   _UxGT("Place shim & measure")
#endif
#ifndef MSG_UBL_BC_INSERT2
  #define MSG_UBL_BC_INSERT2                  _UxGT("Measure")
#endif
#ifndef MSG_UBL_BC_REMOVE
  #define MSG_UBL_BC_REMOVE                   _UxGT("Remove & measure bed")
#endif
#ifndef MSG_UBL_MOVING_TO_NEXT
  #define MSG_UBL_MOVING_TO_NEXT              _UxGT("Moving to next")
#endif
#ifndef MSG_UBL_ACTIVATE_MESH
  #define MSG_UBL_ACTIVATE_MESH               _UxGT("Activate UBL")
#endif
#ifndef MSG_UBL_DEACTIVATE_MESH
  #define MSG_UBL_DEACTIVATE_MESH             _UxGT("Deactivate UBL")
#endif
#ifndef MSG_UBL_SET_TEMP_BED
  #define MSG_UBL_SET_TEMP_BED                _UxGT("Bed Temp")
#endif
#ifndef MSG_UBL_BED_TEMP_CUSTOM
  #define MSG_UBL_BED_TEMP_CUSTOM             MSG_UBL_SET_TEMP_BED
#endif
#ifndef MSG_UBL_SET_TEMP_HOTEND
  #define MSG_UBL_SET_TEMP_HOTEND             _UxGT("Hotend Temp")
#endif
#ifndef MSG_UBL_HOTEND_TEMP_CUSTOM
  #define MSG_UBL_HOTEND_TEMP_CUSTOM          MSG_UBL_SET_TEMP_HOTEND
#endif
#ifndef MSG_UBL_MESH_EDIT
  #define MSG_UBL_MESH_EDIT                   _UxGT("Mesh Edit")
#endif
#ifndef MSG_UBL_EDIT_CUSTOM_MESH
  #define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("Edit Custom Mesh")
#endif
#ifndef MSG_UBL_FINE_TUNE_MESH
  #define MSG_UBL_FINE_TUNE_MESH              _UxGT("Fine Tuning Mesh")
#endif
#ifndef MSG_UBL_DONE_EDITING_MESH
  #define MSG_UBL_DONE_EDITING_MESH           _UxGT("Done Editing Mesh")
#endif
#ifndef MSG_UBL_BUILD_CUSTOM_MESH
  #define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("Build Custom Mesh")
#endif
#ifndef MSG_UBL_BUILD_MESH_MENU
  #define MSG_UBL_BUILD_MESH_MENU             _UxGT("Build Mesh")
#endif
#ifndef MSG_UBL_BUILD_MESH_M1
  #define MSG_UBL_BUILD_MESH_M1               _UxGT("Build Mesh (" PREHEAT_1_LABEL ")")
#endif
#ifndef MSG_UBL_BUILD_MESH_M2
  #define MSG_UBL_BUILD_MESH_M2               _UxGT("Build Mesh (" PREHEAT_2_LABEL ")")
#endif
#ifndef MSG_UBL_BUILD_COLD_MESH
  #define MSG_UBL_BUILD_COLD_MESH             _UxGT("Build Cold Mesh")
#endif
#ifndef MSG_UBL_MESH_HEIGHT_ADJUST
  #define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("Adjust Mesh Height")
#endif
#ifndef MSG_UBL_MESH_HEIGHT_AMOUNT
  #define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("Height Amount")
#endif
#ifndef MSG_UBL_VALIDATE_MESH_MENU
  #define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("Validate Mesh")
#endif
#ifndef MSG_UBL_VALIDATE_MESH_M1
  #define MSG_UBL_VALIDATE_MESH_M1            _UxGT("Validate Mesh (" PREHEAT_1_LABEL ")")
#endif
#ifndef MSG_UBL_VALIDATE_MESH_M2
  #define MSG_UBL_VALIDATE_MESH_M2            _UxGT("Validate Mesh (" PREHEAT_2_LABEL ")")
#endif
#ifndef MSG_UBL_VALIDATE_CUSTOM_MESH
  #define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("Validate Custom Mesh")
#endif
#ifndef MSG_UBL_CONTINUE_MESH
  #define MSG_UBL_CONTINUE_MESH               _UxGT("Continue Bed Mesh")
#endif
#ifndef MSG_UBL_MESH_LEVELING
  #define MSG_UBL_MESH_LEVELING               _UxGT("Mesh Leveling")
#endif
#ifndef MSG_UBL_3POINT_MESH_LEVELING
  #define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("3-Point Leveling")
#endif
#ifndef MSG_UBL_GRID_MESH_LEVELING
  #define MSG_UBL_GRID_MESH_LEVELING          _UxGT("Grid Mesh Leveling")
#endif
#ifndef MSG_UBL_MESH_LEVEL
  #define MSG_UBL_MESH_LEVEL                  _UxGT("Level Mesh")
#endif
#ifndef MSG_UBL_SIDE_POINTS
  #define MSG_UBL_SIDE_POINTS                 _UxGT("Side Points")
#endif
#ifndef MSG_UBL_MAP_TYPE
  #define MSG_UBL_MAP_TYPE                    _UxGT("Map Type")
#endif
#ifndef MSG_UBL_OUTPUT_MAP
  #define MSG_UBL_OUTPUT_MAP                  _UxGT("Output Mesh Map")
#endif
#ifndef MSG_UBL_OUTPUT_MAP_HOST
  #define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("Output for Host")
#endif
#ifndef MSG_UBL_OUTPUT_MAP_CSV
  #define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("Output for CSV")
#endif
#ifndef MSG_UBL_OUTPUT_MAP_BACKUP
  #define MSG_UBL_OUTPUT_MAP_BACKUP           _UxGT("Off Printer Backup")
#endif
#ifndef MSG_UBL_INFO_UBL
  #define MSG_UBL_INFO_UBL                    _UxGT("Output UBL Info")
#endif
#ifndef MSG_EDIT_MESH
  #define MSG_EDIT_MESH                       _UxGT("Edit Mesh")
#endif
#ifndef MSG_UBL_FILLIN_AMOUNT
  #define MSG_UBL_FILLIN_AMOUNT               _UxGT("Fill-in Amount")
#endif
#ifndef MSG_UBL_MANUAL_FILLIN
  #define MSG_UBL_MANUAL_FILLIN               _UxGT("Manual Fill-in")
#endif
#ifndef MSG_UBL_SMART_FILLIN
  #define MSG_UBL_SMART_FILLIN                _UxGT("Smart Fill-in")
#endif
#ifndef MSG_UBL_FILLIN_MESH
  #define MSG_UBL_FILLIN_MESH                 _UxGT("Fill-in Mesh")
#endif
#ifndef MSG_UBL_INVALIDATE_ALL
  #define MSG_UBL_INVALIDATE_ALL              _UxGT("Invalidate All")
#endif
#ifndef MSG_UBL_INVALIDATE_CLOSEST
  #define MSG_UBL_INVALIDATE_CLOSEST          _UxGT("Invalidate Closest")
#endif
#ifndef MSG_UBL_FINE_TUNE_ALL
  #define MSG_UBL_FINE_TUNE_ALL               _UxGT("Fine Tune All")
#endif
#ifndef MSG_UBL_FINE_TUNE_CLOSEST
  #define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("Fine Tune Closest")
#endif
#ifndef MSG_UBL_STORAGE_MESH_MENU
  #define MSG_UBL_STORAGE_MESH_MENU           _UxGT("Mesh Storage")
#endif
#ifndef MSG_UBL_STORAGE_SLOT
  #define MSG_UBL_STORAGE_SLOT                _UxGT("Memory Slot")
#endif
#ifndef MSG_UBL_LOAD_MESH
  #define MSG_UBL_LOAD_MESH                   _UxGT("Load Bed Mesh")
#endif
#ifndef MSG_UBL_SAVE_MESH
  #define MSG_UBL_SAVE_MESH                   _UxGT("Save Bed Mesh")
#endif
#ifndef MSG_MESH_LOADED
  #define MSG_MESH_LOADED                     _UxGT("Mesh %i loaded")
#endif
#ifndef MSG_MESH_SAVED
  #define MSG_MESH_SAVED                      _UxGT("Mesh %i saved")
#endif
#ifndef MSG_NO_STORAGE
  #define MSG_NO_STORAGE                      _UxGT("No storage")
#endif
#ifndef MSG_UBL_SAVE_ERROR
  #define MSG_UBL_SAVE_ERROR                  _UxGT("Err: UBL Save")
#endif
#ifndef MSG_UBL_RESTORE_ERROR
  #define MSG_UBL_RESTORE_ERROR               _UxGT("Err: UBL Restore")
#endif
#ifndef MSG_UBL_Z_OFFSET_STOPPED
  #define MSG_UBL_Z_OFFSET_STOPPED            _UxGT("Z-Offset Stopped")
#endif
#ifndef MSG_UBL_STEP_BY_STEP_MENU
  #define MSG_UBL_STEP_BY_STEP_MENU           _UxGT("Step-By-Step UBL")
#endif

#ifndef MSG_LED_CONTROL
  #define MSG_LED_CONTROL                     _UxGT("LED Control")
#endif
#ifndef MSG_LEDS
  #define MSG_LEDS                            _UxGT("Lights")
#endif
#ifndef MSG_LED_PRESETS
  #define MSG_LED_PRESETS                     _UxGT("Light Presets")
#endif
#ifndef MSG_SET_LEDS_RED
  #define MSG_SET_LEDS_RED                    _UxGT("Red")
#endif
#ifndef MSG_SET_LEDS_ORANGE
  #define MSG_SET_LEDS_ORANGE                 _UxGT("Orange")
#endif
#ifndef MSG_SET_LEDS_YELLOW
  #define MSG_SET_LEDS_YELLOW                 _UxGT("Yellow")
#endif
#ifndef MSG_SET_LEDS_GREEN
  #define MSG_SET_LEDS_GREEN                  _UxGT("Green")
#endif
#ifndef MSG_SET_LEDS_BLUE
  #define MSG_SET_LEDS_BLUE                   _UxGT("Blue")
#endif
#ifndef MSG_SET_LEDS_INDIGO
  #define MSG_SET_LEDS_INDIGO                 _UxGT("Indigo")
#endif
#ifndef MSG_SET_LEDS_VIOLET
  #define MSG_SET_LEDS_VIOLET                 _UxGT("Violet")
#endif
#ifndef MSG_SET_LEDS_WHITE
  #define MSG_SET_LEDS_WHITE                  _UxGT("White")
#endif
#ifndef MSG_SET_LEDS_DEFAULT
  #define MSG_SET_LEDS_DEFAULT                _UxGT("Default")
#endif
#ifndef MSG_CUSTOM_LEDS
  #define MSG_CUSTOM_LEDS                     _UxGT("Custom Lights")
#endif
#ifndef MSG_INTENSITY_R
  #define MSG_INTENSITY_R                     _UxGT("Red Intensity")
#endif
#ifndef MSG_INTENSITY_G
  #define MSG_INTENSITY_G                     _UxGT("Green Intensity")
#endif
#ifndef MSG_INTENSITY_B
  #define MSG_INTENSITY_B                     _UxGT("Blue Intensity")
#endif
#ifndef MSG_INTENSITY_W
  #define MSG_INTENSITY_W                     _UxGT("White Intensity")
#endif
#ifndef MSG_LED_BRIGHTNESS
  #define MSG_LED_BRIGHTNESS                  _UxGT("Brightness")
#endif

#ifndef MSG_MOVING
  #define MSG_MOVING                          _UxGT("Moving...")
#endif
#ifndef MSG_FREE_XY
  #define MSG_FREE_XY                         _UxGT("Free XY")
#endif
#ifndef MSG_MOVE_X
  #define MSG_MOVE_X                          _UxGT("Move X")
#endif
#ifndef MSG_MOVE_Y
  #define MSG_MOVE_Y                          _UxGT("Move Y")
#endif
#ifndef MSG_MOVE_Z
  #define MSG_MOVE_Z                          _UxGT("Move Z")
#endif
#ifndef MSG_MOVE_E
  #define MSG_MOVE_E                          _UxGT("Extruder")
#endif
#ifndef MSG_HOTEND_TOO_COLD
  #define MSG_HOTEND_TOO_COLD                 _UxGT("Hotend too cold")
#endif
#ifndef MSG_MOVE_01MM
  #define MSG_MOVE_01MM                       _UxGT("Move 0.1mm")
#endif
#ifndef MSG_MOVE_1MM
  #define MSG_MOVE_1MM                        _UxGT("Move 1mm")
#endif
#ifndef MSG_MOVE_10MM
  #define MSG_MOVE_10MM                       _UxGT("Move 10mm")
#endif
#ifndef MSG_SPEED
  #define MSG_SPEED                           _UxGT("Speed")
#endif
#ifndef MSG_BED_Z
  #define MSG_BED_Z                           _UxGT("Bed Z")
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE                          _UxGT("Nozzle")
#endif
#ifndef MSG_BED
  #define MSG_BED                             _UxGT("Bed")
#endif
#ifndef MSG_CHAMBER
  #define MSG_CHAMBER                         _UxGT("Enclosure")
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       _UxGT("Fan speed")
#endif
#ifndef MSG_EXTRA_FAN_SPEED
  #define MSG_EXTRA_FAN_SPEED                 _UxGT("Extra fan speed")
#endif
#ifndef MSG_FLOW
  #define MSG_FLOW                            _UxGT("Flow")
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL                         _UxGT("Control")
#endif
#ifndef MSG_MIN
  #define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#endif
#ifndef MSG_MAX
  #define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#endif
#ifndef MSG_FACTOR
  #define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#endif
#ifndef MSG_AUTOTEMP
  #define MSG_AUTOTEMP                        _UxGT("Autotemp")
#endif
#ifndef MSG_LCD_ON
  #define MSG_LCD_ON                          _UxGT("On")
#endif
#ifndef MSG_LCD_OFF
  #define MSG_LCD_OFF                         _UxGT("Off")
#endif
#ifndef MSG_PID_P
  #define MSG_PID_P                           _UxGT("PID-P")
#endif
#ifndef MSG_PID_I
  #define MSG_PID_I                           _UxGT("PID-I")
#endif
#ifndef MSG_PID_D
  #define MSG_PID_D                           _UxGT("PID-D")
#endif
#ifndef MSG_PID_C
  #define MSG_PID_C                           _UxGT("PID-C")
#endif
#ifndef MSG_SELECT
  #define MSG_SELECT                          _UxGT("Select")
#endif
#ifndef MSG_ACC
  #define MSG_ACC                             _UxGT("Accel")
#endif
#ifndef MSG_JERK
  #define MSG_JERK                            _UxGT("Jerk")
#endif
#if IS_KINEMATIC
  #ifndef MSG_VA_JERK
    #define MSG_VA_JERK                       _UxGT("Va-jerk")
  #endif
  #ifndef MSG_VB_JERK
    #define MSG_VB_JERK                       _UxGT("Vb-jerk")
  #endif
  #ifndef MSG_VC_JERK
    #define MSG_VC_JERK                       _UxGT("Vc-jerk")
  #endif
#else
  #ifndef MSG_VA_JERK
    #define MSG_VA_JERK                       _UxGT("Vx-jerk")
  #endif
  #ifndef MSG_VB_JERK
    #define MSG_VB_JERK                       _UxGT("Vy-jerk")
  #endif
  #ifndef MSG_VC_JERK
    #define MSG_VC_JERK                       _UxGT("Vz-jerk")
  #endif
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         _UxGT("Ve-jerk")
#endif
#ifndef MSG_JUNCTION_DEVIATION
  #define MSG_JUNCTION_DEVIATION              _UxGT("Junction Dev")
#endif
#ifndef MSG_VELOCITY
  #define MSG_VELOCITY                        _UxGT("Velocity")
#endif
#ifndef MSG_VMAX
  #define MSG_VMAX                            _UxGT("Vmax ")
#endif
#ifndef MSG_VMIN
  #define MSG_VMIN                            _UxGT("Vmin")
#endif
#ifndef MSG_VTRAV_MIN
  #define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#endif
#ifndef MSG_ACCELERATION
  #define MSG_ACCELERATION                    _UxGT("Acceleration")
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX                            _UxGT("Amax ")
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT                       _UxGT("A-retract")
#endif
#ifndef MSG_A_TRAVEL
  #define MSG_A_TRAVEL                        _UxGT("A-travel")
#endif
#ifndef MSG_STEPS_PER_MM
  #define MSG_STEPS_PER_MM                    _UxGT("Steps/mm")
#endif
#if IS_KINEMATIC
  #ifndef MSG_ASTEPS
    #define MSG_ASTEPS                        _UxGT("Asteps/mm")
  #endif
  #ifndef MSG_BSTEPS
    #define MSG_BSTEPS                        _UxGT("Bsteps/mm")
  #endif
  #ifndef MSG_CSTEPS
    #define MSG_CSTEPS                        _UxGT("Csteps/mm")
  #endif
#else
  #ifndef MSG_ASTEPS
    #define MSG_ASTEPS                        _UxGT("Xsteps/mm")
  #endif
  #ifndef MSG_BSTEPS
    #define MSG_BSTEPS                        _UxGT("Ysteps/mm")
  #endif
  #ifndef MSG_CSTEPS
    #define MSG_CSTEPS                        _UxGT("Zsteps/mm")
  #endif
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS                          _UxGT("Esteps/mm")
#endif
#ifndef MSG_E1STEPS
  #define MSG_E1STEPS                         _UxGT("E1steps/mm")
#endif
#ifndef MSG_E2STEPS
  #define MSG_E2STEPS                         _UxGT("E2steps/mm")
#endif
#ifndef MSG_E3STEPS
  #define MSG_E3STEPS                         _UxGT("E3steps/mm")
#endif
#ifndef MSG_E4STEPS
  #define MSG_E4STEPS                         _UxGT("E4steps/mm")
#endif
#ifndef MSG_E5STEPS
  #define MSG_E5STEPS                         _UxGT("E5steps/mm")
#endif
#ifndef MSG_E6STEPS
  #define MSG_E6STEPS                         _UxGT("E6steps/mm")
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE                     _UxGT("Temperature")
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION                          _UxGT("Motion")
#endif
#ifndef MSG_FILAMENT
  #define MSG_FILAMENT                        _UxGT("Filament")
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm") MSG_CUBED
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#endif
#ifndef MSG_FILAMENT_UNLOAD
  #define MSG_FILAMENT_UNLOAD                 _UxGT("Unload mm")
#endif
#ifndef MSG_FILAMENT_LOAD
  #define MSG_FILAMENT_LOAD                   _UxGT("Load mm")
#endif
#ifndef MSG_ADVANCE_K
  #define MSG_ADVANCE_K                       _UxGT("Advance K")
#endif
#ifndef MSG_CONTRAST
  #define MSG_CONTRAST                        _UxGT("LCD contrast")
#endif
#ifndef MSG_STORE_EEPROM
  #define MSG_STORE_EEPROM                    _UxGT("Store settings")
#endif
#ifndef MSG_LOAD_EEPROM
  #define MSG_LOAD_EEPROM                     _UxGT("Load settings")
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE                _UxGT("Restore failsafe")
#endif
#ifndef MSG_INIT_EEPROM
  #define MSG_INIT_EEPROM                     _UxGT("Initialize EEPROM")
#endif
#ifndef MSG_SD_UPDATE
  #define MSG_SD_UPDATE                       _UxGT("SD Update")
#endif
#ifndef MSG_RESET_PRINTER
  #define MSG_RESET_PRINTER                   _UxGT("Reset Printer")
#endif
#ifndef MSG_REFRESH
  #define MSG_REFRESH                         _UxGT("Refresh")
#endif
#ifndef MSG_WATCH
  #define MSG_WATCH                           _UxGT("Info screen")
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE                         _UxGT("Prepare")
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE                            _UxGT("Tune")
#endif
#ifndef MSG_START_PRINT
  #define MSG_START_PRINT                     _UxGT("Start print")
#endif
#ifndef MSG_BUTTON_PRINT
  #define MSG_BUTTON_PRINT                    _UxGT("Print")
#endif
#ifndef MSG_BUTTON_CANCEL
  #define MSG_BUTTON_CANCEL                   _UxGT("Cancel")
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     _UxGT("Pause print")
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    _UxGT("Resume print")
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      _UxGT("Stop print")
#endif
#ifndef MSG_OUTAGE_RECOVERY
  #define MSG_OUTAGE_RECOVERY                 _UxGT("Outage Recovery")
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU                       _UxGT("Print from SD")
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD                         _UxGT("No SD card")
#endif
#ifndef MSG_DWELL
  #define MSG_DWELL                           _UxGT("Sleep...")
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT                        _UxGT("Click to resume...")
#endif
#ifndef MSG_PRINT_PAUSED
  #define MSG_PRINT_PAUSED                    _UxGT("Print paused")
#endif
#ifndef MSG_PRINTING
  #define MSG_PRINTING                        _UxGT("Printing...")
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED                   _UxGT("Print aborted")
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE                         _UxGT("No move.")
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED                          _UxGT("KILLED. ")
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED                         _UxGT("STOPPED. ")
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT                 _UxGT("Retract mm")
#endif
#ifndef MSG_CONTROL_RETRACT_SWAP
  #define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Re.mm")
#endif
#ifndef MSG_CONTROL_RETRACTF
  #define MSG_CONTROL_RETRACTF                _UxGT("Retract  V")
#endif
#ifndef MSG_CONTROL_RETRACT_ZHOP
  #define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Hop mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet V")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAPF
  #define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("S UnRet V")
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#endif
#ifndef MSG_FILAMENT_SWAP_LENGTH
  #define MSG_FILAMENT_SWAP_LENGTH            _UxGT("Retract Distance")
#endif
#ifndef MSG_TOOL_CHANGE
  #define MSG_TOOL_CHANGE                      _UxGT("Tool Change")
#endif
#ifndef MSG_TOOL_CHANGE_ZLIFT
  #define MSG_TOOL_CHANGE_ZLIFT                _UxGT("Z Raise")
#endif
#ifndef MSG_SINGLENOZZLE_PRIME_SPD
  #define MSG_SINGLENOZZLE_PRIME_SPD          _UxGT("Prime Speed")
#endif
#ifndef MSG_SINGLENOZZLE_RETRACT_SPD
  #define MSG_SINGLENOZZLE_RETRACT_SPD        _UxGT("Retract Speed")
#endif
#ifndef MSG_NOZZLE_STANDBY
  #define MSG_NOZZLE_STANDBY                  _UxGT("Nozzle Standby")
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  _UxGT("Change filament")
#endif
#ifndef MSG_FILAMENTLOAD
  #define MSG_FILAMENTLOAD                    _UxGT("Load filament")
#endif
#ifndef MSG_FILAMENTUNLOAD
  #define MSG_FILAMENTUNLOAD                  _UxGT("Unload filament")
#endif
#ifndef MSG_FILAMENTUNLOAD_ALL
  #define MSG_FILAMENTUNLOAD_ALL              _UxGT("Unload All")
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD                     _UxGT("Init. SD card")
#endif
#ifndef MSG_CHANGE_SDCARD
  #define MSG_CHANGE_SDCARD                   _UxGT("Change SD card")
#endif
#ifndef MSG_ZPROBE_OUT
  #define MSG_ZPROBE_OUT                      _UxGT("Z Probe past bed")
#endif
#ifndef MSG_SKEW_FACTOR
  #define MSG_SKEW_FACTOR                     _UxGT("Skew Factor")
#endif
#ifndef MSG_BLTOUCH
  #define MSG_BLTOUCH                         _UxGT("BLTouch")
#endif
#ifndef MSG_BLTOUCH_SELFTEST
  #define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#endif
#ifndef MSG_BLTOUCH_RESET
  #define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#endif
#ifndef MSG_BLTOUCH_DEPLOY
  #define MSG_BLTOUCH_DEPLOY                  _UxGT("Deploy BLTouch")
#endif
#ifndef MSG_BLTOUCH_SW_MODE
  #define MSG_BLTOUCH_SW_MODE                 _UxGT("SW Deploy BLTouch")
#endif
#ifndef MSG_BLTOUCH_5V_MODE
  #define MSG_BLTOUCH_5V_MODE                 _UxGT("BLTouch 5V Mode")
#endif
#ifndef MSG_BLTOUCH_OD_MODE
  #define MSG_BLTOUCH_OD_MODE                 _UxGT("BLTouch OD Mode")
#endif
#ifndef MSG_BLTOUCH_STOW
  #define MSG_BLTOUCH_STOW                    _UxGT("Stow BLTouch")
#endif
#ifndef MSG_MANUAL_DEPLOY
  #define MSG_MANUAL_DEPLOY                   _UxGT("Deploy Z-Probe")
#endif
#ifndef MSG_MANUAL_STOW
  #define MSG_MANUAL_STOW                     _UxGT("Stow Z-Probe")
#endif
#ifndef MSG_HOME
  #define MSG_HOME                            _UxGT("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST                           _UxGT("first")
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET                  _UxGT("Probe Z Offset")
#endif
#ifndef MSG_BABYSTEP_X
  #define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#endif
#ifndef MSG_BABYSTEP_Y
  #define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#endif
#ifndef MSG_BABYSTEP_Z
  #define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#endif
#ifndef MSG_BABYSTEP_TOTAL
  #define MSG_BABYSTEP_TOTAL                  _UxGT("Total")
#endif
#ifndef MSG_ENDSTOP_ABORT
  #define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD              _UxGT("Heating failed")
#endif
#ifndef MSG_HEATING_FAILED_LCD_BED
  #define MSG_HEATING_FAILED_LCD_BED          _UxGT("Bed heating failed")
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#endif
#ifndef MSG_THERMAL_RUNAWAY_BED
  #define MSG_THERMAL_RUNAWAY_BED             _UxGT("BED THERMAL RUNAWAY")
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP                     _UxGT("Err: MAXTEMP")
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP                     _UxGT("Err: MINTEMP")
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: MAXTEMP BED")
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #define MSG_ERR_MINTEMP_BED                 _UxGT("Err: MINTEMP BED")
#endif
#ifndef MSG_ERR_MAXTEMP_CHAMBER
  #define MSG_ERR_MAXTEMP_CHAMBER             _UxGT("Err: MAXTEMP CHAMBER")
#endif
#ifndef MSG_ERR_MINTEMP_CHAMBER
  #define MSG_ERR_MINTEMP_CHAMBER             _UxGT("Err: MINTEMP CHAMBER")
#endif
#ifndef MSG_ERR_Z_HOMING
  #define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#endif
#ifndef MSG_HALTED
  #define MSG_HALTED                          _UxGT("PRINTER HALTED")
#endif
#ifndef MSG_PLEASE_RESET
  #define MSG_PLEASE_RESET                    _UxGT("Please reset")
#endif
#ifndef MSG_SHORT_DAY
  #define MSG_SHORT_DAY                       _UxGT("d") // One character only
#endif
#ifndef MSG_SHORT_HOUR
  #define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING                         _UxGT("Heating...")
#endif
#ifndef MSG_COOLING
  #define MSG_COOLING                         _UxGT("Cooling...")
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     _UxGT("Bed heating...")
#endif
#ifndef MSG_BED_COOLING
  #define MSG_BED_COOLING                     _UxGT("Bed cooling...")
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE                 _UxGT("Delta Calibration")
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrate X")
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrate Y")
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrate Z")
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrate Center")
#endif
#ifndef MSG_DELTA_SETTINGS
  #define MSG_DELTA_SETTINGS                  _UxGT("Delta Settings")
#endif
#ifndef MSG_DELTA_AUTO_CALIBRATE
  #define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto Calibration")
#endif
#ifndef MSG_DELTA_HEIGHT_CALIBRATE
  #define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Set Delta Height")
#endif
#ifndef MSG_DELTA_Z_OFFSET_CALIBRATE
  #define MSG_DELTA_Z_OFFSET_CALIBRATE        _UxGT("Probe Z-offset")
#endif
#ifndef MSG_DELTA_DIAG_ROD
  #define MSG_DELTA_DIAG_ROD                  _UxGT("Diag Rod")
#endif
#ifndef MSG_DELTA_HEIGHT
  #define MSG_DELTA_HEIGHT                    _UxGT("Height")
#endif
#ifndef MSG_DELTA_RADIUS
  #define MSG_DELTA_RADIUS                    _UxGT("Radius")
#endif
#ifndef MSG_INFO_MENU
  #define MSG_INFO_MENU                       _UxGT("About Printer")
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#endif
#ifndef MSG_3POINT_LEVELING
  #define MSG_3POINT_LEVELING                 _UxGT("3-Point Leveling")
#endif
#ifndef MSG_LINEAR_LEVELING
  #define MSG_LINEAR_LEVELING                 _UxGT("Linear Leveling")
#endif
#ifndef MSG_BILINEAR_LEVELING
  #define MSG_BILINEAR_LEVELING               _UxGT("Bilinear Leveling")
#endif
#ifndef MSG_UBL_LEVELING
  #define MSG_UBL_LEVELING                    _UxGT("Unified Bed Leveling")
#endif
#ifndef MSG_MESH_LEVELING
  #define MSG_MESH_LEVELING                   _UxGT("Mesh Leveling")
#endif
#ifndef MSG_INFO_STATS_MENU
  #define MSG_INFO_STATS_MENU                 _UxGT("Printer Stats")
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU                 _UxGT("Board Info")
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#endif
#ifndef MSG_INFO_BAUDRATE
  #define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#endif
#ifndef MSG_INFO_PROTOCOL
  #define MSG_INFO_PROTOCOL                   _UxGT("Protocol")
#endif
#ifndef MSG_CASE_LIGHT
  #define MSG_CASE_LIGHT                      _UxGT("Case light")
#endif
#ifndef MSG_CASE_LIGHT_BRIGHTNESS
  #define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("Light Brightness")
#endif
#if LCD_WIDTH >= 20
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Print Count")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Total print time")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Longest job time")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruded total")
  #endif
#else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Prints")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Longest")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruded")
  #endif
#endif
#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU                        _UxGT("PSU")
#endif
#ifndef MSG_DRIVE_STRENGTH
  #define MSG_DRIVE_STRENGTH                  _UxGT("Drive Strength")
#endif
#ifndef MSG_DAC_PERCENT
  #define MSG_DAC_PERCENT                     _UxGT("Driver %")
#endif
#ifndef MSG_DAC_EEPROM_WRITE
  #define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM Write")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER_PAUSE
  #define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("PRINT PAUSED")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER_LOAD
  #define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("LOAD FILAMENT")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER_UNLOAD
  #define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("UNLOAD FILAMENT")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("RESUME OPTIONS:")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_PURGE
  #define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("Purge more")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Continue")
#endif
#ifndef MSG_FILAMENT_CHANGE_NOZZLE
  #define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Nozzle: ")
#endif
#ifndef MSG_RUNOUT_SENSOR
  #define MSG_RUNOUT_SENSOR                   _UxGT("Runout Sensor")
#endif
#ifndef MSG_ERR_HOMING_FAILED
  #define MSG_ERR_HOMING_FAILED               _UxGT("Homing failed")
#endif
#ifndef MSG_ERR_PROBING_FAILED
  #define MSG_ERR_PROBING_FAILED              _UxGT("Probing failed")
#endif
#ifndef MSG_M600_TOO_COLD
  #define MSG_M600_TOO_COLD                   _UxGT("M600: Too cold")
#endif

#ifndef MSG_MMU2_FILAMENT_CHANGE_HEADER
  #define MSG_MMU2_FILAMENT_CHANGE_HEADER     _UxGT("FILAMENT CHANGE")
#endif
#ifndef MSG_MMU2_CHOOSE_FILAMENT_HEADER
  #define MSG_MMU2_CHOOSE_FILAMENT_HEADER     _UxGT("CHOOSE FILAMENT")
#endif
#ifndef MSG_MMU2_MENU
  #define MSG_MMU2_MENU                       _UxGT("MMU")
#endif
#ifndef MSG_MMU2_WRONG_FIRMWARE
  #define MSG_MMU2_WRONG_FIRMWARE             _UxGT("Update MMU firmware!")
#endif
#ifndef MSG_MMU2_NOT_RESPONDING
  #define MSG_MMU2_NOT_RESPONDING             _UxGT("MMU needs attention.")
#endif
#ifndef MSG_MMU2_RESUME
  #define MSG_MMU2_RESUME                     _UxGT("Resume print")
#endif
#ifndef MSG_MMU2_RESUMING
  #define MSG_MMU2_RESUMING                   _UxGT("Resuming...")
#endif
#ifndef MSG_MMU2_LOAD_FILAMENT
  #define MSG_MMU2_LOAD_FILAMENT              _UxGT("Load filament")
#endif
#ifndef MSG_MMU2_LOAD_ALL
  #define MSG_MMU2_LOAD_ALL                   _UxGT("Load all")
#endif
#ifndef MSG_MMU2_LOAD_TO_NOZZLE
  #define MSG_MMU2_LOAD_TO_NOZZLE             _UxGT("Load to nozzle")
#endif
#ifndef MSG_MMU2_EJECT_FILAMENT
  #define MSG_MMU2_EJECT_FILAMENT             _UxGT("Eject filament")
#endif
#ifndef MSG_MMU2_EJECT_FILAMENT0
  #define MSG_MMU2_EJECT_FILAMENT0            _UxGT("Eject filament 1")
#endif
#ifndef MSG_MMU2_EJECT_FILAMENT1
  #define MSG_MMU2_EJECT_FILAMENT1            _UxGT("Eject filament 2")
#endif
#ifndef MSG_MMU2_EJECT_FILAMENT2
  #define MSG_MMU2_EJECT_FILAMENT2            _UxGT("Eject filament 3")
#endif
#ifndef MSG_MMU2_EJECT_FILAMENT3
  #define MSG_MMU2_EJECT_FILAMENT3            _UxGT("Eject filament 4")
#endif
#ifndef MSG_MMU2_EJECT_FILAMENT4
  #define MSG_MMU2_EJECT_FILAMENT4            _UxGT("Eject filament 5")
#endif
#ifndef MSG_MMU2_UNLOAD_FILAMENT
  #define MSG_MMU2_UNLOAD_FILAMENT            _UxGT("Unload filament")
#endif
#ifndef MSG_MMU2_LOADING_FILAMENT
  #define MSG_MMU2_LOADING_FILAMENT           _UxGT("Loading fil. %i...")
#endif
#ifndef MSG_MMU2_EJECTING_FILAMENT
  #define MSG_MMU2_EJECTING_FILAMENT          _UxGT("Ejecting fil. ...")
#endif
#ifndef MSG_MMU2_UNLOADING_FILAMENT
  #define MSG_MMU2_UNLOADING_FILAMENT         _UxGT("Unloading fil....")
#endif
#ifndef MSG_MMU2_ALL
  #define MSG_MMU2_ALL                        _UxGT("All")
#endif
#ifndef MSG_MMU2_FILAMENT0
  #define MSG_MMU2_FILAMENT0                  _UxGT("Filament 1")
#endif
#ifndef MSG_MMU2_FILAMENT1
  #define MSG_MMU2_FILAMENT1                  _UxGT("Filament 2")
#endif
#ifndef MSG_MMU2_FILAMENT2
  #define MSG_MMU2_FILAMENT2                  _UxGT("Filament 3")
#endif
#ifndef MSG_MMU2_FILAMENT3
  #define MSG_MMU2_FILAMENT3                  _UxGT("Filament 4")
#endif
#ifndef MSG_MMU2_FILAMENT4
  #define MSG_MMU2_FILAMENT4                  _UxGT("Filament 5")
#endif
#ifndef MSG_MMU2_RESET
  #define MSG_MMU2_RESET                      _UxGT("Reset MMU")
#endif
#ifndef MSG_MMU2_RESETTING
  #define MSG_MMU2_RESETTING                  _UxGT("Resetting MMU...")
#endif
#ifndef MSG_MMU2_EJECT_RECOVER
  #define MSG_MMU2_EJECT_RECOVER              _UxGT("Remove, click")
#endif

#ifndef MSG_MIX
  #define MSG_MIX                             _UxGT("Mix")
#endif
#ifndef MSG_MIX_COMPONENT
  #define MSG_MIX_COMPONENT                   _UxGT("Component")
#endif
#ifndef MSG_MIXER
  #define MSG_MIXER                           _UxGT("Mixer")
#endif
#ifndef MSG_GRADIENT
  #define MSG_GRADIENT                        _UxGT("Gradient")
#endif
#ifndef MSG_FULL_GRADIENT
  #define MSG_FULL_GRADIENT                   _UxGT("Full gradient")
#endif
#ifndef MSG_TOGGLE_MIX
  #define MSG_TOGGLE_MIX                      _UxGT("Toggle mix")
#endif
#ifndef MSG_CYCLE_MIX
  #define MSG_CYCLE_MIX                       _UxGT("Cycle mix")
#endif
#ifndef MSG_GRADIENT_MIX
  #define MSG_GRADIENT_MIX                    _UxGT("Gradient mix")
#endif
#ifndef MSG_REVERSE_GRADIENT
  #define MSG_REVERSE_GRADIENT                _UxGT("Reverse Gradient")
#endif
#ifndef MSG_ACTIVE_VTOOL
  #define MSG_ACTIVE_VTOOL                    _UxGT("Active V-tool")
#endif
#ifndef MSG_START_VTOOL
  #define MSG_START_VTOOL                     _UxGT("Start V-tool")
#endif
#ifndef MSG_END_VTOOL
  #define MSG_END_VTOOL                       _UxGT("  End V-tool")
#endif
#ifndef MSG_GRADIENT_ALIAS
  #define MSG_GRADIENT_ALIAS                  _UxGT("Alias V-tool")
#endif
#ifndef MSG_RESET_VTOOLS
  #define MSG_RESET_VTOOLS                    _UxGT("Reset V-tools")
#endif
#ifndef MSG_COMMIT_VTOOL
  #define MSG_COMMIT_VTOOL                    _UxGT("Commit V-tool Mix")
#endif
#ifndef MSG_VTOOLS_RESET
  #define MSG_VTOOLS_RESET                    _UxGT("V-tools were reset")
#endif
#ifndef MSG_START_Z
  #define MSG_START_Z                         _UxGT("Start Z")
#endif
#ifndef MSG_END_Z
  #define MSG_END_Z                           _UxGT("  End Z")
#endif
#ifndef MSG_BRICKOUT
  #define MSG_BRICKOUT                        _UxGT("Brickout")
#endif
#ifndef MSG_INVADERS
  #define MSG_INVADERS                        _UxGT("Invaders")
#endif
#ifndef MSG_SNAKE
  #define MSG_SNAKE                           _UxGT("Sn4k3")
#endif
#ifndef MSG_MAZE
  #define MSG_MAZE                            _UxGT("Maze")
#endif

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #ifndef MSG_ADVANCED_PAUSE_WAITING_1
    #define MSG_ADVANCED_PAUSE_WAITING_1      _UxGT("Press button")
    #define MSG_ADVANCED_PAUSE_WAITING_2      _UxGT("to resume print")
  #endif
  #ifndef MSG_PAUSE_PRINT_INIT_1
    #define MSG_PAUSE_PRINT_INIT_1            _UxGT("Parking...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_INIT_2        _UxGT("filament change")
    #define MSG_FILAMENT_CHANGE_INIT_3        _UxGT("to start")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("Insert filament")
    #define MSG_FILAMENT_CHANGE_INSERT_2      _UxGT("and press button")
    #define MSG_FILAMENT_CHANGE_INSERT_3      _UxGT("to continue")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEAT_1
    #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("Press button")
    #define MSG_FILAMENT_CHANGE_HEAT_2        _UxGT("to heat nozzle")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("Nozzle heating")
    #define MSG_FILAMENT_CHANGE_HEATING_2     _UxGT("Please wait...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_UNLOAD_2      _UxGT("filament unload")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_LOAD_2        _UxGT("filament load")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_PURGE_1
    #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_PURGE_2       _UxGT("filament purge")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_CONT_PURGE_1
    #define MSG_FILAMENT_CHANGE_CONT_PURGE_1  _UxGT("Click to finish")
    #define MSG_FILAMENT_CHANGE_CONT_PURGE_2  _UxGT("filament purge")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("Wait for print")
    #define MSG_FILAMENT_CHANGE_RESUME_2      _UxGT("to resume...")
  #endif
#else // LCD_HEIGHT < 4
  #ifndef MSG_ADVANCED_PAUSE_WAITING_1
    #define MSG_ADVANCED_PAUSE_WAITING_1      _UxGT("Click to continue")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("Please wait...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("Insert and Click")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEAT_1
    #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("Click to heat")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("Heating...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("Ejecting...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("Loading...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_PURGE_1
    #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("Purging...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_CONT_PURGE_1
    #define MSG_FILAMENT_CHANGE_CONT_PURGE_1  _UxGT("Click to finish")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("Resuming...")
  #endif
#endif // LCD_HEIGHT < 4

#ifndef MSG_TMC_DRIVERS
  #define MSG_TMC_DRIVERS                     _UxGT("TMC drivers")
#endif
#ifndef MSG_TMC_CURRENT
  #define MSG_TMC_CURRENT                     _UxGT("Driver current")
#endif
#ifndef MSG_TMC_HYBRID_THRS
  #define MSG_TMC_HYBRID_THRS                 _UxGT("Hybrid threshold")
#endif
#ifndef MSG_TMC_HOMING_THRS
  #define MSG_TMC_HOMING_THRS                 _UxGT("Sensorless homing")
#endif
#ifndef MSG_TMC_STEPPING_MODE
  #define MSG_TMC_STEPPING_MODE               _UxGT("Stepping mode")
#endif
#ifndef MSG_TMC_STEALTH_ENABLED
  #define MSG_TMC_STEALTH_ENABLED             _UxGT("StealthChop enabled")
#endif

#ifndef MSG_SERVICE_RESET
  #define MSG_SERVICE_RESET                   _UxGT("Reset")
#endif
#ifndef MSG_SERVICE_IN
  #define MSG_SERVICE_IN                      _UxGT(" in:")
#endif

#ifndef MSG_BACKLASH
  #define MSG_BACKLASH                        _UxGT("Backlash")
#endif
#ifndef MSG_BACKLASH_CORRECTION
  #define MSG_BACKLASH_CORRECTION             _UxGT("Correction")
#endif
#ifndef MSG_BACKLASH_SMOOTHING
  #define MSG_BACKLASH_SMOOTHING              _UxGT("Smoothing")
#endif

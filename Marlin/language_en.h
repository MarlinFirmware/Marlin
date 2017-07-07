/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * English
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_EN_H
#define LANGUAGE_EN_H

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" ready.")
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
#ifndef MSG_MAIN
  #define MSG_MAIN                            _UxGT("Main")
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
  #define MSG_PREHEAT_1                       _UxGT("Preheat PLA")
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
  #define MSG_PREHEAT_2                       _UxGT("Preheat ABS")
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
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        _UxGT("Cooldown")
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
#ifndef MSG_EDITING_STOPPED
  #define MSG_EDITING_STOPPED                 _UxGT("Mesh Editing Stopped")
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
#ifndef MSG_UBL_SET_BED_TEMP
  #define MSG_UBL_SET_BED_TEMP                _UxGT("Bed Temp")
#endif
#ifndef MSG_UBL_CUSTOM_BED_TEMP
  #define MSG_UBL_CUSTOM_BED_TEMP             MSG_UBL_SET_BED_TEMP
#endif
#ifndef MSG_UBL_SET_HOTEND_TEMP
  #define MSG_UBL_SET_HOTEND_TEMP             _UxGT("Hotend Temp")
#endif
#ifndef MSG_UBL_CUSTOM_HOTEND_TEMP
  #define MSG_UBL_CUSTOM_HOTEND_TEMP          MSG_UBL_SET_HOTEND_TEMP
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
#ifndef MSG_UBL_BUILD_PLA_MESH
  #define MSG_UBL_BUILD_PLA_MESH              _UxGT("Build PLA Mesh")
#endif
#ifndef MSG_UBL_BUILD_ABS_MESH
  #define MSG_UBL_BUILD_ABS_MESH              _UxGT("Build ABS Mesh")
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
#ifndef MSG_UBL_VALIDATE_PLA_MESH
  #define MSG_UBL_VALIDATE_PLA_MESH           _UxGT("Validate PLA Mesh")
#endif
#ifndef MSG_UBL_VALIDATE_ABS_MESH
  #define MSG_UBL_VALIDATE_ABS_MESH           _UxGT("Validate ABS Mesh")
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
#ifndef MSG_UBL_EDIT_MESH_MENU
  #define MSG_UBL_EDIT_MESH_MENU              _UxGT("Edit Mesh")
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
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       _UxGT("Fan speed")
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
#ifndef MSG_ON
  #define MSG_ON                              _UxGT("On ")
#endif
#ifndef MSG_OFF
  #define MSG_OFF                             _UxGT("Off")
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
#ifndef MSG_VX_JERK
  #define MSG_VX_JERK                         _UxGT("Vx-jerk")
#endif
#ifndef MSG_VY_JERK
  #define MSG_VY_JERK                         _UxGT("Vy-jerk")
#endif
#ifndef MSG_VZ_JERK
  #define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         _UxGT("Ve-jerk")
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
#ifndef MSG_XSTEPS
  #define MSG_XSTEPS                          _UxGT("Xsteps/mm")
#endif
#ifndef MSG_YSTEPS
  #define MSG_YSTEPS                          _UxGT("Ysteps/mm")
#endif
#ifndef MSG_ZSTEPS
  #define MSG_ZSTEPS                          _UxGT("Zsteps/mm")
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
  #define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
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
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     _UxGT("Pause print")
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    _UxGT("Resume print")
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      _UxGT("Stop print")
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
#ifndef MSG_RESUMING
  #define MSG_RESUMING                        _UxGT("Resuming print")
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
#ifndef MSG_CONTROL_RETRACT_ZLIFT
  #define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  _UxGT("Change filament")
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD                     _UxGT("Init. SD card")
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD                      _UxGT("Change SD card")
#endif
#ifndef MSG_ZPROBE_OUT
  #define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
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
#ifndef MSG_BLTOUCH_STOW
  #define MSG_BLTOUCH_STOW                    _UxGT("Stow BLTouch")
#endif
#ifndef MSG_HOME
  #define MSG_HOME                            _UxGT("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST                           _UxGT("first")
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
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
#ifndef MSG_ENDSTOP_ABORT
  #define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD              _UxGT("Heating failed")
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
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
#ifndef MSG_ERR_Z_HOMING
  #define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Forbidden")
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
#ifndef MSG_HEATING_COMPLETE
  #define MSG_HEATING_COMPLETE                _UxGT("Heating done.")
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     _UxGT("Bed Heating.")
#endif
#ifndef MSG_BED_DONE
  #define MSG_BED_DONE                        _UxGT("Bed done.")
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
#ifndef MSG_DELTA_AUTO_CALIBRATE
  #define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto Calibration")
#endif
#ifndef MSG_DELTA_HEIGHT_CALIBRATE
  #define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Set Delta Height")
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
  #define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("Light BRIGHTNESS")
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
  #define MSG_INFO_PSU                        _UxGT("Power Supply")
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
#ifndef MSG_FILAMENT_CHANGE_HEADER
  #define MSG_FILAMENT_CHANGE_HEADER          _UxGT("PRINT PAUSED")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("RESUME OPTIONS:")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_EXTRUDE
  #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Extrude more")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Resume print")
#endif
#ifndef MSG_FILAMENT_CHANGE_MINTEMP
  #define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Minimum Temp is ")
#endif
#ifndef MSG_FILAMENT_CHANGE_NOZZLE
  #define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Nozzle: ")
#endif

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Wait for start")
    #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("of the filament")
    #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("change")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament unload")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert filament")
    #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("and press button")
    #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("to continue...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEAT_1
    #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Press button to")
    #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("heat nozzle.")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Heating nozzle")
    #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Please wait...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament load")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("filament extrude")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wait for print")
    #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("to resume")
  #endif
#else // LCD_HEIGHT < 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Please wait...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ejecting...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert and Click")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Heating...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Loading...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Extruding...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Resuming...")
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_EN_H

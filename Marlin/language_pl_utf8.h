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
 * Polish
 * UTF-8 for Graphical Display 
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_PL_UTF_H
#define LANGUAGE_PL_UTF_H

#define MAPPER_C3C4C5_PL

#define pl 1234
#if LCD_LANGUAGE == pl
  #define DISPLAY_CHARSET_ISO10646_PL
#endif
#undef pl

#ifndef CHARSIZE
  #define CHARSIZE 2
#endif

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" gotowy.")
#endif
#ifndef MSG_BACK
  #define MSG_BACK                            _UxGT("Wróć")
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     _UxGT("Karta włożona")
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      _UxGT("Karta usunięta")
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    _UxGT("Kranców.") // Max length 8 characters
#endif
#ifndef MSG_LCD_SOFT_ENDSTOPS
  #define MSG_LCD_SOFT_ENDSTOPS               _UxGT("Soft Endstops")
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            _UxGT("Menu główne")
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       _UxGT("Autostart")
#endif
#ifndef MSG_DISABLE_STEPPERS
  #define MSG_DISABLE_STEPPERS                _UxGT("Wyłącz silniki")
#endif
#ifndef MSG_DEBUG_MENU
  #define MSG_DEBUG_MENU                      _UxGT("Debug Menu")
#endif
#ifndef MSG_PROGRESS_BAR_TEST
  #define MSG_PROGRESS_BAR_TEST               _UxGT("Progress Bar Test")
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME                       _UxGT("Pozycja zerowa")
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X                     _UxGT("Zeruj X")
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y                     _UxGT("Zeruj Y")
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z                     _UxGT("Zeruj Z")
#endif
#ifndef MSG_TMC_Z_CALIBRATION
  #define MSG_TMC_Z_CALIBRATION               _UxGT("Kalibruj Z")
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING                _UxGT("Pozycja zerowa")
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING               _UxGT("Kliknij by rozp.")
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Następny punkt")
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE                  _UxGT("Wypoziomowano!")
#endif
#ifndef MSG_Z_FADE_HEIGHT
  #define MSG_Z_FADE_HEIGHT                   _UxGT("Fade Height")
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS                _UxGT("Ust. poz. zer.")
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED            _UxGT("Poz. zerowa ust.")
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN                      _UxGT("Ustaw punkt zero")
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1                       _UxGT("Rozgrzej PLA")
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_1_END
  #define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" głowicę")
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" stół")
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" ustaw.")
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2                       _UxGT("Rozgrzej ABS")
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_2_END
  #define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" głowicę")
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" stół")
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" ustaw.")
#endif
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        _UxGT("Chłodzenie")
#endif
#ifndef MSG_SWITCH_PS_ON
  #define MSG_SWITCH_PS_ON                    _UxGT("Włącz zasilacz")
#endif
#ifndef MSG_SWITCH_PS_OFF
  #define MSG_SWITCH_PS_OFF                   _UxGT("Wyłącz zasilacz")
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE                         _UxGT("Ekstruzja")
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT                         _UxGT("Wycofanie")
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS                       _UxGT("Ruch osi")
#endif
#ifndef MSG_BED_LEVELING
  #define MSG_BED_LEVELING                    _UxGT("Poziom. stołu")
#endif
#ifndef MSG_LEVEL_BED
  #define MSG_LEVEL_BED                       _UxGT("Poziom. stołu")
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
#ifndef MSG_USER_MENU
  #define MSG_USER_MENU                       _UxGT("Własne polecenia")
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
  #define MSG_UBL_SAVE_ERROR                  _UxGT("Błąd: UBL Save")
#endif
#ifndef MSG_UBL_RESTORE_ERROR
  #define MSG_UBL_RESTORE_ERROR               _UxGT("Błąd: UBL Restore")
#endif
#ifndef MSG_UBL_Z_OFFSET_STOPPED
  #define MSG_UBL_Z_OFFSET_STOPPED            _UxGT("Z-Offset Stopped")
#endif
#ifndef MSG_UBL_STEP_BY_STEP_MENU
  #define MSG_UBL_STEP_BY_STEP_MENU           _UxGT("Step-By-Step UBL")
#endif

#ifndef MSG_LED_CONTROL
  #define MSG_LED_CONTROL                     _UxGT("Ustawienia ośw.")
#endif
#ifndef MSG_LEDS
  #define MSG_LEDS                            _UxGT("Oświetlenie")
#endif
#ifndef MSG_LED_PRESETS
  #define MSG_LED_PRESETS                     _UxGT("Light Presets")
#endif
#ifndef MSG_SET_LEDS_RED
  #define MSG_SET_LEDS_RED                    _UxGT("Czerwony")
#endif
#ifndef MSG_SET_LEDS_ORANGE
  #define MSG_SET_LEDS_ORANGE                 _UxGT("Pomarańczowy")
#endif
#ifndef MSG_SET_LEDS_YELLOW
  #define MSG_SET_LEDS_YELLOW                 _UxGT("Żółty")
#endif
#ifndef MSG_SET_LEDS_GREEN
  #define MSG_SET_LEDS_GREEN                  _UxGT("Zielony")
#endif
#ifndef MSG_SET_LEDS_BLUE
  #define MSG_SET_LEDS_BLUE                   _UxGT("Niebieski")
#endif
#ifndef MSG_SET_LEDS_INDIGO
  #define MSG_SET_LEDS_INDIGO                 _UxGT("Indygo")
#endif
#ifndef MSG_SET_LEDS_VIOLET
  #define MSG_SET_LEDS_VIOLET                 _UxGT("Fioletowy")
#endif
#ifndef MSG_SET_LEDS_WHITE
  #define MSG_SET_LEDS_WHITE                  _UxGT("Biały")
#endif
#ifndef MSG_SET_LEDS_DEFAULT
  #define MSG_SET_LEDS_DEFAULT                _UxGT("Domyślny")
#endif
#ifndef MSG_CUSTOM_LEDS
  #define MSG_CUSTOM_LEDS                     _UxGT("Własne ośw.")
#endif
#ifndef MSG_INTENSITY_R
  #define MSG_INTENSITY_R                     _UxGT("Czerwony intens.")
#endif
#ifndef MSG_INTENSITY_G
  #define MSG_INTENSITY_G                     _UxGT("Zielony  intens.")
#endif
#ifndef MSG_INTENSITY_B
  #define MSG_INTENSITY_B                     _UxGT("Niebieski  intens.")
#endif
#ifndef MSG_INTENSITY_W
  #define MSG_INTENSITY_W                     _UxGT("Biały  intens.")
#endif
#ifndef MSG_LED_BRIGHTNESS
  #define MSG_LED_BRIGHTNESS                  _UxGT("Jasność")
#endif

#ifndef MSG_MOVING
  #define MSG_MOVING                          _UxGT("Moving...")
#endif
#ifndef MSG_FREE_XY
  #define MSG_FREE_XY                         _UxGT("Free XY")
#endif
#ifndef MSG_MOVE_X
  #define MSG_MOVE_X                          _UxGT("Przesuń w X")
#endif
#ifndef MSG_MOVE_Y
  #define MSG_MOVE_Y                          _UxGT("Przesuń w Y")
#endif
#ifndef MSG_MOVE_Z
  #define MSG_MOVE_Z                          _UxGT("Przesuń w Z")
#endif
#ifndef MSG_MOVE_E
  #define MSG_MOVE_E                          _UxGT("Ekstruder")
#endif
#ifndef MSG_MOVE_01MM
  #define MSG_MOVE_01MM                       _UxGT("+- 0.1mm")
#endif
#ifndef MSG_MOVE_1MM
  #define MSG_MOVE_1MM                        _UxGT("+- 1mm")
#endif
#ifndef MSG_MOVE_10MM
  #define MSG_MOVE_10MM                       _UxGT("+- 10mm")
#endif
#ifndef MSG_SPEED
  #define MSG_SPEED                           _UxGT("Prędkość")
#endif
#ifndef MSG_BED_Z
  #define MSG_BED_Z                           _UxGT("Stół Z")
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE                          _UxGT("Głowica")
#endif
#ifndef MSG_BED
  #define MSG_BED                             _UxGT("Stół")
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       _UxGT("Obroty went.")
#endif
#ifndef MSG_EXTRA_FAN_SPEED
  #define MSG_EXTRA_FAN_SPEED                 _UxGT("Prędkość dod. went.")
#endif
#ifndef MSG_FLOW
  #define MSG_FLOW                            _UxGT("Przepływ")
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL                         _UxGT("Ustawienia")
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
  #define MSG_AUTOTEMP                        _UxGT("Auto. temperatura")
#endif
#ifndef MSG_ON
  #define MSG_ON                              _UxGT("Wł. ")
#endif
#ifndef MSG_OFF
  #define MSG_OFF                             _UxGT("Wył.")
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
  #define MSG_SELECT                          _UxGT("Wybierz")
#endif
#ifndef MSG_ACC
  #define MSG_ACC                             _UxGT("Przyśpieszenie")
#endif
#ifndef MSG_JERK
  #define MSG_JERK                            _UxGT("Zryw")
#endif
#if IS_KINEMATIC
  #ifndef MSG_VA_JERK
    #define MSG_VA_JERK                       _UxGT("Zryw Va")
  #endif
  #ifndef MSG_VB_JERK
    #define MSG_VB_JERK                       _UxGT("Zryw Vb")
  #endif
  #ifndef MSG_VC_JERK
    #define MSG_VC_JERK                       _UxGT("Zryw Vc")
  #endif
#else
  #ifndef MSG_VA_JERK
    #define MSG_VA_JERK                       _UxGT("Zryw Vx")
  #endif
  #ifndef MSG_VB_JERK
    #define MSG_VB_JERK                       _UxGT("Zryw Vy")
  #endif
  #ifndef MSG_VC_JERK
    #define MSG_VC_JERK                       _UxGT("Zryw Vz")
  #endif
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         _UxGT("Zryw Ve")
#endif
#ifndef MSG_JUNCTION_DEVIATION
  #define MSG_JUNCTION_DEVIATION              _UxGT("Junction Dev")
#endif
#ifndef MSG_VELOCITY
  #define MSG_VELOCITY                        _UxGT("Prędkość")
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
  #define MSG_ACCELERATION                    _UxGT("Przyspieszenie")
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX                            _UxGT("Amax ")
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT                       _UxGT("A-wycofanie")
#endif
#ifndef MSG_A_TRAVEL
  #define MSG_A_TRAVEL                        _UxGT("A-przesuń.")
#endif
#ifndef MSG_STEPS_PER_MM
  #define MSG_STEPS_PER_MM                    _UxGT("kroki/mm")
#endif
#if IS_KINEMATIC
  #ifndef MSG_ASTEPS
    #define MSG_ASTEPS                        _UxGT("krokiA/mm")
  #endif
  #ifndef MSG_BSTEPS
    #define MSG_BSTEPS                        _UxGT("krokiB/mm")
  #endif
  #ifndef MSG_CSTEPS
    #define MSG_CSTEPS                        _UxGT("krokiC/mm")
  #endif
#else
  #ifndef MSG_ASTEPS
    #define MSG_ASTEPS                        _UxGT("krokiX/mm")
  #endif
  #ifndef MSG_BSTEPS
    #define MSG_BSTEPS                        _UxGT("krokiY/mm")
  #endif
  #ifndef MSG_CSTEPS
    #define MSG_CSTEPS                        _UxGT("krokiZ/mm")
  #endif
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS                          _UxGT("krokiE/mm")
#endif
#ifndef MSG_E1STEPS
  #define MSG_E1STEPS                         _UxGT("krokiE1/mm")
#endif
#ifndef MSG_E2STEPS
  #define MSG_E2STEPS                         _UxGT("krokiE2/mm")
#endif
#ifndef MSG_E3STEPS
  #define MSG_E3STEPS                         _UxGT("krokiE3/mm")
#endif
#ifndef MSG_E4STEPS
  #define MSG_E4STEPS                         _UxGT("krokiE4/mm")
#endif
#ifndef MSG_E5STEPS
  #define MSG_E5STEPS                         _UxGT("krokiE5/mm")
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE                     _UxGT("Temperatura")
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION                          _UxGT("Ruch")
#endif
#ifndef MSG_FILAMENT
  #define MSG_FILAMENT                        _UxGT("Filament")
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED              _UxGT("E w mm3")
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   _UxGT("Śred. Fila.")
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
  #define MSG_CONTRAST                        _UxGT("Kontrast LCD")
#endif
#ifndef MSG_STORE_EEPROM
  #define MSG_STORE_EEPROM                    _UxGT("Zapisz w pamięci")
#endif
#ifndef MSG_LOAD_EEPROM
  #define MSG_LOAD_EEPROM                     _UxGT("Wczytaj z pamięci")
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE                _UxGT("Ustaw. fabryczne")
#endif
#ifndef MSG_INIT_EEPROM
  #define MSG_INIT_EEPROM                     _UxGT("Inicjalizuj EEPROM")
#endif
#ifndef MSG_REFRESH
  #define MSG_REFRESH                         _UxGT("Odswież")
#endif
#ifndef MSG_WATCH
  #define MSG_WATCH                           _UxGT("Ekran główny")
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE                         _UxGT("Przygotuj")
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE                            _UxGT("Strojenie")
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     _UxGT("Wstrzymaj wydruk")
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    _UxGT("Wznów wydruk")
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      _UxGT("Zatrzymaj wydruk")
#endif
#ifndef MSG_POWER_LOSS_RECOVERY
  #define MSG_POWER_LOSS_RECOVERY             _UxGT("Power-Loss Recovery")
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU                       _UxGT("Drukuj z karty SD")
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD                         _UxGT("Brak karty SD")
#endif
#ifndef MSG_DWELL
  #define MSG_DWELL                           _UxGT("Uśpienie...")
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT                        _UxGT("Click to resume...")
#endif
#ifndef MSG_PRINT_PAUSED
  #define MSG_PRINT_PAUSED                    _UxGT("Druk wstrzymany")
#endif
#ifndef MSG_PRINTING
  #define MSG_PRINTING                        _UxGT("Drukowanie...")
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED                   _UxGT("Druk przerwany")
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE                         _UxGT("Brak ruchu.")
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED                          _UxGT("ZABITY. ")
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED                         _UxGT("ZATRZYMANY. ")
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT                 _UxGT("Wycofaj mm")
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
  #define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet V")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAPF
  #define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("S UnRet V")
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  _UxGT("Zmień filament")
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
  #define MSG_INIT_SDCARD                     _UxGT("Inicjal. karty SD")
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD                      _UxGT("Zmiana karty SD")
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
  #define MSG_BLTOUCH_SELFTEST                _UxGT("Cmd: Self-Test")
#endif
#ifndef MSG_BLTOUCH_RESET
  #define MSG_BLTOUCH_RESET                   _UxGT("Cmd: Reset")
#endif
#ifndef MSG_BLTOUCH_STOW
  #define MSG_BLTOUCH_STOW                    _UxGT("Cmd: Stow")
#endif
#ifndef MSG_BLTOUCH_DEPLOY
  #define MSG_BLTOUCH_DEPLOY                  _UxGT("Cmd: Deploy")
#endif
#ifndef MSG_BLTOUCH_SW_MODE
  #define MSG_BLTOUCH_SW_MODE                 _UxGT("Cmd: SW-Mode")
#endif
#ifndef MSG_BLTOUCH_5V_MODE
  #define MSG_BLTOUCH_5V_MODE                 _UxGT("Cmd: 5V-Mode")
#endif
#ifndef MSG_BLTOUCH_OD_MODE
  #define MSG_BLTOUCH_OD_MODE                 _UxGT("Cmd: OD-Mode")
#endif
#ifndef MSG_BLTOUCH_MODE_STORE
  #define MSG_BLTOUCH_MODE_STORE              _UxGT("Cmd: Mode-Store")
#endif
#ifndef MSG_BLTOUCH_MODE_STORE_5V
  #define MSG_BLTOUCH_MODE_STORE_5V           _UxGT("Set BLTouch to 5V")
#endif
#ifndef MSG_BLTOUCH_MODE_STORE_OD
  #define MSG_BLTOUCH_MODE_STORE_OD           _UxGT("Set BLTouch to OD")
#endif
#ifndef MSG_BLTOUCH_MODE_ECHO
  #define MSG_BLTOUCH_MODE_ECHO               _UxGT("Report Drain")
#endif
#ifndef MSG_BLTOUCH_MODE_CHANGE
  #define MSG_BLTOUCH_MODE_CHANGE             _UxGT("DANGER: Bad settings can cause damage! Proceed anyway?")
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
  #define MSG_ERR_REDUNDANT_TEMP              _UxGT("Błąd: REDUNDANT TEMP")
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#endif
#ifndef MSG_THERMAL_RUNAWAY_BED
  #define MSG_THERMAL_RUNAWAY_BED             _UxGT("BED THERMAL RUNAWAY")
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP                     _UxGT("Błąd: MAXTEMP")
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP                     _UxGT("Błąd: MINTEMP")
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #define MSG_ERR_MAXTEMP_BED                 _UxGT("Błąd: MAXTEMP BED")
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #define MSG_ERR_MINTEMP_BED                 _UxGT("Błąd: MINTEMP BED")
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
  #define MSG_SHORT_HOUR                      _UxGT("g") // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING                         _UxGT("Rozgrzewanie...")
#endif
#ifndef MSG_COOLING
  #define MSG_COOLING                         _UxGT("Chłodzenie...")
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     _UxGT("Rozgrzewanie stołu...")
#endif
#ifndef MSG_BED_COOLING
  #define MSG_BED_COOLING                     _UxGT("Chłodzenie stołu...")
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE                 _UxGT("Kalibrowanie delty")
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibruj X")
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibruj Y")
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibruj Z")
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibruj środek")
#endif
#ifndef MSG_DELTA_SETTINGS
  #define MSG_DELTA_SETTINGS                  _UxGT("Delta Settings")
#endif
#ifndef MSG_DELTA_AUTO_CALIBRATE
  #define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto kalibracja")
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
  #define MSG_INFO_MENU                       _UxGT("O drukarce")
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU               _UxGT("Oprogramowanie")
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
  #define MSG_INFO_STATS_MENU                 _UxGT("Statystyki")
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU                 _UxGT("Płyta główna")
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistory")
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS                  _UxGT("Ekstrudery")
#endif
#ifndef MSG_INFO_BAUDRATE
  #define MSG_INFO_BAUDRATE                   _UxGT("Prędkość")
#endif
#ifndef MSG_INFO_PROTOCOL
  #define MSG_INFO_PROTOCOL                   _UxGT("Protokół")
#endif
#ifndef MSG_CASE_LIGHT
  #define MSG_CASE_LIGHT                      _UxGT("Oświetlenie")
#endif
#ifndef MSG_CASE_LIGHT_BRIGHTNESS
  #define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("Jasność")
#endif
#if LCD_WIDTH >= 20
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Wydrukowano")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Ukończono")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Razem")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Najdł. druk")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Użyty fil.")
  #endif
#else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Wydrukowano")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Ukończono")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Razem")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Najdł. druk")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Użyty fil.")
  #endif
#endif
#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU                        _UxGT("Zasilacz")
#endif
#ifndef MSG_DRIVE_STRENGTH
  #define MSG_DRIVE_STRENGTH                  _UxGT("Siła silnika")
#endif
#ifndef MSG_DAC_PERCENT
  #define MSG_DAC_PERCENT                     _UxGT("Siła %")
#endif
#ifndef MSG_DAC_EEPROM_WRITE
  #define MSG_DAC_EEPROM_WRITE                _UxGT("Zapisz DAC EEPROM")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER_PAUSE
  #define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("DRUK WSTRZYMANY")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER_LOAD
  #define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("LOAD FILAMENT")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER_UNLOAD
  #define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("UNLOAD FILAMENT")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("OPCJE WZNOWIENIA:")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_PURGE
  #define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("Purge more")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Kontynuuj")
#endif
#ifndef MSG_FILAMENT_CHANGE_NOZZLE
  #define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Głowica: ")
#endif
#ifndef MSG_ERR_HOMING_FAILED
  #define MSG_ERR_HOMING_FAILED               _UxGT("Homing failed")
#endif
#ifndef MSG_ERR_PROBING_FAILED
  #define MSG_ERR_PROBING_FAILED              _UxGT("Probing failed")
#endif
#ifndef MSG_M600_TOO_COLD
  #define MSG_M600_TOO_COLD                   _UxGT("M600: Za niska temp.")
#endif

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Czekam na ")
    #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("zmianę filamentu")
    #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Czekam na ")
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("wyjęcie filamentu")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Włóż filament")
    #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("i naciśnij przycisk")
    #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("aby kontynuować...")
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
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Czekam na ")
    #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament load")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_PURGE_1
    #define MSG_FILAMENT_CHANGE_PURGE_1         _UxGT("Czekam na ")
    #define MSG_FILAMENT_CHANGE_PURGE_2         _UxGT("filament purge")
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
  #ifndef MSG_FILAMENT_CHANGE_HEAT_1
    #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Click to heat")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Rozgrzewanie...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Loading...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_PURGE_1
    #define MSG_FILAMENT_CHANGE_PURGE_1         _UxGT("Purging...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wznawianie...")
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_PL_UTF8_H

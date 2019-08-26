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
 * Simplified Chinese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_ZH_CN_H
#define LANGUAGE_ZH_CN_H

#define CHARSIZE 3

#define WELCOME_MSG                         MACHINE_NAME _UxGT("е·Іе°±з»Є.")  //" ready."
#define MSG_BACK                            _UxGT("иї”е›ћ")         // вЂќBackвЂњ
#define MSG_SD_INSERTED                     _UxGT("е­�е‚ЁеЌЎе·ІжЏ’е…Ґ")  //"Card inserted"
#define MSG_SD_REMOVED                      _UxGT("е­�е‚ЁеЌЎиў«ж‹”е‡є")  //"Card removed"
#define MSG_LCD_ENDSTOPS                    _UxGT("жЊЎеќ—")  //"Endstops" // Max length 8 characters
#define MSG_MAIN                            _UxGT("дё»иЏњеЌ•")  //"Main"
#define MSG_AUTOSTART                       _UxGT("и‡ЄеЉЁејЂе§‹")  //"Autostart"
#define MSG_DISABLE_STEPPERS                _UxGT("е…ій—­ж­Ґиї›з”µжњє")  //"Disable steppers"
#define MSG_DEBUG_MENU                      _UxGT("и°ѓиЇ•иЏњеЌ•")    // "Debug Menu"
#define MSG_PROGRESS_BAR_TEST               _UxGT("иї›еє¦жќЎжµ‹иЇ•")  // "Progress Bar Test"
#define MSG_AUTO_HOME                       _UxGT("е›ћеЋџз‚№")  //"Auto home"
#define MSG_AUTO_HOME_X                     _UxGT("е›ћXеЋџдЅЌ")  //"Home X"
#define MSG_AUTO_HOME_Y                     _UxGT("е›ћYеЋџдЅЌ")  //"Home Y"
#define MSG_AUTO_HOME_Z                     _UxGT("е›ћZеЋџдЅЌ")  //"Home Z"
#define MSG_TMC_Z_CALIBRATION               _UxGT("вЉїж Ўе‡†Z")  //"Calibrate Z"
#define MSG_LEVEL_BED_HOMING                _UxGT("е№іеЏ°и°ѓе№іXYZеЅ’еЋџдЅЌ")  //"Homing XYZ"
#define MSG_LEVEL_BED_WAITING               _UxGT("еЌ•е‡»ејЂе§‹зѓ­еєЉи°ѓе№і")  //"Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("дё‹дёЄзѓ­еєЉи°ѓе№із‚№")  //"Next Point"
#define MSG_LEVEL_BED_DONE                  _UxGT("е®Њж€ђзѓ­еєЉи°ѓе№і")  //"Leveling Done!"
#define MSG_Z_FADE_HEIGHT                   _UxGT("ж·Ўе‡єй«�еє¦")     // "Fade Height"
#define MSG_SET_HOME_OFFSETS                _UxGT("и®ѕзЅ®еЋџз‚№еЃЏз§»")  //"Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("еЃЏз§»е·ІеђЇз”Ё")  //"Offsets applied"
#define MSG_SET_ORIGIN                      _UxGT("и®ѕзЅ®еЋџз‚№")  //"Set origin"
#define MSG_PREHEAT_1                       _UxGT("йў„зѓ­PLA")  //"Preheat PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")  //MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" е…ЁйѓЁ")  //MSG_PREHEAT_1 " All"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" зѓ­еєЉ")  //MSG_PREHEAT_1 " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" и®ѕзЅ®")  //MSG_PREHEAT_1 " conf"
#define MSG_PREHEAT_2                       _UxGT("йў„зѓ­ABS")  //"Preheat ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")  //MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" е…ЁйѓЁ")  //MSG_PREHEAT_2 " All"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" зѓ­еєЉ")  //MSG_PREHEAT_2 " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" и®ѕзЅ®")  //MSG_PREHEAT_2 " conf"
#define MSG_COOLDOWN                        _UxGT("й™Ќжё©")  //"Cooldown"
#define MSG_SWITCH_PS_ON                    _UxGT("з”µжєђж‰“ејЂ")  //"Switch power on"
#define MSG_SWITCH_PS_OFF                   _UxGT("з”µжєђе…ій—­")  //"Switch power off"
#define MSG_EXTRUDE                         _UxGT("жЊ¤е‡є")  //"Extrude"
#define MSG_RETRACT                         _UxGT("е›ћжЉЅ")  //"Retract"
#define MSG_MOVE_AXIS                       _UxGT("з§»еЉЁиЅґ")  //"Move axis"
#define MSG_BED_LEVELING                    _UxGT("и°ѓе№ізѓ­еєЉ")  //"Bed leveling"
#define MSG_LEVEL_BED                       _UxGT("и°ѓе№ізѓ­еєЉ")  //"Level bed"
#define MSG_LEVEL_CORNERS                   _UxGT("и°ѓе№іиѕ№и§’") // "Level corners"

#define MSG_NEXT_CORNER                     _UxGT("дё‹дёЄиѕ№и§’") // "Next corner"
#define MSG_EDITING_STOPPED                 _UxGT("зЅ‘ж јзј–иѕ‘е·ІеЃњж­ў") // "Mesh Editing Stopped"

#define MSG_USER_MENU                       _UxGT("е®ље€¶е‘Ѕд»¤") // "Custom Commands"
#define MSG_UBL_DOING_G29                   _UxGT("ж‰§иЎЊG29") // "Doing G29"
#define MSG_UBL_UNHOMED                     _UxGT("е…€е›ћXYZеЋџз‚№") // "Home XYZ first"
#define MSG_UBL_TOOLS                       _UxGT("UBLе·Ґе…·") // "UBL Tools"
#define MSG_UBL_LEVEL_BED                   _UxGT("з»џдёЂзѓ­еєЉи°ѓе№і(UBL)") // "Unified Bed Leveling"
#define MSG_UBL_MANUAL_MESH                 _UxGT("ж‰‹е·Ґе€›и®ѕзЅ‘ж ј") // "Manually Build Mesh"

#define MSG_UBL_BC_INSERT                   _UxGT("ж”ѕзЅ®ећ«з‰‡е№¶жµ‹й‡Џ") // "Place shim & measure"
#define MSG_UBL_BC_INSERT2                  _UxGT("жµ‹й‡Џ") // "Measure"
#define MSG_UBL_BC_REMOVE                   _UxGT("з§»й™¤е№¶жµ‹й‡Џзѓ­еєЉ") // "Remove & measure bed"
#define MSG_UBL_MOVING_TO_NEXT              _UxGT("з§»еЉЁе€°дё‹дёЂдёЄ") // "Moving to next"
#define MSG_UBL_ACTIVATE_MESH               _UxGT("жїЂжґ»UBL") // "Activate UBL"
#define MSG_UBL_DEACTIVATE_MESH             _UxGT("е…ій—­UBL") // "Deactivate UBL"
#define MSG_UBL_SET_BED_TEMP                _UxGT("и®ѕзЅ®зѓ­еєЉжё©еє¦") // "Bed Temp"
#define MSG_UBL_SET_HOTEND_TEMP             _UxGT("зѓ­з«Їжё©еє¦") // "Hotend Temp"
#define MSG_UBL_MESH_EDIT                   _UxGT("зЅ‘ж јзј–иѕ‘") // "Mesh Edit"
#define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("зј–иѕ‘е®ўж€·зЅ‘ж ј") // "Edit Custom Mesh"
#define MSG_UBL_FINE_TUNE_MESH              _UxGT("з»†и°ѓзЅ‘ж ј") // "Fine Tuning Mesh"
#define MSG_UBL_DONE_EDITING_MESH           _UxGT("е®Њж€ђзј–иѕ‘зЅ‘ж ј") // "Done Editing Mesh"
#define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("е€›и®ѕе®ўж€·зЅ‘ж ј") // "Build Custom Mesh"
#define MSG_UBL_BUILD_MESH_MENU             _UxGT("е€›и®ѕзЅ‘ж ј") // "Build Mesh"
#define MSG_UBL_BUILD_PLA_MESH              _UxGT("е€›и®ѕPLAзЅ‘ж ј") // "Build PLA Mesh"
#define MSG_UBL_BUILD_ABS_MESH              _UxGT("е€›и®ѕABSзЅ‘ж ј") // "Build ABS Mesh"
#define MSG_UBL_BUILD_COLD_MESH             _UxGT("е€›и®ѕе†·зЅ‘ж ј") // "Build Cold Mesh"
#define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("и°ѓж•ґзЅ‘ж јй«�еє¦") // "Adjust Mesh Height"
#define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("й«�еє¦еђ€и®Ў") // "Height Amount"
#define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("ж‰№е‡†зЅ‘ж ј") // "Validate Mesh"
#define MSG_UBL_VALIDATE_PLA_MESH           _UxGT("ж‰№е‡†PLAзЅ‘ж ј") // "Validate PLA Mesh"
#define MSG_UBL_VALIDATE_ABS_MESH           _UxGT("ж‰№е‡†ABSзЅ‘ж ј") // "Validate ABS Mesh"
#define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("ж‰№е‡†е®ўж€·зЅ‘ж ј") // "Validate Custom Mesh"
#define MSG_UBL_CONTINUE_MESH               _UxGT("з»§з»­зѓ­еєЉзЅ‘ж ј") // "Continue Bed Mesh"
#define MSG_UBL_MESH_LEVELING               _UxGT("зЅ‘ж ји°ѓе№і") // "Mesh Leveling"
#define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("дё‰з‚№и°ѓе№і") // "3-Point Leveling"
#define MSG_UBL_GRID_MESH_LEVELING          _UxGT("ж је­ђзЅ‘ж ји°ѓе№і") // "Grid Mesh Leveling"
#define MSG_UBL_MESH_LEVEL                  _UxGT("и°ѓе№ізЅ‘ж ј") // "Level Mesh"
#define MSG_UBL_SIDE_POINTS                 _UxGT("иѕ№з‚№") // "Side Points"
#define MSG_UBL_MAP_TYPE                    _UxGT("е›ѕз±»ећ‹") // "Map Type"
#define MSG_UBL_OUTPUT_MAP                  _UxGT("иѕ“е‡єзЅ‘ж је›ѕ") // "Output Mesh Map"
#define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("иѕ“е‡єе€°дё»жњє") // "Output for Host"
#define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("иѕ“е‡єе€°CSV") // "Output for CSV"
#define MSG_UBL_OUTPUT_MAP_BACKUP           _UxGT("иѕ“е‡єе€°е¤‡д»Ѕ") // "Off Printer Backup"
#define MSG_UBL_INFO_UBL                    _UxGT("иѕ“е‡єUBLдїЎжЃЇ") // "Output UBL Info"
#define MSG_UBL_EDIT_MESH_MENU              _UxGT("зј–иѕ‘зЅ‘ж ј") // "Edit Mesh"
#define MSG_UBL_FILLIN_AMOUNT               _UxGT("еЎ«е……еђ€и®Ў") // "Fill-in Amount"
#define MSG_UBL_MANUAL_FILLIN               _UxGT("ж‰‹е·ҐеЎ«е……") // "Manual Fill-in"
#define MSG_UBL_SMART_FILLIN                _UxGT("иЃЄж�ЋеЎ«е……") // "Smart Fill-in"
#define MSG_UBL_FILLIN_MESH                 _UxGT("еЎ«е……зЅ‘ж ј") // "Fill-in Mesh"
#define MSG_UBL_INVALIDATE_ALL              _UxGT("дЅњеєџж‰Ђжњ‰зљ„") // "Invalidate All"
#define MSG_UBL_INVALIDATE_CLOSEST          _UxGT("дЅњеєџжњЂиї‘зљ„") // "Invalidate Closest"
#define MSG_UBL_FINE_TUNE_ALL               _UxGT("з»†и°ѓж‰Ђжњ‰зљ„") // "Fine Tune All"
#define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("з»†и°ѓжњЂиї‘зљ„") // "Fine Tune Closest"
#define MSG_UBL_STORAGE_MESH_MENU           _UxGT("зЅ‘ж је­�е‚Ё") // "Mesh Storage"
#define MSG_UBL_STORAGE_SLOT                _UxGT("е­�е‚Ёж§Ѕ") // "Memory Slot"
#define MSG_UBL_LOAD_MESH                   _UxGT("иЈ…иЅЅзѓ­еєЉзЅ‘ж ј") // "Load Bed Mesh"
#define MSG_UBL_SAVE_MESH                   _UxGT("дїќе­�зѓ­еєЉзЅ‘ж ј") // "Save Bed Mesh"
#define MSG_MESH_LOADED                     _UxGT("зЅ‘ж ј %i е·ІиЈ…иЅЅ") // "Mesh %i loaded"
#define MSG_MESH_SAVED                      _UxGT("зЅ‘ж ј %i е·Ідїќе­�") // "Mesh %i saved"
#define MSG_NO_STORAGE                      _UxGT("жІЎжњ‰е­�е‚Ё") // "No storage"
#define MSG_UBL_SAVE_ERROR                  _UxGT("й”™иЇЇ: UBLдїќе­�") // "Err: UBL Save"
#define MSG_UBL_RESTORE_ERROR               _UxGT("й”™иЇЇ: UBLиї�еЋџ") // "Err: UBL Restore"
#define MSG_UBL_Z_OFFSET_STOPPED            _UxGT("ZеЃЏз§»е·ІеЃњж­ў") // "Z-Offset Stopped"
#define MSG_UBL_STEP_BY_STEP_MENU           _UxGT("дёЂж­Ґж­ҐUBL") // "Step-By-Step UBL"

#define MSG_LED_CONTROL                     _UxGT("зЃЇз®ЎжЋ§е€¶") // "LED Control")
#define MSG_LEDS                            _UxGT("зЃЇ") // "Lights")
#define MSG_LED_PRESETS                     _UxGT("зЃЇйў„зЅ®") // "Light Presets")
#define MSG_SET_LEDS_RED                    _UxGT("зєў") // "Red")
#define MSG_SET_LEDS_ORANGE                 _UxGT("ж©™") // "Orange")
#define MSG_SET_LEDS_YELLOW                 _UxGT("й»„") // "Yellow")
#define MSG_SET_LEDS_GREEN                  _UxGT("з»ї") // "Green")
#define MSG_SET_LEDS_BLUE                   _UxGT("и“ќ") // "Blue")
#define MSG_SET_LEDS_INDIGO                 _UxGT("йќ’") // "Indigo")
#define MSG_SET_LEDS_VIOLET                 _UxGT("зґ«") // "Violet")
#define MSG_SET_LEDS_WHITE                  _UxGT("з™Ѕ") // "White")
#define MSG_SET_LEDS_DEFAULT                _UxGT("зјєзњЃ") // "Default")
#define MSG_CUSTOM_LEDS                     _UxGT("е®ље€¶зЃЇ") // "Custom Lights")
#define MSG_INTENSITY_R                     _UxGT("зєўйҐ±е’Њеє¦") // "Red Intensity")
#define MSG_INTENSITY_G                     _UxGT("з»їйҐ±е’Њеє¦") // "Green Intensity")
#define MSG_INTENSITY_B                     _UxGT("и“ќйҐ±е’Њеє¦") // "Blue Intensity")
#define MSG_INTENSITY_W                     _UxGT("з™ЅйҐ±е’Њеє¦") // "White Intensity")
#define MSG_LED_BRIGHTNESS                  _UxGT("дє®еє¦") // "Brightness")
#define MSG_MOVING                          _UxGT("з§»еЉЁ ...") // "Moving...")
#define MSG_FREE_XY                         _UxGT("й‡Љж”ѕ XY") // "Free XY")

#define MSG_MOVE_X                          _UxGT("з§»еЉЁX")  //"Move X"
#define MSG_MOVE_Y                          _UxGT("з§»еЉЁY")  //"Move Y"
#define MSG_MOVE_Z                          _UxGT("з§»еЉЁZ")  //"Move Z"
#define MSG_MOVE_E                          _UxGT("жЊ¤е‡єжњє")  //"Extruder"
#define MSG_MOVE_01MM                       _UxGT("з§»еЉЁ 0.1 mm")  //"Move 0.1mm"
#define MSG_MOVE_1MM                        _UxGT("з§»еЉЁ 1 mm")  //"Move 1mm"
#define MSG_MOVE_10MM                       _UxGT("з§»еЉЁ 10 mm")  //"Move 10mm"
#define MSG_SPEED                           _UxGT("йЂџзЋ‡")  //"Speed"
#define MSG_BED_Z                           _UxGT("зѓ­еєЉZ")  //"Bed Z"
#define MSG_NOZZLE                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" е–·е�ґ")  //"Nozzle" е™ґе�ґ
#define MSG_BED                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" зѓ­еєЉ")  //"Bed"
#define MSG_FAN_SPEED                       _UxGT("йЈЋж‰‡йЂџзЋ‡")  //"Fan speed"
#define MSG_EXTRA_FAN_SPEED                 _UxGT("йўќе¤–йЈЋж‰‡йЂџзЋ‡") // "Extra fan speed"
#define MSG_FLOW                            _UxGT("жЊ¤е‡єйЂџзЋ‡")  //"Flow"
#define MSG_CONTROL                         _UxGT("жЋ§е€¶")  //"Control"
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" жњЂе°Џ") //" " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" жњЂе¤§")  //" " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" е› ж•°")  //" " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        _UxGT("и‡ЄеЉЁжЋ§жё©")  //"Autotemp"
#define MSG_ON                              _UxGT("ејЂ ")  //"On "
#define MSG_OFF                             _UxGT("е…і ")  //"Off"
#define MSG_PID_P                           _UxGT("PID-P")  //"PID-P"
#define MSG_PID_I                           _UxGT("PID-I")  //"PID-I"
#define MSG_PID_D                           _UxGT("PID-D")  //"PID-D"
#define MSG_PID_C                           _UxGT("PID-C")  //"PID-C"
#define MSG_SELECT                          _UxGT("йЂ‰ж‹©")  //"Select"
#define MSG_ACC                             _UxGT("еЉ йЂџеє¦")  //"Accel" acceleration
#define MSG_JERK                            _UxGT("жЉ–еЉЁйЂџзЋ‡")  // "Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("AиЅґжЉ–еЉЁйЂџзЋ‡")  //"Va-jerk"
  #define MSG_VB_JERK                       _UxGT("BиЅґжЉ–еЉЁйЂџзЋ‡")  //"Vb-jerk"
  #define MSG_VC_JERK                       _UxGT("CиЅґжЉ–еЉЁйЂџзЋ‡")  //"Vc-jerk"
#else
  #define MSG_VA_JERK                       _UxGT("XиЅґжЉ–еЉЁйЂџзЋ‡")  //"Vx-jerk"
  #define MSG_VB_JERK                       _UxGT("YиЅґжЉ–еЉЁйЂџзЋ‡")  //"Vy-jerk"
  #define MSG_VC_JERK                       _UxGT("ZиЅґжЉ–еЉЁйЂџзЋ‡")  //"Vz-jerk"
#endif
#define MSG_VE_JERK                         _UxGT("жЊ¤е‡єжњєжЉ–еЉЁйЂџзЋ‡")  //"Ve-jerk"
#define MSG_VELOCITY                        _UxGT("йЂџеє¦")          // "Velocity"
#define MSG_VMAX                            _UxGT("жњЂе¤§иї›ж–™йЂџзЋ‡")  //"Vmax " max_feedrate_mm_s
#define MSG_VMIN                            _UxGT("жњЂе°Џиї›ж–™йЂџзЋ‡")  //"Vmin"  min_feedrate_mm_s
#define MSG_VTRAV_MIN                       _UxGT("жњЂе°Џз§»еЉЁйЂџзЋ‡")  //"VTrav min" min_travel_feedrate_mm_s, (target) speed of the move
#define MSG_ACCELERATION                    _UxGT("еЉ йЂџеє¦")       // "Acceleration"
#define MSG_AMAX                            _UxGT("жњЂе¤§ж‰“еЌ°еЉ йЂџеє¦")  //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
#define MSG_A_RETRACT                       _UxGT("ж”¶иї›еЉ йЂџеє¦")  //"A-retract" retract_acceleration, E acceleration in mm/s^2 for retracts
#define MSG_A_TRAVEL                        _UxGT("йќћж‰“еЌ°з§»еЉЁеЉ йЂџеє¦")  //"A-travel" travel_acceleration, X, Y, Z acceleration in mm/s^2 for travel (non printing) moves
#define MSG_STEPS_PER_MM                    _UxGT("иЅґж­Ґж•°/mm")  //"Steps/mm" axis_steps_per_mm, axis steps-per-unit G92
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("AиЅґж­Ґж•°/mm")  //"Asteps/mm"
  #define MSG_BSTEPS                        _UxGT("BиЅґж­Ґж•°/mm")  //"Bsteps/mm"
  #define MSG_CSTEPS                        _UxGT("CиЅґж­Ґж•°/mm")  //"Csteps/mm"
#else
  #define MSG_ASTEPS                        _UxGT("XиЅґж­Ґж•°/mm")  //"Xsteps/mm"
  #define MSG_BSTEPS                        _UxGT("YиЅґж­Ґж•°/mm")  //"Ysteps/mm"
  #define MSG_CSTEPS                        _UxGT("ZиЅґж­Ґж•°/mm")  //"Zsteps/mm"
#endif
#define MSG_ESTEPS                          _UxGT("жЊ¤е‡єжњєж­Ґж•°/mm")  //"Esteps/mm"
#define MSG_E1STEPS                         _UxGT("жЊ¤е‡єжњє1ж­Ґж•°/mm") //"E1steps/mm"
#define MSG_E2STEPS                         _UxGT("жЊ¤е‡єжњє2ж­Ґж•°/mm") //"E2steps/mm"
#define MSG_E3STEPS                         _UxGT("жЊ¤е‡єжњє3ж­Ґж•°/mm") //"E3steps/mm"
#define MSG_E4STEPS                         _UxGT("жЊ¤е‡єжњє4ж­Ґж•°/mm") //"E4steps/mm"
#define MSG_E5STEPS                         _UxGT("жЊ¤е‡єжњє5ж­Ґж•°/mm") //"E5steps/mm"
#define MSG_TEMPERATURE                     _UxGT("жё©еє¦")  //"Temperature"
#define MSG_MOTION                          _UxGT("иїђеЉЁ")  //"Motion"
#define MSG_FILAMENT                        _UxGT("дёќж–™жµ‹е®№")  //"Filament" lcd_control_volumetric_menu
#define MSG_VOLUMETRIC_ENABLED              _UxGT("жµ‹е®№з§ЇmmВі")  //"E in mm3" volumetric_enabled
#define MSG_FILAMENT_DIAM                   _UxGT("дёќж–™з›ґеѕ„")  //"Fil. Dia."
#define MSG_FILAMENT_UNLOAD                 _UxGT("еЌёиЅЅ mm") // "Unload mm"
#define MSG_FILAMENT_LOAD                   _UxGT("иЈ…иЅЅ mm")   // "Load mm"
#define MSG_ADVANCE_K                       _UxGT("Advance K") // "Advance K"
#define MSG_CONTRAST                        _UxGT("LCDеЇ№жЇ”еє¦")  //"LCD contrast"
#define MSG_STORE_EEPROM                    _UxGT("дїќе­�и®ѕзЅ®")  //"Store memory"
#define MSG_LOAD_EEPROM                     _UxGT("иЈ…иЅЅи®ѕзЅ®")  //"Load memory"
#define MSG_RESTORE_FAILSAFE                _UxGT("жЃўе¤Ќе®‰е…ЁеЂј")  //"Restore failsafe"
#define MSG_INIT_EEPROM                     _UxGT("е€ќе§‹еЊ–и®ѕзЅ®") // "Initialize EEPROM"
#define MSG_REFRESH                         _UxGT("е€·ж–°")  //"Refresh"
#define MSG_WATCH                           _UxGT("дїЎжЃЇе±Џ")  //"Info screen"
#define MSG_PREPARE                         _UxGT("е‡†е¤‡")  //"Prepare"
#define MSG_TUNE                            _UxGT("и°ѓж•ґ")  //"Tune"
#define MSG_PAUSE_PRINT                     _UxGT("жљ‚еЃњж‰“еЌ°")  //"Pause print"
#define MSG_RESUME_PRINT                    _UxGT("жЃўе¤Ќж‰“еЌ°")  //"Resume print"
#define MSG_STOP_PRINT                      _UxGT("еЃњж­ўж‰“еЌ°")  //"Stop print"
#define MSG_CARD_MENU                       _UxGT("д»Ће­�е‚ЁеЌЎдёЉж‰“еЌ°")  //"Print from SD"
#define MSG_NO_CARD                         _UxGT("ж— е­�е‚ЁеЌЎ")  //"No SD card"
#define MSG_DWELL                           _UxGT("дј‘зњ дё­ ...")  //"Sleep..."
#define MSG_USERWAIT                        _UxGT("з‚№е‡»з»§з»­ ...")  //"Click to resume..."
#define MSG_PRINT_PAUSED                    _UxGT("жљ«еЃњж‰“еЌ°") // "Print paused"
#define MSG_PRINT_ABORTED                   _UxGT("е·ІеЏ–ж¶€ж‰“еЌ°")  //"Print aborted"
#define MSG_NO_MOVE                         _UxGT("ж— з§»еЉЁ")  //"No move."
#define MSG_KILLED                          _UxGT("е·ІжќЂжЋ‰")  //"KILLED. "
#define MSG_STOPPED                         _UxGT("е·ІеЃњж­ў")  //"STOPPED. "
#define MSG_CONTROL_RETRACT                 _UxGT("е›ћжЉЅй•їеє¦mm")  //"Retract mm" retract_length, retract length (positive mm)
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("жЌўж‰‹е›ћжЉЅй•їеє¦mm")  //"Swap Re.mm" swap_retract_length, swap retract length (positive mm), for extruder change
#define MSG_CONTROL_RETRACTF                _UxGT("е›ћжЉЅйЂџзЋ‡mm/s")  //"Retract  V" retract_feedrate_mm_s, feedrate for retracting (mm/s)
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")  //"Hop mm" retract_zlift, retract Z-lift
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("е›ћжЉЅжЃўе¤Ќй•їеє¦mm")  //"UnRet +mm" retract_recover_length, additional recover length (mm, added to retract length when recovering)
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("жЌўж‰‹е›ћжЉЅжЃўе¤Ќй•їеє¦mm")  //"S UnRet+mm" swap_retract_recover_length, additional swap recover length (mm, added to retract length when recovering from extruder change)
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("е›ћжЉЅжЃўе¤ЌеђЋиї›ж–™йЂџзЋ‡mm/s")  //"UnRet  V" retract_recover_feedrate_mm_s, feedrate for recovering from retraction (mm/s)
#define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("S UnRet V") // "S UnRet V"
#define MSG_AUTORETRACT                     _UxGT("и‡ЄеЉЁжЉЅе›ћ")  //"AutoRetr." autoretract_enabled,
#define MSG_FILAMENTCHANGE                  _UxGT("ж›ґжЌўдёќж–™")  //"Change filament"
#define MSG_FILAMENTLOAD                    _UxGT("иЈ…иЅЅдёќж–™") // "Load filament"
#define MSG_FILAMENTUNLOAD                  _UxGT("еЌёиЅЅдёќж–™") // "Unload filament"
#define MSG_FILAMENTUNLOAD_ALL              _UxGT("еЌёиЅЅе…ЁйѓЁ") // "Unload All"
#define MSG_INIT_SDCARD                     _UxGT("е€ќе§‹еЊ–е­�е‚ЁеЌЎ")  //"Init. SD card"
#define MSG_CNG_SDCARD                      _UxGT("ж›ґжЌўе­�е‚ЁеЌЎ")  //"Change SD card"
#define MSG_ZPROBE_OUT                      _UxGT("ZжЋўй’€ењЁзѓ­еєЉд№‹е¤–")  //"Z probe out. bed" Z probe is not within the physical limits
#define MSG_SKEW_FACTOR                     _UxGT("еЃЏж–ње› ж•°") // "Skew Factor"
#define MSG_BLTOUCH                         _UxGT("BLTouch")     // "BLTouch"
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch и‡ЄжЈЂ") // "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   _UxGT("й‡ЌзЅ®BLTouch")  // "Reset BLTouch"
#define MSG_BLTOUCH_DEPLOY                  _UxGT("йѓЁзЅІBLTouch") // "Deploy BLTouch"
#define MSG_BLTOUCH_STOW                    _UxGT("иЈ…иЅЅBLTouch")   // "Stow BLTouch"
#define MSG_HOME                            _UxGT("еЅ’дЅЌ")  //"Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("е…€")  //"first"
#define MSG_ZPROBE_ZOFFSET                  _UxGT("ZеЃЏз§»")  //"Z Offset"
#define MSG_BABYSTEP_X                      _UxGT("еѕ®й‡Џи°ѓж•ґXиЅґ")  //"Babystep X" lcd_babystep_x, Babystepping enables the user to control the axis in tiny amounts
#define MSG_BABYSTEP_Y                      _UxGT("еѕ®й‡Џи°ѓж•ґYиЅґ")  //"Babystep Y"
#define MSG_BABYSTEP_Z                      _UxGT("еѕ®й‡Џи°ѓж•ґZиЅґ")  //"Babystep Z"
#define MSG_ENDSTOP_ABORT                   _UxGT("жЊЎеќ—з»€ж­ў")  //"Endstop abort"
#define MSG_HEATING_FAILED_LCD              _UxGT("еЉ зѓ­е¤±иґҐ")  //"Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("й”™иЇЇпјљREDUNDANT TEMP")  //"Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 _UxGT("жё©жЋ§е¤±жЋ§")  //"THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     _UxGT("й”™иЇЇпјљжњЂй«�жё©еє¦")  //"Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     _UxGT("й”™иЇЇпјљжњЂдЅЋжё©еє¦")  //"Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 _UxGT("й”™иЇЇпјљжњЂй«�зѓ­еєЉжё©еє¦")  //"Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 _UxGT("й”™иЇЇпјљжњЂдЅЋзѓ­еєЉжё©еє¦")  //"Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("ж‰“еЌ°еЃњжњє")  //"PRINTER HALTED"
#define MSG_PLEASE_RESET                    _UxGT("иЇ·й‡ЌзЅ®")  //"Please reset"
#define MSG_SHORT_DAY                       _UxGT("е¤©")  //"d" // One character only
#define MSG_SHORT_HOUR                      _UxGT("ж—¶")  //"h" // One character only
#define MSG_SHORT_MINUTE                    _UxGT("е€†")  //"m" // One character only
#define MSG_HEATING                         _UxGT("еЉ зѓ­дё­ ...")  //"Heating..."
#define MSG_BED_HEATING                     _UxGT("еЉ зѓ­зѓ­еєЉдё­...")  //"Bed Heating..."
#define MSG_DELTA_CALIBRATE                 _UxGT("вЉїж Ўе‡†")  //"Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               _UxGT("вЉїж Ўе‡†X")  //"Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               _UxGT("вЉїж Ўе‡†Y")  //"Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               _UxGT("вЉїж Ўе‡†Z")  //"Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("вЉїж Ўе‡†дё­еїѓ")  //"Calibrate Center"
#define MSG_DELTA_SETTINGS                  _UxGT("вЉїи®ѕзЅ®") // "Delta Settings"
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("вЉїи‡ЄеЉЁж Ўе‡†") // "Auto Calibration"
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("и®ѕзЅ®вЉїй«�еє¦") // "Set Delta Height"
#define MSG_DELTA_DIAG_ROD                  _UxGT("вЉїж–њжџ±") // "Diag Rod"
#define MSG_DELTA_HEIGHT                    _UxGT("вЉїй«�еє¦") // "Height"
#define MSG_DELTA_RADIUS                    _UxGT("вЉїеЌЉеѕ„") // "Radius"
#define MSG_INFO_MENU                       _UxGT("е…ідєЋж‰“еЌ°жњє")  //"About Printer"
#define MSG_INFO_PRINTER_MENU               _UxGT("ж‰“еЌ°жњєдїЎжЃЇ")  //"Printer Info"
#define MSG_3POINT_LEVELING                 _UxGT("дё‰з‚№и°ѓе№і") // "3-Point Leveling"
#define MSG_LINEAR_LEVELING                 _UxGT("зєїжЂ§и°ѓе№і") // "Linear Leveling"
#define MSG_BILINEAR_LEVELING               _UxGT("еЏЊзєїжЂ§и°ѓе№і") // "Bilinear Leveling"
#define MSG_UBL_LEVELING                    _UxGT("з»џдёЂзѓ­еєЉи°ѓе№і(UBL)") // "Unified Bed Leveling"
#define MSG_MESH_LEVELING                   _UxGT("зЅ‘ж ји°ѓе№і") // "Mesh Leveling"
#define MSG_INFO_STATS_MENU                 _UxGT("ж‰“еЌ°жњєз»џи®Ў")  //"Printer Stats"
#define MSG_INFO_BOARD_MENU                 _UxGT("дё»жќїдїЎжЃЇ")  //"Board Info"
#define MSG_INFO_THERMISTOR_MENU            _UxGT("жё©еє¦и®Ў")  //"Thermistors"
#define MSG_INFO_EXTRUDERS                  _UxGT("жЊ¤е‡єжњє")  //"Extruders"
#define MSG_INFO_BAUDRATE                   _UxGT("жіўз‰№зЋ‡")  //"Baud"
#define MSG_INFO_PROTOCOL                   _UxGT("еЌЏи®®")  //"Protocol"
#define MSG_CASE_LIGHT                      _UxGT("е¤–еЈізЃЇ") // "Case light"
#define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("зЃЇдє®еє¦") // "Light BRIGHTNESS"

#if LCD_WIDTH > 19
#define MSG_INFO_PRINT_COUNT              _UxGT("ж‰“еЌ°и®Ўж•°")  //"Print Count"
#define MSG_INFO_COMPLETED_PRINTS         _UxGT("е®Њж€ђдє†")  //"Completed"
#define MSG_INFO_PRINT_TIME               _UxGT("жЂ»ж‰“еЌ°ж—¶й—ґ")  //"Total print time"
#define MSG_INFO_PRINT_LONGEST            _UxGT("жњЂй•їе·ҐдЅњж—¶й—ґ")  //"Longest job time"
#define MSG_INFO_PRINT_FILAMENT           _UxGT("жЂ»и®ЎжЊ¤е‡є")  //"Extruded total"
#else
#define MSG_INFO_PRINT_COUNT              _UxGT("ж‰“еЌ°ж•°")  //"Prints"
#define MSG_INFO_COMPLETED_PRINTS         _UxGT("е®Њж€ђ")  //"Completed"
#define MSG_INFO_PRINT_TIME               _UxGT("жЂ»е…±")  //"Total"
#define MSG_INFO_PRINT_LONGEST            _UxGT("жњЂй•ї")  //"Longest"
#define MSG_INFO_PRINT_FILAMENT           _UxGT("е·ІжЊ¤е‡є")  //"Extruded"
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("жњЂдЅЋжё©еє¦")  //"Min Temp"
#define MSG_INFO_MAX_TEMP                   _UxGT("жњЂй«�жё©еє¦")  //"Max Temp"
#define MSG_INFO_PSU                        _UxGT("з”µжєђдѕ›еє”")  //"Power Supply"
#define MSG_DRIVE_STRENGTH                  _UxGT("й©±еЉЁеЉ›еє¦") // "Drive Strength"
#define MSG_DAC_PERCENT                     _UxGT("й©±еЉЁ %") // "Driver %"
#define MSG_DAC_EEPROM_WRITE                _UxGT("дїќе­�й©±еЉЁи®ѕзЅ®") // "DAC EEPROM Write"
#define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("ж‰“еЌ°е·Іжљ‚еЃњ") // "PRINT PAUSED"
#define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("иЈ…иЅЅдёќж–™") // "LOAD FILAMENT"
#define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("еЌёиЅЅдёќж–™") // "UNLOAD FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("жЃўе¤ЌйЂ‰йЎ№:") // "RESUME OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("жё…й™¤ж›ґе¤љ") // "Purge more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("жЃўе¤Ќж‰“еЌ°")  //"Resume print"
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  е–·е�ґ: ") // "  Nozzle: "
#define MSG_ERR_HOMING_FAILED               _UxGT("еЅ’еЋџдЅЌе¤±иґҐ") // "Homing failed"
#define MSG_ERR_PROBING_FAILED              _UxGT("жЋўй’€жЋўжµ‹е¤±иґҐ") // "Probing failed"
#define MSG_M600_TOO_COLD                   _UxGT("M600: е¤Єе‡‰") // "M600: Too cold"

#if LCD_HEIGHT >= 4
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("з­‰еѕ…ејЂе§‹")  //"Wait for start"
#define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("дёќж–™")  //"of the filament"
#define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("еЏ�ж›ґ")  //"change"
#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("з­‰еѕ…")  //"Wait for"
#define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("еЌёдё‹дёќж–™")  //"filament unload"
#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("жЏ’е…Ґдёќж–™")  //"Insert filament"
#define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("е№¶жЊ‰й”®")  //"and press button"
#define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("жќҐз»§з»­ ...")  //"to continue..."
#define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("жЊ‰дё‹жЊ‰й’®жќҐ") // "Press button to"
#define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("еЉ зѓ­е–·е�ґ.") // "heat nozzle."
#define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("еЉ зѓ­е–·е�ґ") // "Heating nozzle"
#define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("иЇ·з­‰еѕ… ...") // "Please wait..."
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("з­‰еѕ…")  //"Wait for"
#define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("иї›ж–™")  //"filament load"
#define MSG_FILAMENT_CHANGE_PURGE_1         _UxGT("з­‰еѕ…") // "Wait for"
#define MSG_FILAMENT_CHANGE_PURGE_2         _UxGT("дёќж–™жё…й™¤") // "filament purge"
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("з­‰еѕ…ж‰“еЌ°")  //"Wait for print"
#define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("жЃўе¤Ќ")  //"to resume"

#else // LCD_HEIGHT < 4
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("иЇ·з­‰еѕ… ...")  //"Please wait..."
#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("йЂЂе‡єдё­ ...")  //"Ejecting..."
#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("жЏ’е…Ґе№¶еЌ•е‡»")  //"Insert and Click"
#define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("еЉ зѓ­дё­ ...") // "Heating..."
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("иЈ…иЅЅдё­ ...")  //"Loading..."
#define MSG_FILAMENT_CHANGE_PURGE_1         _UxGT("жё…й™¤дё­ ...") // "Purging..."
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("жЃўе¤Ќдё­ ...")  //"Resuming..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_ZH_CN_H


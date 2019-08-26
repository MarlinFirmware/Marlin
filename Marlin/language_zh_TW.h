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
 * Traditional Chinese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_ZH_TW_H
#define LANGUAGE_ZH_TW_H

#define CHARSIZE 3

#define WELCOME_MSG                         MACHINE_NAME _UxGT("е·Іе°±з·’.")  //" ready."
#define MSG_BACK                            _UxGT("иї”е›ћ")         // вЂќBackвЂњ
#define MSG_SD_INSERTED                     _UxGT("иЁ�ж†¶еЌЎе·ІжЏ’е…Ґ")  //"Card inserted"
#define MSG_SD_REMOVED                      _UxGT("иЁ�ж†¶еЌЎиў«ж‹”е‡є")  //"Card removed"
#define MSG_LCD_ENDSTOPS                    _UxGT("ж“‹еЎЉ")  //"Endstops" // Max length 8 characters
#define MSG_MAIN                            _UxGT("дё»йЃёе–®")  //"Main"
#define MSG_AUTOSTART                       _UxGT("и‡Єе‹•й–‹е§‹")  //"Autostart"
#define MSG_DISABLE_STEPPERS                _UxGT("й—њй–‰ж­ҐйЂІй¦¬йЃ”")  //"Disable steppers"
#define MSG_DEBUG_MENU                      _UxGT("й™¤йЊЇйЃёе–®")    // "Debug Menu"
#define MSG_PROGRESS_BAR_TEST               _UxGT("йЂІеє¦жўќжё¬и©¦")  // "Progress Bar Test"
#define MSG_AUTO_HOME                       _UxGT("и‡Єе‹•е›ћеЋџй»ћ")  //"Auto home"
#define MSG_AUTO_HOME_X                     _UxGT("е›ћXеЋџй»ћ")  //"Home X"
#define MSG_AUTO_HOME_Y                     _UxGT("е›ћYеЋџй»ћ")  //"Home Y"
#define MSG_AUTO_HOME_Z                     _UxGT("е›ћZеЋџй»ћ")  //"Home Z"
#define MSG_TMC_Z_CALIBRATION               _UxGT("вЉїж Ўжє–Z")  //"Calibrate Z"
#define MSG_LEVEL_BED_HOMING                _UxGT("е№іеЏ°иЄїе№іXYZж­ёеЋџй»ћ")  //"Homing XYZ"
#define MSG_LEVEL_BED_WAITING               _UxGT("е–®ж“Љй–‹е§‹з†±еєЉиЄїе№і")  //"Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("дё‹еЂ‹з†±еєЉиЄїе№ій»ћ")  //"Next Point"
#define MSG_LEVEL_BED_DONE                  _UxGT("е®Њж€ђз†±еєЉиЄїе№і")  //"Leveling Done!"
#define MSG_Z_FADE_HEIGHT                   _UxGT("ж·Ўе‡єй«�еє¦")     // "Fade Height"
#define MSG_SET_HOME_OFFSETS                _UxGT("иЁ­зЅ®еЋџй»ћеЃЏз§»")  //"Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("еЃЏз§»е·Іе•џз”Ё")  //"Offsets applied"
#define MSG_SET_ORIGIN                      _UxGT("иЁ­зЅ®еЋџй»ћ")  //"Set origin"
#define MSG_PREHEAT_1                       _UxGT("й ђз†±PLA")  //"Preheat PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")  //MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" е…ЁйѓЁ")  //MSG_PREHEAT_1 " All"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" з†±еєЉ")  //MSG_PREHEAT_1 " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" иЁ­зЅ®")  //MSG_PREHEAT_1 " conf"
#define MSG_PREHEAT_2                       _UxGT("й ђз†±ABS")  //"Preheat ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")  //MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" е…ЁйѓЁ")  //MSG_PREHEAT_2 " All"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" з†±еєЉ")  //MSG_PREHEAT_2 " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" иЁ­зЅ®")  //MSG_PREHEAT_2 " conf"
#define MSG_COOLDOWN                        _UxGT("й™Ќжє«")  //"Cooldown"
#define MSG_SWITCH_PS_ON                    _UxGT("й›»жєђж‰“й–‹")  //"Switch power on"
#define MSG_SWITCH_PS_OFF                   _UxGT("й›»жєђй—њй–‰")  //"Switch power off"
#define MSG_EXTRUDE                         _UxGT("ж“ е‡є")  //"Extrude"
#define MSG_RETRACT                         _UxGT("е›ћзё®")  //"Retract"
#define MSG_MOVE_AXIS                       _UxGT("з§»е‹•и»ё")  //"Move axis"
#define MSG_BED_LEVELING                    _UxGT("иЄїе№із†±еєЉ")  //"Bed leveling"
#define MSG_LEVEL_BED                       _UxGT("иЄїе№із†±еєЉ")  //"Level bed"
#define MSG_LEVEL_CORNERS                   _UxGT("иЄїе№ій‚Љи§’") // "Level corners"

#define MSG_NEXT_CORNER                     _UxGT("дё‹дёЄй‚Љи§’") // "Next corner"
#define MSG_EDITING_STOPPED                 _UxGT("з¶Іж јз·ЁијЇе·ІеЃњж­ў") // "Mesh Editing Stopped"

#define MSG_USER_MENU                       _UxGT("е®ўе€¶е‘Ѕд»¤") // "Custom Commands"
#define MSG_UBL_DOING_G29                   _UxGT("ж‰§иЎЊG29") // "Doing G29"
#define MSG_UBL_UNHOMED                     _UxGT("е…€е›ћXYZеЋџй»ћ") // "Home XYZ first"
#define MSG_UBL_TOOLS                       _UxGT("UBLе·Ґе…·") // "UBL Tools"
#define MSG_UBL_LEVEL_BED                   _UxGT("зµ±дёЂз†±еєЉиЄїе№і(UBL)") // "Unified Bed Leveling"
#define MSG_UBL_MANUAL_MESH                 _UxGT("ж‰‹е·Ґе»єзЅ‘") // "Manually Build Mesh"

#define MSG_UBL_BC_INSERT                   _UxGT("ж”ѕзЅ®еўЉз‰‡дё¦жё¬й‡Џ") // "Place shim & measure"
#define MSG_UBL_BC_INSERT2                  _UxGT("жё¬й‡Џ") // "Measure"
#define MSG_UBL_BC_REMOVE                   _UxGT("з§»й™¤дё¦жё¬й‡Џз†±еєЉ") // "Remove & measure bed"
#define MSG_UBL_MOVING_TO_NEXT              _UxGT("з§»е‹•е€°дё‹дёЂеЂ‹") // "Moving to next"
#define MSG_UBL_ACTIVATE_MESH               _UxGT("жїЂжґ»UBL") // "Activate UBL"
#define MSG_UBL_DEACTIVATE_MESH             _UxGT("й—њй–‰UBL") // "Deactivate UBL"
#define MSG_UBL_SET_BED_TEMP                _UxGT("иЁ­зЅ®з†±еєЉжє«еє¦") // "Bed Temp"
#define MSG_UBL_SET_HOTEND_TEMP             _UxGT("з†±з«Їжє«еє¦") // "Hotend Temp"
#define MSG_UBL_MESH_EDIT                   _UxGT("з¶Іж јз·ЁијЇ") // "Mesh Edit"
#define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("з·ЁијЇе®ўж€¶з¶Іж ј") // "Edit Custom Mesh"
#define MSG_UBL_FINE_TUNE_MESH              _UxGT("зґ°иЄїз¶Іж ј") // "Fine Tuning Mesh"
#define MSG_UBL_DONE_EDITING_MESH           _UxGT("е®Њж€ђз·ЁијЇз¶Іж ј") // "Done Editing Mesh"
#define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("е‰µиЁ­е®ўж€¶з¶Іж ј") // "Build Custom Mesh"
#define MSG_UBL_BUILD_MESH_MENU             _UxGT("е‰µиЁ­з¶Іж ј") // "Build Mesh"
#define MSG_UBL_BUILD_PLA_MESH              _UxGT("е‰µиЁ­PLAз¶Іж ј") // "Build PLA Mesh"
#define MSG_UBL_BUILD_ABS_MESH              _UxGT("е‰µиЁ­ABSз¶Іж ј") // "Build ABS Mesh"
#define MSG_UBL_BUILD_COLD_MESH             _UxGT("е‰µиЁ­е†·з¶Іж ј") // "Build Cold Mesh"
#define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("иЄїж•ґз¶Іж јй«�еє¦") // "Adjust Mesh Height"
#define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("й«�еє¦еђ€иЁ€") // "Height Amount"
#define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("ж‰№е‡†з¶Іж ј") // "Validate Mesh"
#define MSG_UBL_VALIDATE_PLA_MESH           _UxGT("ж‰№е‡†PLAз¶Іж ј") // "Validate PLA Mesh"
#define MSG_UBL_VALIDATE_ABS_MESH           _UxGT("ж‰№е‡†ABSз¶Іж ј") // "Validate ABS Mesh"
#define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("ж‰№е‡†е®ўж€¶з¶Іж ј") // "Validate Custom Mesh"
#define MSG_UBL_CONTINUE_MESH               _UxGT("з№јзєЊз†±еєЉз¶Іж ј") // "Continue Bed Mesh"
#define MSG_UBL_MESH_LEVELING               _UxGT("з¶Іж јиЄїе№і") // "Mesh Leveling"
#define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("дё‰й»ћиЄїе№і") // "3-Point Leveling"
#define MSG_UBL_GRID_MESH_LEVELING          _UxGT("ж је­ђз¶Іж јиЄїе№і") // "Grid Mesh Leveling"
#define MSG_UBL_MESH_LEVEL                  _UxGT("иЄїе№із¶Іж ј") // "Level Mesh"
#define MSG_UBL_SIDE_POINTS                 _UxGT("й‚Љй»ћ") // "Side Points"
#define MSG_UBL_MAP_TYPE                    _UxGT("ењ–йЎћећ‹") // "Map Type"
#define MSG_UBL_OUTPUT_MAP                  _UxGT("ијёе‡єз¶Іж јењ–") // "Output Mesh Map"
#define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("ијёе‡єе€°дё»ж©џ") // "Output for Host"
#define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("ијёе‡єе€°CSV") // "Output for CSV"
#define MSG_UBL_OUTPUT_MAP_BACKUP           _UxGT("ијёе‡єе€°е‚™д»Ѕ") // "Off Printer Backup"
#define MSG_UBL_INFO_UBL                    _UxGT("ијёе‡єUBLдїЎжЃЇ") // "Output UBL Info"
#define MSG_UBL_EDIT_MESH_MENU              _UxGT("з·ЁијЇз¶Іж ј") // "Edit Mesh"
#define MSG_UBL_FILLIN_AMOUNT               _UxGT("еЎ«е……еђ€иЁ€") // "Fill-in Amount"
#define MSG_UBL_MANUAL_FILLIN               _UxGT("ж‰‹е·ҐеЎ«е……") // "Manual Fill-in"
#define MSG_UBL_SMART_FILLIN                _UxGT("иЃ°ж�ЋеЎ«е……") // "Smart Fill-in"
#define MSG_UBL_FILLIN_MESH                 _UxGT("еЎ«е……з¶Іж ј") // "Fill-in Mesh"
#define MSG_UBL_INVALIDATE_ALL              _UxGT("дЅње»ўж‰Ђжњ‰зљ„") // "Invalidate All"
#define MSG_UBL_INVALIDATE_CLOSEST          _UxGT("дЅње»ўжњЂиї‘зљ„") // "Invalidate Closest"
#define MSG_UBL_FINE_TUNE_ALL               _UxGT("зґ°иЄїж‰Ђжњ‰зљ„") // "Fine Tune All"
#define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("зґ°иЄїжњЂиї‘зљ„") // "Fine Tune Closest"
#define MSG_UBL_STORAGE_MESH_MENU           _UxGT("з¶Іж је­�е„І") // "Mesh Storage"
#define MSG_UBL_STORAGE_SLOT                _UxGT("е­�е„Іж§Ѕ") // "Memory Slot"
#define MSG_UBL_LOAD_MESH                   _UxGT("иЈќиј‰з†±еєЉз¶Іж ј") // "Load Bed Mesh"
#define MSG_UBL_SAVE_MESH                   _UxGT("дїќе­�з†±еєЉз¶Іж ј") // "Save Bed Mesh"
#define MSG_MESH_LOADED                     _UxGT("з¶Іж ј %i е·ІиЈќиј‰") // "Mesh %i loaded"
#define MSG_MESH_SAVED                      _UxGT("з¶Іж ј %i е·Ідїќе­�") // "Mesh %i saved"
#define MSG_NO_STORAGE                      _UxGT("жІ’жњ‰е­�е„І") // "No storage"
#define MSG_UBL_SAVE_ERROR                  _UxGT("йЊЇиЄ¤: UBLдїќе­�") // "Err: UBL Save"
#define MSG_UBL_RESTORE_ERROR               _UxGT("йЊЇиЄ¤: UBLй‚„еЋџ") // "Err: UBL Restore"
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
#define MSG_INTENSITY_R                     _UxGT("зєўйЈЅе’Њеє¦") // "Red Intensity")
#define MSG_INTENSITY_G                     _UxGT("з»їйЈЅе’Њеє¦") // "Green Intensity")
#define MSG_INTENSITY_B                     _UxGT("и“ќйЈЅе’Њеє¦") // "Blue Intensity")
#define MSG_INTENSITY_W                     _UxGT("з™ЅйЈЅе’Њеє¦") // "White Intensity")
#define MSG_LED_BRIGHTNESS                  _UxGT("дє®еє¦") // "Brightness")
#define MSG_MOVING                          _UxGT("з§»еЉЁ ...") // "Moving...")
#define MSG_FREE_XY                         _UxGT("й‡Љж”ѕ XY") // "Free XY")

#define MSG_MOVE_X                          _UxGT("з§»е‹•X")  //"Move X"
#define MSG_MOVE_Y                          _UxGT("з§»е‹•Y")  //"Move Y"
#define MSG_MOVE_Z                          _UxGT("з§»е‹•Z")  //"Move Z"
#define MSG_MOVE_E                          _UxGT("ж“ е‡єж©џ")  //"Extruder"
#define MSG_MOVE_01MM                       _UxGT("з§»е‹• 0.1 mm")  //"Move 0.1mm"
#define MSG_MOVE_1MM                        _UxGT("з§»е‹• 1 mm")  //"Move 1mm"
#define MSG_MOVE_10MM                       _UxGT("з§»е‹• 10 mm")  //"Move 10mm"
#define MSG_SPEED                           _UxGT("йЂџзЋ‡")  //"Speed"
#define MSG_BED_Z                           _UxGT("з†±еєЉZ")  //"Bed Z"
#define MSG_NOZZLE                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" е™ґе�ґ")  //"Nozzle" е™ґе�ґ
#define MSG_BED                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" з†±еєЉ")  //"Bed"
#define MSG_FAN_SPEED                       _UxGT("йўЁж‰‡йЂџзЋ‡")  //"Fan speed"
#define MSG_EXTRA_FAN_SPEED                 _UxGT("йЎЌе¤–йўЁж‰‡йЂџзЋ‡") // "Extra fan speed"
#define MSG_FLOW                            _UxGT("ж“ е‡єйЂџзЋ‡")  //"Flow"
#define MSG_CONTROL                         _UxGT("жЋ§е€¶")  //"Control"
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" жњЂе°Џ") //" " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" жњЂе¤§")  //" " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" зі»ж•ё")  //" " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        _UxGT("и‡Єе‹•жЋ§жє«")  //"Autotemp"
#define MSG_ON                              _UxGT("й–‹ ")  //"On "
#define MSG_OFF                             _UxGT("й—њ ")  //"Off"
#define MSG_PID_P                           _UxGT("PID-P")  //"PID-P"
#define MSG_PID_I                           _UxGT("PID-I")  //"PID-I"
#define MSG_PID_D                           _UxGT("PID-D")  //"PID-D"
#define MSG_PID_C                           _UxGT("PID-C")  //"PID-C"
#define MSG_SELECT                          _UxGT("йЃёж“‡")  //"Select"
#define MSG_ACC                             _UxGT("еЉ йЂџеє¦")  //"Accel" acceleration
#define MSG_JERK                            _UxGT("жЉ–е‹•йЂџзЋ‡")  //"Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Aи»ёжЉ–е‹•йЂџзЋ‡")  //"Va-jerk"
  #define MSG_VB_JERK                       _UxGT("Bи»ёжЉ–е‹•йЂџзЋ‡")  //"Vb-jerk"
  #define MSG_VC_JERK                       _UxGT("Cи»ёжЉ–е‹•йЂџзЋ‡")  //"Vc-jerk"
#else
  #define MSG_VA_JERK                       _UxGT("Xи»ёжЉ–е‹•йЂџзЋ‡")  //"Vx-jerk"
  #define MSG_VB_JERK                       _UxGT("Yи»ёжЉ–е‹•йЂџзЋ‡")  //"Vy-jerk"
  #define MSG_VC_JERK                       _UxGT("Zи»ёжЉ–е‹•йЂџзЋ‡")  //"Vz-jerk"
#endif
#define MSG_VE_JERK                         _UxGT("ж“ е‡єж©џжЉ–е‹•йЂџзЋ‡")  //"Ve-jerk"
#define MSG_VELOCITY                        _UxGT("йЂџеє¦")          // "Velocity"
#define MSG_VMAX                            _UxGT("жњЂе¤§йЂІж–™йЂџзЋ‡")  //"Vmax " max_feedrate_mm_s
#define MSG_VMIN                            _UxGT("жњЂе°ЏйЂІж–™йЂџзЋ‡")  //"Vmin"  min_feedrate_mm_s
#define MSG_VTRAV_MIN                       _UxGT("жњЂе°Џз§»е‹•йЂџзЋ‡")  //"VTrav min" min_travel_feedrate_mm_s, (target) speed of the move
#define MSG_ACCELERATION                    _UxGT("еЉ йЂџеє¦")       // "Acceleration"
#define MSG_AMAX                            _UxGT("жњЂе¤§е€—еЌ°еЉ йЂџеє¦")  //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
#define MSG_A_RETRACT                       _UxGT("е›ћзё®еЉ йЂџеє¦")  //"A-retract" retract_acceleration, E acceleration in mm/s^2 for retracts
#define MSG_A_TRAVEL                        _UxGT("йќће€—еЌ°з§»е‹•еЉ йЂџеє¦")  //"A-travel" travel_acceleration, X, Y, Z acceleration in mm/s^2 for travel (non printing) moves
#define MSG_STEPS_PER_MM                    _UxGT("и»ёж­Ґж•ё/mm")  //"Steps/mm" axis_steps_per_mm, axis steps-per-unit G92
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("Aи»ёж­Ґж•ё/mm")  //"Asteps/mm" axis_steps_per_mm, axis steps-per-unit G92
  #define MSG_BSTEPS                        _UxGT("Bи»ёж­Ґж•ё/mm")  //"Bsteps/mm"
  #define MSG_CSTEPS                        _UxGT("Cи»ёж­Ґж•ё/mm")  //"Csteps/mm"
#else
  #define MSG_ASTEPS                        _UxGT("Xи»ёж­Ґж•ё/mm")  //"Xsteps/mm" axis_steps_per_mm, axis steps-per-unit G92
  #define MSG_BSTEPS                        _UxGT("Yи»ёж­Ґж•ё/mm")  //"Ysteps/mm"
  #define MSG_CSTEPS                        _UxGT("Zи»ёж­Ґж•ё/mm")  //"Zsteps/mm"
#endif
#define MSG_ESTEPS                          _UxGT("ж“ е‡єж©џж­Ґж•ё/mm")  //"Esteps/mm"
#define MSG_E1STEPS                         _UxGT("ж“ е‡єж©џ1ж­Ґж•ё/mm") //"E1steps/mm"
#define MSG_E2STEPS                         _UxGT("ж“ е‡єж©џ2ж­Ґж•ё/mm") //"E2steps/mm"
#define MSG_E3STEPS                         _UxGT("ж“ е‡єж©џ3ж­Ґж•ё/mm") //"E3steps/mm"
#define MSG_E4STEPS                         _UxGT("ж“ е‡єж©џ4ж­Ґж•ё/mm") //"E4steps/mm"
#define MSG_E5STEPS                         _UxGT("ж“ е‡єж©џ5ж­Ґж•ё/mm") //"E5steps/mm"
#define MSG_TEMPERATURE                     _UxGT("жє«еє¦")  //"Temperature"
#define MSG_MOTION                          _UxGT("йЃ‹дЅњ")  //"Motion"
#define MSG_FILAMENT                        _UxGT("зµІж–™жё¬е®№")  //"Filament" lcd_control_volumetric_menu
#define MSG_VOLUMETRIC_ENABLED              _UxGT("жё¬е®№з©ЌmmВі")  //"E in mm3" volumetric_enabled
#define MSG_FILAMENT_DIAM                   _UxGT("зµІж–™з›ґеѕ‘")  //"Fil. Dia."
#define MSG_FILAMENT_UNLOAD                 _UxGT("еЌёиј‰ mm") // "Unload mm"
#define MSG_FILAMENT_LOAD                   _UxGT("иЈ…иј‰ mm")   // "Load mm"
#define MSG_ADVANCE_K                       _UxGT("Advance K") // "Advance K"
#define MSG_CONTRAST                        _UxGT("LCDе°ЌжЇ”еє¦")  //"LCD contrast"
#define MSG_STORE_EEPROM                    _UxGT("дїќе­�иЁ­зЅ®")  //"Store memory"
#define MSG_LOAD_EEPROM                     _UxGT("иј‰е…ҐиЁ­зЅ®")  //"Load memory"
#define MSG_RESTORE_FAILSAFE                _UxGT("жЃўеѕ©е®‰е…ЁеЂј")  //"Restore failsafe"
#define MSG_INIT_EEPROM                     _UxGT("е€ќе§‹еЊ–иЁ­зЅ®") // "Initialize EEPROM"
#define MSG_REFRESH                         _UxGT("е€·ж–°")  //"Refresh"
#define MSG_WATCH                           _UxGT("иі‡иЁЉз•Њйќў")  //"Info screen"
#define MSG_PREPARE                         _UxGT("жє–е‚™")  //"Prepare"
#define MSG_TUNE                            _UxGT("иЄїж•ґ")  //"Tune"
#define MSG_PAUSE_PRINT                     _UxGT("жљ«еЃње€—еЌ°")  //"Pause print"
#define MSG_RESUME_PRINT                    _UxGT("жЃўеѕ©е€—еЌ°")  //"Resume print"
#define MSG_STOP_PRINT                      _UxGT("еЃњж­ўе€—еЌ°")  //"Stop print"
#define MSG_CARD_MENU                       _UxGT("еѕћиЁ�ж†¶еЌЎдёЉе€—еЌ°")  //"Print from SD"
#define MSG_NO_CARD                         _UxGT("з„ЎиЁ�ж†¶еЌЎ")  //"No SD card"
#define MSG_DWELL                           _UxGT("дј‘зњ  ...")  //"Sleep..."
#define MSG_USERWAIT                        _UxGT("й»ћж“Љз№јзєЊ ...")  //"Click to resume..."
#define MSG_PRINT_PAUSED                    _UxGT("е€—еЌ°е·Іжљ«еЃњ") // "Print paused"
#define MSG_PRINT_ABORTED                   _UxGT("е·ІеЏ–ж¶€е€—еЌ°")  //"Print aborted"
#define MSG_NO_MOVE                         _UxGT("з„Ўз§»е‹•")  //"No move."
#define MSG_KILLED                          _UxGT("е·Із ЌжЋ‰")  //"KILLED. "
#define MSG_STOPPED                         _UxGT("е·ІеЃњж­ў")  //"STOPPED. "
#define MSG_CONTROL_RETRACT                 _UxGT("е›ћзё®й•·еє¦mm")  //"Retract mm" retract_length, retract length (positive mm)
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("жЏ›ж‰‹е›ћжЉЅй•·еє¦mm")  //"Swap Re.mm" swap_retract_length, swap retract length (positive mm), for extruder change
#define MSG_CONTROL_RETRACTF                _UxGT("е›ћзё®йЂџзЋ‡mm/s")  //"Retract  V" retract_feedrate_mm_s, feedrate for retracting (mm/s)
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")  //"Hop mm" retract_zlift, retract Z-lift
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("е›ћзё®жЃўеѕ©й•·еє¦mm")  //"UnRet +mm" retract_recover_length, additional recover length (mm, added to retract length when recovering)
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("жЏ›ж‰‹е›ћзё®жЃўеѕ©й•·еє¦mm")  //"S UnRet+mm" swap_retract_recover_length, additional swap recover length (mm, added to retract length when recovering from extruder change)
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("е›ћзё®жЃўеѕ©еѕЊйЂІж–™йЂџзЋ‡mm/s")  //"UnRet V" retract_recover_feedrate_mm_s, feedrate for recovering from retraction (mm/s)
#define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("S UnRet V") // "S UnRet V"
#define MSG_AUTORETRACT                     _UxGT("и‡Єе‹•е›ћзё®")  //"AutoRetr." autoretract_enabled,
#define MSG_FILAMENTCHANGE                  _UxGT("ж›ґжЏ›зµІж–™")  //"Change filament"
#define MSG_FILAMENTLOAD                    _UxGT("иЈќиј‰зµІж–™") // "Load filament"
#define MSG_FILAMENTUNLOAD                  _UxGT("еЌёиј‰зµІж–™") // "Unload filament"
#define MSG_FILAMENTUNLOAD_ALL              _UxGT("еЌёиј‰е…ЁйѓЁ") // "Unload All"
#define MSG_INIT_SDCARD                     _UxGT("е€ќе§‹еЊ–иЁ�ж†¶еЌЎ")  //"Init. SD card"
#define MSG_CNG_SDCARD                      _UxGT("ж›ґжЏ›иЁ�ж†¶еЌЎ")  //"Change SD card"
#define MSG_ZPROBE_OUT                      _UxGT("ZжЋўй‡ќењЁз†±еєЉд№‹е¤–")  //"Z probe out. bed" Z probe is not within the physical limits
#define MSG_SKEW_FACTOR                     _UxGT("еЃЏж–ње› ж•ё") // "Skew Factor"
#define MSG_BLTOUCH                         _UxGT("BLTouch")     // "BLTouch"
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch и‡ЄжЄў") // "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   _UxGT("й‡ЌзЅ®BLTouch")  // "Reset BLTouch"
#define MSG_BLTOUCH_DEPLOY                  _UxGT("йѓЁзЅІBLTouch") // "Deploy BLTouch"
#define MSG_BLTOUCH_STOW                    _UxGT("иЈќиј‰BLTouch")   // "Stow BLTouch"
#define MSG_HOME                            _UxGT("ж­ёдЅЌ")  //"Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("е…€")  //"first"
#define MSG_ZPROBE_ZOFFSET                  _UxGT("ZеЃЏз§»")  //"Z Offset"
#define MSG_BABYSTEP_X                      _UxGT("еѕ®й‡ЏиЄїж•ґXи»ё")  //"Babystep X" lcd_babystep_x, Babystepping enables the user to control the axis in tiny amounts
#define MSG_BABYSTEP_Y                      _UxGT("еѕ®й‡ЏиЄїж•ґYи»ё")  //"Babystep Y"
#define MSG_BABYSTEP_Z                      _UxGT("еѕ®й‡ЏиЄїж•ґZи»ё")  //"Babystep Z"
#define MSG_ENDSTOP_ABORT                   _UxGT("ж“‹еЎЉзµ‚ж­ў")  //"Endstop abort"
#define MSG_HEATING_FAILED_LCD              _UxGT("еЉ з†±е¤±ж•—")  //"Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("йЊЇиЄ¤пјљREDUNDANT TEMP")  //"Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 _UxGT("жє«жЋ§е¤±жЋ§")  //"THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     _UxGT("йЊЇиЄ¤пјљжњЂй«�жє«еє¦")  //"Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     _UxGT("йЊЇиЄ¤пјљжњЂдЅЋжє«еє¦")  //"Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 _UxGT("йЊЇиЄ¤пјљжњЂй«�з†±еєЉжє«еє¦")  //"Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 _UxGT("йЊЇиЄ¤пјљжњЂдЅЋз†±еєЉжє«еє¦")  //"Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("еЌ°иЎЁж©џеЃњж©џ")  //"PRINTER HALTED"
#define MSG_PLEASE_RESET                    _UxGT("и«‹й‡ЌзЅ®")  //"Please reset"
#define MSG_SHORT_DAY                       _UxGT("е¤©")  //"d" // One character only
#define MSG_SHORT_HOUR                      _UxGT("ж™‚")  //"h" // One character only
#define MSG_SHORT_MINUTE                    _UxGT("е€†")  //"m" // One character only
#define MSG_HEATING                         _UxGT("еЉ з†±дё­ ...")  //"Heating..."
#define MSG_BED_HEATING                     _UxGT("еЉ з†±з†±еєЉдё­...")  //"Bed Heating..."
#define MSG_DELTA_CALIBRATE                 _UxGT("вЉїж Ўжє–")  //"Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               _UxGT("вЉїж Ўжє–X")  //"Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               _UxGT("вЉїж Ўжє–Y")  //"Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               _UxGT("вЉїж Ўжє–Z")  //"Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("вЉїж Ўжє–дё­еїѓ")  //"Calibrate Center"
#define MSG_DELTA_SETTINGS                  _UxGT("вЉїиЁ­зЅ®") // "Delta Settings"
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("вЉїи‡Єе‹•ж Ўжє–") // "Auto Calibration"
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("иЁ­зЅ®вЉїй«�еє¦") // "Set Delta Height"
#define MSG_DELTA_DIAG_ROD                  _UxGT("вЉїж–њжџ±") // "Diag Rod"
#define MSG_DELTA_HEIGHT                    _UxGT("вЉїй«�еє¦") // "Height"
#define MSG_DELTA_RADIUS                    _UxGT("вЉїеЌЉеѕ‘") // "Radius"
#define MSG_INFO_MENU                       _UxGT("й—њж–јеЌ°иЎЁж©џ")  //"About Printer"
#define MSG_INFO_PRINTER_MENU               _UxGT("еЌ°иЎЁж©џиЁЉжЃЇ")  //"Printer Info"
#define MSG_3POINT_LEVELING                 _UxGT("дё‰й»ћиЄїе№і") // "3-Point Leveling"
#define MSG_LINEAR_LEVELING                 _UxGT("з·љжЂ§иЄїе№і") // "Linear Leveling"
#define MSG_BILINEAR_LEVELING               _UxGT("й›™з·љжЂ§иЄїе№і") // "Bilinear Leveling"
#define MSG_UBL_LEVELING                    _UxGT("зµ±дёЂз†±еєЉиЄїе№і(UBL)") // "Unified Bed Leveling"
#define MSG_MESH_LEVELING                   _UxGT("з¶Іж јиЄїе№і") // "Mesh Leveling"
#define MSG_INFO_STATS_MENU                 _UxGT("еЌ°иЎЁж©џзµ±иЁ€")  //"Printer Stats"
#define MSG_INFO_BOARD_MENU                 _UxGT("дё»жќїиЁЉжЃЇ")  //"Board Info"
#define MSG_INFO_THERMISTOR_MENU            _UxGT("жє«еє¦иЁ€")  //"Thermistors"
#define MSG_INFO_EXTRUDERS                  _UxGT("ж“ е‡єж©џ")  //"Extruders"
#define MSG_INFO_BAUDRATE                   _UxGT("е‚іијёзЋ‡")  //"Baud"
#define MSG_INFO_PROTOCOL                   _UxGT("еЌ”и­°")  //"Protocol"
#define MSG_CASE_LIGHT                      _UxGT("е¤–ж®јз‡€") // "Case light"
#define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("з‡€дє®еє¦") // "Light BRIGHTNESS"

#if LCD_WIDTH > 19
#define MSG_INFO_PRINT_COUNT              _UxGT("е€—еЌ°иЁ€ж•ё")  //"Print Count"
#define MSG_INFO_COMPLETED_PRINTS         _UxGT("е·Іе®Њж€ђ")  //"Completed"
#define MSG_INFO_PRINT_TIME               _UxGT("зёЅе€—еЌ°ж™‚й–“")  //"Total print time"
#define MSG_INFO_PRINT_LONGEST            _UxGT("жњЂй•·е·ҐдЅњж™‚й–“")  //"Longest job time"
#define MSG_INFO_PRINT_FILAMENT           _UxGT("зёЅиЁ€ж“ е‡є")  //"Extruded total"
#else
#define MSG_INFO_PRINT_COUNT              _UxGT("е€—еЌ°ж•ё")  //"Prints"
#define MSG_INFO_COMPLETED_PRINTS         _UxGT("е®Њж€ђ")  //"Completed"
#define MSG_INFO_PRINT_TIME               _UxGT("зёЅе…±")  //"Total"
#define MSG_INFO_PRINT_LONGEST            _UxGT("жњЂй•·")  //"Longest"
#define MSG_INFO_PRINT_FILAMENT           _UxGT("е·Іж“ е‡є")  //"Extruded"
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("жњЂдЅЋжє«еє¦")  //"Min Temp"
#define MSG_INFO_MAX_TEMP                   _UxGT("жњЂй«�жє«еє¦")  //"Max Temp"
#define MSG_INFO_PSU                        _UxGT("й›»жєђдѕ›ж‡‰")  //"Power Supply"
#define MSG_DRIVE_STRENGTH                  _UxGT("й©…е‹•еЉ›еє¦") // "Drive Strength"
#define MSG_DAC_PERCENT                     _UxGT("й©…е‹• %") // "Driver %"
#define MSG_DAC_EEPROM_WRITE                _UxGT("дїќе­�й©…е‹•иЁ­зЅ®") // "DAC EEPROM Write"
#define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("е€—еЌ°е·Іжљ«еЃњ") // "PRINT PAUSED"
#define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("иЈќиј‰зµІж–™") // "LOAD FILAMENT"
#define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("еЌёиј‰зµІж–™") // "UNLOAD FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("жЃўи¤‡йЃёй …:") // "RESUME OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("жё…й™¤ж›ґе¤љ") // "Purge more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("жЃўеѕ©е€—еЌ°")  //"Resume print"
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  е™ґе�ґ: ") // "  Nozzle: "
#define MSG_ERR_HOMING_FAILED               _UxGT("ж­ёеЋџдЅЌе¤±ж•—") // "Homing failed"
#define MSG_ERR_PROBING_FAILED              _UxGT("жЋўй‡ќжЋўжё¬е¤±ж•—") // "Probing failed"
#define MSG_M600_TOO_COLD                   _UxGT("M600: е¤Єж¶ј") // "M600: Too cold"

#if LCD_HEIGHT >= 4
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("з­‰еѕ…й–‹е§‹")  //"Wait for start"
#define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("зµІж–™")  //"of the filament"
#define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("и®Љж›ґ")  //"change"
#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("з­‰еѕ…")  //"Wait for"
#define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("еЌёдё‹зµІж–™")  //"filament unload"
#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("жЏ’е…ҐзµІж–™")  //"Insert filament"
#define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("дё¦жЊ‰йЌµ")  //"and press button"
#define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("з№јзєЊ ...")  //"to continue..."
#define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("жЊ‰дё‹жЊ‰й€•дѕ†") // "Press button to"
#define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("еЉ з†±е™ґе�ґ.") // "heat nozzle."
#define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("еЉ з†±е™ґе�ґ") // "Heating nozzle"
#define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("и«‹з­‰еѕ… ...") // "Please wait..."
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("з­‰еѕ…")  //"Wait for"
#define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("йЂІж–™")  //"filament load"
#define MSG_FILAMENT_CHANGE_PURGE_1         _UxGT("з­‰еѕ…") // "Wait for"
#define MSG_FILAMENT_CHANGE_PURGE_2         _UxGT("зµІж–™жё…й™¤") // "filament purge"
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("з­‰еѕ…е€—еЌ°")  //"Wait for print"
#define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("жЃўеѕ©")  //"to resume"

#else // LCD_HEIGHT < 4
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("и«‹з­‰еѕ… ...")  //"Please wait..."
#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("йЂЂе‡єдё­ ...")  //"Ejecting..."
#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("жЏ’е…Ґдё¦й»ћж“Љ")  //"Insert and Click"
#define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("еЉ з†±дё­ ...") // "Heating..."
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("иј‰е…Ґдё­ ...")  //"Loading..."
#define MSG_FILAMENT_CHANGE_PURGE_1         _UxGT("жё…й™¤дё­ ...") // "Purging..."
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("жЃўеѕ©дё­ ...")  //"Resuming..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_ZH_TW_H


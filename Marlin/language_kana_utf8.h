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
 * Japanese (Kana)
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#ifndef LANGUAGE_KANA_UTF_H
#define LANGUAGE_KANA_UTF_H

#define MAPPER_E382E383
#define DISPLAY_CHARSET_ISO10646_KANA
#define CHARSIZE 3

// This just to show the potential benefit of unicode.
// This translation can be improved by using the full charset of unicode codeblock U+30A0 to U+30FF.

// з‰‡д»®еђЌиЎЁз¤єе®љзѕ©
#define WELCOME_MSG                         MACHINE_NAME _UxGT(" ready.")
#define MSG_SD_INSERTED                     _UxGT("г‚«гѓјгѓ‰г‚¬г‚Ѕг‚¦гѓ‹гѓҐг‚¦г‚µгѓ¬гѓћг‚·г‚ї")        // "Card inserted"
#define MSG_SD_REMOVED                      _UxGT("г‚«гѓјгѓ‰г‚¬г‚ўгѓЄгѓћг‚»гѓі")               // "Card removed"
#define MSG_LCD_ENDSTOPS                    _UxGT("г‚Ёгѓігѓ‰г‚№гѓ€гѓѓгѓ—")                  // "Endstops" // Max length 8 characters
#define MSG_MAIN                            _UxGT("гѓЎг‚¤гѓі")                       // "Main"
#define MSG_AUTOSTART                       _UxGT("г‚ёгѓ‰г‚¦г‚«г‚¤г‚·")                   // "Autostart"
#define MSG_DISABLE_STEPPERS                _UxGT("гѓўгѓјг‚їгѓјгѓ‡гѓіг‚Ігѓі г‚Єгѓ•")            // "Disable steppers"
#define MSG_DEBUG_MENU                      _UxGT("гѓ‡гѓђгѓѓг‚°гѓЎгѓ‹гѓҐгѓј")                // "Debug Menu"
#define MSG_PROGRESS_BAR_TEST               _UxGT("гѓ—гѓ­г‚°гѓ¬г‚№гѓђгѓј гѓ†г‚№гѓ€")            // "Progress Bar Test"
#define MSG_AUTO_HOME                       _UxGT("г‚Ігѓігѓ†гѓігѓ•гѓѓг‚­")                  // "Auto home"
#define MSG_AUTO_HOME_X                     _UxGT("Xг‚ёг‚Ї г‚Ігѓігѓ†гѓігѓ•гѓѓг‚­")             // "Home X"
#define MSG_AUTO_HOME_Y                     _UxGT("Yг‚ёг‚Ї г‚Ігѓігѓ†гѓігѓ•гѓѓг‚­")             // "Home Y"
#define MSG_AUTO_HOME_Z                     _UxGT("Zг‚ёг‚Ї г‚Ігѓігѓ†гѓігѓ•гѓѓг‚­")             // "Home Z"
#define MSG_TMC_Z_CALIBRATION               _UxGT("Zг‚ёг‚Ї г‚іг‚¦г‚»г‚¤")                 // "Calibrate Z"
#define MSG_LEVEL_BED_HOMING                _UxGT("г‚Ігѓігѓ†гѓігѓ•гѓѓг‚­гѓЃгѓҐг‚¦")              // "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               _UxGT("гѓ¬гѓ™гѓЄгѓіг‚°г‚«г‚¤г‚·")                // "Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("гѓ„г‚®гѓЋг‚Ѕг‚Їгѓ†г‚¤гѓ†гѓігѓ�")             // "Next Point"
#define MSG_LEVEL_BED_DONE                  _UxGT("гѓ¬гѓ™гѓЄгѓіг‚°г‚«гѓігѓЄгѓ§г‚¦")              // "Leveling Done!"
#define MSG_SET_HOME_OFFSETS                _UxGT("г‚­г‚ёгѓҐгѓіг‚Єгѓ•г‚»гѓѓгѓ€г‚»гѓѓгѓ†г‚¤")         // "Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("г‚Єгѓ•г‚»гѓѓгѓ€г‚¬гѓ†г‚­гѓЁг‚¦г‚µгѓ¬гѓћг‚·г‚ї")       // "Offsets applied"
#define MSG_SET_ORIGIN                      _UxGT("г‚­г‚ёгѓҐгѓіг‚»гѓѓгѓ€")                 // "Set origin"
#define MSG_PREHEAT_1                       _UxGT("PLA гѓЁгѓЌгѓ„")                   // "Preheat PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   _UxGT("PLA г‚№гѓ™гѓ†гѓЁгѓЌгѓ„")              // " All"
#define MSG_PREHEAT_1_BEDONLY               _UxGT("PLA гѓ™гѓѓгѓ‰гѓЁгѓЌгѓ„")              // " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT("г‚»гѓѓгѓ†г‚¤")       // " conf"
#define MSG_PREHEAT_2                       _UxGT("ABS гѓЁгѓЌгѓ„")                  // "Preheat ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   _UxGT("ABS г‚№гѓ™гѓ†гѓЁгѓЌгѓ„")              // " All"
#define MSG_PREHEAT_2_BEDONLY               _UxGT("ABS гѓ™гѓѓгѓ‰гѓЁгѓЌгѓ„")              // " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT("г‚»гѓѓгѓ†г‚¤")       // " conf"
#define MSG_COOLDOWN                        _UxGT("г‚«гѓЌгѓ„гѓ†г‚¤г‚·")                  // "Cooldown"
#define MSG_SWITCH_PS_ON                    _UxGT("гѓ‡гѓіг‚Ігѓі г‚Єгѓі")                 // "Switch power on"
#define MSG_SWITCH_PS_OFF                   _UxGT("гѓ‡гѓіг‚Ігѓі г‚Єгѓ•")                 // "Switch power off"
#define MSG_EXTRUDE                         _UxGT("г‚Єг‚·гѓЂг‚·")                     // "Extrude"
#define MSG_RETRACT                         _UxGT("гѓ’г‚­г‚ігѓџг‚»гѓѓгѓ†г‚¤")                // "Retract"
#define MSG_MOVE_AXIS                       _UxGT("г‚ёг‚Їг‚¤гѓ‰г‚¦")                    // "Move axis"
#define MSG_BED_LEVELING                    _UxGT("гѓ™гѓѓгѓ‰гѓ¬гѓ™гѓЄгѓіг‚°")                // "Bed leveling"
#define MSG_LEVEL_BED                       _UxGT("гѓ™гѓѓгѓ‰гѓ¬гѓ™гѓЄгѓіг‚°")                // "Level bed"
#define MSG_MOVING                          _UxGT("г‚¤гѓ‰г‚¦гѓЃгѓҐг‚¦...")                // "Moving..."
#define MSG_FREE_XY                         _UxGT("XYг‚ёг‚Ї г‚«г‚¤гѓ›г‚¦")                // "Free XY"
#define MSG_MOVE_X                          _UxGT("Xг‚ёг‚Ї г‚¤гѓ‰г‚¦")                  // "Move X"
#define MSG_MOVE_Y                          _UxGT("Yг‚ёг‚Ї г‚¤гѓ‰г‚¦")                  // "Move Y"
#define MSG_MOVE_Z                          _UxGT("Zг‚ёг‚Ї г‚¤гѓ‰г‚¦")                  // "Move Z"
#define MSG_MOVE_E                          _UxGT("г‚Ёг‚Їг‚№гѓ€гѓ«гѓјгѓЂгѓј")                // "Extruder"
#define MSG_MOVE_01MM                       _UxGT("0.1mm г‚¤гѓ‰г‚¦")                 // "Move 0.1mm"
#define MSG_MOVE_1MM                        _UxGT("  1mm г‚¤гѓ‰г‚¦")                 // "Move 1mm"
#define MSG_MOVE_10MM                       _UxGT(" 10mm г‚¤гѓ‰г‚¦")                 // "Move 10mm"
#define MSG_SPEED                           _UxGT("г‚Ѕг‚Їгѓ‰")                       // "Speed"
#define MSG_BED_Z                           _UxGT("Zг‚Єгѓ•г‚»гѓѓгѓ€")                   // "Bed Z"
#define MSG_NOZZLE                          _UxGT("гѓЋг‚єгѓ«")                       // "Nozzle"
#define MSG_BED                             _UxGT("гѓ™гѓѓгѓ‰")                       // "Bed"
#define MSG_FAN_SPEED                       _UxGT("гѓ•г‚Ўгѓіг‚Ѕг‚Їгѓ‰")                    // "Fan speed"
#define MSG_FLOW                            _UxGT("гѓ€г‚·гѓҐгѓ„гѓЄгѓ§г‚¦")                   // "Flow"
#define MSG_CONTROL                         _UxGT("г‚»г‚¤г‚®гѓ§")                      // "Control"
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" г‚µг‚¤гѓ†г‚¤") // " Min"
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" г‚µг‚¤г‚іг‚¦") // " Max"
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" гѓ•г‚Ўг‚Їг‚їгѓј") // " Fact"
#define MSG_AUTOTEMP                        _UxGT("г‚ёгѓ‰г‚¦г‚Єгѓігѓ‰г‚»г‚¤г‚®гѓ§")               // "Autotemp"
#define MSG_ON                              _UxGT("г‚Єгѓі ")                         // "On "
#define MSG_OFF                             _UxGT("г‚Єгѓ• ")                         // "Off"
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("г‚»гѓіг‚їг‚Ї")                     // "Select"
#define MSG_ACC                             _UxGT("г‚«г‚Ѕг‚Їгѓ‰ mm/s2")               // "Accel"
#define MSG_JERK                            _UxGT("гѓ¤г‚Їгѓ‰ mm/s")                  // "Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Aг‚ёг‚Ї гѓ¤г‚Їгѓ‰ mm/s")             // "Va-jerk"
  #define MSG_VB_JERK                       _UxGT("Bг‚ёг‚Ї гѓ¤г‚Їгѓ‰ mm/s")             // "Vb-jerk"
  #define MSG_VC_JERK                       _UxGT("Cг‚ёг‚Ї гѓ¤г‚Їгѓ‰ mm/s")             // "Vc-jerk"
#else
  #define MSG_VA_JERK                       _UxGT("Xг‚ёг‚Ї гѓ¤г‚Їгѓ‰ mm/s")             // "Vx-jerk"
  #define MSG_VB_JERK                       _UxGT("Yг‚ёг‚Ї гѓ¤г‚Їгѓ‰ mm/s")             // "Vy-jerk"
  #define MSG_VC_JERK                       _UxGT("Zг‚ёг‚Ї гѓ¤г‚Їгѓ‰ mm/s")             // "Vz-jerk"
#endif
#define MSG_VE_JERK                         _UxGT("г‚Ёг‚Їг‚№гѓ€гѓ«гѓјгѓЂгѓј гѓ¤г‚Їгѓ‰")          // "Ve-jerk"
#define MSG_VMAX                            _UxGT("г‚µг‚¤гѓЂг‚¤г‚Єг‚ЇгѓЄг‚Ѕг‚Їгѓ‰ ")            // "Vmax "
#define MSG_VMIN                            _UxGT("г‚µг‚¤г‚·гѓ§г‚¦г‚Єг‚ЇгѓЄг‚Ѕг‚Їгѓ‰")           // "Vmin"
#define MSG_VTRAV_MIN                       _UxGT("г‚µг‚¤г‚·гѓ§г‚¦г‚¤гѓ‰г‚¦г‚Ѕг‚Їгѓ‰")           // "VTrav min"
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("г‚µг‚¤гѓЂг‚¤г‚«г‚Ѕг‚Їгѓ‰ ")              // "Amax "
#define MSG_A_RETRACT                       _UxGT("гѓ’г‚­г‚ігѓџг‚«г‚Ѕг‚Їгѓ‰")               // "A-retract"
#define MSG_A_TRAVEL                        _UxGT("г‚¤гѓ‰г‚¦г‚«г‚Ѕг‚Їгѓ‰")                // "A-travel"
#define MSG_TEMPERATURE                     _UxGT("г‚Єгѓігѓ‰")                      // "Temperature"
#define MSG_MOTION                          _UxGT("г‚¦г‚ґг‚­г‚»гѓѓгѓ†г‚¤")                // "Motion"
#define MSG_FILAMENT                        _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€")                   // "Filament"
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€гѓЃгѓ§гѓѓг‚±г‚¤")            // "Fil. Dia."
#define MSG_CONTRAST                        _UxGT("LCDг‚ігѓігѓ€гѓ©г‚№гѓ€")               // "LCD contrast"
#define MSG_STORE_EEPROM                    _UxGT("гѓЎгѓўгѓЄгѓ�г‚«г‚ЇгѓЋг‚¦")               // "Store memory"
#define MSG_LOAD_EEPROM                     _UxGT("гѓЎгѓўгѓЄг‚«гѓ©гѓЁгѓџг‚ігѓџ")               // "Load memory"
#define MSG_RESTORE_FAILSAFE                _UxGT("г‚»гѓѓгѓ†г‚¤гѓЄг‚»гѓѓгѓ€")               // "Restore failsafe"
#define MSG_REFRESH                         _UxGT("гѓЄгѓ•гѓ¬гѓѓг‚·гѓҐ")                  // "Refresh"
#define MSG_WATCH                           _UxGT("г‚ёгѓ§г‚¦гѓ›г‚¦г‚¬гѓЎгѓі")               // "Info screen"
#define MSG_PREPARE                         _UxGT("г‚ёгѓҐгѓігѓ“г‚»гѓѓгѓ†г‚¤")               // "Prepare"
#define MSG_TUNE                            _UxGT("гѓЃгѓ§г‚¦г‚»г‚¤")                    // "Tune"
#define MSG_PAUSE_PRINT                     _UxGT("г‚¤гѓЃг‚ёгѓ†г‚¤г‚·")                  // "Pause print"
#define MSG_RESUME_PRINT                    _UxGT("гѓ—гѓЄгѓігѓ€г‚µг‚¤г‚«г‚¤")                // "Resume print"
#define MSG_STOP_PRINT                      _UxGT("гѓ—гѓЄгѓігѓ€гѓ†г‚¤г‚·")                 // "Stop print"
#define MSG_CARD_MENU                       _UxGT("SDг‚«гѓјгѓ‰г‚«гѓ©гѓ—гѓЄгѓігѓ€")            // "Print from SD"
#define MSG_NO_CARD                         _UxGT("SDг‚«гѓјгѓ‰г‚¬г‚ўгѓЄгѓћг‚»гѓі")            // "No SD card"
#define MSG_DWELL                           _UxGT("г‚­гѓҐг‚¦г‚·...")                  // "Sleep..."
#define MSG_USERWAIT                        _UxGT("г‚·гѓђгѓ©г‚Їг‚ЄгѓћгѓЃг‚ЇгѓЂг‚µг‚¤...")        // "Wait for user..."
#define MSG_PRINT_ABORTED                   _UxGT("гѓ—гѓЄгѓігѓ€г‚¬гѓЃгѓҐг‚¦г‚·г‚µгѓ¬гѓћг‚·г‚ї")       // "Print aborted"
#define MSG_NO_MOVE                         _UxGT("г‚¦г‚ґг‚­гѓћг‚»гѓі")                  // "No move."
#define MSG_KILLED                          _UxGT("гѓ’г‚ёгѓ§г‚¦гѓ†г‚¤г‚·")                  // "KILLED. "
#define MSG_STOPPED                         _UxGT("гѓ†г‚¤г‚·г‚·гѓћг‚·г‚ї")                  // "STOPPED. "
#define MSG_CONTROL_RETRACT                 _UxGT("гѓ’г‚­г‚ігѓџгѓЄгѓ§г‚¦ mm")                // "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("гѓ’г‚­г‚ігѓџгѓЄгѓ§г‚¦S mm")               // "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                _UxGT("гѓ’г‚­г‚ігѓџг‚Ѕг‚Їгѓ‰ mm/s")             // "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("гѓЋг‚єгѓ«г‚їг‚¤гѓ’ mm")                // "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("гѓ›г‚·гѓ§г‚¦гѓЄгѓ§г‚¦ mm")               // "UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("гѓ›г‚·гѓ§г‚¦гѓЄгѓ§г‚¦S mm")              // "S UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("гѓ›г‚·гѓ§г‚¦г‚Ѕг‚Їгѓ‰ mm/s")            // "UnRet  V"
#define MSG_AUTORETRACT                     _UxGT("г‚ёгѓ‰г‚¦гѓ’г‚­г‚ігѓџ")                 // "AutoRetr."
#define MSG_FILAMENTCHANGE                  _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€г‚іг‚¦г‚«гѓі")              // "Change filament"
#define MSG_INIT_SDCARD                     _UxGT("SDг‚«гѓјгѓ‰г‚µг‚¤гѓЁгѓџг‚ігѓџ")             // "Init. SD card"
#define MSG_CNG_SDCARD                      _UxGT("SDг‚«гѓјгѓ‰г‚іг‚¦г‚«гѓі")               // "Change SD card"
#define MSG_ZPROBE_OUT                      _UxGT("Zгѓ—гѓ­гѓјгѓ– гѓ™гѓѓгѓ‰г‚¬г‚¤")            // "Z probe out. bed"
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch г‚ёг‚іг‚·гѓігѓЂгѓі")          // "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   _UxGT("BLTouch гѓЄг‚»гѓѓгѓ€")             // "Reset BLTouch"
#define MSG_HOME                            _UxGT("г‚µг‚­гѓ‹")                      // "Home" // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("гѓІгѓ•гѓѓг‚­г‚µг‚»гѓ†г‚ЇгѓЂг‚µг‚¤")           // "first"
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Zг‚Єгѓ•г‚»гѓѓгѓ€")                   // "Z Offset"
#define MSG_BABYSTEP_X                      _UxGT("Xг‚ёг‚Ї гѓ“гѓ‰г‚¦")                  // "Babystep X"
#define MSG_BABYSTEP_Y                      _UxGT("Yг‚ёг‚Ї гѓ“гѓ‰г‚¦")                  // "Babystep Y"
#define MSG_BABYSTEP_Z                      _UxGT("Zг‚ёг‚Ї гѓ“гѓ‰г‚¦")                  // "Babystep Z"
#define MSG_ENDSTOP_ABORT                   _UxGT("г‚¤гѓ‰г‚¦г‚Ігѓіг‚«г‚¤г‚±гѓігѓЃг‚­гѓЋг‚¦")         // "Endstop abort"
#define MSG_HEATING_FAILED_LCD              _UxGT("г‚«гѓЌгѓ„г‚·гѓѓгѓ‘г‚¤")                 // "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("г‚Ёгѓ©гѓј:г‚ёгѓ§г‚¦гѓЃгѓ§г‚¦г‚µгѓјгѓџг‚№г‚їгѓјг‚­гѓЋг‚¦")  // "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 _UxGT("гѓЌгѓ„гѓњг‚¦г‚Ѕг‚¦")                   // "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     _UxGT("г‚Ёгѓ©гѓј:г‚µг‚¤г‚іг‚¦г‚ЄгѓігѓЃгѓ§г‚¦г‚«")         // "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     _UxGT("г‚Ёгѓ©гѓј:г‚µг‚¤гѓ†г‚¤г‚Єгѓігѓџгѓћгѓі")          // "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 _UxGT("г‚Ёгѓ©гѓј:гѓ™гѓѓгѓ‰ г‚µг‚¤г‚іг‚¦г‚ЄгѓігѓЃгѓ§г‚¦г‚«")    // "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 _UxGT("г‚Ёгѓ©гѓј:гѓ™гѓѓгѓ‰ г‚µг‚¤гѓ†г‚¤г‚Єгѓігѓџгѓћгѓі")     // "Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST // "G28 Z Forbidden"
#define MSG_HALTED                          _UxGT("гѓ—гѓЄгѓіг‚їгѓјгѓЏгѓ†г‚¤г‚·г‚·гѓћг‚·г‚ї")         // "PRINTER HALTED"
#define MSG_PLEASE_RESET                    _UxGT("гѓЄг‚»гѓѓгѓ€г‚·гѓ†г‚ЇгѓЂг‚µг‚¤")              // "Please reset"
#define MSG_SHORT_DAY                       _UxGT("d")                          // One character only
#define MSG_SHORT_HOUR                      _UxGT("h")                          // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m")                          // One character only
#define MSG_HEATING                         _UxGT("г‚«гѓЌгѓ„гѓЃгѓҐг‚¦...")                // "Heating..."
#define MSG_BED_HEATING                     _UxGT("гѓ™гѓѓгѓ‰ г‚«гѓЌгѓ„гѓЃгѓҐг‚¦...")           // "Bed Heating..."
#define MSG_DELTA_CALIBRATE                 _UxGT("гѓ‡гѓ«г‚ї г‚іг‚¦г‚»г‚¤")                // "Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               _UxGT("Xг‚ёг‚Ї г‚іг‚¦г‚»г‚¤")                 // "Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Yг‚ёг‚Ї г‚іг‚¦г‚»г‚¤")                 // "Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Zг‚ёг‚Ї г‚іг‚¦г‚»г‚¤")                 // "Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("гѓЃгѓҐг‚¦г‚·гѓі г‚іг‚¦г‚»г‚¤")              // "Calibrate Center"
#define MSG_INFO_MENU                       _UxGT("г‚ігѓЋгѓ—гѓЄгѓіг‚їгѓјгѓ‹гѓ„г‚¤гѓ†")             // "About Printer"
#define MSG_INFO_PRINTER_MENU               _UxGT("гѓ—гѓЄгѓіг‚їгѓјг‚ёгѓ§г‚¦гѓ›г‚¦")              // "Printer Info"
#define MSG_INFO_STATS_MENU                 _UxGT("гѓ—гѓЄгѓігѓ€г‚ёгѓ§г‚¦г‚­гѓ§г‚¦")              // "Printer Stats"
#define MSG_INFO_BOARD_MENU                 _UxGT("г‚»г‚¤г‚®гѓ§г‚±г‚¤г‚ёгѓ§г‚¦гѓ›г‚¦")            // "Board Info"
#define MSG_INFO_THERMISTOR_MENU            _UxGT("г‚µгѓјгѓџг‚№г‚їгѓј")                   // "Thermistors"
#define MSG_INFO_EXTRUDERS                  _UxGT("г‚Ёг‚Їг‚№гѓ€гѓ«гѓјгѓЂгѓјг‚№г‚¦")             // "Extruders"
#define MSG_INFO_BAUDRATE                   _UxGT("гѓњгѓјгѓ¬гѓјгѓ€")                    // "Baud"
#define MSG_INFO_PROTOCOL                   _UxGT("гѓ—гѓ­гѓ€г‚ігѓ«")                    // "Protocol"
#define MSG_CASE_LIGHT                      _UxGT("г‚­гѓ§г‚¦г‚їг‚¤гѓЉг‚¤г‚·гѓ§г‚¦гѓЎг‚¤")       // "Case light"
#define MSG_INFO_PRINT_COUNT                _UxGT("гѓ—гѓЄгѓігѓ€г‚№г‚¦ ")                  // "Print Count"
#define MSG_INFO_COMPLETED_PRINTS           _UxGT("г‚«гѓігѓЄгѓ§г‚¦г‚№г‚¦")                  // "Completed"
#define MSG_INFO_PRINT_TIME                 _UxGT("гѓ—гѓЄгѓігѓ€г‚ёг‚«гѓігѓ«г‚¤г‚±г‚¤")            // "Total print time"
#define MSG_INFO_PRINT_LONGEST              _UxGT("г‚µг‚¤гѓЃгѓ§г‚¦гѓ—гѓЄгѓігѓ€г‚ёг‚«гѓі")           // "Longest job time"
#define MSG_INFO_PRINT_FILAMENT             _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€г‚·гѓЁг‚¦гѓЄгѓ§г‚¦гѓ«г‚¤г‚±г‚¤")       // "Extruded total"
#define MSG_INFO_MIN_TEMP                   _UxGT("г‚»гѓѓгѓ†г‚¤г‚µг‚¤гѓ†г‚¤г‚Єгѓі")              // "Min Temp"
#define MSG_INFO_MAX_TEMP                   _UxGT("г‚»гѓѓгѓ†г‚¤г‚µг‚¤г‚іг‚¦г‚Єгѓі")              // "Max Temp"
#define MSG_INFO_PSU                        _UxGT("гѓ‡гѓіг‚Ігѓіг‚·гѓҐгѓ™гѓ„")                // "Power Supply"
#define MSG_DRIVE_STRENGTH                  _UxGT("гѓўгѓјг‚їгѓјг‚Їгѓ‰г‚¦гѓЄгѓ§г‚Ї")              // "Drive Strength"
#define MSG_DAC_PERCENT                     _UxGT("DACг‚·гѓҐгѓ„гѓЄгѓ§г‚Ї %")               // "Driver %"
#define MSG_DAC_EEPROM_WRITE                MSG_STORE_EEPROM                    // "DAC EEPROM Write"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("гѓ—гѓЄгѓігѓ€г‚µг‚¤г‚«г‚¤")                // "Resume print"
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("г‚іг‚¦г‚«гѓігѓІг‚«г‚¤г‚·г‚·гѓћг‚№")            // "Wait for start"
#define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("г‚·гѓђгѓ©г‚Їг‚ЄгѓћгѓЃг‚ЇгѓЂг‚µг‚¤")            // "of the filament"
#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€гѓЊг‚­гѓЂг‚·гѓЃгѓҐг‚¦")          // "Wait for"
#define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("г‚·гѓђгѓ©г‚Їг‚ЄгѓћгѓЃг‚ЇгѓЂг‚µг‚¤")            // "filament unload"
#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€гѓІг‚Ѕг‚¦гѓ‹гѓҐг‚¦г‚·,")         // "Insert filament"
#define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("г‚ЇгѓЄгѓѓг‚Їг‚№гѓ«гѓ€г‚ѕгѓѓг‚іг‚¦г‚·гѓћг‚№")         // "and press button"
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("гѓ•г‚Јгѓ©гѓЎгѓігѓ€г‚Ѕг‚¦гѓ†гѓігѓЃгѓҐг‚¦")          // "Wait for"
#define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("г‚·гѓђгѓ©г‚Їг‚ЄгѓћгѓЃг‚ЇгѓЂг‚µг‚¤")            // "filament load"
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("гѓ—гѓЄгѓігѓ€гѓІг‚µг‚¤г‚«г‚¤г‚·гѓћг‚№")           // "Wait for print"
#define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("г‚·гѓђгѓ©г‚Їг‚ЄгѓћгѓЃг‚ЇгѓЂг‚µг‚¤")            // "to resume"

#endif // LANGUAGE_KANA_UTF_H


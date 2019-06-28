/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * Japanese (Kana)
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_KANA
#define CHARSIZE 3

// This just to show the potential benefit of unicode.
// This translation can be improved by using the full charset of unicode codeblock U+30A0 to U+30FF.

// 片仮名表示定義
#define WELCOME_MSG                         MACHINE_NAME _UxGT(" ready.")
#define MSG_SD_INSERTED                     _UxGT("カードガソウニュウサレマシタ")        // "Card inserted"
#define MSG_SD_REMOVED                      _UxGT("カードガアリマセン")               // "Card removed"
#define MSG_LCD_ENDSTOPS                    _UxGT("エンドストップ")                  // "Endstops" // Max length 8 characters
#define MSG_MAIN                            _UxGT("メイン")                       // "Main"
#define MSG_AUTOSTART                       _UxGT("ジドウカイシ")                   // "Autostart"
#define MSG_DISABLE_STEPPERS                _UxGT("モーターデンゲン オフ")            // "Disable steppers"
#define MSG_DEBUG_MENU                      _UxGT("デバッグメニュー")                // "Debug Menu"
#define MSG_PROGRESS_BAR_TEST               _UxGT("プログレスバー テスト")            // "Progress Bar Test"
#define MSG_AUTO_HOME                       _UxGT("ゲンテンフッキ")                  // "Auto home"
#define MSG_AUTO_HOME_X                     _UxGT("Xジク ゲンテンフッキ")             // "Home X"
#define MSG_AUTO_HOME_Y                     _UxGT("Yジク ゲンテンフッキ")             // "Home Y"
#define MSG_AUTO_HOME_Z                     _UxGT("Zジク ゲンテンフッキ")             // "Home Z"
#define MSG_LEVEL_BED_HOMING                _UxGT("ゲンテンフッキチュウ")              // "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               _UxGT("レベリングカイシ")                // "Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("ツギノソクテイテンヘ")             // "Next Point"
#define MSG_LEVEL_BED_DONE                  _UxGT("レベリングカンリョウ")              // "Leveling Done!"
#define MSG_SET_HOME_OFFSETS                _UxGT("キジュンオフセットセッテイ")         // "Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("オフセットガテキヨウサレマシタ")       // "Offsets applied"
#define MSG_SET_ORIGIN                      _UxGT("キジュンセット")                 // "Set origin"
#define MSG_PREHEAT_1                       _UxGT(PREHEAT_1_LABEL " ヨネツ")       // "Preheat " PREHEAT_1_LABEL
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   _UxGT(PREHEAT_1_LABEL " スベテヨネツ")  // " All"
#define MSG_PREHEAT_1_BEDONLY               _UxGT(PREHEAT_1_LABEL " ベッドヨネツ")  // " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT("セッテイ")       // " conf"
#define MSG_PREHEAT_2                       _UxGT(PREHEAT_2_LABEL " ヨネツ")      // "Preheat " PREHEAT_2_LABEL
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   _UxGT(PREHEAT_2_LABEL " スベテヨネツ")  // " All"
#define MSG_PREHEAT_2_BEDONLY               _UxGT(PREHEAT_2_LABEL " ベッドヨネツ")  // " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT("セッテイ")       // " conf"
#define MSG_COOLDOWN                        _UxGT("カネツテイシ")                  // "Cooldown"
#define MSG_SWITCH_PS_ON                    _UxGT("デンゲン オン")                 // "Switch power on"
#define MSG_SWITCH_PS_OFF                   _UxGT("デンゲン オフ")                 // "Switch power off"
#define MSG_EXTRUDE                         _UxGT("オシダシ")                     // "Extrude"
#define MSG_RETRACT                         _UxGT("ヒキコミセッテイ")                // "Retract"
#define MSG_MOVE_AXIS                       _UxGT("ジクイドウ")                    // "Move axis"
#define MSG_BED_LEVELING                    _UxGT("ベッドレベリング")                // "Bed leveling"
#define MSG_LEVEL_BED                       _UxGT("ベッドレベリング")                // "Level bed"

#define MSG_MOVING                          _UxGT("イドウチュウ")                   // "Moving..."
#define MSG_FREE_XY                         _UxGT("XYジク カイホウ")                // "Free XY"
#define MSG_MOVE_X                          _UxGT("Xジク イドウ")                  // "Move X"
#define MSG_MOVE_Y                          _UxGT("Yジク イドウ")                  // "Move Y"
#define MSG_MOVE_Z                          _UxGT("Zジク イドウ")                  // "Move Z"
#define MSG_MOVE_E                          _UxGT("エクストルーダー")                // "Extruder"
#define MSG_MOVE_01MM                       _UxGT("0.1mm イドウ")                 // "Move 0.1mm"
#define MSG_MOVE_1MM                        _UxGT("  1mm イドウ")                 // "Move 1mm"
#define MSG_MOVE_10MM                       _UxGT(" 10mm イドウ")                 // "Move 10mm"
#define MSG_SPEED                           _UxGT("ソクド")                       // "Speed"
#define MSG_BED_Z                           _UxGT("Zオフセット")                   // "Bed Z"
#define MSG_NOZZLE                          _UxGT("ノズル")                       // "Nozzle"
#define MSG_BED                             _UxGT("ベッド")                       // "Bed"
#define MSG_FAN_SPEED                       _UxGT("ファンソクド")                    // "Fan speed"
#define MSG_FLOW                            _UxGT("トシュツリョウ")                   // "Flow"
#define MSG_CONTROL                         _UxGT("セイギョ")                      // "Control"
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" サイテイ") // " Min"
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" サイコウ") // " Max"
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" ファクター") // " Fact"
#define MSG_AUTOTEMP                        _UxGT("ジドウオンドセイギョ")               // "Autotemp"
#define MSG_LCD_ON                          _UxGT("オン")                         // "On"
#define MSG_LCD_OFF                         _UxGT("オフ")                         // "Off"
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("センタク")                     // "Select"
#define MSG_ACC                             _UxGT("カソクド mm/s2")               // "Accel"
#define MSG_JERK                            _UxGT("ヤクド mm/s")                  // "Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Aジク ヤクド mm/s")             // "Va-jerk"
  #define MSG_VB_JERK                       _UxGT("Bジク ヤクド mm/s")             // "Vb-jerk"
  #define MSG_VC_JERK                       _UxGT("Cジク ヤクド mm/s")             // "Vc-jerk"
#else
  #define MSG_VA_JERK                       _UxGT("Xジク ヤクド mm/s")             // "Vx-jerk"
  #define MSG_VB_JERK                       _UxGT("Yジク ヤクド mm/s")             // "Vy-jerk"
  #define MSG_VC_JERK                       _UxGT("Zジク ヤクド mm/s")             // "Vz-jerk"
#endif
#define MSG_VE_JERK                         _UxGT("エクストルーダー ヤクド")          // "Ve-jerk"
#define MSG_VMAX                            _UxGT("サイダイオクリソクド ")            // "Vmax "
#define MSG_VMIN                            _UxGT("サイショウオクリソクド")           // "Vmin"
#define MSG_VTRAV_MIN                       _UxGT("サイショウイドウソクド")           // "VTrav min"
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("サイダイカソクド ")              // "Amax "
#define MSG_A_RETRACT                       _UxGT("ヒキコミカソクド")               // "A-retract"
#define MSG_A_TRAVEL                        _UxGT("イドウカソクド")                // "A-travel"
#define MSG_TEMPERATURE                     _UxGT("オンド")                      // "Temperature"
#define MSG_MOTION                          _UxGT("ウゴキセッテイ")                // "Motion"
#define MSG_FILAMENT                        _UxGT("フィラメント")                   // "Filament"
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("フィラメントチョッケイ")            // "Fil. Dia."
#define MSG_CONTRAST                        _UxGT("LCDコントラスト")               // "LCD contrast"
#define MSG_STORE_EEPROM                    _UxGT("メモリヘカクノウ")               // "Store memory"
#define MSG_LOAD_EEPROM                     _UxGT("メモリカラヨミコミ")               // "Load memory"
#define MSG_RESTORE_FAILSAFE                _UxGT("セッテイリセット")               // "Restore failsafe"
#define MSG_REFRESH                         _UxGT("リフレッシュ")                  // "Refresh"
#define MSG_WATCH                           _UxGT("ジョウホウガメン")               // "Info screen"
#define MSG_PREPARE                         _UxGT("ジュンビセッテイ")               // "Prepare"
#define MSG_TUNE                            _UxGT("チョウセイ")                    // "Tune"
#define MSG_PAUSE_PRINT                     _UxGT("イチジテイシ")                  // "Pause print"
#define MSG_RESUME_PRINT                    _UxGT("プリントサイカイ")                // "Resume print"
#define MSG_STOP_PRINT                      _UxGT("プリントテイシ")                 // "Stop print"
#define MSG_CARD_MENU                       _UxGT("SDカードカラプリント")            // "Print from SD"
#define MSG_NO_CARD                         _UxGT("SDカードガアリマセン")            // "No SD card"
#define MSG_DWELL                           _UxGT("キュウシ")                     // "Sleep..."
#define MSG_USERWAIT                        _UxGT("シバラクオマチクダサイ")           // "Wait for user..."
#define MSG_PRINT_ABORTED                   _UxGT("プリントガチュウシサレマシタ")       // "Print aborted"
#define MSG_NO_MOVE                         _UxGT("ウゴキマセン")                  // "No move."
#define MSG_KILLED                          _UxGT("ヒジョウテイシ")                  // "KILLED. "
#define MSG_STOPPED                         _UxGT("テイシシマシタ")                  // "STOPPED. "
#define MSG_CONTROL_RETRACT                 _UxGT("ヒキコミリョウ mm")                // "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("ヒキコミリョウS mm")               // "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                _UxGT("ヒキコミソクド mm/s")             // "Retract  V"
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("ノズルタイヒ mm")                // "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("ホショウリョウ mm")               // "UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("ホショウリョウS mm")              // "S UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("ホショウソクド mm/s")            // "UnRet  V"
#define MSG_AUTORETRACT                     _UxGT("ジドウヒキコミ")                 // "AutoRetr."
#define MSG_FILAMENTCHANGE                  _UxGT("フィラメントコウカン")              // "Change filament"
#define MSG_INIT_SDCARD                     _UxGT("SDカードサイヨミコミ")             // "Init. SD card"
#define MSG_CHANGE_SDCARD                   _UxGT("SDカードコウカン")               // "Change SD card"
#define MSG_ZPROBE_OUT                      _UxGT("Zプローブ ベッドガイ")            // "Z probe out. bed"
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch ジコシンダン")          // "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   _UxGT("BLTouch リセット")             // "Reset BLTouch"
#define MSG_HOME                            _UxGT("サキニ")                      // "Home" // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("ヲフッキサセテクダサイ")           // "first"
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Zオフセット")                   // "Z Offset"
#define MSG_BABYSTEP_X                      _UxGT("Xジク ビドウ")                  // "Babystep X"
#define MSG_BABYSTEP_Y                      _UxGT("Yジク ビドウ")                  // "Babystep Y"
#define MSG_BABYSTEP_Z                      _UxGT("Zジク ビドウ")                  // "Babystep Z"
#define MSG_ENDSTOP_ABORT                   _UxGT("イドウゲンカイケンチキノウ")         // "Endstop abort"
#define MSG_HEATING_FAILED_LCD              _UxGT("カネツシッパイ")                 // "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("エラー:ジョウチョウサーミスターキノウ")  // "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 _UxGT("ネツボウソウ")                   // "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     _UxGT("エラー:サイコウオンチョウカ")         // "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     _UxGT("エラー:サイテイオンミマン")          // "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 _UxGT("エラー:ベッド サイコウオンチョウカ")    // "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 _UxGT("エラー:ベッド サイテイオンミマン")     // "Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST // "Home XY first"
#define MSG_HALTED                          _UxGT("プリンターハテイシシマシタ")         // "PRINTER HALTED"
#define MSG_PLEASE_RESET                    _UxGT("リセットシテクダサイ")              // "Please reset"
#define MSG_SHORT_DAY                       _UxGT("d")                          // One character only
#define MSG_SHORT_HOUR                      _UxGT("h")                          // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m")                          // One character only
#define MSG_HEATING                         _UxGT("カネツチュウ")                   // "Heating..."
#define MSG_BED_HEATING                     _UxGT("ベッド カネツチュウ")              // "Bed Heating..."
#define MSG_DELTA_CALIBRATE                 _UxGT("デルタ コウセイ")                // "Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               _UxGT("Xジク コウセイ")                 // "Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Yジク コウセイ")                 // "Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Zジク コウセイ")                 // "Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("チュウシン コウセイ")              // "Calibrate Center"
#define MSG_INFO_MENU                       _UxGT("コノプリンターニツイテ")             // "About Printer"
#define MSG_INFO_PRINTER_MENU               _UxGT("プリンタージョウホウ")              // "Printer Info"
#define MSG_INFO_STATS_MENU                 _UxGT("プリントジョウキョウ")              // "Printer Stats"
#define MSG_INFO_BOARD_MENU                 _UxGT("セイギョケイジョウホウ")            // "Board Info"
#define MSG_INFO_THERMISTOR_MENU            _UxGT("サーミスター")                   // "Thermistors"
#define MSG_INFO_EXTRUDERS                  _UxGT("エクストルーダースウ")             // "Extruders"
#define MSG_INFO_BAUDRATE                   _UxGT("ボーレート")                    // "Baud"
#define MSG_INFO_PROTOCOL                   _UxGT("プロトコル")                    // "Protocol"
#define MSG_CASE_LIGHT                      _UxGT("キョウタイナイショウメイ")       // "Case light"
#define MSG_INFO_PRINT_COUNT                _UxGT("プリントスウ ")                  // "Print Count"
#define MSG_INFO_COMPLETED_PRINTS           _UxGT("カンリョウスウ")                  // "Completed"
#define MSG_INFO_PRINT_TIME                 _UxGT("プリントジカンルイケイ")            // "Total print time"
#define MSG_INFO_PRINT_LONGEST              _UxGT("サイチョウプリントジカン")           // "Longest job time"
#define MSG_INFO_PRINT_FILAMENT             _UxGT("フィラメントシヨウリョウルイケイ")       // "Extruded total"
#define MSG_INFO_MIN_TEMP                   _UxGT("セッテイサイテイオン")              // "Min Temp"
#define MSG_INFO_MAX_TEMP                   _UxGT("セッテイサイコウオン")              // "Max Temp"
#define MSG_INFO_PSU                        _UxGT("デンゲンシュベツ")                // "Power Supply"
#define MSG_DRIVE_STRENGTH                  _UxGT("モータークドウリョク")              // "Drive Strength"
#define MSG_DAC_PERCENT                     _UxGT("DACシュツリョク %")               // "Driver %"
#define MSG_DAC_EEPROM_WRITE                MSG_STORE_EEPROM                    // "DAC EEPROM Write"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("プリントサイカイ")                // "Resume print"
#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("コウカンヲカイシシマス")            // "Wait for start"
#define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("シバラクオマチクダサイ")            // "of the filament"
#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("フィラメントヌキダシチュウ")          // "Wait for"
#define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("シバラクオマチクダサイ")            // "filament unload"
#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("フィラメントヲソウニュウシ,")         // "Insert filament"
#define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("クリックスルトゾッコウシマス")         // "and press button"
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("フィラメントソウテンチュウ")          // "Wait for"
#define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("シバラクオマチクダサイ")            // "filament load"
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("プリントヲサイカイシマス")           // "Wait for print"
#define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("シバラクオマチクダサイ")            // "to resume"

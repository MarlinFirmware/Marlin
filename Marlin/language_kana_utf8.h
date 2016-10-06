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
 * Japanese (Kana UTF8 version)
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */

#ifndef LANGUAGE_KANA_UTF_H
#define LANGUAGE_KANA_UTF_H

#define MAPPER_E382E383
#define DISPLAY_CHARSET_ISO10646_KANA

// This just to show the potential benefit of unicode.
// This translation can be improved by using the full charset of unicode codeblock U+30A0 to U+30FF.

// 片仮名表示定義
#define WELCOME_MSG                         MACHINE_NAME " ready."
#define MSG_SD_INSERTED                     "カードガソウニュウサレマシタ"        // "Card inserted"
#define MSG_SD_REMOVED                      "カードガアリマセン"               // "Card removed"
#define MSG_LCD_ENDSTOPS                    "エンドストップ"                  // "Endstops" // Max length 8 characters
#define MSG_MAIN                            "メイン"                       // "Main"
#define MSG_AUTOSTART                       "ジドウカイシ"                   // "Autostart"
#define MSG_DISABLE_STEPPERS                "モーターデンゲン オフ"            // "Disable steppers"
#define MSG_AUTO_HOME                       "ゲンテンフッキ"                  // "Auto home"
#define MSG_AUTO_HOME_X                     "Xジク ゲンテンフッキ"             // "Home X"
#define MSG_AUTO_HOME_Y                     "Yジク ゲンテンフッキ"             // "Home Y"
#define MSG_AUTO_HOME_Z                     "Zジク ゲンテンフッキ"             // "Home Z"
#define MSG_LEVEL_BED_HOMING                "ゲンテンフッキチュウ"              // "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "レベリングカイシ"                // "Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            "ツギノソクテイテンヘ"             // "Next Point"
#define MSG_LEVEL_BED_DONE                  "レベリングカンリョウ"              // "Leveling Done!"
#define MSG_LEVEL_BED_CANCEL                "トリヤメ"                      // "Cancel"
#define MSG_SET_HOME_OFFSETS                "キジュンオフセットセッテイ"         // "Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            "オフセットガテキヨウサレマシタ"       // "Offsets applied"
#define MSG_SET_ORIGIN                      "キジュンセット"                 // "Set origin"
#define MSG_PREHEAT_1                       "PLA ヨネツ"                   // "Preheat PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   "PLA スベテヨネツ"              // " All"
#define MSG_PREHEAT_1_BEDONLY               "PLA ベッドヨネツ"              // " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 "セッテイ"       // " conf"
#define MSG_PREHEAT_2                       "ABS ヨネツ"                  // "Preheat ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   "ABS スベテヨネツ"              // " All"
#define MSG_PREHEAT_2_BEDONLY               "ABS ベッドヨネツ"              // " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 "セッテイ"       // " conf"
#define MSG_COOLDOWN                        "カネツテイシ"                  // "Cooldown"
#define MSG_SWITCH_PS_ON                    "デンゲン オン"                 // "Switch power on"
#define MSG_SWITCH_PS_OFF                   "デンゲン オフ"                 // "Switch power off"
#define MSG_EXTRUDE                         "オシダシ"                     // "Extrude"
#define MSG_RETRACT                         "ヒキコミセッテイ"                // "Retract"
#define MSG_MOVE_AXIS                       "ジクイドウ"                    // "Move axis"
#define MSG_LEVEL_BED                       "ベッドレベリング"                // "Level bed"
#define MSG_MOVE_X                          "Xジク イドウ"                  // "Move X"
#define MSG_MOVE_Y                          "Yジク イドウ"                  // "Move Y"
#define MSG_MOVE_Z                          "Zジク イドウ"                  // "Move Z"
#define MSG_MOVE_E                          "エクストルーダー"                // "Extruder"
#define MSG_MOVE_01MM                       "0.1mm イドウ"                 // "Move 0.1mm"
#define MSG_MOVE_1MM                        "  1mm イドウ"                 // "Move 1mm"
#define MSG_MOVE_10MM                       " 10mm イドウ"                 // "Move 10mm"
#define MSG_SPEED                           "ソクド"                       // "Speed"
#define MSG_BED_Z                           "Zオフセット"                   // "Bed Z"
#define MSG_NOZZLE                          "ノズル"                       // "Nozzle"
#define MSG_BED                             "ベッド"                       // "Bed"
#define MSG_FAN_SPEED                       "ファンソクド"                    // "Fan speed"
#define MSG_FLOW                            "トシュツリョウ"                   // "Flow"
#define MSG_CONTROL                         "セイギョ"                      // "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " サイテイ" // " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " サイコウ" // " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " ファクター" // " Fact"
#define MSG_AUTOTEMP                        "ジドウオンドセイギョ"               // "Autotemp"
#define MSG_ON                              "オン "                         // "On "
#define MSG_OFF                             "オフ "                         // "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "センタク"                     // "Select"
#define MSG_ACC                             "カソクド mm/s2"               // "Accel"
#define MSG_VX_JERK                         "Xジク ヤクド mm/s"             // "Vx-jerk"
#define MSG_VY_JERK                         "Yジク ヤクド mm/s"             // "Vy-jerk"
#define MSG_VZ_JERK                         "Zジク ヤクド mm/s"             // "Vz-jerk"
#define MSG_VE_JERK                         "エクストルーダー ヤクド"          // "Ve-jerk"
#define MSG_VMAX                            "サイダイオクリソクド "            // "Vmax "
#define MSG_VMIN                            "サイショウオクリソクド"           // "Vmin"
#define MSG_VTRAV_MIN                       "サイショウイドウソクド"           // "VTrav min"
#define MSG_AMAX                            "サイダイカソクド "              // "Amax "
#define MSG_A_RETRACT                       "ヒキコミカソクド"               // "A-retract"
#define MSG_A_TRAVEL                        "イドウカソクド"                // "A-travel"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "オンド"                      // "Temperature"
#define MSG_MOTION                          "ウゴキセッテイ"                // "Motion"
#define MSG_VOLUMETRIC                      "フィラメント"                   // "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "フィラメントチョッケイ"            // "Fil. Dia."
#define MSG_CONTRAST                        "LCDコントラスト"               // "LCD contrast"
#define MSG_STORE_EPROM                     "メモリヘカクノウ"               // "Store memory"
#define MSG_LOAD_EPROM                      "メモリカラヨミコミ"               // "Load memory"
#define MSG_RESTORE_FAILSAFE                "セッテイリセット"               // "Restore failsafe"
#define MSG_REFRESH                         "リフレッシュ"                  // "Refresh"
#define MSG_WATCH                           "ジョウホウガメン"               // "Info screen"
#define MSG_PREPARE                         "ジュンビセッテイ"               // "Prepare"
#define MSG_TUNE                            "チョウセイ"                    // "Tune"
#define MSG_PAUSE_PRINT                     "イチジテイシ"                  // "Pause print"
#define MSG_RESUME_PRINT                    "プリントサイカイ"                // "Resume print"
#define MSG_STOP_PRINT                      "プリントテイシ"                 // "Stop print"
#define MSG_CARD_MENU                       "SDカードカラプリント"            // "Print from SD"
#define MSG_NO_CARD                         "SDカードガアリマセン"            // "No SD card"
#define MSG_DWELL                           "キュウシ"                     // "Sleep..."
#define MSG_USERWAIT                        "シバラクオマチクダサイ"           // "Wait for user..."
#define MSG_RESUMING                        "プリントサイカイ"                // "Resuming print"
#define MSG_PRINT_ABORTED                   "プリントガチュウシサレマシタ"       // "Print aborted"
#define MSG_NO_MOVE                         "ウゴキマセン"                  // "No move."
#define MSG_KILLED                          "ヒジョウテイシ"                  // "KILLED. "
#define MSG_STOPPED                         "テイシシマシタ"                  // "STOPPED. "
#define MSG_CONTROL_RETRACT                 "ヒキコミリョウ mm"                // "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "ヒキコミリョウS mm"               // "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "ヒキコミソクド mm/s"             // "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "ノズルタイヒ mm"                // "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "ホショウリョウ mm"               // "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "ホショウリョウS mm"              // "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "ホショウソクド mm/s"            // "UnRet  V"
#define MSG_AUTORETRACT                     "ジドウヒキコミ"                 // "AutoRetr."
#define MSG_FILAMENTCHANGE                  "フィラメントコウカン"              // "Change filament"
#define MSG_INIT_SDCARD                     "SDカードサイヨミコミ"             // "Init. SD card"
#define MSG_CNG_SDCARD                      "SDカードコウカン"               // "Change SD card"
#define MSG_ZPROBE_OUT                      "Zプローブ ベッドガイ"            // "Z probe out. bed"
#define MSG_BLTOUCH_SELFTEST                "BLTouch ジコシンダン"          // "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   "BLTouch リセット"             // "Reset BLTouch"
#define MSG_HOME                            "サキニ"                      // "Home" // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "ヲフッキサセテクダサイ"           // "first"
#define MSG_ZPROBE_ZOFFSET                  "Zオフセット"                   // "Z Offset"
#define MSG_BABYSTEP_X                      "Xジク ビドウ"                  // "Babystep X"
#define MSG_BABYSTEP_Y                      "Yジク ビドウ"                  // "Babystep Y"
#define MSG_BABYSTEP_Z                      "Zジク ビドウ"                  // "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "イドウゲンカイケンチキノウ"         // "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "カネツシッパイ"                 // "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              "エラー:ジョウチョウサーミスターキノウ"  // "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 "ネツボウソウ"                   // "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "エラー:サイコウオンチョウカ"         // "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "エラー:サイテイオンミマン"          // "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "エラー:ベッド サイコウオンチョウカ"    // "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 "エラー:ベッド サイテイオンミマン"     // "Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    MSG_HOME " " MSG_X MSG_Y " " MSG_FIRST // "G28 Z Forbidden"
#define MSG_HALTED                          "プリンターハテイシシマシタ"         // "PRINTER HALTED"
#define MSG_PLEASE_RESET                    "リセットシテクダサイ"              // "Please reset"
#define MSG_SHORT_DAY                       "d"                          // One character only
#define MSG_SHORT_HOUR                      "h"                          // One character only
#define MSG_SHORT_MINUTE                    "m"                          // One character only
#define MSG_HEATING                         "カネツチュウ"                   // "Heating..."
#define MSG_HEATING_COMPLETE                "カネツカンリョウ"                 // "Heating done."
#define MSG_BED_HEATING                     "ベッド カネツチュウ"              // "Bed Heating."
#define MSG_BED_DONE                        "ベッド カネツカンリョウ"            // "Bed done."
#define MSG_DELTA_CALIBRATE                 "デルタ コウセイ"                // "Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               "Xジク コウセイ"                 // "Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               "Yジク コウセイ"                 // "Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               "Zジク コウセイ"                 // "Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          "チュウシン コウセイ"              // "Calibrate Center"
#define MSG_INFO_MENU                       "コノプリンターニツイテ"             // "About Printer"
#define MSG_INFO_PRINTER_MENU               "プリンタージョウホウ"              // "Printer Info"
#define MSG_INFO_STATS_MENU                 "プリントジョウキョウ"              // "Printer Stats"
#define MSG_INFO_BOARD_MENU                 "セイギョケイジョウホウ"            // "Board Info"
#define MSG_INFO_THERMISTOR_MENU            "サーミスター"                   // "Thermistors"
#define MSG_INFO_EXTRUDERS                  "エクストルーダースウ"             // "Extruders"
#define MSG_INFO_BAUDRATE                   "ボーレート"                    // "Baud"
#define MSG_INFO_PROTOCOL                   "プロトコル"                    // "Protocol"
#define MSG_INFO_PRINT_COUNT                "プリントスウ   "                // "Print Count"
#define MSG_INFO_COMPLETED_PRINTS           "カンリョウスウ"                  // "Completed"
#define MSG_INFO_PRINT_TIME                 "プリントジカンルイケイ"            // "Total print time"
#define MSG_INFO_PRINT_LONGEST              "サイチョウプリントジカン"           // "Longest job time"
#define MSG_INFO_PRINT_FILAMENT             "フィラメントシヨウリョウルイケイ"       // "Extruded total"
#define MSG_INFO_MIN_TEMP                   "セッテイサイテイオン"              // "Min Temp"
#define MSG_INFO_MAX_TEMP                   "セッテイサイコウオン"              // "Max Temp"
#define MSG_INFO_PSU                        "デンゲンシュベツ"                // "Power Supply"
#define MSG_FILAMENT_CHANGE_HEADER          "フィラメントコウカン"              // "CHANGE FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "ドウサヲセンタクシテクダサイ"        // "CHANGE OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "サラニオシダス"                 // "Extrude more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "プリントサイカイ"                // "Resume print"
#define MSG_FILAMENT_CHANGE_INIT_1          "コウカンヲカイシシマス"            // "Wait for start"
#define MSG_FILAMENT_CHANGE_INIT_2          "シバラクオマチクダサイ"            // "of the filament"
#define MSG_FILAMENT_CHANGE_INIT_3          ""                           // "change"
#define MSG_FILAMENT_CHANGE_UNLOAD_1        "フィラメントヌキダシチュウ"          // "Wait for"
#define MSG_FILAMENT_CHANGE_UNLOAD_2        "シバラクオマチクダサイ"            // "filament unload"
#define MSG_FILAMENT_CHANGE_UNLOAD_3        ""
#define MSG_FILAMENT_CHANGE_INSERT_1        "フィラメントヲソウニュウシ,"         // "Insert filament"
#define MSG_FILAMENT_CHANGE_INSERT_2        "クリックスルトゾッコウシマス"         // "and press button"
#define MSG_FILAMENT_CHANGE_INSERT_3        ""                           // "to continue..."
#define MSG_FILAMENT_CHANGE_LOAD_1          "フィラメントソウテンチュウ"          // "Wait for"
#define MSG_FILAMENT_CHANGE_LOAD_2          "シバラクオマチクダサイ"            // "filament load"
#define MSG_FILAMENT_CHANGE_LOAD_3          ""
#define MSG_FILAMENT_CHANGE_EXTRUDE_1       "フィラメントオシダシチュウ"          // "Wait for"
#define MSG_FILAMENT_CHANGE_EXTRUDE_2       "シバラクオマチクダサイ"            // "filament extrude"
#define MSG_FILAMENT_CHANGE_EXTRUDE_3       ""
#define MSG_FILAMENT_CHANGE_RESUME_1        "プリントヲサイカイシマス"           // "Wait for print"
#define MSG_FILAMENT_CHANGE_RESUME_2        "シバラクオマチクダサイ"            // "to resume"
#define MSG_FILAMENT_CHANGE_RESUME_3        ""

#endif // LANGUAGE_KANA_UTF_H

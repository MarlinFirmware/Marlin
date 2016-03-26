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
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_KANA

// This is very crude replacement of the codes used in language_kana.h from somebody who really does not know what he is doing.
// Just to show the potential benefit of unicode.
// This translation can be improved by using the full charset of unicode codeblock U+30A0 to U+30FF.

// 片仮名表示定義
#define WELCOME_MSG                         MACHINE_NAME " ready."
#define MSG_SD_INSERTED                     "カード ソウニュウサレマシタ"        // "Card inserted"
#define MSG_SD_REMOVED                      "カードガアリマセン"               // "Card removed"
#define MSG_MAIN                            "メイン"                       // "Main"
#define MSG_AUTOSTART                       "ジドウカイシ"                   // "Autostart"
#define MSG_DISABLE_STEPPERS                "モーターデンゲン オフ"            // "Disable steppers"
#define MSG_AUTO_HOME                       "ゲンテンニイドウ"                // "Auto home"
#define MSG_LEVEL_BED_HOMING                "ゲンテンニイドウ"                // "Homing"
#define MSG_SET_HOME_OFFSETS                "キジュンオフセットセッテイ"         // "Set home offsets"
#define MSG_SET_ORIGIN                      "キジュンセット"                 // "Set origin"
#define MSG_PREHEAT_PLA                     "PLA ヨネツ"                   // "Preheat PLA"
#define MSG_PREHEAT_PLA_N                   MSG_PREHEAT_PLA " "
#define MSG_PREHEAT_PLA_ALL                 MSG_PREHEAT_PLA " スベテ"     // " All"
#define MSG_PREHEAT_PLA_BEDONLY             MSG_PREHEAT_PLA " ベッド"     // "Bed"
#define MSG_PREHEAT_PLA_SETTINGS            MSG_PREHEAT_PLA " セッテイ"    // "conf"
#define MSG_PREHEAT_ABS                     "ABS ヨネツ"                  // "Preheat ABS"
#define MSG_PREHEAT_ABS_N                   MSG_PREHEAT_ABS " "
#define MSG_PREHEAT_ABS_ALL                 MSG_PREHEAT_ABS " スベテ"     // " All"
#define MSG_PREHEAT_ABS_BEDONLY             MSG_PREHEAT_ABS " ベッド"     // "Bed"
#define MSG_PREHEAT_ABS_SETTINGS            MSG_PREHEAT_ABS " セッテイ"    // "conf"
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
#define MSG_SPEED                           "スピード"                     // "Speed"
#define MSG_BED_Z                           "Zオフセット"                   // "Bed Z"
#define MSG_NOZZLE                          "ノズル"                       // "Nozzle"
#define MSG_BED                             "ベッド"                       // "Bed"
#define MSG_FAN_SPEED                       "ファンソクド"                    // "Fan speed"
#define MSG_FLOW                            "オクリリョウ"                     // "Flow"
#define MSG_CONTROL                         "コントロール"                    // "Control"
#define MSG_MIN                             LCD_STR_THERMOMETER " サイテイ" // " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " サイコウ" // " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " ファクター" // " Fact"
#define MSG_AUTOTEMP                        "ジドウオンド"                    // "Autotemp"
#define MSG_ON                              "オン "                         // "On "
#define MSG_OFF                             "オフ "                         // "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "カソクド mm/s^2"              // "Accel"
#define MSG_VXY_JERK                        "XYジク ヤクド mm/s"            // "Vxy-jerk"
#define MSG_VZ_JERK                         "Zジク ヤクド mm/s"             // "Vz-jerk"
#define MSG_VE_JERK                         "エクストルーダー ヤクド"          // "Ve-jerk"
#define MSG_VMAX                            "サイダイフィードレート "           // "Vmax "
#define MSG_X                               "X"                         // "x"
#define MSG_Y                               "Y"                         // "y"
#define MSG_Z                               "Z"                         // "z"
#define MSG_E                               "E"                         // "e"
#define MSG_VMIN                            "サイショウフィードレート"          // "Vmin"
#define MSG_VTRAV_MIN                       "サイショウトラベルレート"          // "VTrav min"
#define MSG_AMAX                            "サイダイカソクド "              // "Amax "
#define MSG_A_RETRACT                       "ヒキコミカソクド"               // "A-retract"
#define MSG_A_TRAVEL                        "トラベルカソクド"               // "A-travel"
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
#define MSG_WATCH                           "インフォ"                     // "Info screen"
#define MSG_PREPARE                         "ジュンビセッテイ"               // "Prepare"
#define MSG_TUNE                            "チョウセイ"                    // "Tune"
#define MSG_PAUSE_PRINT                     "イチジテイシ"                  // "Pause print"
#define MSG_RESUME_PRINT                    "プリントサイカイ"                // "Resume print"
#define MSG_STOP_PRINT                      "プリントテイシ"                 // "Stop print"
#define MSG_CARD_MENU                       "SDカードカラプリント"            // "Print from SD"
#define MSG_NO_CARD                         "SDカードガアリマセン"            // "No SD card"
#define MSG_DWELL                           "スリープ"                     // "Sleep..."
#define MSG_USERWAIT                        "シバラクオマチクダサイ"           // "Wait for user..."
#define MSG_RESUMING                        "プリントサイカイ"                // "Resuming print"
#define MSG_PRINT_ABORTED                   "プリントチュウシサレマシタ"          // "Print aborted"
#define MSG_NO_MOVE                         "ウゴキマセン"                  // "No move."
#define MSG_KILLED                          "ヒジョウテイシ"                  // "KILLED. "
#define MSG_STOPPED                         "テイシシマシタ"                  // "STOPPED. "
#define MSG_CONTROL_RETRACT                 "ヒキコミリョウ mm"                // "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "ヒキコミリョウS mm"               // "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "ヒキコミレート mm/s"             // "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "ノズルタイヒ mm"                // "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "リカバー +mm"                 // "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "リカバーS +mm"                // "S UnRet+mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "リカバーレート mm/s"            // "UnRet  V"
#define MSG_AUTORETRACT                     "ジドウヒキコミ"                 // "AutoRetr."
#define MSG_FILAMENTCHANGE                  "フィラメントコウカン"              // "Change filament"
#define MSG_INIT_SDCARD                     "SDカードサイヨミコミ"             // "Init. SD card"
#define MSG_CNG_SDCARD                      "SDカードコウカン"               // "Change SD card"
#define MSG_ZPROBE_OUT                      "Zプローブ ベッドガイ"            // "Z probe out. bed"
#define MSG_YX_UNHOMED                      "ゲンテンハXYイドウゴZ"           // "Home X/Y before Z"
#define MSG_ZPROBE_ZOFFSET                  "Zオフセット"                   // "Z Offset"
#define MSG_BABYSTEP_X                      "Xジク ビドウ"                  // "Babystep X"
#define MSG_BABYSTEP_Y                      "Yジク ビドウ"                  // "Babystep Y"
#define MSG_BABYSTEP_Z                      "Zジク ビドウ"                  // "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "エンドストップ サドウ"            // "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "カネツシッパイ"                 // "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              "エラー:ジョウチョウサーミスターキノウ"  // "Err: REDUNDANT TEMP ERROR"
#define MSG_THERMAL_RUNAWAY                 "ネツボウソウ"                   // "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "エラー:サイコウオンチョウカ"         // "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "エラー:サイテイオンミマン"          // "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "エラー:ベッド サイコウオンチョウカ"    // "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 "エラー:ベッド サイテイオンミマン"     // "Err: MINTEMP BED"
#define MSG_END_HOUR                        "ジカン"                       // "hours"
#define MSG_END_MINUTE                      "フン"                         // "minutes"
#define MSG_HEATING                         "カネツチュウ"                   // "Heating..."
#define MSG_HEATING_COMPLETE                "カネツカンリョウ"                 // "Heating done."
#define MSG_BED_HEATING                     "ベッド カネツチュウ"              // "Bed Heating."
#define MSG_BED_DONE                        "ベッド カネツカンリョウ"            // "Bed done."

#if ENABLED(DELTA_CALIBRATION_MENU)
  #define MSG_DELTA_CALIBRATE               "デルタ コウセイ"                // "Delta Calibration"
  #define MSG_DELTA_CALIBRATE_X             "Xジク コウセイ"                 // "Calibrate X"
  #define MSG_DELTA_CALIBRATE_Y             "Yジク コウセイ"                 // "Calibrate Y"
  #define MSG_DELTA_CALIBRATE_Z             "Zジク コウセイ"                 // "Calibrate Z"
  #define MSG_DELTA_CALIBRATE_CENTER        "チュウシン コウセイ"              // "Calibrate Center"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_KANA_UTF_H

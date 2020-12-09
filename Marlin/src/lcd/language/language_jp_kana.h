/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Japanese (Kana)
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */

//#define DISPLAY_CHARSET_ISO10646_KANA

namespace Language_jp_kana {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 3;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Japanese");

  // This is just to show the potential benefit of Unicode.
  // This translation can be improved by using the full charset of unicode codeblock U+30A0 to U+30FF.

  // 片仮名表示定義
  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" ジュンビカンリョウ");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("メディアガソウニュウサレマシタ");        // "Card inserted"
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("メディアガアリマセン");               // "Card removed"
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("メディアノトリダシ");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("エンドストップ");                  // "Endstops" // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("メイン");                       // "Main"
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("ジドウカイシ");                   // "Autostart"
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("モーターデンゲン オフ");            // "Disable steppers"
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("デバッグメニュー");                // "Debug Menu"
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("プログレスバー テスト");            // "Progress Bar Test"
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("ゲンテンフッキ");                  // "Auto home"
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Xジク ゲンテンフッキ");             // "Home X"
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Yジク ゲンテンフッキ");             // "Home Y"
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Zジク ゲンテンフッキ");             // "Home Z"
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("ゲンテンフッキチュウ");              // "Homing XYZ"
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("レベリングカイシ");                // "Click to Begin"
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("ツギノソクテイテンヘ");             // "Next Point"
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("レベリングカンリョウ");              // "Leveling Done!"
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("キジュンオフセットセッテイ");         // "Set home offsets"
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("オフセットガテキヨウサレマシタ");       // "Offsets applied"
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("キジュンセット");                 // "Set origin"
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = PREHEAT_1_LABEL _UxGT(" ヨネツ");       // "Preheat " PREHEAT_1_LABEL
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = PREHEAT_1_LABEL _UxGT(" ヨネツ ~");       // "Preheat " PREHEAT_1_LABEL
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = PREHEAT_1_LABEL _UxGT(" ヨネツノズル");  // " Nozzle"
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = PREHEAT_1_LABEL _UxGT(" ヨネツノズル ~");  // " Nozzle"
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = PREHEAT_1_LABEL _UxGT(" スベテヨネツ");  // " All"
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = PREHEAT_1_LABEL _UxGT(" ベッドヨネツ");  // " Bed"
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = PREHEAT_1_LABEL _UxGT(" ヨネツセッテイ");  // " conf"

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("$ ヨネツ");       // "Preheat " PREHEAT_1_LABEL
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("$ ヨネツ ~");       // "Preheat " PREHEAT_1_LABEL
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("$ ヨネツノズル");  // " Nozzle"
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("$ ヨネツノズル ~");  // " Nozzle"
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("$ スベテヨネツ");  // " All"
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("$ ベッドヨネツ");  // " Bed"
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("$ ヨネツセッテイ");  // " conf"
  #endif
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("カネツテイシ");                  // "Cooldown"
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("デンゲン オン");                 // "Switch power on"
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("デンゲン オフ");                 // "Switch power off"
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("オシダシ");                     // "Extrude"
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("ヒキコミセッテイ");                // "Retract"
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("ジクイドウ");                    // "Move axis"
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("ベッドレベリング");                // "Bed leveling"
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("ベッドレベリング");                // "Level bed"

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("イドウチュウ");                   // "Moving..."
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("XYジク カイホウ");                // "Free XY"
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Xジク イドウ");                  // "Move X"
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Yジク イドウ");                  // "Move Y"
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Zジク イドウ");                  // "Move Z"
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("エクストルーダー");                // "Extruder"
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("%smm イドウ");                  // "Move 0.025mm"
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("0.1mm イドウ");                 // "Move 0.1mm"
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("  1mm イドウ");                 // "Move 1mm"
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT(" 10mm イドウ");                 // "Move 10mm"
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("ソクド");                       // "Speed"
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Zオフセット");                   // "Bed Z"
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("ノズル");                       // "Nozzle"
  PROGMEM Language_Str MSG_BED                             = _UxGT("ベッド");                       // "Bed"
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("ファンソクド");                    // "Fan speed"
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("トシュツリョウ");                   // "Flow"
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("セイギョ");                      // "Control"
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" サイテイ"); // " Min"
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" サイコウ"); // " Max"
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" ファクター"); // " Fact"
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("ジドウオンドセイギョ");               // "Autotemp"
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("オン");                         // "On"
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("オフ");                         // "Off"
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("センタク");                     // "Select"
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("センタク *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("カソクド mm/s") SUPERSCRIPT_TWO;               // "Accel"
  PROGMEM Language_Str MSG_JERK                            = _UxGT("ヤクドウ mm/s");                  // "Jerk"
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("ジク ヤクドウ mm/s") LCD_STR_A;             // "Va-jerk"
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("ジク ヤクドウ mm/s") LCD_STR_B;             // "Vb-jerk"
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("ジク ヤクドウ mm/s") LCD_STR_C;             // "Vc-jerk"
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("ステップ/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("ステップ/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("ステップ/mm");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("エクストルーダー ヤクド");          // "Ve-jerk"
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("サイダイオクリソクド ") LCD_STR_A;  // "Vmax A"
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("サイダイオクリソクド ") LCD_STR_A;  // "Vmax B"
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("サイダイオクリソクド ") LCD_STR_A;  // "Vmax C"
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("サイダイオクリソクド ") LCD_STR_A;  // "Vmax E"
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("サイダイオクリソクド *");  // "Vmax E1"
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("サイショウオクリソクド");           // "Vmin"
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("サイショウイドウソクド");           // "VTrav min"
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("カソクド mm/s") SUPERSCRIPT_TWO;               // "Accel"
  PROGMEM Language_Str MSG_AMAX                            = _UxGT("サイダイカソクド ");              // "Amax "
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("ヒキコミカソクド");               // "A-retract"
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("イドウカソクド");                // "A-travel"
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("オンド");                      // "Temperature"
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("ウゴキセッテイ");                // "Motion"
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("フィラメント");                   // "Filament"
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("フィラメントチョッケイ");            // "Fil. Dia."
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("フィラメントチョッケイ *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCDコントラスト");               // "LCD contrast"
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("EEPROMヘホゾン");               // "Store memory"
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("EEPROMカラヨミコミ");               // "Load memory"
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("セッテイリセット");               // "Restore Defaults"
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("リフレッシュ");                  // "Refresh"
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("ジョウホウガメン");               // "Info screen"
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("ジュンビセッテイ");               // "Prepare"
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("チョウセイ");                    // "Tune"
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("イチジテイシ");                  // "Pause print"
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("プリントサイカイ");                // "Resume print"
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("プリントテイシ");                 // "Stop print"
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("メディアカラプリント");            // "Print from SD"
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("メディアガアリマセン");               // "Card removed"
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("キュウシ");                     // "Sleep..."
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("ツヅケルニハクリックシテクダサイ");  // "Wait for user..."
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("プリントガチュウシサレマシタ");       // "Print aborted"
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("ウゴキマセン");                  // "No move."
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ヒジョウテイシ");                  // "KILLED. "
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("テイシシマシタ");                  // "STOPPED. "
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("ヒキコミリョウ mm");                // "Retract mm"
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("ヒキコミリョウS mm");               // "Swap Re.mm"
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("ヒキコミソクド mm/s");             // "Retract V"
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("ノズルタイヒ mm");                // "Hop mm"
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("ホショウリョウ mm");               // "Unretr. mm"
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("ホショウリョウS mm");              // "S Unretr. mm"
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("ホショウソクド mm/s");            // "Unretract V"
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("ジドウヒキコミ");                 // "Auto-Retract"
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("フィラメントコウカン");              // "Change filament"
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("メディアサイヨミコミ");             // "Init. SD card"
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("メディアコウカン");               // "Change SD card"
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Zプローブ ベッドガイ");            // "Z probe out. bed"
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch ジコシンダン");          // "BLTouch Self-Test"
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("BLTouch リセット");             // "Reset BLTouch"
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("サキニ %s%s%s ヲフッキサセテクダサイ"); // "Home ... first"
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Zオフセット");                   // "Z Offset"
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Xジク ビドウ");                  // "Babystep X"
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Yジク ビドウ");                  // "Babystep Y"
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Zジク ビドウ");                  // "Babystep Z"
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("イドウゲンカイケンチキノウ");         // "Endstop abort"
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("カネツシッパイ");                 // "Heating failed"
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("エラー:ジョウチョウサーミスターキノウ");  // "Err: REDUNDANT TEMP"
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ネツボウソウ");                   // "THERMAL RUNAWAY"
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("エラー:サイコウオンチョウカ");         // "Err: MAXTEMP"
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("エラー:サイテイオンミマン");          // "Err: MINTEMP"
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("プリンターハテイシシマシタ");         // "PRINTER HALTED"
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("リセットシテクダサイ");              // "Please reset"
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d");                          // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h");                          // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m");                          // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("カネツチュウ");                   // "Heating..."
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("ベッド カネツチュウ");              // "Bed Heating..."
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("デルタ コウセイ");                // "Delta Calibration"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Xジク コウセイ");                 // "Calibrate X"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Yジク コウセイ");                 // "Calibrate Y"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Zジク コウセイ");                 // "Calibrate Z"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("チュウシン コウセイ");              // "Calibrate Center"
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("コノプリンターニツイテ");             // "About Printer"
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("プリンタージョウホウ");              // "Printer Info"
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("プリントジョウキョウ");              // "Printer Stats"
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("セイギョケイジョウホウ");            // "Board Info"
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("サーミスター");                   // "Thermistors"
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("エクストルーダースウ");             // "Extruders"
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("ボーレート");                    // "Baud"
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("プロトコル");                    // "Protocol"
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("キョウタイナイショウメイ");       // "Case light"
  PROGMEM Language_Str MSG_INFO_PRINT_COUNT                = _UxGT("プリントスウ ");                  // "Print Count"
  PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS           = _UxGT("カンリョウスウ");                  // "Completed"
  PROGMEM Language_Str MSG_INFO_PRINT_TIME                 = _UxGT("プリントジカンルイケイ");            // "Total print time"
  PROGMEM Language_Str MSG_INFO_PRINT_LONGEST              = _UxGT("サイチョウプリントジカン");           // "Longest job time"
  PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT             = _UxGT("フィラメントシヨウリョウルイケイ");       // "Extruded total"
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("セッテイサイテイオン");              // "Min Temp"
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("セッテイサイコウオン");              // "Max Temp"
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("デンゲンシュベツ");                // "Power Supply"
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("モータークドウリョク");              // "Drive Strength"
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X DACシュツリョク %");               // "X Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y DACシュツリョク %");               // "Y Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z DACシュツリョク %");               // "Z Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E DACシュツリョク %");               // "E Driver %"
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("EEPROMヘホゾン");               // "Store memory"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("イチジテイシ");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("プリントサイカイ");                // "Resume print"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT            = _UxGT(MSG_2_LINE("コウカンヲカイシシマス", "シバラクオマチクダサイ"));   // "Wait for start of the filament"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD          = _UxGT(MSG_2_LINE("フィラメントヌキダシチュウ", "シバラクオマチクダサイ"));   // "Wait for filament unload"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT          = _UxGT(MSG_2_LINE("フィラメントヲソウニュウシ,", "クリックスルトゾッコウシマス"));   // "Insert filament and press button"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD            = _UxGT(MSG_2_LINE("フィラメントソウテンチュウ", "シバラクオマチクダサイ"));   // "Wait for filament load"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME          = _UxGT(MSG_2_LINE("プリントヲサイカイシマス", "シバラクオマチクダサイ"));   // "Wait for print to resume"

  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("マチガッタプリンター");               // "Wrong printer"

  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("セッテイカンリ");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("ショウサイセッテイ");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("コショカイフク");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("EEPROMショキカ");

  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("ツギヘ");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("ショキカ");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("ストップ");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("プリント");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("リセット");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("キャンセル");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("カンリョウ");

  PROGMEM Language_Str MSG_YES                             = _UxGT("ハイ");
  PROGMEM Language_Str MSG_NO                              = _UxGT("イイエ");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("モドリ");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("ソクド");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("ステップ/mm");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("ユーザーコマンド");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("プリントガイチジテイシサレマシタ");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("プリントチュウ...");
}

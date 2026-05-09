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
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

//#define DISPLAY_CHARSET_ISO10646_KANA

namespace LanguageNarrow_jp_kana {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 3;
  LSTR LANGUAGE                           = _UxGT("Japanese");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" ジュンビカンリョウ");            // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("メディアガソウニュウサレマシタ");                          // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("メディアガアリマセン");                               // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("エンドストップ");                                  // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("メイン");                                      // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("モーターデンゲン オフ");                              // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("ゲンテンフッキ");                                  // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Xジク ゲンテンフッキ");                              // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Yジク ゲンテンフッキ");                              // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Zジク ゲンテンフッキ");                              // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("ゲンテンフッキチュウ");                               // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("レベリングカイシ");                                 // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("ツギノソクテイテンヘ");                               // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("レベリングカンリョウ");                               // Leveling Done!
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("キジュンオフセットセッテイ");                            // Set Home Offsets
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("オフセットガテキヨウサレマシタ");                          // Offsets Applied
  LSTR MSG_PREHEAT_1                      = PREHEAT_1_LABEL _UxGT(" ヨネツ");                     // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = PREHEAT_1_LABEL _UxGT(" ヨネツ ~");                   // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = PREHEAT_1_LABEL _UxGT(" ヨネツノズル");                  // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = PREHEAT_1_LABEL _UxGT(" ヨネツノズル ~");                // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = PREHEAT_1_LABEL _UxGT(" スベテヨネツ");                  // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = PREHEAT_1_LABEL _UxGT(" ベッドヨネツ");                  // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = PREHEAT_1_LABEL _UxGT(" ヨネツセッテイ");                 // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("$ ヨネツ");                                    // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("$ ヨネツ ~");                                  // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("$ ヨネツノズル");                                 // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("$ ヨネツノズル ~");                               // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("$ スベテヨネツ");                                 // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("$ ベッドヨネツ");                                 // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("$ ヨネツセッテイ");                                // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("カネツテイシ");                                   // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("デンゲン オン");                                  // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("デンゲン オフ");                                  // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("オシダシ");                                     // Extrude
  LSTR MSG_RETRACT                        = _UxGT("ヒキコミセッテイ");                                 // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("ジクイドウ");                                    // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("プローブ アンド レベリング");                           // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("ベッドレベリング");                                 // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("ベッドレベリング");                                 // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Xジク イドウ");                                  // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Yジク イドウ");                                  // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Zジク イドウ");                                  // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("@ジク イドウ");                                  // Move @
  LSTR MSG_MOVE_E                         = _UxGT("エクストルーダー");                                 // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("* エクストルーダー");                               // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("$mm イドウ");                                  // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("$in イドウ");                                  // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("$") LCD_STR_DEGREE _UxGT(" イドウ");           // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("ソクド");                                      // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Zオフセット");                                   // Bed Z
  LSTR MSG_NOZZLE                         = _UxGT("ノズル");                                      // Nozzle
  LSTR MSG_BED                            = _UxGT("ベッド");                                      // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("ファンソクド");                                   // Fan Speed
  LSTR MSG_FLOW                           = _UxGT("トシュツリョウ");                                  // Flow
  LSTR MSG_CONTROL                        = _UxGT("セイギョ");                                     // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("ジドウオンドセイギョ");                               // Autotemp
  LSTR MSG_SELECT_E                       = _UxGT("センタク *");                                   // Select *
  LSTR MSG_ACC                            = _UxGT("カソクド mm/s") SUPERSCRIPT_TWO;                // Accel
  LSTR MSG_VTRAV_MIN                      = _UxGT("サイショウイドウソクド");                              // Min Travel Speed
  LSTR MSG_ACCELERATION                   = _UxGT("カソクド mm/s") SUPERSCRIPT_TWO;                // Acceleration
  LSTR MSG_A_RETRACT                      = _UxGT("ヒキコミカソクド");                                 // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("イドウカソクド");                                  // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("ステップ/mm");                                  // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT("ステップ/mm");                            // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT("ステップ/mm");                            // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT("ステップ/mm");                            // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ステップ/mm");                                 // @ steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("オンド");                                      // Temperature
  LSTR MSG_MOTION                         = _UxGT("ウゴキセッテイ");                                  // Motion
  LSTR MSG_FILAMENT                       = _UxGT("フィラメント");                                   // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("フィラメントチョッケイ");                              // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("フィラメントチョッケイ *");                            // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("LCDコントラスト");                                // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("EEPROMヘホゾン");                               // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("EEPROMカラヨミコミ");                             // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("セッテイリセット");                                 // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("リフレッシュ");                   // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("ジョウホウガメン");                                 // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("ジュンビセッテイ");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("チョウセイ");                                    // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("イチジテイシ");                                   // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("プリントサイカイ");                                 // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("プリントテイシ");                                  // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("メディアカラプリント");                               // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("メディアガアリマセン");                               // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("キュウシ");                                     // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("ツヅケルニハクリックシテクダサイ");                         // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("プリントガチュウシサレマシタ");                           // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("ウゴキマセン");                                   // No Move.
  LSTR MSG_KILLED                         = _UxGT("ヒジョウテイシ");                                  // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("テイシシマシタ");                                  // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("ヒキコミリョウ mm");                               // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("ヒキコミリョウS mm");                              // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("ヒキコミソクド mm/s");                             // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("ノズルタイヒ mm");                                // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("ホショウリョウ mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("ホショウリョウS mm");                              // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("ホショウソクド mm/s");                             // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("ジドウヒキコミ");                                  // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("フィラメントコウカン");                               // Change Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("SDカードをマウントする");                             // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("SDカードをマウントする");                             // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("USBメモリをマウントする");                            // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("メディアコウカン");                                 // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("ジドウカイシ");                                   // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Zプローブ ベッドガイ");                              // Z Probe Past Bed
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("BLTouch リセット");                             // Reset
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Xオフセット");                                   // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Yオフセット");                                   // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Zオフセット");                                   // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("@オフセット");                                   // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("プローブZオフセット");                               // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Xジク ビドウ");                                  // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Yジク ビドウ");                                  // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Zジク ビドウ");                                  // Babystep Z
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("イドウゲンカイケンチキノウ");                            // Endstop Abort
  LSTR MSG_ERR_HEATING_FAILED             = _UxGT("カネツシッパイ");                                  // Heating Failed
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("エラー:ジョウチョウサーミスターキノウ");                      // Err: REDUNDANT TEMP
  LSTR MSG_ERR_THERMAL_RUNAWAY            = _UxGT("ネツボウソウ");                                   // THERMAL RUNAWAY
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("エラー:サイコウオンチョウカ");                           // Err: MAXTEMP
  LSTR MSG_ERR_MINTEMP                    = _UxGT("エラー:サイテイオンミマン");                            // Err: MINTEMP
  LSTR MSG_HALTED                         = _UxGT("プリンターハテイシシマシタ");                            // PRINTER HALTED
  LSTR MSG_PLEASE_RESET                   = _UxGT("リセットシテクダサイ");                               // Please Reset
  LSTR MSG_HEATING                        = _UxGT("カネツチュウ");                                   // Heating...
  LSTR MSG_BED_HEATING                    = _UxGT("ベッド カネツチュウ");                               // Bed Heating...
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("デルタ コウセイ");                                 // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Xジク コウセイ");                                 // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Yジク コウセイ");                                 // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Zジク コウセイ");                                 // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("チュウシン コウセイ");                               // Calibrate Center
  LSTR MSG_INFO_MENU                      = _UxGT("コノプリンターニツイテ");                              // About Printer
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("プリンタージョウホウ");                               // Printer Info
  LSTR MSG_INFO_STATS_MENU                = _UxGT("プリントジョウキョウ");                               // Stats
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("セイギョケイジョウホウ");                              // Board Info
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("サーミスター");                                   // Thermistors
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("エクストルーダースウ");                               // Extruders
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("ボーレート");                                    // Baud
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("プロトコル");                                    // Protocol
  LSTR MSG_CASE_LIGHT                     = _UxGT("キョウタイナイショウメイ");                             // Case Light
  LSTR MSG_INFO_PRINT_COUNT               = _UxGT("プリントスウ ");                                  // Prints
  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("カンリョウスウ");                                  // Completed
  LSTR MSG_INFO_PRINT_TIME                = _UxGT("プリントジカンルイケイ");                              // Total
  LSTR MSG_INFO_PRINT_LONGEST             = _UxGT("サイチョウプリントジカン");                             // Longest
  LSTR MSG_INFO_PRINT_FILAMENT            = _UxGT("フィラメントシヨウリョウルイケイ");                         // Extruded
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("セッテイサイテイオン");                               // Min Temp
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("セッテイサイコウオン");                               // Max Temp
  LSTR MSG_INFO_PSU                       = _UxGT("デンゲンシュベツ");                                 // PSU
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("モータークドウリョク");                               // Drive Strength
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("EEPROMヘホゾン");                               // DAC EEPROM Write
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("プリントサイカイ");                                 // Continue
  LSTR MSG_FILAMENT_CHANGE_INIT           = _UxGT(MSG_2_LINE("コウカンヲカイシシマス", "シバラクオマチクダサイ"));   // |Please wait...
  LSTR MSG_FILAMENT_CHANGE_INSERT         = _UxGT(MSG_2_LINE("フィラメントヲソウニュウシ,", "クリックスルトゾッコウシマス")); // |Insert and Click
  LSTR MSG_FILAMENT_CHANGE_UNLOAD         = _UxGT(MSG_2_LINE("フィラメントヌキダシチュウ", "シバラクオマチクダサイ")); // |Ejecting...
  LSTR MSG_FILAMENT_CHANGE_LOAD           = _UxGT(MSG_2_LINE("フィラメントソウテンチュウ", "シバラクオマチクダサイ")); // |Loading...
  LSTR MSG_FILAMENT_CHANGE_RESUME         = _UxGT(MSG_2_LINE("プリントヲサイカイシマス", "シバラクオマチクダサイ"));  // |Resuming...
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" サイテイ");            // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" サイコウ");            // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" ファクター");           // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("オン");                                       // On
  LSTR MSG_LCD_OFF                        = _UxGT("オフ");                                       // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("マチガッタプリンター");                               // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("デバッグメニュー");                                 // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("プログレスバー テスト");                              // Progress Bar Test
  LSTR MSG_MOVING                         = _UxGT("イドウチュウ");                                   // Moving...
  LSTR MSG_FREE_XY                        = _UxGT("XYジク カイホウ");                                // Free XY
  LSTR MSG_HOME_FIRST                     = _UxGT("サキニ %s ヲフッキサセテクダサイ");                       // Home %s First
  LSTR MSG_YES                            = _UxGT("ハイ");                                       // YES
  LSTR MSG_NO                             = _UxGT("イイエ");                                      // NO
  LSTR MSG_BACK                           = _UxGT("モドリ");                                      // Back
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("ショウサイセッテイ");                                // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("セッテイカンリ");                                  // Configuration
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("ユーザーコマンド");                                 // Custom Commands
  LSTR MSG_JERK                           = _UxGT("ヤクドウ mm/s");                                // Jerk
  LSTR MSG_VA_JERK                        = _UxGT("ジク ヤクドウ mm/s") STR_A;                       // Max (STR_A) Jerk
  LSTR MSG_VB_JERK                        = _UxGT("ジク ヤクドウ mm/s") STR_B;                       // Max (STR_B) Jerk
  LSTR MSG_VC_JERK                        = _UxGT("ジク ヤクドウ mm/s") STR_C;                       // Max (STR_C) Jerk
  LSTR MSG_VN_JERK                        = _UxGT("ジク ヤクドウ mm/s@");                            // Max @ Jerk
  LSTR MSG_VE_JERK                        = _UxGT("エクストルーダー ヤクド");                             // Max E Jerk
  LSTR MSG_MAX_SPEED                      = _UxGT("ソクド");                                      // Max Speed (mm/s)
  LSTR MSG_VMAX_A                         = _UxGT("サイダイオクリソクド ") STR_A;                        // Max (STR_A) Speed
  LSTR MSG_VMAX_B                         = _UxGT("サイダイオクリソクド ") STR_B;                        // Max (STR_B) Speed
  LSTR MSG_VMAX_C                         = _UxGT("サイダイオクリソクド ") STR_C;                        // Max (STR_C) Speed
  LSTR MSG_VMAX_E                         = _UxGT("サイダイオクリソクド E");                             // Max E Speed
  LSTR MSG_VMAX_EN                        = _UxGT("サイダイオクリソクド *");                             // Max * Speed
  LSTR MSG_VMIN                           = _UxGT("サイショウオクリソクド");                              // Min Velocity
  LSTR MSG_INIT_EEPROM                    = _UxGT("EEPROMショキカ");                               // Initialize EEPROM
  LSTR MSG_BUTTON_NEXT                    = _UxGT("ツギヘ");                                      // Next
  LSTR MSG_BUTTON_INIT                    = _UxGT("ショキカ");                                     // Init
  LSTR MSG_BUTTON_STOP                    = _UxGT("ストップ");                                     // Stop
  LSTR MSG_BUTTON_PRINT                   = _UxGT("プリント");                                     // Print
  LSTR MSG_BUTTON_RESET                   = _UxGT("リセット");                                     // Reset
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("キャンセル");                                    // Cancel
  LSTR MSG_BUTTON_DONE                    = _UxGT("カンリョウ");                                    // Done
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("コショカイフク");                                  // Power Outage
  LSTR MSG_PRINT_PAUSED                   = _UxGT("プリントガイチジテイシサレマシタ");                         // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("プリントチュウ...");                               // Printing...
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("メディアノトリダシ");                                // Release (MEDIA_TYPE_EN)
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch ジコシンダン");                           // Self-Test
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("@ DACシュツリョク %");                            // @ Driver %
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("イチジテイシ");                                   // PRINT PAUSED
}

namespace LanguageWide_jp_kana {
  using namespace LanguageNarrow_jp_kana;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_jp_kana {
  using namespace LanguageWide_jp_kana;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_jp_kana {
  using namespace LanguageTall_jp_kana;
}

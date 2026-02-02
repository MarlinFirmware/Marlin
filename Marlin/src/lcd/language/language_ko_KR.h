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
 * Korean
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

namespace LanguageNarrow_ko_KR {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 1;
  LSTR LANGUAGE                           = _UxGT("Korean");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" 준비.");                  // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("카드 삽입됨");                                   // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("카드 제거됨");                                   // (MEDIA_TYPE_EN) Removed
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("엔드스탑");                                     // Endstops
  LSTR MSG_MAIN_MENU                      = _UxGT("뒤로");                                       // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("모터 정지");                                    // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("오토홈");                                      // Auto Home
  LSTR MSG_AUTO_HOME_X                    = _UxGT("X 홈으로");                                    // Home X
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Y 홈으로");                                    // Home Y
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Z 홈으로");                                    // Home Z
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("XYZ 홈으로");                                  // Homing XYZ
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("누르면 시작합니다");                                // Click to Begin
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("다음 Point");                                 // Next Point
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("레벨링 완료!");                                  // Leveling Done!
  LSTR MSG_PREHEAT_1                      = _UxGT("예열하기 - ") PREHEAT_1_LABEL;                  // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("예열하기 - ") PREHEAT_1_LABEL " ~";             // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("예열하기 - ") PREHEAT_1_LABEL _UxGT(" 노즐");     // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("예열하기 - ") PREHEAT_1_LABEL _UxGT(" 노즐 ~");   // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_M                      = _UxGT("예열하기 - $");                                 // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("예열하기 - $ ~");                               // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("예열하기 - $ 노즐");                              // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("예열하기 - $ 노즐 ~");                            // Preheat $ End ~
  LSTR MSG_COOLDOWN                       = _UxGT("식히기");                                      // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("스위치 전원 켜기");                                // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("스위치 전원 끄기");                                // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("밀어내기");                                     // Extrude
  LSTR MSG_RETRACT                        = _UxGT("당기기");                                      // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("축 이동");                                     // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("프로브 & 레벨링");                                // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("베드 레벨링");                                   // Bed Leveling
  LSTR MSG_NOZZLE                         = _UxGT("노즐");                                       // Nozzle
  LSTR MSG_NOZZLE_N                       = _UxGT("노즐 ~");                                     // Nozzle ~
  LSTR MSG_BED                            = _UxGT("베드");                                       // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("펜 속도");                                     // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("펜 속도 ~");                                   // Fan Speed ~
  LSTR MSG_TEMPERATURE                    = _UxGT("온도");                                       // Temperature
  LSTR MSG_MOTION                         = _UxGT("동작");                                       // Motion
  LSTR MSG_STORE_EEPROM                   = _UxGT("설정 저장하기");                                  // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("설정 읽어오기");                                  // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("설정 되돌리기");                                  // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("새로고침");                     // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("처음으로");                                     // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("준비하기");                                     // Prepare
  LSTR MSG_PAUSE_PRINT                    = _UxGT("일시정지");                                     // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("재시작");                                      // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("출력중지");                                     // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("SD 카드출력");                                  // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("SD 카드없음");                                  // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("슬립모드...");                                  // Sleep...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("취소됨");                                      // Print Aborted
  LSTR MSG_KILLED                         = _UxGT("죽음. ");                                     // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("멈춤. ");                                     // STOPPED.
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("자동 시작");                                    // Run Autofiles
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Z 값 조정");                                   // Babystep Probe Z
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("잘못된 프린터");                                  // INCORRECT PRINTER
  LSTR MSG_DEBUG_MENU                     = _UxGT("디버깅 메뉴");                                   // Debug Menu
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("프로그레스바 테스트");                               // Progress Bar Test
  LSTR MSG_BACK                           = _UxGT("뒤로");                                       // Back
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("소프트 엔드스탑");                                 // Soft Endstops
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("고급 설정");                                    // Advanced Settings
  LSTR MSG_CONFIGURATION                  = _UxGT("설정");                                       // Configuration
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Custom 예열");                                // Preheat Custom
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("미러 사본");                                    // Mirrored Copy
  LSTR MSG_UBL_DOING_G29                  = _UxGT("오토레벨링 하기");                                 // Doing G29
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("엑스트라 펜 속도");                                // Extra Fan Speed
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("엑스트라 펜 속도 ~");                              // Extra Fan Speed ~
  LSTR MSG_INIT_EEPROM                    = _UxGT("EEPROM 초기화");                               // Initialize EEPROM
  LSTR MSG_PRINT_PAUSED                   = _UxGT("일시 정지됨");                                   // Print Paused
  LSTR MSG_PRINTING                       = _UxGT("출력중...");                                   // Printing...
}

namespace LanguageWide_ko_KR {
  using namespace LanguageNarrow_ko_KR;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_ko_KR {
  using namespace LanguageWide_ko_KR;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_ko_KR {
  using namespace LanguageTall_ko_KR;
}

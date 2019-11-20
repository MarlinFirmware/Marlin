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
 * Korean
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#ifndef LANGUAGE_KO_KR_H
#define LANGUAGE_KO_KR_H

#define DISPLAY_CHARSET_ISO10646_KO_KR

#define WELCOME_MSG                         MACHINE_NAME " \xdd\xaf" //"준비" ("Ready.")
#define MSG_SD_INSERTED                     "\xeb\x97 \xb0\xce"  // "카드 삽입" ("Card inserted")
#define MSG_SD_REMOVED                      "\xeb\x97 \xd9\x86" // "카드 제거" ("Card removed")
#define MSG_LCD_ENDSTOPS                    "Endstops"   // Max length 8 characters
#define MSG_MAIN                            "\xa5\xcc"      // "메인" ("Main")
#define MSG_AUTOSTART                       "\xcf\x95 \xba\xd0" // "자동 시작" ("Autostart")
#define MSG_DISABLE_STEPPERS                "\xa8\xec \xd8\xe0" // "모터 정지" ("Disable steppers")
#define MSG_DEBUG_MENU                      "Debug Menu" // ("Debug Menu")
#define MSG_PROGRESS_BAR_TEST               "Progress Bar Test" // ("Progress Bar Test")
#define MSG_AUTO_HOME                       "\xf5\xc8\x9f"      // "홈으로" ("Auto home")
#define MSG_AUTO_HOME_X                     "X \xf5\xc8\x9f"  // "X 홈으로" ("Home X")
#define MSG_AUTO_HOME_Y                     "Y \xf5\xc8\x9f"  // "Y 홈으로" ("Home Y")
#define MSG_AUTO_HOME_Z                     "Z \xf5\xc8\x9f"  // "Z 홈으로" ("Home Z")
#define MSG_TMC_Z_CALIBRATION               "Z \xac\xd8"  // "Z 보정" ("Calibrate Z")
#define MSG_LEVEL_BED_HOMING                "XYZ \xf5\xc8\x9f"  // "XYZ 홈으로" ("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               "\xba\xd0\xf4\x9d\xa7 \x90\xa1\xb4\xc4" // "시작하려면 누르세요" ("Click to Begin")
#define MSG_LEVEL_BED_NEXT_POINT            "\x91\xc9 \xf1\xcc\xed" // "다음 포인트" ("Next Point")
#define MSG_LEVEL_BED_DONE                  "\x9c\xab\xa4 \xc3\xa0" // "레벨링 완료" ("Leveling Done!")
#define MSG_SET_HOME_OFFSETS                "\xf5 \xc1\xf2\xb6 \xb3\xd8"  // "홈 오프셋 설정" ("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            "\xc1\xf2\xb6 \xd4\xc5 \xc3\xa0"  // "오프셋 적용 완료" ("Offsets applied")
#define MSG_SET_ORIGIN                      "\xe3\x8d \xb3\xd8" // "초기 설정" ("Set origin")
#define MSG_PREHEAT_1                       "PLA \xc0\xbf"  // "PLA 예열" ("Preheat PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   "PLA \xd5\xe2 \xc0\xbf "  // "PLA 전체 예열" (" All")
#define MSG_PREHEAT_1_BEDONLY               "PLA \xaa\x97 \xc0\xbf "  // "PLA 베드 예열" (" Bed")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " \xb3\xd8" //"PLA 예열 설정" (" Setting")
#define MSG_PREHEAT_2                       "ABS \xc0\xbf"  //"ABS 예열" Preheat ABS
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   "ABS \xd5\xe2 \xc0\xbf " // "ABS 전체 예열" (" All")
#define MSG_PREHEAT_2_BEDONLY               "ABS \xaa\x97 \xc0\xbf " // "ABS 베드 예열" (" Bed")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " \xb3\xd8" //"ABS 예열 설정" (" Setting")
#define MSG_COOLDOWN                        "\x8e\x83"  // "냉각" ("Cooldown")
#define MSG_SWITCH_PS_ON                    "\xb9\xc7\xe9 \xd5\xc6 on"  // "스위치 전원 on" ("Switch power on")
#define MSG_SWITCH_PS_OFF                   "\xb9\xc7\xe9 \xd5\xc6 off" // "스위치 전원 off" ("Switch power off")
#define MSG_EXTRUDE                         "\xbc\xe6"  // "압출" ("Extrude")
#define MSG_RETRACT                         "\xa2\xed\x9a\xed"  // "리트랙트" ("Retract")
#define MSG_MOVE_AXIS                       "\xe5 \xcb\x95" // "축 이동" ("Move axis")
#define MSG_BED_LEVELING                    "\xaa\x97 \x9c\xab\xa4" // "베드 레벨링" ("Bed Leveling")
#define MSG_LEVEL_BED                       "\xaa\x97 \x9c\xab\xa4 \xc3\xa0"  // "베드 레벨링 완료" ("Level bed")
#define MSG_MOVING                          "\xcb\x95 \xde"  // "이동 중" ("Moving...")
#define MSG_FREE_XY                         "Free XY" // ("Free XY")
#define MSG_MOVE_X                          "X\xe5 \xcb\x95"  // "X축 이동" ("Move X")
#define MSG_MOVE_Y                          "Y\xe5 \xcb\x95"  // "Y축 이동" ("Move Y")
#define MSG_MOVE_Z                          "Z\xe5 \xcb\x95"  // "Z축 이동" ("Move Z")
#define MSG_MOVE_E                          "\xbc\xe6"  // "압출" ("Extrude")
#define MSG_MOVE_01MM                       "0.1mm \xcb\x95"  // "0.1mm 이동" ("Move 0.1mm")
#define MSG_MOVE_1MM                        "  1mm \xcb\x95"  // "  1mm 이동" ("Move 1mm")
#define MSG_MOVE_10MM                       " 10mm \xcb\x95"  // " 10mm 이동" ("Move 10mm")
#define MSG_SPEED                           "\xb8\x93"  // "속도" ("Speed")
#define MSG_BED_Z                           "Z \xaa\x97"  // "Z 베드" ("Bed Z")
#define MSG_NOZZLE                          "\x8f\xdf"  // "노즐" ("Nozzle")
#define MSG_BED                             "\xaa\x97"  // "베드" ("Bed")
#define MSG_FAN_SPEED                       "\xf0 \xb8\x93" // "팬 속도" ("Fan speed")
#define MSG_FLOW                            "\xbc\xe6\x9b"  // "압출량" ("Flow")
#define MSG_CONTROL                         "\xb3\xd8"  // "설정"("Control")
#define MSG_MIN                             LCD_STR_THERMOMETER " \xe4\xb7" // " 최소" (" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER " \xe4\x92" // " 최대" (" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Fact" // (" Fact")
#define MSG_AUTOTEMP                        "\xcf\x95 \xc2\x93 \xb3\xd8"  // "자동 온도 설정" ("Autotemp")
#define MSG_ON                              "\xc2   "  // "온 " intentional space to shift wide symbol to the left
#define MSG_OFF                            "\xc1\xf2   "  // "오프 " intentional space to shift wide symbol to the left
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Select"
#define MSG_JERK                            "Jerk"
#if LCD_WIDTH >= 20
  #define MSG_ACC                           "\x82\xb8\x93 mm/s2"  // "가속도 mm/s2" ("Accel")
  #if IS_KINEMATIC
    #define MSG_VA_JERK                     "Va-jerk"  // "Va-jerk" ("Va-jerk")
    #define MSG_VB_JERK                     "Vb-jerk" // "Vb-jerk" ("Vb-jerk")
    #define MSG_VC_JERK                     "Vc-jerk" // "Vc-jerk" ("Vc-jerk")
  #else
    #define MSG_VA_JERK                     "Vx-jerk mm/s"  // "Vx-jerk mm/s" ("Vx-jerk")
    #define MSG_VB_JERK                     "Vy-jerk mm/s" // "Vy-jerk mm/s" ("Vy-jerk")
    #define MSG_VC_JERK                     "Vz-jerk mm/s" // "Vz-jerk mm/s" ("Vz-jerk")
  #endif
  #define MSG_VE_JERK                       "Ve-jerk"
  #define MSG_VMAX                          "Vmax "
  #define MSG_VMIN                          "Vmin"
  #define MSG_VTRAV_MIN                     "VTrav min"
  #define MSG_AMAX                          "Amax "
#else
  #define MSG_ACC                           "\x82\xb8"  // "가속" ("Accel")
  #if IS_KINEMATIC
    #define MSG_VA_JERK                     "Va-jerk" // "Va-jerk" ("Va-jerk")
    #define MSG_VB_JERK                     "Vb-jerk" // "Vb-jerk" ("Vb-jerk")
    #define MSG_VC_JERK                     "Vc-jerk" // "Vc-jerk" ("Vc-jerk")
  #else
    #define MSG_VA_JERK                     "Vx-jerk" // "Vx-jerk" ("Vx-jerk")
    #define MSG_VB_JERK                     "Vy-jerk" // "Vy-jerk" ("Vy-jerk")
    #define MSG_VC_JERK                     "Vz-jerk" // "Vz-jerk" ("Vz-jerk")
  #endif
  #define MSG_VE_JERK                       "Ve-jerk"  // "Ve-jerk" ("Ve-jerk")
  #define MSG_VMAX                          "Vmax "
  #define MSG_VMIN                          "Vmin"
  #define MSG_VTRAV_MIN                     "VTrav min"
  #define MSG_AMAX                          "Amax "
#endif
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-travel"
#if LCD_WIDTH >= 20
  #define MSG_STEPS_PER_MM                  "Steps/mm"
  #if IS_KINEMATIC
    #define MSG_ASTEPS                      "Asteps/mm"
    #define MSG_BSTEPS                      "Bsteps/mm"
    #define MSG_CSTEPS                      "Csteps/mm"
  #else
    #define MSG_ASTEPS                      "Xsteps/mm"
    #define MSG_BSTEPS                      "Ysteps/mm"
    #define MSG_CSTEPS                      "Zsteps/mm"
  #endif
  #define MSG_ESTEPS                        "Esteps/mm"
  #define MSG_E1STEPS                       "E1steps/mm"
  #define MSG_E2STEPS                       "E2steps/mm"
  #define MSG_E3STEPS                       "E3steps/mm"
  #define MSG_E4STEPS                       "E4steps/mm"
  #define MSG_E5STEPS                       "E5steps/mm"
#else
  #define MSG_STEPS_PER_MM                  "Steps"
  #if IS_KINEMATIC
    #define MSG_ASTEPS                      "Asteps"
    #define MSG_BSTEPS                      "Bsteps"
    #define MSG_CSTEPS                      "Csteps"
  #else
    #define MSG_ASTEPS                      "Xsteps"
    #define MSG_BSTEPS                      "Ysteps"
    #define MSG_CSTEPS                      "Zsteps"
  #endif
  #define MSG_ESTEPS                        "Esteps"
  #define MSG_E1STEPS                       "E1steps"
  #define MSG_E2STEPS                       "E2steps"
  #define MSG_E3STEPS                       "E3steps"
  #define MSG_E4STEPS                       "E4steps"
  #define MSG_E5STEPS                       "E5steps"
#endif
#define MSG_TEMPERATURE                     "\xc2\x93"  // "온도" ("Temperature")
#define MSG_MOTION                          "\x95\xd0"  // "동작" ("Motion")
#define MSG_FILAMENT                        "\xf3\x99\xa6\xed"  // "필라멘트" ("Filament")
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "\xf3\x99\xa6\xed Dia." //"필라멘트 Dia" ("Fil. Dia.")
#define MSG_CONTRAST                        "LCD contrast" // ("LCD contrast")
#define MSG_STORE_EEPROM                    "\xb3\xd8 \xd3\xd1"  // "설정 저장" ("Store memory")
#define MSG_LOAD_EEPROM                     "\xb3\xd8 \x9f\x97"  // "설정 로드" ("Load memory")
#define MSG_RESTORE_FAILSAFE                "\xb3\xd8 \x96\x94\xa2\x8d"  // "설정 되돌리기" ("Restore failsafe")
#define MSG_REFRESH                         "\xb1\x9f \x88\xea"  // "새로 고침" ("Refresh")
#define MSG_WATCH                           "\xa5\xcc"  // "메인" ("Info screen")
#define MSG_PREPARE                         "\xdd\xaf"  // "준비" ("Prepare")
#define MSG_TUNE                            "Tune"  // ("Tune")
#define MSG_PAUSE_PRINT                     "\xe6\x9e \xcd\xba\xd8\xe0" // "출력 일시정지" ("Pause print")
#define MSG_RESUME_PRINT                    "\xe6\x9e \xd2\x85" // "출력 재개" ("Resume print")
#define MSG_STOP_PRINT                      "\xe6\x9e \xd8\xe0" // "출력 정지" ("Stop print")
#define MSG_CARD_MENU                       "SD \xeb\x97 \xe6\x9e" // "SD 카드 출력" ("Print from SD")
#define MSG_NO_CARD                         "SD \xeb\x97 \xbe\xc9"  // "SD 카드 없음" ("No SD card")
#define MSG_DWELL                           "\xd6\xd5"  // "절전" ("Sleep...")
#define MSG_USERWAIT                        "\x92\x8d \xde" // "대기 중" ("Wait for user...")
#define MSG_PRINT_ABORTED                   "\xe6\x9e \xe8\xb7" // "출력 취소" ("Print aborted")
#define MSG_NO_MOVE                         "No move"  // ("No move.")
#define MSG_KILLED                          "\x84\xd9 \xdb\xa0" // "강제 종료" ("KILLED. ")
#define MSG_STOPPED                         "\xd8\xe0"  // "정지" ("STOPPED. ")
#define MSG_CONTROL_RETRACT                 "Retract mm" // ("Retract mm")
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"  //  ("Swap Re.mm")
#define MSG_CONTROL_RETRACTF                "Retract  V" //  ("Retract  V")
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm" // ("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet mm" // ("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet mm"  // ("S UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V" // ("UnRet  V")
#define MSG_AUTORETRACT                     "AutoRetr." //  ("AutoRetr.")
#define MSG_FILAMENTCHANGE                  "\xf3\x99\xa6\xed \x8b\xe2" // "필라멘트 교체" ("Change filament")
#define MSG_INIT_SDCARD                     "SD \xeb\x97 \xe3\x8d\xf6"  // "SD 카드 초기화" ("Init. SD card")
#define MSG_CNG_SDCARD                      "SD \xeb\x97 \x8b\xe2"  // "SD 카드 교체" ("Change SD card")
#define MSG_ZPROBE_OUT                      "Z probe out. bed"    // ("Z probe out. bed")
#define MSG_BLTOUCH_SELFTEST                "BLTouch \xcf\x82 \xd7\x87"  // "BLTouch 자가 점검" ("BLTouch Self-Test")
#define MSG_BLTOUCH_RESET                   "BLTouch \xa2\xb6"  // "BLTouch 리셋" ("Reset BLTouch")
#define MSG_HOME                            "\xf5\xc8\x9f"  // "홈으로" ("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "\xe1\xc9\xc8\x9f"  // "처음으로" ("first")
#define MSG_ZPROBE_ZOFFSET                  "Z \xc1\xf2\xb6" // "Z 오프셋" ("Z Offset")
#define MSG_BABYSTEP_X                      "Babystep X" // "Babystep X" ("Babystep X")
#define MSG_BABYSTEP_Y                      "Babystep Y"  // "Babystep Y" ("Babystep Y")
#define MSG_BABYSTEP_Z                      "Babystep Z"  // "Babystep Z" ("Babystep Z")
#define MSG_ENDSTOP_ABORT                   "Endstop \xe8\xb7"  // "Endstop 취소" ("Endstop abort")
#define MSG_HEATING_FAILED_LCD              "\x82\xbf \xbb\xef"  // "가열 실패" ("Heating failed")
#define MSG_ERR_REDUNDANT_TEMP              "Err: REDUNDANT TEMP"  //  ("Err: REDUNDANT TEMP")
#define MSG_THERMAL_RUNAWAY                 "ERR: \xc2\x93\xb5\xb2"  // "ERR: 온도센서" ("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     "ERR: \xc2\x93 \xe4\x92" // "ERR: 온도 최대" ("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     "ERR: \xc2\x93 \xe4\xb7" // "ERR: 온도 최저" ("Err: MINTEMP")
#if LCD_WIDTH >= 20
  #define MSG_ERR_MAXTEMP_BED               "ERR: \xaa\x97 \xc2\x93 \xe4\x92"  // "ERR: 베드 온도 최대" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "ERR: \xaa\x97 \xc2\x93 \xe4\xb7"     // "ERR: 베드 온도 최소" ("Err: MINTEMP BED")
#else
  #define MSG_ERR_MAXTEMP_BED               "ERR: \xaa\x97 \xc2\x93 \xe4\x92"  // "ERR: 베드 온도 최대" ("Err: MAXTEMP BED")
  #define MSG_ERR_MINTEMP_BED               "ERR: \xaa\x97 \xc2\x93 \xe4\xb7"     // "ERR: 베드 온도 최소" ("Err: MINTEMP BED")
#endif
#define MSG_ERR_Z_HOMING                    MSG_HOME " " MSG_X MSG_Y " " MSG_FIRST                             // "ｻｷﾆ XY ｦﾌｯｷｻｾﾃｸﾀﾞｻｲ" or "ｻｷﾆ XY ｦﾌｯｷｻｾﾖ" ("G28 Z Forbidden")
#define MSG_HALTED                          "\xf2\xa3\xec \xd8\xe0" // "프린터 위험" ("PRINTER HALTED")
#define MSG_PLEASE_RESET                    "\xa2\xb6"  // "리셋" ("Please reset")
#define MSG_SHORT_DAY                       "d"                                                                // One character only
#define MSG_SHORT_HOUR                      "h"                                                                // One character only
#define MSG_SHORT_MINUTE                    "m"                                                                // One character only
#define MSG_HEATING                         "\x82\xbf \xde..." // "가열 중..." ("Heating...")
#define MSG_BED_HEATING                     "\xaa\x97 \x82\xbf \xde..." // "베드 가열 중..." ("Bed Heating...")
#define MSG_DELTA_CALIBRATE                 "Delta \xac\xd8" // "Delta 보정" ("Delta Calibration")
#define MSG_DELTA_CALIBRATE_X               "X \xac\xd8"  // "X 보정" ("Calibrate X")
#define MSG_DELTA_CALIBRATE_Y               "Y \xac\xd8"  // "Y 보정" ("Calibrate Y")
#define MSG_DELTA_CALIBRATE_Z               "Z \xac\xd8"  // "Z 보정" ("Calibrate Z")
#define MSG_DELTA_CALIBRATE_CENTER          "Center \xac\xd8" // "Center 보정" ("Calibrate Center")
#define MSG_INFO_MENU                       "\xf2\xa3\xec \xd8\xac" // "프린터 정보" ("About Printer")
#define MSG_INFO_PRINTER_MENU               "\xf2\xa3\xec \xd8\xac" // "프린터 정보" ("Printer Info")
#define MSG_INFO_STATS_MENU                 "\xf2\xa3\xec \x90\xd4" // "프린터 누적" ("Printer Stats")
#define MSG_INFO_BOARD_MENU                 "\xac\x97 \xd8\xac" // "보드 정보" ("Board Info")
#define MSG_INFO_THERMISTOR_MENU            "\xc2\x93\xb5\xb2" // "서미스터" ("Thermistors")
#define MSG_INFO_EXTRUDERS                  "\xbc\xe6\x8d"  // "압출기" ("Extruders")
#define MSG_INFO_BAUDRATE                   "Baud"  // "Baud" ("Baud")
#define MSG_INFO_PROTOCOL                   "Protocol"  // ("Protocol")
#define MSG_CASE_LIGHT                      "Case light" //  ("Case light")
#define MSG_INFO_PRINT_COUNT                "Print Count" //  ("Print Count")
#define MSG_INFO_COMPLETED_PRINTS           "\xc3\xa0"  // "완료" ("Completed")
#define MSG_INFO_PRINT_TIME                 "Total print time"  //  ("Total print time")
#define MSG_INFO_PRINT_LONGEST              "Longest job time" //  ("Longest job time")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_FILAMENT           "Extruded total" // ("Extruded total")
#else
  #define MSG_INFO_PRINT_FILAMENT           "Extruded"  // ("Extruded")
#endif
#define MSG_INFO_MIN_TEMP                   "\xe4\xd3 \xc2\x93"  // "최저 온도" ("Min Temp")
#define MSG_INFO_MAX_TEMP                   "\xe4\x88 \xc2\x93" // "최고 온도" ("Max Temp")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PSU                      "\xd5\xc6 \x89\x8c" // "전원 공급" ("Power Supply")
#else
  #define MSG_INFO_PSU                      "\xd5\xc6 \x89\x8c" // "전원 공급" ("Power Supply")
#endif
#define MSG_DRIVE_STRENGTH                  "Drive Strength" // ("Drive Strength")
#define MSG_DAC_PERCENT                     "Driver %" // ("Driver %")
#define MSG_DAC_EEPROM_WRITE                MSG_STORE_EEPROM                                                   // "ﾒﾓﾘﾍｶｸﾉｳ" ("DAC EEPROM Write")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "\xe6\x9e \xcb\xbd\xf4\x86" // "출력 이어하기" ("Resume print")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1        "\xba\xd0 \x92\x8d \xde" // "시작 대기 중" ("Wait for start")
  #define MSG_FILAMENT_CHANGE_INIT_2        "\xf3\x99\xa6\xed\xca"  // "필라멘트의" ("of the filament")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      "\x92\x8d \xde" // "대기 중" ("Wait for")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2      "\xf3\x99\xa6\xed \xad\xda"  // "필라멘트 부족" ("filament unload")
  #define MSG_FILAMENT_CHANGE_INSERT_1      "\xf3\x99\xa6\xed \xb0\xce,"  // "필라멘트 삽입," ("Insert filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2      "\xa9\xee \x90\xa1\x86" // "버튼 누르기" ("and press button")
  #define MSG_FILAMENT_CHANGE_LOAD_1        "\x92\x8d \xde" // "대기 중" ("Wait for")
  #define MSG_FILAMENT_CHANGE_LOAD_2        "\xf3\x99\xa6\xed \xe7\xae" // "필라멘트 충분" ("filament load")
  #define MSG_FILAMENT_CHANGE_RESUME_1      "\xe6\x9e \x92\x8d \xde"  // "출력 대기 중" ("Wait for print")
  #define MSG_FILAMENT_CHANGE_RESUME_2      "\xcb\xbd\xf4\x86"  // "이어하기" ("to resume")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1        "\x92\x8d"  // "대기" ("Please wait...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      "Ejecting..."  //  ("Ejecting...")
  #define MSG_FILAMENT_CHANGE_INSERT_1      "Insert and Click"  // ("Insert and Click")
  #define MSG_FILAMENT_CHANGE_LOAD_1        "\x9f\x98 \xde..."  // "로딩 중..." ("Loading...")
  #define MSG_FILAMENT_CHANGE_RESUME_1      "\xd2\x85 \xde..."  // "재개 중..." ("Resuming...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_KO_KR_H

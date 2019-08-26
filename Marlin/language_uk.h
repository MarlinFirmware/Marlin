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
 * Ukrainian
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_UK_H
#define LANGUAGE_UK_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" РіРѕС‚РѕРІРёР№.")
#define MSG_SD_INSERTED                     _UxGT("РљР°СЂС‚РєР° РІСЃС‚Р°РІР»РµРЅР°")
#define MSG_SD_REMOVED                      _UxGT("РљР°СЂС‚РєР° РІРёРґР°Р»РµРЅР°")
#define MSG_LCD_ENDSTOPS                    _UxGT("РљС–РЅС†РµРІРёРє") // Max length 8 characters
#define MSG_MAIN                            _UxGT("РњРµРЅСЋ")
#define MSG_AUTOSTART                       _UxGT("РђРІС‚РѕСЃС‚Р°СЂС‚")
#define MSG_DISABLE_STEPPERS                _UxGT("Р’РёРјРє. РґРІРёРіСѓРЅРё")
#define MSG_AUTO_HOME                       _UxGT("РђРІС‚Рѕ РїР°СЂРєСѓРІР°РЅРЅСЏ")
#define MSG_AUTO_HOME_X                     _UxGT("РџР°СЂРєСѓРІР°РЅРЅСЏ X")
#define MSG_AUTO_HOME_Y                     _UxGT("РџР°СЂРєСѓРІР°РЅРЅСЏ Y")
#define MSG_AUTO_HOME_Z                     _UxGT("РџР°СЂРєСѓРІР°РЅРЅСЏ Z")
#define MSG_TMC_Z_CALIBRATION               _UxGT("РљР°Р»С–Р±СЂСѓРІР°РЅРЅСЏ Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("РџР°СЂРєСѓРІР°РЅРЅСЏ XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("РџРѕС‡Р°С‚Рё")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("РЎР»С–РґСѓСЋС‡Р° РўРѕС‡РєР°")
#define MSG_LEVEL_BED_DONE                  _UxGT("Р—Р°РІРµСЂС€РµРЅРѕ!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Р—Р±РµСЂРµРіС‚Рё РїР°СЂРєСѓРІ.")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Р—РјС–С‰РµРЅРЅСЏ Р·Р°СЃС‚РѕСЃ.")
#define MSG_SET_ORIGIN                      _UxGT("Р’СЃС‚Р°РЅРѕРІ. РїРѕС‡Р°С‚РѕРє")
#define MSG_PREHEAT_1                       _UxGT("РќР°РіСЂС–РІ PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Р’СЃРµ")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" РЎС‚С–Р»")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" РЅР°Р».")
#define MSG_PREHEAT_2                       _UxGT("РќР°РіСЂС–РІ ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Р’СЃРµ")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" РЎС‚С–Р»")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" РЅР°Р».")
#define MSG_COOLDOWN                        _UxGT("РћС…РѕР»РѕРґР¶РµРЅРЅСЏ")
#define MSG_SWITCH_PS_ON                    _UxGT("РЈРІС–РјРєРЅСѓС‚Рё Р¶РёРІР»РµРЅРЅСЏ")
#define MSG_SWITCH_PS_OFF                   _UxGT("Р’РёРјРєРЅСѓС‚Рё Р¶РёРІР»РµРЅРЅСЏ")
#define MSG_EXTRUDE                         _UxGT("Р•РєСЃС‚СЂСѓР·С–СЏ")
#define MSG_RETRACT                         _UxGT("Р’С‚СЏРіСѓРІР°РЅРЅСЏ")
#define MSG_MOVE_AXIS                       _UxGT("Р СѓС… РїРѕ РѕСЃСЏРј")
#define MSG_BED_LEVELING                    _UxGT("РќС–РІРµР»СЋРІР°РЅРЅСЏ СЃС‚РѕР»Сѓ")
#define MSG_LEVEL_BED                       _UxGT("РќС–РІРµР»СЋРІР°РЅРЅСЏ СЃС‚РѕР»Сѓ")
#define MSG_MOVE_X                          _UxGT("Р СѓС… РїРѕ X")
#define MSG_MOVE_Y                          _UxGT("Р СѓС… РїРѕ Y")
#define MSG_MOVE_Z                          _UxGT("Р СѓС… РїРѕ Z")
#define MSG_MOVE_E                          _UxGT("Р•РєСЃС‚СЂСѓРґРµСЂ")
#define MSG_MOVE_01MM                       _UxGT("Р СѓС… РїРѕ 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Р СѓС… РїРѕ 1mm")
#define MSG_MOVE_10MM                       _UxGT("Р СѓС… РїРѕ 10mm")
#define MSG_SPEED                           _UxGT("РЁРІРёРґРєС–СЃС‚СЊ")
#define MSG_BED_Z                           _UxGT("Z РЎС‚РѕР»Сѓ")
#define MSG_NOZZLE                          _UxGT("РЎРѕРїР»Рѕ")
#define MSG_BED                             _UxGT("РЎС‚С–Р»")
#define MSG_FAN_SPEED                       _UxGT("РћС…РѕР»РѕРґР¶.")
#define MSG_FLOW                            _UxGT("РџРѕС‚С–Рє")
#define MSG_CONTROL                         _UxGT("РќР°Р»Р°С€С‚СѓРІР°РЅРЅСЏ")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" РњС–РЅ")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" РњР°РєСЃ")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Р¤Р°РєС‚")
#define MSG_AUTOTEMP                        _UxGT("РђРІС‚РѕС‚РµРјРїРµСЂ.")
#define MSG_ON                              _UxGT("РЈРІС–РјРє.")
#define MSG_OFF                             _UxGT("Р’РёРјРє. ")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Р’РёР±СЂР°С‚Рё")
#define MSG_ACC                             _UxGT("РџСЂРёСЃРє.")
#define MSG_JERK                            _UxGT("Р РёРІРѕРє")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Va-СЂРёРІРѕРє")
  #define MSG_VB_JERK                       _UxGT("Vb-СЂРёРІРѕРє")
  #define MSG_VC_JERK                       _UxGT("Vc-СЂРёРІРѕРє")
#else
  #define MSG_VA_JERK                       _UxGT("Vx-СЂРёРІРѕРє")
  #define MSG_VB_JERK                       _UxGT("Vy-СЂРёРІРѕРє")
  #define MSG_VC_JERK                       _UxGT("Vz-СЂРёРІРѕРє")
#endif
#define MSG_VE_JERK                         _UxGT("Ve-СЂРёРІРѕРє")
#define MSG_VMAX                            _UxGT("VРјР°РєСЃ")
#define MSG_VMIN                            _UxGT("VРјС–РЅ")
#define MSG_VTRAV_MIN                       _UxGT("VСЂСѓС…Сѓ РјС–РЅ")
#define MSG_AMAX                            _UxGT("AРјР°РєСЃ ")
#define MSG_A_RETRACT                       _UxGT("A-РІС‚СЏРіСѓРІ.")
#define MSG_A_TRAVEL                        _UxGT("A-СЂСѓС…Сѓ")
#define MSG_STEPS_PER_MM                    _UxGT("РљСЂРѕРєС–РІ/РјРј")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("AРєСЂРѕРєС–РІ/РјРј")
  #define MSG_BSTEPS                        _UxGT("BРєСЂРѕРєС–РІ/РјРј")
  #define MSG_CSTEPS                        _UxGT("CРєСЂРѕРєС–РІ/РјРј")
#else
  #define MSG_ASTEPS                        _UxGT("XРєСЂРѕРєС–РІ/РјРј")
  #define MSG_BSTEPS                        _UxGT("YРєСЂРѕРєС–РІ/РјРј")
  #define MSG_CSTEPS                        _UxGT("ZРєСЂРѕРєС–РІ/РјРј")
#endif
#define MSG_ESTEPS                          _UxGT("EРєСЂРѕРєС–РІ/РјРј")
#define MSG_E1STEPS                         _UxGT("E1РєСЂРѕРєС–РІ/РјРј")
#define MSG_E2STEPS                         _UxGT("E2РєСЂРѕРєС–РІ/РјРј")
#define MSG_E3STEPS                         _UxGT("E3РєСЂРѕРєС–РІ/РјРј")
#define MSG_E4STEPS                         _UxGT("E4РєСЂРѕРєС–РІ/РјРј")
#define MSG_E5STEPS                         _UxGT("E5РєСЂРѕРєС–РІ/РјРј")
#define MSG_TEMPERATURE                     _UxGT("РўРµРјРїРµСЂР°С‚СѓСЂР°")
#define MSG_MOTION                          _UxGT("Р СѓС…")
#define MSG_FILAMENT                        _UxGT("Р’РѕР»РѕРєРЅРѕ")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E РІ РјРј3")
#define MSG_FILAMENT_DIAM                   _UxGT("Р”С–Р°Рј. РІРѕР»РѕРє.")
#define MSG_CONTRAST                        _UxGT("РєРѕРЅС‚СЂР°СЃС‚ LCD")
#define MSG_STORE_EEPROM                    _UxGT("Р—Р±РµСЂРµРіС‚Рё РІ РџР—Рџ")
#define MSG_LOAD_EEPROM                     _UxGT("Р—С‡РёС‚Р°С‚Рё Р· РџР—Рџ")
#define MSG_RESTORE_FAILSAFE                _UxGT("Р’С–РґРЅРѕРІРёС‚Рё Р±Р°Р·РѕРІС–")
#define MSG_REFRESH                         _UxGT("РџРѕРЅРѕРІРёС‚Рё")
#define MSG_WATCH                           _UxGT("Р†РЅС„РѕСЂРјР°С†С–СЏ")
#define MSG_PREPARE                         _UxGT("РџС–РґРіРѕС‚СѓРІР°С‚Рё")
#define MSG_TUNE                            _UxGT("РџС–РґР»Р°С€С‚СѓРІР°РЅРЅСЏ")
#define MSG_PAUSE_PRINT                     _UxGT("РџСЂРёР·СѓРїРёРЅРёС‚Рё РґСЂСѓРє")
#define MSG_RESUME_PRINT                    _UxGT("Р’С–РґРЅРѕРІРёС‚Рё РґСЂСѓРє")
#define MSG_STOP_PRINT                      _UxGT("РЎРєР°СЃСѓРІР°С‚Рё РґСЂСѓРє")
#define MSG_CARD_MENU                       _UxGT("Р”СЂСѓРєСѓРІР°С‚Рё Р· SD")
#define MSG_NO_CARD                         _UxGT("Р’С–РґСЃСѓС‚РЅСЏ SD РєР°СЂС‚.")
#define MSG_DWELL                           _UxGT("РЎРїР»СЏС‡РєР°...")
#define MSG_USERWAIT                        _UxGT("РћС‡С–РєСѓРІР°РЅРЅСЏ РґС–Р№...")
#define MSG_PRINT_ABORTED                   _UxGT("Р”СЂСѓРє СЃРєР°СЃРѕРІР°РЅРѕ")
#define MSG_NO_MOVE                         _UxGT("РќРµРјР°С” СЂСѓС…Сѓ.")
#define MSG_KILLED                          _UxGT("РџР•Р Р•Р Р’РђРќРћ. ")
#define MSG_STOPPED                         _UxGT("Р—РЈРџР�РќР•РќРћ. ")
#define MSG_FILAMENTCHANGE                  _UxGT("Р—РјС–РЅР° РІРѕР»РѕРєРЅР°")
#define MSG_INIT_SDCARD                     _UxGT("РЎС‚Р°СЂС‚ SD РєР°СЂС‚РєРё")
#define MSG_CNG_SDCARD                      _UxGT("Р—Р°РјС–РЅР° SD РєР°СЂС‚Рё")
#define MSG_ZPROBE_OUT                      _UxGT("Z РґРµС‚. РЅРµ РІ РјРµР¶Р°С…")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch РЎР°РјРѕ-РўРµСЃС‚")
#define MSG_BLTOUCH_RESET                   _UxGT("РЎРєРёРЅСѓС‚Рё BLTouch")
#define MSG_HOME                            _UxGT("Р”С–Рј")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("РїРµСЂС€РёР№")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Р—РјС–С‰РµРЅРЅСЏ Z")
#define MSG_BABYSTEP_X                      _UxGT("РњС–РєСЂРѕРєСЂРѕРє X")
#define MSG_BABYSTEP_Y                      _UxGT("РњС–РєСЂРѕРєСЂРѕРє Y")
#define MSG_BABYSTEP_Z                      _UxGT("РњС–РєСЂРѕРєСЂРѕРє Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("РЅРµРІРґР°С‡Р° РєС–РЅС†РµРІРёРєР°")
#define MSG_HEATING_FAILED_LCD              _UxGT("РќРµРІРґР°Р»РёР№ РЅР°РіСЂС–РІ")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Р—Р‘Р†Р™ РўР•РњРџР•Р РђРўРЈР Р�")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("РџР Р�РќРўР•Р  Р—РЈРџР�РќР•РќРћ")
#define MSG_PLEASE_RESET                    _UxGT("РџРµСЂРµР·Р°РІР°РЅС‚Р°Р¶С‚Рµ")
#define MSG_SHORT_DAY                       _UxGT("Рґ") // One character only
#define MSG_SHORT_HOUR                      _UxGT("Рі") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("С…") // One character only
#define MSG_HEATING                         _UxGT("РќР°РіСЂС–РІР°РЅРЅСЏ...")
#define MSG_BED_HEATING                     _UxGT("РќР°РіСЂС–РІР°РЅРЅСЏ СЃС‚РѕР»Сѓ...")
#define MSG_DELTA_CALIBRATE                 _UxGT("РљР°Р»С–Р±СЂ. Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("РљР°Р»С–Р±СЂСѓРІР°РЅРЅСЏ X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("РљР°Р»С–Р±СЂСѓРІР°РЅРЅСЏ Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("РљР°Р»С–Р±СЂСѓРІР°РЅРЅСЏ Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("РљР°Р»С–Р±СЂ. Р¦РµРЅС‚СЂСѓ")

#define MSG_INFO_MENU                       _UxGT("РџСЂРѕ РїСЂРёРЅС‚РµСЂ")
#define MSG_INFO_PRINTER_MENU               _UxGT("Р†РЅС„РѕСЂРјР°С†С–СЏ")
#define MSG_INFO_STATS_MENU                 _UxGT("РЎС‚Р°С‚РёСЃС‚РёРєР°")
#define MSG_INFO_BOARD_MENU                 _UxGT("РџСЂРѕ РїР»Р°С‚Сѓ")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("РўРµСЂРјС–СЃС‚РѕСЂРё")
#define MSG_INFO_EXTRUDERS                  _UxGT("Р•РєСЃС‚СЂСѓРґРµСЂРё")
#define MSG_INFO_BAUDRATE                   _UxGT("Р±С–С‚/СЃ")
#define MSG_INFO_PROTOCOL                   _UxGT("РџСЂРѕС‚РѕРєРѕР»")
#define MSG_CASE_LIGHT                      _UxGT("РџС–РґСЃРІС–С‚РєР°")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Рљ-СЃС‚СЊ РґСЂСѓРєС–РІ")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Р—Р°РІРµСЂС€РµРЅРѕ")
  #define MSG_INFO_PRINT_TIME               _UxGT("Р’РµСЃСЊ С‡Р°СЃ РґСЂСѓРєСѓ")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("РќР°Р№РґРѕРІС€РёР№ С‡Р°СЃ")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Р•РєСЃС‚СЂСѓРґРѕРІР°РЅРѕ")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Р”СЂСѓРєС–РІ")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Р—Р°РІРµСЂС€РµРЅРѕ")
  #define MSG_INFO_PRINT_TIME               _UxGT("Р—Р°РіР°Р»РѕРј")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("РќР°Р№РґРѕРІС€РёР№")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Р•РєСЃРґСЂСѓРґ.")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("РњС–РЅ РўРµРјРї.")
#define MSG_INFO_MAX_TEMP                   _UxGT("РњР°РєСЃ РўРµРјРї.")
#define MSG_INFO_PSU                        _UxGT("Р”Р¶РµСЂРµР»Рѕ Р¶РёРІ.")

#define MSG_DRIVE_STRENGTH                  _UxGT("РЎРёР»Р° РјРѕС‚РѕСЂСѓ")
#define MSG_DAC_PERCENT                     _UxGT("% РјРѕС‚РѕСЂСѓ")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Р—Р°РїРёСЃ Р¦РђРџ РЅР° РџР—Рџ")

#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Р’С–РґРЅРѕРІРёС‚Рё РґСЂСѓРє")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Р—Р°С‡РµРєР°Р№С‚Рµ РЅР°")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("РїРѕС‡Р°С‚РѕРє Р·Р°РјС–РЅРё")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("РІРѕР»РѕРєРЅР°")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Р—Р°С‡РµРєР°Р№С‚Рµ РЅР°")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("РІРёРІС–Рґ РІРѕР»РѕРєРЅР°")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Р’СЃС‚Р°РІС‚Рµ РІРѕР»РѕРєРЅРѕ")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("С‚Р° РЅР°С‚РёСЃРЅС–С‚СЊ РґР»СЏ")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("РїСЂРѕРґРѕРІР¶РµРЅРЅСЏ...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Р—Р°С‡РµРєР°Р№С‚Рµ РЅР°")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("РІРІС–Рґ РІРѕР»РѕРєРЅР°")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Р—Р°С‡РµРєР°Р№С‚Рµ РЅР°")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("РІС–РґРЅРѕРІР»РµРЅРЅСЏ")
  #define MSG_FILAMENT_CHANGE_RESUME_3        _UxGT("РґСЂСѓРєСѓ")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Р—Р°С‡РµРєР°Р№С‚Рµ...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Р’РёРІС–Рґ...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Р’СЃС‚Р°РІС‚Рµ С– РЅР°С‚.")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Р’РІС–Рґ...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Р’С–РґРЅРѕРІР»РµРЅРЅСЏ...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_UK_H


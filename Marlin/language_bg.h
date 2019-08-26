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
 * Bulgarian
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_BG_H
#define LANGUAGE_BG_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" Р“РѕС‚РѕРІ.")
#define MSG_SD_INSERTED                     _UxGT("РљР°СЂС‚Р°С‚Р° Рµ РїРѕСЃС‚Р°РІРµРЅР°")
#define MSG_SD_REMOVED                      _UxGT("РљР°СЂС‚Р°С‚Р° Рµ РёР·РІР°РґРµРЅР°")
#define MSG_MAIN                            _UxGT("РњРµРЅСЋ")
#define MSG_AUTOSTART                       _UxGT("РђРІС‚РѕСЃС‚Р°СЂС‚")
#define MSG_DISABLE_STEPPERS                _UxGT("Р�Р·РєР». РґРІРёРіР°С‚РµР»Рё")
#define MSG_AUTO_HOME                       _UxGT("РџР°СЂРєРёСЂР°РЅРµ")
#define MSG_TMC_Z_CALIBRATION               _UxGT("РљР°Р»РёР±СЂРѕРІРєР° Z")
#define MSG_SET_HOME_OFFSETS                _UxGT("Р—Р°РґР°Р№ РќР°С‡Р°Р»Рѕ")
#define MSG_SET_ORIGIN                      _UxGT("Р�Р·С…РѕРґРЅР° С‚РѕС‡РєР°")
#define MSG_PREHEAT_1                       _UxGT("РџРѕРґРіСЂСЏРІР°РЅРµ PLA")
#define MSG_PREHEAT_1_N                     _UxGT("РџРѕРґРіСЂСЏРІР°РЅРµ PLA")
#define MSG_PREHEAT_1_ALL                   _UxGT("РџРѕРґРіСЂ. PLA Р’СЃРёС‡РєРё")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("РџРѕРґРіСЂ. PLA Р›РµРіР»Рѕ")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("РќР°СЃС‚СЂРѕР№РєРё PLA")
#define MSG_PREHEAT_2                       _UxGT("РџРѕРґРіСЂСЏРІР°РЅРµ ABS")
#define MSG_PREHEAT_2_N                     _UxGT("РџРѕРґРіСЂСЏРІР°РЅРµ ABS")
#define MSG_PREHEAT_2_ALL                   _UxGT("РџРѕРґРіСЂ. ABS Р’СЃРёС‡РєРё")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("РџРѕРґРіСЂ. ABS Р›РµРіР»Рѕ")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("РќР°СЃС‚СЂРѕР№РєРё ABS")
#define MSG_COOLDOWN                        _UxGT("РћС…Р»Р°Р¶РґР°РЅРµ")
#define MSG_SWITCH_PS_ON                    _UxGT("Р’РєР». Р·Р°С…СЂР°РЅРІР°РЅРµ")
#define MSG_SWITCH_PS_OFF                   _UxGT("Р�Р·РєР». Р·Р°С…СЂР°РЅРІР°РЅРµ")
#define MSG_EXTRUDE                         _UxGT("Р•РєСЃС‚СЂСѓР·РёСЏ")
#define MSG_RETRACT                         _UxGT("РћС‚РєР°С‚")
#define MSG_MOVE_AXIS                       _UxGT("Р”РІРёР¶РµРЅРёРµ РїРѕ РѕСЃ")
#define MSG_BED_LEVELING                    _UxGT("РќРёРІРµР»РёСЂР°РЅРµ")
#define MSG_LEVEL_BED                       _UxGT("РќРёРІРµР»РёСЂР°РЅРµ")
#define MSG_MOVE_X                          _UxGT("Р”РІРёР¶РµРЅРёРµ РїРѕ X")
#define MSG_MOVE_Y                          _UxGT("Р”РІРёР¶РµРЅРёРµ РїРѕ Y")
#define MSG_MOVE_Z                          _UxGT("Р”РІРёР¶РµРЅРёРµ РїРѕ Z")
#define MSG_MOVE_E                          _UxGT("Р•РєСЃС‚СЂСѓРґРµСЂ")
#define MSG_MOVE_01MM                       _UxGT("РџСЂРµРјРµСЃС‚Рё СЃ 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("РџСЂРµРјРµСЃС‚Рё СЃ 1mm")
#define MSG_MOVE_10MM                       _UxGT("РџСЂРµРјРµСЃС‚Рё СЃ 10mm")
#define MSG_SPEED                           _UxGT("РЎРєРѕСЂРѕСЃС‚")
#define MSG_BED_Z                           _UxGT("Bed Z")
#define MSG_NOZZLE                          LCD_STR_THERMOMETER _UxGT(" Р”СЋР·Р°")
#define MSG_BED                             LCD_STR_THERMOMETER _UxGT(" Р›РµРіР»Рѕ")
#define MSG_FAN_SPEED                       _UxGT("Р’РµРЅС‚РёР»Р°С‚РѕСЂ")
#define MSG_FLOW                            _UxGT("РџРѕС‚РѕРє")
#define MSG_CONTROL                         _UxGT("РЈРїСЂР°РІР»РµРЅРёРµ")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" РњРёРЅРёРјСѓРј")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" РњР°РєСЃРёРјСѓРј")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Р¤Р°РєС‚РѕСЂ")
#define MSG_AUTOTEMP                        _UxGT("РђРІС‚Рѕ-С‚РµРјРї.")
#define MSG_ON                              _UxGT("Р’РєР». ")
#define MSG_OFF                             _UxGT("Р�Р·РєР». ")
#define MSG_A_RETRACT                       _UxGT("A-РѕС‚РєР°С‚")
#define MSG_A_TRAVEL                        _UxGT("A-travel")
#define MSG_STEPS_PER_MM                    _UxGT("РЎС‚СЉРїРєРё/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("AСЃС‚СЉРїРєРё/mm")
  #define MSG_BSTEPS                        _UxGT("BСЃС‚СЉРїРєРё/mm")
  #define MSG_CSTEPS                        _UxGT("CСЃС‚СЉРїРєРё/mm")
#else
  #define MSG_ASTEPS                        _UxGT("XСЃС‚СЉРїРєРё/mm")
  #define MSG_BSTEPS                        _UxGT("YСЃС‚СЉРїРєРё/mm")
  #define MSG_CSTEPS                        _UxGT("ZСЃС‚СЉРїРєРё/mm")
#endif
#define MSG_ESTEPS                          _UxGT("E СЃС‚СЉРїРєРё/mm")
#define MSG_E1STEPS                         _UxGT("E1 СЃС‚СЉРїРєРё/mm")
#define MSG_E2STEPS                         _UxGT("E2 СЃС‚СЉРїРєРё/mm")
#define MSG_E3STEPS                         _UxGT("E3 СЃС‚СЉРїРєРё/mm")
#define MSG_E4STEPS                         _UxGT("E4 СЃС‚СЉРїРєРё/mm")
#define MSG_E5STEPS                         _UxGT("E5 СЃС‚СЉРїРєРё/mm")
#define MSG_TEMPERATURE                     _UxGT("РўРµРјРїРµСЂР°С‚СѓСЂР°")
#define MSG_MOTION                          _UxGT("Р”РІРёР¶РµРЅРёРµ")
#define MSG_FILAMENT                        _UxGT("РќРёС€РєР°")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Р”РёР°Рј. РЅРёС€РєР°")
#define MSG_CONTRAST                        _UxGT("LCD РєРѕРЅС‚СЂР°СЃС‚")
#define MSG_STORE_EEPROM                    _UxGT("Р—Р°РїР°Р·Рё РІ EPROM")
#define MSG_LOAD_EEPROM                     _UxGT("Р—Р°СЂРµРґРё РѕС‚ EPROM")
#define MSG_RESTORE_FAILSAFE                _UxGT("Р¤Р°Р±СЂРёС‡РЅРё РЅР°СЃС‚СЂРѕР№РєРё")
#define MSG_REFRESH                         LCD_STR_REFRESH _UxGT("РћР±РЅРѕРІРё")
#define MSG_WATCH                           _UxGT("РџСЂРµРіР»РµРґ")
#define MSG_PREPARE                         _UxGT("Р”РµР№СЃС‚РІРёСЏ")
#define MSG_TUNE                            _UxGT("РќР°СЃС‚СЂРѕР№РєР°")
#define MSG_PAUSE_PRINT                     _UxGT("РџР°СѓР·Р°")
#define MSG_RESUME_PRINT                    _UxGT("Р’СЉР·РѕР±РЅРѕРІРё РїРµС‡Р°С‚Р°")
#define MSG_STOP_PRINT                      _UxGT("РЎРїСЂРё РїРµС‡Р°С‚Р°")
#define MSG_CARD_MENU                       _UxGT("РњРµРЅСЋ РєР°СЂС‚Р°")
#define MSG_NO_CARD                         _UxGT("РќСЏРјР° РєР°СЂС‚Р°")
#define MSG_DWELL                           _UxGT("РџРѕС‡РёРІРєР°...")
#define MSG_USERWAIT                        _UxGT("Р�Р·С‡Р°РєРІР°РЅРµ")
#define MSG_PRINT_ABORTED                   _UxGT("РџРµС‡Р°С‚СЉС‚ Рµ РїСЂРµРєСЉСЃРЅР°С‚")
#define MSG_NO_MOVE                         _UxGT("РќСЏРјР° РґРІРёР¶РµРЅРёРµ")
#define MSG_KILLED                          _UxGT("РЈР‘Р�РўРћ.")
#define MSG_STOPPED                         _UxGT("РЎРџР РЇРќРћ.")
#define MSG_CONTROL_RETRACT                 _UxGT("РћС‚РєР°С‚ mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("РЎРјСЏРЅР° РћС‚РєР°С‚ mm")
#define MSG_CONTROL_RETRACTF                _UxGT("РћС‚РєР°С‚  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("РЎРєРѕРє mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Р’СЉР·РІСЂР°С‚ mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("РЎРјСЏРЅР° Р’СЉР·РІСЂР°С‚ mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Р’СЉР·РІСЂР°С‚  V")
#define MSG_AUTORETRACT                     _UxGT("РђРІС‚РѕoС‚РєР°С‚")
#define MSG_FILAMENTCHANGE                  _UxGT("РЎРјСЏРЅР° РЅРёС€РєР°")
#define MSG_INIT_SDCARD                     _UxGT("Р�РЅРёС†. SD-РљР°СЂС‚Р°")
#define MSG_CNG_SDCARD                      _UxGT("РЎРјСЏРЅР° SD-РљР°СЂС‚Р°")
#define MSG_ZPROBE_OUT                      _UxGT("Z-СЃРѕРЅРґР°С‚Р° Рµ РёР·РІР°РґРµРЅР°")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z РћС‚СЃС‚РѕСЏРЅРёРµ")
#define MSG_BABYSTEP_X                      _UxGT("РњРёРЅРёСЃС‚СЉРїРєР° X")
#define MSG_BABYSTEP_Y                      _UxGT("РњРёРЅРёСЃС‚СЉРїРєР° Y")
#define MSG_BABYSTEP_Z                      _UxGT("РњРёРЅРёСЃС‚СЉРїРєР° Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("РЎС‚РѕРї РљСЂ.Р�Р·РєР»СЋС‡РІР°С‚РµР»Рё")
#define MSG_DELTA_CALIBRATE                 _UxGT("Р”РµР»С‚Р° РљР°Р»РёР±СЂРѕРІРєР°")
#define MSG_DELTA_CALIBRATE_X               _UxGT("РљР°Р»РёР±СЂРѕРІРєР° X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("РљР°Р»РёР±СЂРѕРІРєР° Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("РљР°Р»РёР±СЂРѕРІРєР° Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("РљР°Р»РёР±СЂРѕРІРєР° Р¦РµРЅС‚СЉСЂ")

#endif // LANGUAGE_BG_H


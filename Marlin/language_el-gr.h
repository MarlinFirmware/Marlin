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
 * Greek (Greece)
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_EL_GR_H
#define LANGUAGE_EL_GR_H

#define MAPPER_CECF
#define DISPLAY_CHARSET_ISO10646_GREEK
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" О­П„ОїО№ОјОї.")
#define MSG_SD_INSERTED                     _UxGT("О•О№ПѓО±ОіП‰ОіО® ОєО¬ПЃП„О±П‚")
#define MSG_SD_REMOVED                      _UxGT("О‘П†О±ОЇПЃОµПѓО· ОєО¬ПЃП„О±П‚")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("О’О±ПѓО№ОєО® ОџОёПЊОЅО·")
#define MSG_AUTOSTART                       _UxGT("О‘П…П„ПЊОјО±П„О· ОµОєОєОЇОЅО·ПѓО·")
#define MSG_DISABLE_STEPPERS                _UxGT("О‘ПЂОµОЅОµПЃОіОїПЂОїОЇО·ПѓО· ОІО·ОјО±П„О№ПѓП„О®")
#define MSG_AUTO_HOME                       _UxGT("О‘П…П„ОїОј. ОµПЂО±ОЅО±П†ОїПЃО¬ ПѓП„Ої О±ПЃП‡О№ОєПЊ ПѓО·ОјОµОЇОї")
#define MSG_AUTO_HOME_X                     _UxGT("О‘ПЃП‡О№ОєПЊ ПѓО·ОјОµОЇОї X")
#define MSG_AUTO_HOME_Y                     _UxGT("О‘ПЃП‡О№ОєПЊ ПѓО·ОјОµОЇОї Y")
#define MSG_AUTO_HOME_Z                     _UxGT("О‘ПЃП‡О№ОєПЊ ПѓО·ОјОµОЇОї Z")
#define MSG_TMC_Z_CALIBRATION               _UxGT("О’О±ОёОјОїОЅПЊОјО·ПѓО· Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("О•ПЂО±ОЅО±П†ОїПЃО¬ ПѓП„Ої О±ПЃП‡О№ОєПЊ ПѓО·ОјОµОЇОї О§ОҐО–")
#define MSG_LEVEL_BED_WAITING               _UxGT("ОљО¬ОЅП„Оµ ОєО»О№Оє ОіО№О± ОЅО± ОѕОµОєО№ОЅО®ПѓОµП„Оµ")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("О•ПЂПЊОјОµОЅОї ПѓО·ОјОµОЇОї")
#define MSG_LEVEL_BED_DONE                  _UxGT("ОџО»ОїОєО»О®ПЃП‰ПѓО· ОµПЂО№ПЂОµОґОїПЂОїОЇО·ПѓО·П‚!")
#define MSG_SET_HOME_OFFSETS                _UxGT("ОџПЃО№ПѓОјПЊП‚ ОІО±ПѓО№ОєПЋОЅ ОјОµП„О±П„ОїПЂОЇПѓОµП‰ОЅ")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("О•П†О±ПЃОјПЊПѓП„О·ОєО±ОЅ ОїО№ ОјОµП„О±П„ОїПЂОЇПѓОµО№П‚")
#define MSG_SET_ORIGIN                      _UxGT("ОџПЃО№ПѓОјПЊП‚ ПЂПЃОїО­О»ОµП…ПѓО·П‚")
#define MSG_PREHEAT_1                       _UxGT("О ПЃОїОёО­ПЃОјО±ОЅПѓО· PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" ПЊО»О±")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" ОєО»ОЇОЅО·")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" ОµПЂО№ОІОµОІО±ОЇП‰ПѓО·")
#define MSG_PREHEAT_2                       _UxGT("О ПЃОїОёО­ПЃОјО±ОЅПѓО· ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" ПЊО»О±")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Bed")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" ОµПЂО№ОІОµОІО±ОЇП‰ПѓО·")
#define MSG_COOLDOWN                        _UxGT("ОњОµО№П‰ПѓО· ОёОµПЃОјОїОєПЃО±ПѓО№О±П‚")
#define MSG_SWITCH_PS_ON                    _UxGT("О•ОЅОµПЃОіОїПЂОїОЇО·ПѓО·")
#define MSG_SWITCH_PS_OFF                   _UxGT("О‘ПЂОµОЅОµПЃОіОїПЂОїОЇО·ПѓО·")
#define MSG_EXTRUDE                         _UxGT("О•ОѕПЋОёО·ПѓО·")
#define MSG_RETRACT                         _UxGT("О‘ОЅО¬ПѓП…ПЃПѓО·")
#define MSG_MOVE_AXIS                       _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· О¬ОѕОїОЅО±")
#define MSG_BED_LEVELING                    _UxGT("О•ПЂО№ПЂОµОґОїПЂОїОЇО·ПѓО· ОєО»ОЇОЅО·П‚")
#define MSG_LEVEL_BED                       _UxGT("О•ПЂО№ПЂОµОґОїПЂОїОЇО·ПѓО· ОєО»ОЇОЅО·П‚")
#define MSG_MOVE_X                          _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· X")
#define MSG_MOVE_Y                          _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· Y")
#define MSG_MOVE_Z                          _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· Z")
#define MSG_MOVE_E                          _UxGT("О•ОѕП‰ОёО·П„О®ПЃО±П‚")
#define MSG_MOVE_01MM                       _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· 0,1 ОјОј")
#define MSG_MOVE_1MM                        _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· 1 ОјОј")
#define MSG_MOVE_10MM                       _UxGT("ОњОµП„О±ОєОЇОЅО·ПѓО· 10 ОјОј")
#define MSG_SPEED                           _UxGT("О¤О±П‡ПЌП„О·П„О±")
#define MSG_BED_Z                           _UxGT("ОљО»ОЇОЅО· Z")
#define MSG_NOZZLE                          _UxGT("О‘ОєПЃОїП†ПЌПѓО№Ої")
#define MSG_BED                             _UxGT("ОљО»ОЇОЅО·")
#define MSG_FAN_SPEED                       _UxGT("О¤О±П‡ПЌП„О·П„О± О±ОЅОµОјО№ПѓП„О®ПЃО±")
#define MSG_FLOW                            _UxGT("ОЎОїО®")
#define MSG_CONTROL                         _UxGT("О€О»ОµОіП‡ОїП‚")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("О‘П…П„ОїОј. ПЃПЌОёОјО№ПѓО· ОёОµПЃОјОїОєПЃО±ПѓОЇО±П‚")
#define MSG_ON                              _UxGT("О•ОЅОµПЃОіОїПЂОїО№О·ОјО­ОЅОї")
#define MSG_OFF                             _UxGT("О‘ПЂОµОЅОµПЃОіОїПЂОїО№О·ОјО­ОЅОї")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("О•ПЂО№П„О¬П‡П…ОЅПѓО·")
#define MSG_JERK                            _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО·")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· A")
  #define MSG_VB_JERK                       _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· B")
  #define MSG_VC_JERK                       _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· C")
#else
  #define MSG_VA_JERK                       _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· X")
  #define MSG_VB_JERK                       _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· Y")
  #define MSG_VC_JERK                       _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· Z")
#endif
#define MSG_VE_JERK                         _UxGT("VО±ОЅП„ОЇОґПЃО±ПѓО· E")
#define MSG_VMAX                            _UxGT("VОјОµОі ")
#define MSG_VMIN                            _UxGT("VОµО»О±П‡")
#define MSG_VTRAV_MIN                       _UxGT("VОµО»О¬П‡. ОјОµП„О±П„ПЊПЂО№ПѓО·")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("AОјОµОі ")
#define MSG_A_RETRACT                       _UxGT("О‘-О±ОЅО¬ПѓП…ПЃПѓО·")
#define MSG_A_TRAVEL                        _UxGT("О‘-ОјОµП„О±П„ПЊПЂО№ПѓО·")
#define MSG_STEPS_PER_MM                    _UxGT("BО®ОјО±П„О± О±ОЅО¬ ОјОј")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("BО®ОјО±П„О± A О±ОЅО¬ ОјОј")
  #define MSG_BSTEPS                        _UxGT("BО®ОјО±П„О± B О±ОЅО¬ ОјОј")
  #define MSG_CSTEPS                        _UxGT("BО®ОјО±П„О± C О±ОЅО¬ ОјОј")
#else
  #define MSG_ASTEPS                        _UxGT("BО®ОјО±П„О± X О±ОЅО¬ ОјОј")
  #define MSG_BSTEPS                        _UxGT("BО®ОјО±П„О± Y О±ОЅО¬ ОјОј")
  #define MSG_CSTEPS                        _UxGT("BО®ОјО±П„О± Z О±ОЅО¬ ОјОј")
#endif
#define MSG_ESTEPS                          _UxGT("BО®ОјО±П„О± О• О±ОЅО¬ ОјОј")
#define MSG_E1STEPS                         _UxGT("BО®ОјО±П„О± О•1 О±ОЅО¬ ОјОј")
#define MSG_E2STEPS                         _UxGT("BО®ОјО±П„О± О•2 О±ОЅО¬ ОјОј")
#define MSG_E3STEPS                         _UxGT("BО®ОјО±П„О± О•3 О±ОЅО¬ ОјОј")
#define MSG_E4STEPS                         _UxGT("BО®ОјО±П„О± О•4 О±ОЅО¬ ОјОј")
#define MSG_E5STEPS                         _UxGT("BО®ОјО±П„О± О•5 О±ОЅО¬ ОјОј")
#define MSG_TEMPERATURE                     _UxGT("О�ОµПЃОјОїОєПЃО±ПѓОЇО±")
#define MSG_MOTION                          _UxGT("ОљОЇОЅО·ПѓО·")
#define MSG_FILAMENT                        _UxGT("ОќО®ОјО±")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("О• ПѓОµ ОјОј3")
#define MSG_FILAMENT_DIAM                   _UxGT("О”О№О¬ОјОµП„ПЃОїП‚ ОЅО®ОјО±П„ОїП‚")
#define MSG_CONTRAST                        _UxGT("ОљОїОЅП„ПЃО¬ПѓП„ LCD")
#define MSG_STORE_EEPROM                    _UxGT("О‘ПЂОїОёО®ОєОµП…ПѓО·")
#define MSG_LOAD_EEPROM                     _UxGT("О¦ПЊПЃП„П‰ПѓО·")
#define MSG_RESTORE_FAILSAFE                _UxGT("О•ПЂО±ОЅО±П†ОїПЃО¬ О±ПѓП†О±О»ОїПЌП‚ О±ОЅП„О№ОіПЃО¬П†ОїП…")
#define MSG_REFRESH                         _UxGT("О‘ОЅО±ОЅО­П‰ПѓО·")
#define MSG_WATCH                           _UxGT("ОџОёПЊОЅО· ПЂО»О·ПЃОїП†ПЊПЃО·ПѓО·П‚")
#define MSG_PREPARE                         _UxGT("О ПЃОїОµП„ОїО№ОјО±ПѓОЇО±")
#define MSG_TUNE                            _UxGT("ОЈП…ОЅП„ОїОЅО№ПѓОјПЊП‚")
#define MSG_PAUSE_PRINT                     _UxGT("О О±ПЌПѓО· ОµОєП„ПЌПЂП‰ПѓО·П‚")
#define MSG_RESUME_PRINT                    _UxGT("ОЈП…ОЅО­П‡О№ПѓО· ОµОєП„ПЌПЂП‰ПѓО·П‚")
#define MSG_STOP_PRINT                      _UxGT("О”О№О±ОєОїПЂО® ОµОєП„ПЌПЂП‰ПѓО·П‚")
#define MSG_CARD_MENU                       _UxGT("О•ОєП„ПЌПЂП‰ПѓО· О±ПЂПЊ SD")
#define MSG_NO_CARD                         _UxGT("О”ОµОЅ ОІПЃО­ОёО·ОєОµ SD")
#define MSG_DWELL                           _UxGT("О‘ОЅО±ПѓП„ОїО»О® О»ОµО№П„ОїП…ПЃОіОЇО±П‚вЂ¦")
#define MSG_USERWAIT                        _UxGT("О‘ОЅО±ОјОїОЅО® ОіО№О± П‡ПЃО®ПѓП„О·вЂ¦")
#define MSG_PRINT_ABORTED                   _UxGT("О”О№О±ОєПЊПЂП„ОµП„О±О№ О· ОµОєП„ПЌПЂП‰ПѓО·")
#define MSG_NO_MOVE                         _UxGT("ОљО±ОјОЇО± ОєОЇОЅО·ПѓО·.")
#define MSG_KILLED                          _UxGT("О¤О•ОЎОњО‘О¤О™ОЈОњОџОЈ. ")
#define MSG_STOPPED                         _UxGT("О”О™О‘ОљОџО О—. ")
#define MSG_CONTROL_RETRACT                 _UxGT("О‘ОЅО¬ПѓП…ПЃПѓО· ОјОј")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("О•ОЅО±О»О»О±ОіО® О±ОЅО¬ПѓП…ПЃПѓО·П‚ ОјОј")
#define MSG_CONTROL_RETRACTF                _UxGT("О‘ОЅО¬ПѓП…ПЃПѓО· V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("ОњОµП„О±ПЂО®ОґО·ПѓО· ОјОј")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("О‘П…П„ПЊОјО±П„О· О±ОЅО¬ПѓП…ПЃПѓО·")
#define MSG_FILAMENTCHANGE                  _UxGT("О‘О»О»О±ОіО® ОЅО®ОјО±П„ОїП‚")
#define MSG_INIT_SDCARD                     _UxGT("О ПЃОїОµП„ОїО№ОјО±ПѓОЇО± ОєО¬ПЃП„О±П‚ SD")
#define MSG_CNG_SDCARD                      _UxGT("О‘О»О»О±ОіО® ОєО¬ПЃП„О±П‚ SD")
#define MSG_ZPROBE_OUT                      _UxGT("О”О№ОµПЃОµПЌОЅО·ПѓО· Z ОµОєП„ПЊП‚ ОєО»ОЇОЅО·П‚")
#define MSG_YX_UNHOMED                      _UxGT("О•ПЂО±ОЅО±П†ОїПЃО¬ О§/ОҐ ПЂПЃО№ОЅ О±ПЂПЊ О–")
#define MSG_XYZ_UNHOMED                     _UxGT("О•ПЂО±ОЅО±П†ОїПЃО¬ О§ОҐО– ПЂПЃПЋП„О±")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("ОњОµП„О±П„ПЊПЂО№ПѓО· О–")
#define MSG_BABYSTEP_X                      _UxGT("ОњО№ОєПЃПЊ ОІО®ОјО± О§")
#define MSG_BABYSTEP_Y                      _UxGT("ОњО№ОєПЃПЊ ОІО®ОјО± ОҐ")
#define MSG_BABYSTEP_Z                      _UxGT("ОњО№ОєПЃПЊ ОІО®ОјО± О–")
#define MSG_ENDSTOP_ABORT                   _UxGT("ОњО±П„О±ОЇП‰ПѓО· endstop ")
#define MSG_HEATING_FAILED_LCD              _UxGT("О‘ОЅОµПЂО№П„П…П‡О®П‚ ОёО­ПЃОјО±ОЅПѓО·")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("О›О¬ОёОїП‚: О О›О•ОџОќО‘О–ОџОҐОЈО‘ О�О•ОЎОњОџО¤О—О¤О‘")
#define MSG_THERMAL_RUNAWAY                 _UxGT("О”О™О‘О¦ОҐО“О— О�О•ОЎОњОџО¤О—О¤О‘ОЈ")
#define MSG_ERR_MAXTEMP                     _UxGT("О›О¬ОёОїП‚: ОњО•О“О™ОЈО¤О— О�О•ОЎОњОџО¤О—О¤О‘")
#define MSG_ERR_MINTEMP                     _UxGT("О›О¬ОёОїП‚: О•О›О‘О§О™ОЈО¤О— О�О•ОЎОњОџО¤О—О¤О‘")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("О›О¬ОёОїП‚: ОњО•О“О™ОЈО¤О— О�О•ОЎОњОџО¤О—О¤О‘ ОљО›О™ОќО—ОЈ")
#define MSG_ERR_MINTEMP_BED                 _UxGT("О›О¬ОёОїП‚: О•О›О‘О§О™ОЈО¤О— О�О•ОЎОњОџО¤О—О¤О‘ ОљО›О™ОќО—ОЈ")
#define MSG_HEATING                         _UxGT("О�ОµПЃОјО±ОЇОЅОµП„О±О№вЂ¦")
#define MSG_BED_HEATING                     _UxGT("О�О­ПЃОјО±ОЅПѓО· ОєО»ОЇОЅО·П‚вЂ¦")
#define MSG_DELTA_CALIBRATE                 _UxGT("О’О±ОёОјОїОЅПЊОјО·ПѓО· Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("О’О±ОёОјОїОЅПЊОјО·ПѓО· X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("О’О±ОёОјОїОЅПЊОјО·ПѓО· Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("О’О±ОёОјОїОЅПЊОјО·ПѓО· Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("О’О±ОёОјОїОЅПЊОјО·ПѓО· ОєО­ОЅП„ПЃОїП…")

#endif // LANGUAGE_EL_GR_H


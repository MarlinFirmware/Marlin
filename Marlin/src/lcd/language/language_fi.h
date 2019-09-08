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
 * Finnish
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" valmis.")
#define MSG_MEDIA_INSERTED                  _UxGT("Kortti asetettu")
#define MSG_MEDIA_REMOVED                   _UxGT("Kortti poistettu")
#define MSG_MAIN                            _UxGT("Palaa")
#define MSG_AUTOSTART                       _UxGT("Automaatti")
#define MSG_DISABLE_STEPPERS                _UxGT("Vapauta moottorit")
#define MSG_AUTO_HOME                       _UxGT("Aja referenssiin")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#define MSG_SET_ORIGIN                      _UxGT("Aseta origo")
#define MSG_PREHEAT_1                       _UxGT("Esilämmitä " PREHEAT_1_LABEL)
#define MSG_PREHEAT_1_N                     _UxGT("Esilämmitä " PREHEAT_1_LABEL " ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Esilä. " PREHEAT_1_LABEL " Kaikki")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Esilä. " PREHEAT_1_LABEL " Alusta")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Esilämm. " PREHEAT_1_LABEL " konf")
#define MSG_PREHEAT_2                       _UxGT("Esilämmitä " PREHEAT_2_LABEL)
#define MSG_PREHEAT_2_N                     _UxGT("Esilämmitä " PREHEAT_2_LABEL " ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Esilä. " PREHEAT_2_LABEL " Kaikki")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Esilä. " PREHEAT_2_LABEL " Alusta")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Esilämm. " PREHEAT_2_LABEL " konf")
#define MSG_COOLDOWN                        _UxGT("Jäähdytä")
#define MSG_SWITCH_PS_ON                    _UxGT("Virta päälle")
#define MSG_SWITCH_PS_OFF                   _UxGT("Virta pois")
#define MSG_EXTRUDE                         _UxGT("Pursota")
#define MSG_RETRACT                         _UxGT("Vedä takaisin")
#define MSG_MOVE_AXIS                       _UxGT("Liikuta akseleita")
#define MSG_MOVE_X                          _UxGT("Liikuta X")
#define MSG_MOVE_Y                          _UxGT("Liikuta Y")
#define MSG_MOVE_Z                          _UxGT("Liikuta Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_Z_DIST                     _UxGT("Liikuta %smm")
#define MSG_MOVE_01MM                       _UxGT("Liikuta 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Liikuta 1mm")
#define MSG_MOVE_10MM                       _UxGT("Liikuta 10mm")
#define MSG_SPEED                           _UxGT("Nopeus")
#define MSG_NOZZLE                          _UxGT("Suutin")
#define MSG_BED                             _UxGT("Alusta")
#define MSG_FAN_SPEED                       _UxGT("Tuul. nopeus")
#define MSG_FLOW                            _UxGT("Virtaus")
#define MSG_CONTROL                         _UxGT("Kontrolli")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Kerr")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_LCD_ON                          _UxGT("On")
#define MSG_LCD_OFF                         _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Kiihtyv")
#define MSG_JERK                            _UxGT("Jerk")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Va-jerk")
  #define MSG_VB_JERK                       _UxGT("Vb-jerk")
  #define MSG_VC_JERK                       _UxGT("Vc-jerk")
#else
  #define MSG_VA_JERK                       _UxGT("Vx-jerk")
  #define MSG_VB_JERK                       _UxGT("Vy-jerk")
  #define MSG_VC_JERK                       _UxGT("Vz-jerk")
#endif
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VLiike min")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-peruuta")
#define MSG_STEPS_PER_MM                    _UxGT("Steps/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("Asteps/mm")
  #define MSG_BSTEPS                        _UxGT("Bsteps/mm")
  #define MSG_CSTEPS                        _UxGT("Csteps/mm")
#else
  #define MSG_ASTEPS                        _UxGT("Xsteps/mm")
  #define MSG_BSTEPS                        _UxGT("Ysteps/mm")
  #define MSG_CSTEPS                        _UxGT("Zsteps/mm")
#endif
#define MSG_ESTEPS                          _UxGT("Esteps/mm")
#define MSG_E1STEPS                         _UxGT("E1steps/mm")
#define MSG_E2STEPS                         _UxGT("E2steps/mm")
#define MSG_E3STEPS                         _UxGT("E3steps/mm")
#define MSG_E4STEPS                         _UxGT("E4steps/mm")
#define MSG_E5STEPS                         _UxGT("E5steps/mm")
#define MSG_E6STEPS                         _UxGT("E6steps/mm")
#define MSG_TEMPERATURE                     _UxGT("Lämpötila")
#define MSG_MOTION                          _UxGT("Liike")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm³")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("LCD kontrasti")
#define MSG_STORE_EEPROM                    _UxGT("Tallenna muistiin")
#define MSG_LOAD_EEPROM                     _UxGT("Lataa muistista")
#define MSG_RESTORE_FAILSAFE                _UxGT("Palauta oletus")
#define MSG_REFRESH                         _UxGT("Päivitä")
#define MSG_WATCH                           _UxGT("Seuraa")
#define MSG_PREPARE                         _UxGT("Valmistele")
#define MSG_TUNE                            _UxGT("Säädä")
#define MSG_PAUSE_PRINT                     _UxGT("Keskeytä tulostus")
#define MSG_RESUME_PRINT                    _UxGT("Jatka tulostusta")
#define MSG_STOP_PRINT                      _UxGT("Pysäytä tulostus")
#define MSG_MEDIA_MENU                      _UxGT("Korttivalikko")
#define MSG_NO_MEDIA                        _UxGT("Ei korttia")
#define MSG_DWELL                           _UxGT("Nukkumassa...")
#define MSG_USERWAIT                        _UxGT("Odotet. valintaa")
#define MSG_PRINT_ABORTED                   _UxGT("Print aborted")
#define MSG_NO_MOVE                         _UxGT("Ei liiketta.")
#define MSG_KILLED                          _UxGT("KILLED. ")
#define MSG_STOPPED                         _UxGT("STOPPED. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Vedä mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Va. Vedä mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Vedä V")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Z mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Va. UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoVeto.")
#define MSG_FILAMENTCHANGE                  _UxGT("Change filament")
#define MSG_INIT_MEDIA                      _UxGT("Init. SD-Card")
#define MSG_CHANGE_MEDIA                    _UxGT("Change SD-Card")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibrointi")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibroi X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibroi Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibroi Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibroi Center")

#define MSG_EXPECTED_PRINTER                _UxGT("Väärä tulostin")

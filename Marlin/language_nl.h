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
 * Dutch
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_NL_H
#define LANGUAGE_NL_H

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" gereed.")
#define MSG_SD_INSERTED                     _UxGT("Kaart ingestoken")
#define MSG_SD_REMOVED                      _UxGT("Kaart verwijderd")
#define MSG_MAIN                            _UxGT("Hoofdmenu")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Motoren uit")
#define MSG_AUTO_HOME                       _UxGT("Auto home")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Klik voor begin")
#define MSG_LEVEL_BED_DONE                  _UxGT("Bed level kompl.")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Bed level afbr.")
#define MSG_SET_HOME_OFFSETS                _UxGT("Zet home offsets")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("H offset toegep.")
#define MSG_SET_ORIGIN                      _UxGT("Nulpunt instellen")
#define MSG_PREHEAT_1                       _UxGT("PLA voorverwarmen")
#define MSG_PREHEAT_1_N                     _UxGT("PLA voorverw. ")
#define MSG_PREHEAT_1_ALL                   _UxGT("PLA voorverw. aan")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("PLA voorverw. Bed")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("PLA verw. conf")
#define MSG_PREHEAT_2                       _UxGT("ABS voorverwarmen")
#define MSG_PREHEAT_2_N                     _UxGT("ABS voorverw. ")
#define MSG_PREHEAT_2_ALL                   _UxGT("ABS voorverw. aan")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("ABS voorverw. Bed")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("ABS verw. conf")
#define MSG_COOLDOWN                        _UxGT("Afkoelen")
#define MSG_SWITCH_PS_ON                    _UxGT("Stroom aan")
#define MSG_SWITCH_PS_OFF                   _UxGT("Stroom uit")
#define MSG_EXTRUDE                         _UxGT("Extrude")
#define MSG_RETRACT                         _UxGT("Retract")
#define MSG_MOVE_AXIS                       _UxGT("As verplaatsen")
#define MSG_MOVE_X                          _UxGT("Verplaats X")
#define MSG_MOVE_Y                          _UxGT("Verplaats Y")
#define MSG_MOVE_Z                          _UxGT("Verplaats Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_01MM                       _UxGT("Verplaats 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Verplaats 1mm")
#define MSG_MOVE_10MM                       _UxGT("Verplaats 10mm")
#define MSG_SPEED                           _UxGT("Snelheid")
#define MSG_BED_Z                           _UxGT("Bed Z")
#define MSG_NOZZLE                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Nozzle")
#define MSG_BED                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Bed")
#define MSG_FAN_SPEED                       _UxGT("Fan snelheid")
#define MSG_FLOW                            _UxGT("Flow")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_ON                              _UxGT("Aan ")
#define MSG_OFF                             _UxGT("Uit")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Versn")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retract")
#define MSG_A_TRAVEL                        _UxGT("A-travel")
#define MSG_XSTEPS                          _UxGT("Xsteps/mm")
#define MSG_YSTEPS                          _UxGT("Ysteps/mm")
#define MSG_ZSTEPS                          _UxGT("Zsteps/mm")
#define MSG_ESTEPS                          _UxGT("Esteps/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatuur")
#define MSG_MOTION                          _UxGT("Beweging")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("LCD contrast")
#define MSG_STORE_EPROM                     _UxGT("Geheugen opslaan")
#define MSG_LOAD_EPROM                      _UxGT("Geheugen laden")
#define MSG_RESTORE_FAILSAFE                _UxGT("Noodstop reset")
#define MSG_REFRESH                         _UxGT("Ververs")
#define MSG_WATCH                           _UxGT("Info scherm")
#define MSG_PREPARE                         _UxGT("Voorbereiden")
#define MSG_TUNE                            _UxGT("Afstellen")
#define MSG_PAUSE_PRINT                     _UxGT("Print pauzeren")
#define MSG_RESUME_PRINT                    _UxGT("Print hervatten")
#define MSG_STOP_PRINT                      _UxGT("Print stoppen")
#define MSG_CARD_MENU                       _UxGT("Print van SD")
#define MSG_NO_CARD                         _UxGT("Geen SD kaart")
#define MSG_DWELL                           _UxGT("Slapen...")
#define MSG_USERWAIT                        _UxGT("Wachten...")
#define MSG_RESUMING                        _UxGT("Print hervatten")
#define MSG_PRINT_ABORTED                   _UxGT("Print afgebroken")
#define MSG_NO_MOVE                         _UxGT("Geen beweging.")
#define MSG_KILLED                          _UxGT("Afgebroken. ")
#define MSG_STOPPED                         _UxGT("Gestopt. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retract mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Ruil Retract mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retract  F")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet +mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Ruil UnRet +mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  F")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Verv. Filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. SD kaart")
#define MSG_CNG_SDCARD                      _UxGT("Verv. SD Kaart")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe uit. bed")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystap X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystap Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystap Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop afbr.")
#define MSG_HEATING_FAILED_LCD              _UxGT("voorverw. fout")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Redun. temp fout")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Therm. wegloop")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: Max. temp")
#define MSG_ERR_MINTEMP                     _UxGT("Err: Min. temp")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: Max.tmp bed")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: Min.tmp bed")
#define MSG_HEATING                         _UxGT("Voorwarmen...")
#define MSG_HEATING_COMPLETE                _UxGT("Voorverw. kompl.")
#define MSG_BED_HEATING                     _UxGT("Bed voorverw.")
#define MSG_BED_DONE                        _UxGT("Bed is voorverw.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Calibratie")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibreer X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibreer Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibreer Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibreer Midden")

#endif // LANGUAGE_NL_H

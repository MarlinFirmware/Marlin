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
 * Croatian (Hrvatski)
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_HR_H
#define LANGUAGE_HR_H

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" spreman.")
#define MSG_SD_INSERTED                     _UxGT("SD kartica umetnuta")
#define MSG_SD_REMOVED                      _UxGT("SD kartica uklonjena")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops" // Max length 8 characters
#define MSG_MAIN                            _UxGT("Main")
#define MSG_AUTOSTART                       _UxGT("Automatsko pokretanje")
#define MSG_DISABLE_STEPPERS                _UxGT("Ugasi steppere")
#define MSG_AUTO_HOME                       _UxGT("Automatski homing")
#define MSG_AUTO_HOME_X                     _UxGT("Home-aj X")
#define MSG_AUTO_HOME_Y                     _UxGT("Home-aj Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Home-aj Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Home-aj XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Sljedeća točka")
#define MSG_LEVEL_BED_DONE                  _UxGT("Niveliranje gotovo!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Otkaži")
#define MSG_SET_HOME_OFFSETS                _UxGT("Postavi home offsete")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets postavljeni")
#define MSG_SET_ORIGIN                      _UxGT("Postavi ishodište")
#define MSG_PREHEAT_1                       _UxGT("Predgrij PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Sve")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Bed")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" conf")
#define MSG_PREHEAT_2                       _UxGT("Predgrij ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Sve")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Bed")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" conf")
#define MSG_COOLDOWN                        _UxGT("Hlađenje")
#define MSG_SWITCH_PS_ON                    _UxGT("Uključi napajanje")
#define MSG_SWITCH_PS_OFF                   _UxGT("Isključi napajanje")
#define MSG_EXTRUDE                         _UxGT("Extrude")
#define MSG_RETRACT                         _UxGT("Retract")
#define MSG_MOVE_AXIS                       _UxGT("Miči os")
#define MSG_LEVEL_BED                       _UxGT("Niveliraj bed")
#define MSG_MOVE_X                          _UxGT("Miči X")
#define MSG_MOVE_Y                          _UxGT("Miči Y")
#define MSG_MOVE_Z                          _UxGT("Miči Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_01MM                       _UxGT("Miči 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Miči 1mm")
#define MSG_MOVE_10MM                       _UxGT("Miči 10mm")
#define MSG_SPEED                           _UxGT("Brzina")
#define MSG_BED_Z                           _UxGT("Bed Z")
#define MSG_NOZZLE                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Nozzle")
#define MSG_BED                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Bed")
#define MSG_FAN_SPEED                       _UxGT("Brzina ventilatora")
#define MSG_FLOW                            _UxGT("Flow")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_ON                              _UxGT("On ")
#define MSG_OFF                             _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Accel")
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
#define MSG_TEMPERATURE                     _UxGT("Temperature")
#define MSG_MOTION                          _UxGT("Motion")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("Kontrast LCD-a")
#define MSG_STORE_EPROM                     _UxGT("Pohrani u memoriju")
#define MSG_LOAD_EPROM                      _UxGT("Učitaj memoriju")
#define MSG_RESTORE_FAILSAFE                _UxGT("Učitaj failsafe")
#define MSG_REFRESH                         _UxGT("Osvježi")
#define MSG_WATCH                           _UxGT("Info screen")
#define MSG_PREPARE                         _UxGT("Pripremi")
#define MSG_TUNE                            _UxGT("Tune")
#define MSG_PAUSE_PRINT                     _UxGT("Pauziraj print")
#define MSG_RESUME_PRINT                    _UxGT("Nastavi print")
#define MSG_STOP_PRINT                      _UxGT("Zaustavi print")
#define MSG_CARD_MENU                       _UxGT("Printaj s SD kartice")
#define MSG_NO_CARD                         _UxGT("Nema SD kartice")
#define MSG_DWELL                           _UxGT("Sleep...")
#define MSG_USERWAIT                        _UxGT("Čekaj korisnika...")
#define MSG_RESUMING                        _UxGT("Nastavljam print")
#define MSG_PRINT_ABORTED                   _UxGT("Print otkazan")
#define MSG_NO_MOVE                         _UxGT("No move.")
#define MSG_KILLED                          _UxGT("KILLED. ")
#define MSG_STOPPED                         _UxGT("ZAUSTAVLJEN. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retract mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Re.mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retract  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet +mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet+mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Promijeni filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. SD karticu")
#define MSG_CNG_SDCARD                      _UxGT("Promijeni SD karticu")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#define MSG_HEATING_FAILED_LCD              _UxGT("Heating failed")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     _UxGT("Err: MINTEMP")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: MAXTEMP BED")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: MINTEMP BED")
#define MSG_HEATING                         _UxGT("Grijanje...")
#define MSG_HEATING_COMPLETE                _UxGT("Grijanje gotovo.")
#define MSG_BED_HEATING                     _UxGT("Grijanje Bed-a.")
#define MSG_BED_DONE                        _UxGT("Bed gotov.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibracija")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibriraj X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibriraj Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibriraj Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibriraj Središte")

#endif // LANGUAGE_HR_H

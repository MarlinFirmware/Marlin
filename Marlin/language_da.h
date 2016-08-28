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
 * Danish
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_DA_H
#define LANGUAGE_DA_H

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" er klar")
#define MSG_SD_INSERTED                     _UxGT("Kort isat")
#define MSG_SD_REMOVED                      _UxGT("Kort fjernet")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menu")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Slå alle steppere fra")
#define MSG_AUTO_HOME                       _UxGT("Auto Home") // G28
#define MSG_AUTO_HOME_X                     _UxGT("Home X")
#define MSG_AUTO_HOME_Y                     _UxGT("Home Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Home Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Klik når du er klar")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Næste punkt")
#define MSG_LEVEL_BED_DONE                  _UxGT("Bed level er færdig!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Annuller bed level")
#define MSG_SET_HOME_OFFSETS                _UxGT("Sæt forsk. af home")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Forsk. er nu aktiv")
#define MSG_SET_ORIGIN                      _UxGT("Sæt origin")
#define MSG_PREHEAT_1                       _UxGT("Forvarm PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Forvarm PLA ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Forvarm PLA Alle")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Forvarm PLA Bed")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Forvarm PLA conf")
#define MSG_PREHEAT_2                       _UxGT("Forvarm ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Forvarm ABS ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Forvarm ABS Alle")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Forvarm ABS Bed")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Forvarm ABS conf")
#define MSG_COOLDOWN                        _UxGT("Afkøl")
#define MSG_SWITCH_PS_ON                    _UxGT("Slå strøm til")
#define MSG_SWITCH_PS_OFF                   _UxGT("Slå strøm fra")
#define MSG_EXTRUDE                         _UxGT("Extruder")
#define MSG_RETRACT                         _UxGT("Retract")
#define MSG_MOVE_AXIS                       _UxGT("Flyt akser")
#define MSG_LEVEL_BED                       _UxGT("Juster bed")
#define MSG_MOVE_X                          _UxGT("Flyt X")
#define MSG_MOVE_Y                          _UxGT("Flyt Y")
#define MSG_MOVE_Z                          _UxGT("Flyt Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_01MM                       _UxGT("Flyt 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Flyt 1mm")
#define MSG_MOVE_10MM                       _UxGT("Flyt 10mm")
#define MSG_SPEED                           _UxGT("Hastighed")
#define MSG_BED_Z                           _UxGT("Plade Z")
#define MSG_NOZZLE                          _UxGT("Dyse")

#define MSG_BED                             _UxGT("Plade")
#define MSG_FAN_SPEED                       _UxGT("Blæser hastighed")
#define MSG_FLOW                            _UxGT("Flow")
#define MSG_CONTROL                         _UxGT("Kontrol")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_ON                              _UxGT("Til ")
#define MSG_OFF                             _UxGT("Fra")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Vælg")
#define MSG_ACC                             _UxGT("Accel")
#define MSG_VXY_JERK                        _UxGT("Vxy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retract")
#define MSG_A_TRAVEL                        _UxGT("A-rejse")
#define MSG_XSTEPS                          _UxGT("Xsteps/mm")
#define MSG_YSTEPS                          _UxGT("Ysteps/mm")
#define MSG_ZSTEPS                          _UxGT("Zsteps/mm")
#define MSG_ESTEPS                          _UxGT("Esteps/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatur")
#define MSG_MOTION                          _UxGT("Bevægelse")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("LCD kontrast")
#define MSG_STORE_EPROM                     _UxGT("Gem i EEPROM")
#define MSG_LOAD_EPROM                      _UxGT("Hent fra EEPROM")
#define MSG_RESTORE_FAILSAFE                _UxGT("Gendan failsafe")
#define MSG_REFRESH                         _UxGT("Genopfrisk")
#define MSG_WATCH                           _UxGT("Info skærm")
#define MSG_PREPARE                         _UxGT("Forbered")
#define MSG_TUNE                            _UxGT("Tune")
#define MSG_PAUSE_PRINT                     _UxGT("Pause printet")
#define MSG_RESUME_PRINT                    _UxGT("Forsæt printet")
#define MSG_STOP_PRINT                      _UxGT("Stop printet")
#define MSG_CARD_MENU                       _UxGT("Print fra SD")
#define MSG_NO_CARD                         _UxGT("Intet SD kort")
#define MSG_DWELL                           _UxGT("Dvale...")
#define MSG_USERWAIT                        _UxGT("Venter på bruger...")
#define MSG_RESUMING                        _UxGT("Forsætter printet")
#define MSG_PRINT_ABORTED                   _UxGT("Print annulleret")
#define MSG_NO_MOVE                         _UxGT("Ingen bevægelse.")
#define MSG_KILLED                          _UxGT("DRÆBT. ")
#define MSG_STOPPED                         _UxGT("STOPPET. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Tilbagetræk mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Skift Re.mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Tilbagetræk V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet +mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet+mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Skift filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. SD card")
#define MSG_CNG_SDCARD                      _UxGT("Skift SD kort")
#define MSG_ZPROBE_OUT                      _UxGT("Probe udenfor plade")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("først")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#define MSG_HEATING_FAILED_LCD              _UxGT("Opvarmning fejlet")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Fejl: reserve temp")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Temp løber løbsk")
#define MSG_ERR_MAXTEMP                     _UxGT("Fejl: MAKSTEMP")
#define MSG_ERR_MINTEMP                     _UxGT("Fejl: MINTEMP")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Fejl: MAKSTEMP PLADE")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Fejl: MINTEMP PLADE")
#define MSG_HALTED                          _UxGT("PRINTER HALTED")
#define MSG_PLEASE_RESET                    _UxGT("Reset venligst")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Opvarmer...")
#define MSG_HEATING_COMPLETE                _UxGT("Opvarmet")
#define MSG_BED_HEATING                     _UxGT("Opvarmer plade")
#define MSG_BED_DONE                        _UxGT("Plade opvarmet")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibrering")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibrer X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibrer Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibrer Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibrerings Center")

#define MSG_INFO_MENU                       _UxGT("Om Printer")
#define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#define MSG_INFO_STATS_MENU                 _UxGT("Printer Stat.")
#define MSG_INFO_BOARD_MENU                 _UxGT("Board Info")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrudere")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocol")

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              _UxGT("Antal Print")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Færdige")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total print tid")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Længste print tid")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total extruderet")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Prints")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Færdige")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Længste")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruderet")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Strømforsyning")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("SKIFT FILAMENT")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "SKIFTE MULIGHEDER:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extruder mere")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "forsæt print")

#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Vent på")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("skift af ")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("filament")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Vent på")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament tømning")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("indsæt filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("og tryk for")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("at fortsætte...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Vent på")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament fødning")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Vent på")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("filament extrudere")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Vent på at")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("print forsætter")
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Vent venligst...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Tømmer...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Indsæt og klik")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Føder...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Extrudere...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Fortsætter...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_DA_H


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
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_DA_H
#define LANGUAGE_DA_H

#define DISPLAY_CHARSET_ISO10646_1
#define CHARSIZE 2

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
#define MSG_BED_LEVELING                    _UxGT("Juster bed")
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
#define MSG_MIN                             _UxGT(" \002 Min")
#define MSG_MAX                             _UxGT(" \002 Max")
#define MSG_FACTOR                          _UxGT(" \002 Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_ON                              _UxGT("Til ")
#define MSG_OFF                             _UxGT("Fra")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Vælg")
#define MSG_ACC                             _UxGT("Accel")
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
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retract")
#define MSG_A_TRAVEL                        _UxGT("A-rejse")
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
#define MSG_TEMPERATURE                     _UxGT("Temperatur")
#define MSG_MOTION                          _UxGT("Bevægelse")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E i mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("LCD kontrast")
#define MSG_STORE_EEPROM                    _UxGT("Gem i EEPROM")
#define MSG_LOAD_EEPROM                     _UxGT("Hent fra EEPROM")
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
#define MSG_PRINT_ABORTED                   _UxGT("Print annulleret")
#define MSG_NO_MOVE                         _UxGT("Ingen bevægelse.")
#define MSG_KILLED                          _UxGT("DRÆBT. ")
#define MSG_STOPPED                         _UxGT("STOPPET. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Tilbagetræk mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Skift Re.mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Tilbagetræk V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Skift UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Skift filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. SD card")
#define MSG_CNG_SDCARD                      _UxGT("Skift SD kort")
#define MSG_ZPROBE_OUT                      _UxGT("Probe udenfor plade")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Selv-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
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
#define MSG_ERR_MAXTEMP                     _UxGT("Fejl: Maks temp")
#define MSG_ERR_MINTEMP                     _UxGT("Fejl: Min temp")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Fejl: Maks Plade temp")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Fejl: Min Plade temp")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("PRINTER STOPPET")
#define MSG_PLEASE_RESET                    _UxGT("Reset Venligst")
#define MSG_SHORT_DAY                       _UxGT("d") // Kun et bogstav
#define MSG_SHORT_HOUR                      _UxGT("h") // Kun et bogstav
#define MSG_SHORT_MINUTE                    _UxGT("m") // Kun et bogstav
#define MSG_HEATING                         _UxGT("Opvarmer...")
#define MSG_BED_HEATING                     _UxGT("Opvarmer plade...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibrering")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibrer X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibrer Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibrer Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibrerings Center")

#define MSG_INFO_MENU                       _UxGT("Om Printer")
#define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#define MSG_INFO_STATS_MENU                 _UxGT("Printer Stats")
#define MSG_INFO_BOARD_MENU                 _UxGT("Kort Info")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocol")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Ant. Prints")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Færdige")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total print tid")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Længste print")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total Extruderet")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Prints")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Færdige")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Længste")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruderet")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Strømfors.")

#define MSG_DRIVE_STRENGTH                  _UxGT("Driv Styrke")
#define MSG_DAC_PERCENT                     _UxGT("Driv %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM Skriv")

#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Forsæt print")

#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Vent på start")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("af filament")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("skift")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Vent på")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament udskyd.")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Indsæt filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("og tryk på knap")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("for at fortsætte...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Vent på")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament indtag")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Vent på at print")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("fortsætter")
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Vent venligst...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Udskyder...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Indsæt og klik")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Indtager...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Fortsætter...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_DA_H

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
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_HR_H
#define LANGUAGE_HR_H

#define DISPLAY_CHARSET_ISO10646_1 // use the better font on full graphic displays.
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" spreman.")
#define MSG_SD_INSERTED                     _UxGT("SD kartica umetnuta")
#define MSG_SD_REMOVED                      _UxGT("SD kartica uklonjena")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Main")
#define MSG_AUTOSTART                       _UxGT("Auto pokretanje")
#define MSG_DISABLE_STEPPERS                _UxGT("Ugasi steppere")
#define MSG_AUTO_HOME                       _UxGT("Automatski homing")
#define MSG_AUTO_HOME_X                     _UxGT("Home-aj X")
#define MSG_AUTO_HOME_Y                     _UxGT("Home-aj Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Home-aj Z")
#define MSG_TMC_Z_CALIBRATION               _UxGT("Kalibriraj Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Home-aj XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Klikni za početak")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Sljedeća točka")
#define MSG_LEVEL_BED_DONE                  _UxGT("Niveliranje gotovo!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Postavi home offsete")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets postavljeni")
#define MSG_SET_ORIGIN                      _UxGT("Postavi ishodište")
#define MSG_PREHEAT_1                       _UxGT("Predgrij " PREHEAT_1_LABEL)
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Sve")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Bed")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" conf")
#define MSG_PREHEAT_2                       _UxGT("Predgrij " PREHEAT_2_LABEL)
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
#define MSG_BED_LEVELING                    _UxGT("Niveliraj bed")
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
#define MSG_NOZZLE                          _UxGT("Dizna")
#define MSG_BED                             _UxGT("Bed")
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
#define MSG_SELECT                          _UxGT("Odaberi")
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
#define MSG_A_TRAVEL                        _UxGT("A-travel")
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
#define MSG_TEMPERATURE                     _UxGT("Temperature")
#define MSG_MOTION                          _UxGT("Gibanje")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("Kontrast LCD-a")
#define MSG_STORE_EEPROM                    _UxGT("Pohrani u memoriju")
#define MSG_LOAD_EEPROM                     _UxGT("Učitaj memoriju")
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
#define MSG_PRINT_ABORTED                   _UxGT("Print otkazan")
#define MSG_NO_MOVE                         _UxGT("No move.")
#define MSG_KILLED                          _UxGT("KILLED. ")
#define MSG_STOPPED                         _UxGT("ZAUSTAVLJEN. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retract mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Re.mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retract  V")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Promijeni filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. SD karticu")
#define MSG_CHANGE_SDCARD                   _UxGT("Promijeni SD karticu")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#define MSG_HEATING_FAILED_LCD              _UxGT("Grijanje neuspješno")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     _UxGT("Err: MINTEMP")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: MAXTEMP BED")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: MINTEMP BED")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("PRINTER HALTED")
#define MSG_PLEASE_RESET                    _UxGT("Please reset")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Grijanje...")
#define MSG_BED_HEATING                     _UxGT("Grijanje Bed-a...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibracija")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibriraj X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibriraj Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibriraj Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibriraj Središte")
#define MSG_INFO_MENU                       _UxGT("O printeru")
#define MSG_INFO_PRINTER_MENU               _UxGT("Podaci o printeru")
#define MSG_INFO_STATS_MENU                 _UxGT("Statistika printera")
#define MSG_INFO_BOARD_MENU                 _UxGT("Podaci o elektronici")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistori")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extruderi")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protokol")
#define MSG_CASE_LIGHT                      _UxGT("Osvjetljenje")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Broj printova")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Završeni")
  #define MSG_INFO_PRINT_TIME               _UxGT("Ukupno printanja")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Najduži print")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrudirano ukupno")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Printovi")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Završeni")
  #define MSG_INFO_PRINT_TIME               _UxGT("Ukupno")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Najduži")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrudirano")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Napajanje")
#define MSG_DRIVE_STRENGTH                  _UxGT("Drive Strength")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM Write")

#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Nastavi print")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Čekaj početak")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("filamenta")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("promijeni")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Čekaj")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament unload")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Umetni filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("i pritisni tipku")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("za nastavak...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Pričekaj")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament load")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wait for print")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("to resume")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Pričekaj...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ejecting...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert and Click")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Loading...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Nastavljam...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_HR_H

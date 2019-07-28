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
 * Dutch
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" gereed.")
#define MSG_BACK                            _UxGT("Terug")
#define MSG_SD_INSERTED                     _UxGT("Kaart ingestoken")
#define MSG_SD_REMOVED                      _UxGT("Kaart verwijderd")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Hoofdmenu")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Motoren uit")
#define MSG_DEBUG_MENU                      _UxGT("Debug Menu") //accepted English terms
#define MSG_PROGRESS_BAR_TEST               _UxGT("Vooruitgang Test")
#define MSG_AUTO_HOME                       _UxGT("Auto home")
#define MSG_AUTO_HOME_X                     _UxGT("Home X")
#define MSG_AUTO_HOME_Y                     _UxGT("Home Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Home Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Klik voor begin")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Volgende Plaats")
#define MSG_LEVEL_BED_DONE                  _UxGT("Bed level kompl.")
#define MSG_SET_HOME_OFFSETS                _UxGT("Zet home offsets")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("H offset toegep.")
#define MSG_SET_ORIGIN                      _UxGT("Nulpunt instellen")
#define MSG_PREHEAT_1                       _UxGT(PREHEAT_1_LABEL " voorverwarmen")
#define MSG_PREHEAT_1_N                     _UxGT(PREHEAT_1_LABEL " voorverw. ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1_N _UxGT("aan")
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" Einde")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1_N _UxGT("Bed")
#define MSG_PREHEAT_1_SETTINGS              _UxGT(PREHEAT_1_LABEL " verw. conf")
#define MSG_PREHEAT_2                       _UxGT(PREHEAT_2_LABEL " voorverwarmen")
#define MSG_PREHEAT_2_N                     _UxGT(PREHEAT_2_LABEL " voorverw. ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2_N _UxGT("aan")
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" Einde")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2_N _UxGT("Bed")
#define MSG_PREHEAT_2_SETTINGS              _UxGT(PREHEAT_2_LABEL " verw. conf")
#define MSG_COOLDOWN                        _UxGT("Afkoelen")
#define MSG_SWITCH_PS_ON                    _UxGT("Stroom aan")
#define MSG_SWITCH_PS_OFF                   _UxGT("Stroom uit")
#define MSG_EXTRUDE                         _UxGT("Extrude")
#define MSG_RETRACT                         _UxGT("Retract")
#define MSG_MOVE_AXIS                       _UxGT("As verplaatsen")
#define MSG_BED_LEVELING                    _UxGT("Bed Leveling")
#define MSG_LEVEL_BED                       _UxGT("Level bed")

#define MSG_MOVING                          _UxGT("Verplaatsen...")
#define MSG_FREE_XY                         _UxGT("Vrij XY")
#define MSG_MOVE_X                          _UxGT("Verplaats X")
#define MSG_MOVE_Y                          _UxGT("Verplaats Y")
#define MSG_MOVE_Z                          _UxGT("Verplaats Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_0025MM                     _UxGT("Verplaats 0.025mm")
#define MSG_MOVE_01MM                       _UxGT("Verplaats 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Verplaats 1mm")
#define MSG_MOVE_10MM                       _UxGT("Verplaats 10mm")
#define MSG_SPEED                           _UxGT("Snelheid")
#define MSG_BED_Z                           _UxGT("Bed Z")
#define MSG_NOZZLE                          _UxGT("Nozzle")
#define MSG_BED                             _UxGT("Bed")
#define MSG_FAN_SPEED                       _UxGT("Fan snelheid")
#define MSG_FLOW                            _UxGT("Flow")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_LCD_ON                          _UxGT("Aan")
#define MSG_LCD_OFF                         _UxGT("Uit")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Selecteer")
#define MSG_ACC                             _UxGT("Versn")
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
#define MSG_TEMPERATURE                     _UxGT("Temperatuur")
#define MSG_MOTION                          _UxGT("Beweging")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_ADVANCE_K                       _UxGT("Advance K") //accepted english dutch
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("LCD contrast")
#define MSG_STORE_EEPROM                    _UxGT("Geheugen opslaan")
#define MSG_LOAD_EEPROM                     _UxGT("Geheugen laden")
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
#define MSG_PRINT_ABORTED                   _UxGT("Print afgebroken")
#define MSG_NO_MOVE                         _UxGT("Geen beweging.")
#define MSG_KILLED                          _UxGT("Afgebroken. ")
#define MSG_STOPPED                         _UxGT("Gestopt. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retract mm")  //accepted English term in Dutch
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Ruil Retract mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retract  F")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Hop mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Ruil UnRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  F")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Verv. Filament")
#define MSG_INIT_SDCARD                     _UxGT("Init. SD kaart")
#define MSG_CHANGE_SDCARD                   _UxGT("Verv. SD Kaart")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe uit. bed")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Zelf-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("Eerst")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")  //accepted English term in Dutch
#define MSG_BABYSTEP_X                      _UxGT("Babystap X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystap Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystap Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop afbr.")
#define MSG_HEATING_FAILED_LCD              _UxGT("Voorverw. fout")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Redun. temp fout")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Therm. wegloop")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: Max. temp")
#define MSG_ERR_MINTEMP                     _UxGT("Err: Min. temp")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: Max.tmp bed")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: Min.tmp bed")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("PRINTER GESTOPT")
#define MSG_PLEASE_RESET                    _UxGT("Reset A.U.B.")
#define MSG_SHORT_DAY                       _UxGT("d") //  One character only. Keep English standard
#define MSG_SHORT_HOUR                      _UxGT("h") //  One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") //  One character only
#define MSG_HEATING                         _UxGT("Voorwarmen...")
#define MSG_BED_HEATING                     _UxGT("Bed voorverw...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Calibratie")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibreer X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibreer Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibreer Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibreer Midden")
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto Calibratie")
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Zet Delta Hoogte")

#define MSG_INFO_STATS_MENU                 _UxGT("Printer Stats")
#define MSG_INFO_BOARD_MENU                 _UxGT("Board Info") //accepted English term in Dutch
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_MENU                       _UxGT("Over Printer")
#define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocol")
#define MSG_CASE_LIGHT                      _UxGT("Case licht")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Printed Aantal")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Totaal Voltooid")
  #define MSG_INFO_PRINT_TIME               _UxGT("Totale Printtijd")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Langste Printtijd")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Totaal Extrudeert")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Aantal")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Voltooid")
  #define MSG_INFO_PRINT_TIME               _UxGT("Printtijd ")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Langste")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrud.")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("PSU")  //accepted English term in Dutch

#define MSG_DRIVE_STRENGTH                  _UxGT("Motorstroom")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")  //accepted English term in Dutch
#define MSG_DAC_EEPROM_WRITE                _UxGT("DAC Opslaan")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Hervat print")
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT(" Nozzle: ") //accepeted English term
//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  // Up to 3 lines
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Wacht voor start")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("filament te")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("verwisselen")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wacht voor")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament uit")
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        _UxGT("te laden")
  #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Klik knop om...")
  #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("verw. nozzle.") //nozzle accepted English term
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Nozzle verw.")
  #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Wacht a.u.b.")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Laad filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("en druk knop")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("om verder...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Wacht voor")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament te")
  #define MSG_FILAMENT_CHANGE_LOAD_3          _UxGT("laden")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wacht voor print")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("om verder")
  #define MSG_FILAMENT_CHANGE_RESUME_3        _UxGT("te gaan")
#else // LCD_HEIGHT < 4
  // Up to 2 lines
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Wacht voor")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("start...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wacht voor")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("uitladen...")
  #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Klik knop om...")
  #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("verw. nozzle.") //nozzle accepted English term
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Verwarmen...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Laad filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("en druk knop")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Wacht voor")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("inladen...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wacht voor")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("printing...")
#endif // LCD_HEIGHT < 4

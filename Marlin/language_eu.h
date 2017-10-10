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
 * Basque-Euskera
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_EU_H
#define LANGUAGE_EU_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" prest.")
#define MSG_BACK                            _UxGT("Atzera")
#define MSG_SD_INSERTED                     _UxGT("Txartela sartuta")
#define MSG_SD_REMOVED                      _UxGT("Txartela kenduta")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menu nagusia")
#define MSG_AUTOSTART                       _UxGT("Auto hasiera")
#define MSG_DISABLE_STEPPERS                _UxGT("Itzali motoreak")
#define MSG_DEBUG_MENU                      _UxGT("Arazketa Menua")
#define MSG_PROGRESS_BAR_TEST               _UxGT("Prog. Barra Proba")
#define MSG_AUTO_HOME                       _UxGT("Hasierara joan")
#define MSG_AUTO_HOME_X                     _UxGT("X jatorria")
#define MSG_AUTO_HOME_Y                     _UxGT("Y jatorria")
#define MSG_AUTO_HOME_Z                     _UxGT("Z jatorria")
#define MSG_LEVEL_BED_HOMING                _UxGT("XYZ hasieraratzen")
#define MSG_LEVEL_BED_WAITING               _UxGT("Klik egin hasteko")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Hurrengo Puntua")
#define MSG_LEVEL_BED_DONE                  _UxGT("Berdintzea eginda")
#define MSG_SET_HOME_OFFSETS                _UxGT("Etxe. offset eza.")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsetak ezarrita")
#define MSG_SET_ORIGIN                      _UxGT("Hasiera ipini")
#define MSG_PREHEAT_1                       _UxGT("Berotu PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Guztia")
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" Amaia")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Ohea")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" konf.")
#define MSG_PREHEAT_2                       _UxGT("Berotu ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_1 _UxGT(" Guztia")
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" Amaia")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_1 _UxGT(" Ohea")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_1 _UxGT(" konf.")
#define MSG_COOLDOWN                        _UxGT("Hoztu")
#define MSG_SWITCH_PS_ON                    _UxGT("Energia piztu")
#define MSG_SWITCH_PS_OFF                   _UxGT("Energia itzali")
#define MSG_EXTRUDE                         _UxGT("Estruitu")
#define MSG_RETRACT                         _UxGT("Atzera eragin")
#define MSG_MOVE_AXIS                       _UxGT("Ardatzak mugitu")
#define MSG_BED_LEVELING                    _UxGT("Ohea Berdindu")
#define MSG_LEVEL_BED                       _UxGT("Ohea Berdindu")
#define MSG_MOVING                          _UxGT("Mugitzen...")
#define MSG_FREE_XY                         _UxGT("Askatu XY")
#define MSG_MOVE_X                          _UxGT("Mugitu X")
#define MSG_MOVE_Y                          _UxGT("Mugitu Y")
#define MSG_MOVE_Z                          _UxGT("Mugitu Z")
#define MSG_MOVE_E                          _UxGT("Estrusorea")
#define MSG_MOVE_01MM                       _UxGT("Mugitu 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mugitu 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mugitu 10mm")
#define MSG_SPEED                           _UxGT("Abiadura")
#define MSG_BED_Z                           _UxGT("Z Ohea")
#define MSG_NOZZLE                          _UxGT("Pita")
#define MSG_BED                             _UxGT("Ohea")
#define MSG_FAN_SPEED                       _UxGT("Haizagailu abiada")
#define MSG_FLOW                            _UxGT("Fluxua")
#define MSG_CONTROL                         _UxGT("Kontrola")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fakt")
#define MSG_AUTOTEMP                        _UxGT("Auto tenperatura")
#define MSG_ON                              _UxGT("On ")
#define MSG_OFF                             _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Aukeratu")
#define MSG_ACC                             _UxGT("Azelerazioa")
#define MSG_JERK                            _UxGT("Astindua")
#define MSG_VX_JERK                         _UxGT("Vx-astindua")
#define MSG_VY_JERK                         _UxGT("Vy-astindua")
#define MSG_VZ_JERK                         _UxGT("Vz-astindua")
#define MSG_VE_JERK                         _UxGT("Ve-astindua")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VBidaia min")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retrakt")
#define MSG_A_TRAVEL                        _UxGT("A-bidaia")
#define MSG_STEPS_PER_MM                    _UxGT("Pausoak/mm")
#define MSG_XSTEPS                          _UxGT("X pausoak/mm")
#define MSG_YSTEPS                          _UxGT("Y pausoak/mm")
#define MSG_ZSTEPS                          _UxGT("Z pausoak/mm")
#define MSG_ESTEPS                          _UxGT("E pausoak/mm")
#define MSG_E1STEPS                         _UxGT("E1 pausoak/mm")
#define MSG_E2STEPS                         _UxGT("E2 pausoak/mm")
#define MSG_E3STEPS                         _UxGT("E3 pausoak/mm")
#define MSG_E4STEPS                         _UxGT("E4 pausoak/mm")
#define MSG_E5STEPS                         _UxGT("E5 pausoak/mm")
#define MSG_TEMPERATURE                     _UxGT("Tenperatura")
#define MSG_MOTION                          _UxGT("Mugimendua")
#define MSG_FILAMENT                        _UxGT("Harizpia")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E mm3-tan")
#define MSG_FILAMENT_DIAM                   _UxGT("Hariz. Dia.")
#define MSG_ADVANCE_K                       _UxGT("K Aurrerapena")
#define MSG_CONTRAST                        _UxGT("LCD kontrastea")
#define MSG_STORE_EEPROM                    _UxGT("Gorde memoria")
#define MSG_LOAD_EEPROM                     _UxGT("Kargatu memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Larri. berriz.")
#define MSG_REFRESH                         _UxGT("Berriz kargatu")
#define MSG_WATCH                           _UxGT("Pantaila info")
#define MSG_PREPARE                         _UxGT("Prestatu")
#define MSG_TUNE                            _UxGT("Doitu")
#define MSG_PAUSE_PRINT                     _UxGT("Pausatu inprimak.")
#define MSG_RESUME_PRINT                    _UxGT("Jarraitu inprima.")
#define MSG_STOP_PRINT                      _UxGT("Gelditu inprima.")
#define MSG_CARD_MENU                       _UxGT("SD-tik inprimatu")
#define MSG_NO_CARD                         _UxGT("Ez dago SD-rik")
#define MSG_DWELL                           _UxGT("Lo egin...")
#define MSG_USERWAIT                        _UxGT("Aginduak zain...")
#define MSG_RESUMING                        _UxGT("Jarraitzen inpri.")
#define MSG_PRINT_ABORTED                   _UxGT("Inprim. deusezta.")
#define MSG_NO_MOVE                         _UxGT("Mugimendu gabe.")
#define MSG_KILLED                          _UxGT("AKABATUTA. ")
#define MSG_STOPPED                         _UxGT("GELDITUTA. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Atzera egin mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Atzera mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Atzera egin V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Igo mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Atzera egin mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Swap Atzera mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Atzera egin V")
#define MSG_AUTORETRACT                     _UxGT("Atzera egin")
#define MSG_FILAMENTCHANGE                  _UxGT("Aldatu harizpia")
#define MSG_INIT_SDCARD                     _UxGT("Hasieratu SD-a")
#define MSG_CNG_SDCARD                      _UxGT("Aldatu txartela")
#define MSG_ZPROBE_OUT                      _UxGT("Z zunda kanpora")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch AutoProba")
#define MSG_BLTOUCH_RESET                   _UxGT("BLTouch berrabia.")
#define MSG_HOME                            _UxGT("Etxera")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("lehenengo")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Konpentsatu")
#define MSG_BABYSTEP_X                      _UxGT("Mikro-urratsa X")
#define MSG_BABYSTEP_Y                      _UxGT("Mikro-urratsa Y")
#define MSG_BABYSTEP_Z                      _UxGT("Mikro-urratsa Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop deusezta.")
#define MSG_HEATING_FAILED_LCD              _UxGT("Err: Beroketa")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: Tenperatura")
#define MSG_THERMAL_RUNAWAY                 _UxGT("TENP. KONTROL EZA")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: Tenp Maximoa")
#define MSG_ERR_MINTEMP                     _UxGT("Err: Tenp Minimoa")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: Ohe Tenp Max")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: Ohe Tenp Min")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Debekatua")
#define MSG_HALTED                          _UxGT("INPRIMA. GELDIRIK")
#define MSG_PLEASE_RESET                    _UxGT("Berrabia. Mesedez")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Berotzen...")
#define MSG_HEATING_COMPLETE                _UxGT("Berotzea prest.")
#define MSG_BED_HEATING                     _UxGT("Ohea Berotzen.")
#define MSG_BED_DONE                        _UxGT("Ohea Berotuta.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibraketa")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibratu X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibratu Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibratu Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibratu Zentrua")
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto Kalibraketa")
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Delta Alt. Ezar.")
#define MSG_INFO_MENU                       _UxGT("Inprimagailu Inf.")
#define MSG_INFO_PRINTER_MENU               _UxGT("Inprimagailu Inf.")
#define MSG_INFO_STATS_MENU                 _UxGT("Inprima. estatis.")
#define MSG_INFO_BOARD_MENU                 _UxGT("Txartelaren Info.")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistoreak")
#define MSG_INFO_EXTRUDERS                  _UxGT("Estrusoreak")
#define MSG_INFO_BAUDRATE                   _UxGT("Baudioak")
#define MSG_INFO_PROTOCOL                   _UxGT("Protokoloa")
#define MSG_CASE_LIGHT                      _UxGT("Kabina Argia")

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              _UxGT("Inprim. Zenbaketa")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Burututa")
  #define MSG_INFO_PRINT_TIME               _UxGT("Inprim. denbora")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Imprimatze luzeena")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Estruituta guztira")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Inprimatze")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Burututa")
  #define MSG_INFO_PRINT_TIME               _UxGT("Guztira")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Luzeena")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Estrusio")
#endif
#define MSG_INFO_MIN_TEMP                   _UxGT("Tenp. Minimoa")
#define MSG_INFO_MAX_TEMP                   _UxGT("Tenp. Maximoa")
#define MSG_INFO_PSU                        _UxGT("Elikadura Iturria")
#define MSG_DRIVE_STRENGTH                  _UxGT("Driver Potentzia")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Idatzi DAC EEPROM")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("HARIZPIA ALDATU")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("ALDAKETA AUKERAK:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Gehiago estruitu")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Inprima. jarraitu")

#define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Tenp Minimoa ")
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Pita: ")

#endif // LANGUAGE_EU_H

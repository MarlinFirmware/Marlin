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
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_EU_H
#define LANGUAGE_EU_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" prest.")
#define MSG_SD_INSERTED                     _UxGT("Txartela sartuta")
#define MSG_SD_REMOVED                      _UxGT("Txartela kenduta")
#define MSG_MAIN                            _UxGT("Menu nagusia")
#define MSG_AUTOSTART                       _UxGT("Auto hasiera")
#define MSG_DISABLE_STEPPERS                _UxGT("Itzali motoreak")
#define MSG_AUTO_HOME                       _UxGT("Hasierara joan")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Cancel")
#define MSG_SET_HOME_OFFSETS                _UxGT("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#define MSG_SET_ORIGIN                      _UxGT("Hasiera ipini")
#define MSG_PREHEAT_1                       _UxGT("Aurreberotu PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Aurreberotu PLA ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Berotu PLA Guztia")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Berotu PLA Ohea")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Berotu PLA Konfig")
#define MSG_PREHEAT_2                       _UxGT("Aurreberotu ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Aurreberotu ABS ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Berotu ABS Guztia")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Berotu ABS Ohea")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Berotu ABS Konfig")
#define MSG_COOLDOWN                        _UxGT("Hoztu")
#define MSG_SWITCH_PS_ON                    _UxGT("Energia piztu")
#define MSG_SWITCH_PS_OFF                   _UxGT("Energia itzali")
#define MSG_EXTRUDE                         _UxGT("Estruitu")
#define MSG_RETRACT                         _UxGT("Atzera eragin")
#define MSG_MOVE_AXIS                       _UxGT("Ardatzak mugitu")
#define MSG_MOVE_X                          _UxGT("Mugitu X")
#define MSG_MOVE_Y                          _UxGT("Mugitu Y")
#define MSG_MOVE_Z                          _UxGT("Mugitu Z")
#define MSG_MOVE_E                          _UxGT("Estrusorea")
#define MSG_MOVE_01MM                       _UxGT("Mugitu 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mugitu 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mugitu 10mm")
#define MSG_SPEED                           _UxGT("Abiadura")
#define MSG_NOZZLE                          _UxGT("Pita")
#define MSG_BED                             _UxGT("Ohea")
#define MSG_FAN_SPEED                       _UxGT("Haizagailua")
#define MSG_FLOW                            _UxGT("Fluxua")
#define MSG_CONTROL                         _UxGT("Kontrola")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Faktorea")
#define MSG_AUTOTEMP                        _UxGT("Auto tenperatura")
#define MSG_ON                              _UxGT("On ")
#define MSG_OFF                             _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Azelerazioa")
#define MSG_VX_JERK                         _UxGT("Vx-astindua")
#define MSG_VY_JERK                         _UxGT("Vy-astindua")
#define MSG_VZ_JERK                         _UxGT("Vz-astindua")
#define MSG_VE_JERK                         _UxGT("Ve-astindua")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retrakt")
#define MSG_XSTEPS                          _UxGT("X pausoak/mm")
#define MSG_YSTEPS                          _UxGT("Y pausoak/mm")
#define MSG_ZSTEPS                          _UxGT("Z pausoak/mm")
#define MSG_ESTEPS                          _UxGT("E pausoak/mm")
#define MSG_E1STEPS                         _UxGT("E1 pausoak/mm")
#define MSG_E2STEPS                         _UxGT("E2 pausoak/mm")
#define MSG_E3STEPS                         _UxGT("E3 pausoak/mm")
#define MSG_E4STEPS                         _UxGT("E4 pausoak/mm")
#define MSG_TEMPERATURE                     _UxGT("Tenperatura")
#define MSG_MOTION                          _UxGT("Mugimendua")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("LCD kontrastea")
#define MSG_STORE_EPROM                     _UxGT("Gorde memoria")
#define MSG_LOAD_EPROM                      _UxGT("Kargatu memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Larri. berriz.")
#define MSG_REFRESH                         _UxGT("Berriz kargatu")
#define MSG_WATCH                           _UxGT("Pantaila info")
#define MSG_PREPARE                         _UxGT("Prestatu")
#define MSG_TUNE                            _UxGT("Doitu")
#define MSG_PAUSE_PRINT                     _UxGT("Pausatu inprimak.")
#define MSG_RESUME_PRINT                    _UxGT("Jarraitu inprima.")
#define MSG_STOP_PRINT                      _UxGT("Gelditu inprima.")
#define MSG_CARD_MENU                       _UxGT("SD-tik inprimatu")
#define MSG_NO_CARD                         _UxGT("Ez dago txartelik")
#define MSG_DWELL                           _UxGT("Lo egin...")
#define MSG_USERWAIT                        _UxGT("Aginduak zain...")
#define MSG_RESUMING                        _UxGT("Jarraitzen inpri.")
#define MSG_PRINT_ABORTED                   _UxGT("Print aborted")
#define MSG_NO_MOVE                         _UxGT("Mugimendu gabe")
#define MSG_KILLED                          _UxGT("LARRIALDI GELDIA")
#define MSG_STOPPED                         _UxGT("GELDITUTA. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Atzera egin mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Atzera egin mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Atzera egin V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Igo mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Atzera egin mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Swap Atzera egin mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Atzera egin V")
#define MSG_AUTORETRACT                     _UxGT("Atzera egin")
#define MSG_FILAMENTCHANGE                  _UxGT("Aldatu filament.")
#define MSG_INIT_SDCARD                     _UxGT("Hasieratu txartela")
#define MSG_CNG_SDCARD                      _UxGT("Aldatu txartela")
#define MSG_ZPROBE_OUT                      _UxGT("Z ohe hasiera")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z konpentsatu")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop deuseztat")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Calibration")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrate X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrate Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrate Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrate Center")

#endif // LANGUAGE_EU_H

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
 * Aragonese
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_AN_H
#define LANGUAGE_AN_H

#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" parada.")
#define MSG_SD_INSERTED                     _UxGT("Tarcheta colocada")
#define MSG_SD_REMOVED                      _UxGT("Tarcheta retirada")
#define MSG_MAIN                            _UxGT("Menu prencipal")
#define MSG_AUTOSTART                       _UxGT(" Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Amortar motors")
#define MSG_AUTO_HOME                       _UxGT("Levar a l'orichen")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Cancel")
#define MSG_SET_HOME_OFFSETS                _UxGT("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#define MSG_SET_ORIGIN                      _UxGT("Establir zero")
#define MSG_PREHEAT_1                       _UxGT("Precalentar PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Precalentar PLA ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Precalentar PLA a")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Prec. PLA Base")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Achustar tem. PLA")
#define MSG_PREHEAT_2                       _UxGT("Precalentar ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Precalentar ABS ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Precalentar ABS a")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Prec. ABS Base")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Achustar tem. ABS")
#define MSG_COOLDOWN                        _UxGT("Enfriar")
#define MSG_SWITCH_PS_ON                    _UxGT("Enchegar Fuent")
#define MSG_SWITCH_PS_OFF                   _UxGT("Desenchegar Fuent")
#define MSG_EXTRUDE                         _UxGT("Extruir")
#define MSG_RETRACT                         _UxGT("Retraer")
#define MSG_MOVE_AXIS                       _UxGT("Mover Eixes")
#define MSG_MOVE_X                          _UxGT("Move X")
#define MSG_MOVE_Y                          _UxGT("Move Y")
#define MSG_MOVE_Z                          _UxGT("Move Z")
#define MSG_MOVE_E                          _UxGT("Extruder")
#define MSG_MOVE_01MM                       _UxGT("Move 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Move 1mm")
#define MSG_MOVE_10MM                       _UxGT("Move 10mm")
#define MSG_SPEED                           _UxGT("Velocidat")
#define MSG_NOZZLE                          _UxGT("Nozzle")
#define MSG_BED                             _UxGT("Base")
#define MSG_FAN_SPEED                       _UxGT("Ixoriador")
#define MSG_FLOW                            _UxGT("Fluxo")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Autotemp")
#define MSG_ON                              _UxGT("On")
#define MSG_OFF                             _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Acel")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ves-jerk")
#define MSG_VMAX                            _UxGT("Vmax")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax")
#define MSG_A_RETRACT                       _UxGT("A-retrac.")
#define MSG_XSTEPS                          _UxGT("X trangos/mm")
#define MSG_YSTEPS                          _UxGT("Y trangos/mm")
#define MSG_ZSTEPS                          _UxGT("Z trangos/mm")
#define MSG_ESTEPS                          _UxGT("E trangos/mm")
#define MSG_E1STEPS                         _UxGT("E1 trangos/mm")
#define MSG_E2STEPS                         _UxGT("E2 trangos/mm")
#define MSG_E3STEPS                         _UxGT("E3 trangos/mm")
#define MSG_E4STEPS                         _UxGT("E4 trangos/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movimiento")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("Contrast")
#define MSG_STORE_EPROM                     _UxGT("Alzar Memoria")
#define MSG_LOAD_EPROM                      _UxGT("Cargar Memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Rest. d'emerchen.")
#define MSG_REFRESH                         _UxGT("Tornar a cargar")
#define MSG_WATCH                           _UxGT("Monitorizar")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Achustar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausar impresion")
#define MSG_RESUME_PRINT                    _UxGT("Contin. impresion")
#define MSG_STOP_PRINT                      _UxGT("Detener Impresion")
#define MSG_CARD_MENU                       _UxGT("Menu de SD")
#define MSG_NO_CARD                         _UxGT("No i hai tarcheta")
#define MSG_DWELL                           _UxGT("Reposo...")
#define MSG_USERWAIT                        _UxGT("Asperan. ordines")
#define MSG_RESUMING                        _UxGT("Contin. impresion")
#define MSG_PRINT_ABORTED                   _UxGT("Print aborted")
#define MSG_NO_MOVE                         _UxGT("Sin movimiento")
#define MSG_KILLED                          _UxGT("ATURADA D'EMERCH.")
#define MSG_STOPPED                         _UxGT("ATURADA.")
#define MSG_CONTROL_RETRACT                 _UxGT("Retraer mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Retraer mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retraer  F")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Devantar mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("DesRet +mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Swap DesRet +mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("DesRet F")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Cambear")
#define MSG_INIT_SDCARD                     _UxGT("Encetan. tarcheta")
#define MSG_CNG_SDCARD                      _UxGT("Cambiar tarcheta")
#define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Calibration")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrate X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrate Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrate Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrate Center")

#endif // LANGUAGE_AN_H

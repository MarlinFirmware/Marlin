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
 * Catalan
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_CA_H
#define LANGUAGE_CA_H

#define MAPPER_C2C3  // because of "ó"
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" preparada.")
#define MSG_SD_INSERTED                     _UxGT("SD detectada.")
#define MSG_SD_REMOVED                      _UxGT("SD expulsada.")
#define MSG_MAIN                            _UxGT("Menu principal")
#define MSG_AUTOSTART                       _UxGT("Inici automatic")
#define MSG_DISABLE_STEPPERS                _UxGT("Apagar motors")
#define MSG_AUTO_HOME                       _UxGT("Home global")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Cancel")
#define MSG_SET_HOME_OFFSETS                _UxGT("Set home offsets")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#define MSG_SET_ORIGIN                      _UxGT("Establir origen")
#define MSG_PREHEAT_1                       _UxGT("Preescalfar PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Preescalfar PLA ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Preesc. tot PLA")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Preesc. llit PLA")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Configuració PLA")
#define MSG_PREHEAT_2                       _UxGT("Preescalfar ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Preescalfar ABS ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Preesc. tot ABS")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Preesc. llit ABS")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Configuració ABS")
#define MSG_COOLDOWN                        _UxGT("Refredar")
#define MSG_SWITCH_PS_ON                    _UxGT("Switch power on")
#define MSG_SWITCH_PS_OFF                   _UxGT("Switch power off")
#define MSG_EXTRUDE                         _UxGT("Extruir")
#define MSG_RETRACT                         _UxGT("Refredar")
#define MSG_MOVE_AXIS                       _UxGT("Moure eixos")
#define MSG_MOVE_X                          _UxGT("Moure X")
#define MSG_MOVE_Y                          _UxGT("Moure Y")
#define MSG_MOVE_Z                          _UxGT("Moure Z")
#define MSG_MOVE_E                          _UxGT("Extrusor")
#define MSG_MOVE_01MM                       _UxGT("Moure 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Moure 1mm")
#define MSG_MOVE_10MM                       _UxGT("Moure 10mm")
#define MSG_SPEED                           _UxGT("Velocitat")
#define MSG_NOZZLE                          _UxGT("Nozzle")
#define MSG_BED                             _UxGT("Llit")
#define MSG_FAN_SPEED                       _UxGT("Vel. Ventilador")
#define MSG_FLOW                            _UxGT("Fluxe")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Fact")
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
#define MSG_XSTEPS                          _UxGT("Xpassos/mm")
#define MSG_YSTEPS                          _UxGT("Ypassos/mm")
#define MSG_ZSTEPS                          _UxGT("Zpassos/mm")
#define MSG_ESTEPS                          _UxGT("Epassos/mm")
#define MSG_E1STEPS                         _UxGT("E1passos/mm")
#define MSG_E2STEPS                         _UxGT("E2passos/mm")
#define MSG_E3STEPS                         _UxGT("E3passos/mm")
#define MSG_E4STEPS                         _UxGT("E4passos/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Moviment")
#define MSG_VOLUMETRIC                      _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_CONTRAST                        _UxGT("Contrast de LCD")
#define MSG_STORE_EPROM                     _UxGT("Desar a memoria")
#define MSG_LOAD_EPROM                      _UxGT("Carregar de mem.")
#define MSG_RESTORE_FAILSAFE                _UxGT("Rest. emergencia")
#define MSG_REFRESH                         _UxGT("Refrescar")
#define MSG_WATCH                           _UxGT("Pantalla Info.")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Calibrar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausa imp.")
#define MSG_RESUME_PRINT                    _UxGT("Reprendre imp.")
#define MSG_STOP_PRINT                      _UxGT("Parar inp.")
#define MSG_CARD_MENU                       _UxGT("Imprimir de SD")
#define MSG_NO_CARD                         _UxGT("-Sense targeta SD")
#define MSG_DWELL                           _UxGT("Repos...")
#define MSG_USERWAIT                        _UxGT("Esperant usuari..")
#define MSG_RESUMING                        _UxGT("Reprenent imp.")
#define MSG_PRINT_ABORTED                   _UxGT("Print aborted")
#define MSG_NO_MOVE                         _UxGT("Sense moviment.")
#define MSG_KILLED                          _UxGT("PARADA DE EMERG. ")
#define MSG_STOPPED                         _UxGT("ATURAT. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retreure mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Retreure mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retreure  F")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Aixecar mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Swap DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("DesRet  F")
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Canviar filament")
#define MSG_INIT_SDCARD                     _UxGT("Iniciant SD")
#define MSG_CNG_SDCARD                      _UxGT("Canviar SD")
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

#endif // LANGUAGE_CA_H

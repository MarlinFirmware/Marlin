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
 * Portuguese
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" pronta.")
#define MSG_MEDIA_INSERTED                  _UxGT("Cartão inserido")
#define MSG_MEDIA_REMOVED                   _UxGT("Cartão removido")
#define MSG_MAIN                            _UxGT("Menu principal")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Desactivar motores")
#define MSG_AUTO_HOME                       _UxGT("Ir para origem")
#define MSG_AUTO_HOME_X                     _UxGT("Ir para origem X")
#define MSG_AUTO_HOME_Y                     _UxGT("Ir para origem Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Ir para origem Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Indo para origem")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click para iniciar")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Próximo ponto")
#define MSG_LEVEL_BED_DONE                  _UxGT("Pronto !")
#define MSG_SET_HOME_OFFSETS                _UxGT("Definir desvio")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets aplicados")
#define MSG_SET_ORIGIN                      _UxGT("Definir origem")
#define MSG_PREHEAT_1                       _UxGT("Pre-aquecer " PREHEAT_1_LABEL)
#define MSG_PREHEAT_1_N                     _UxGT("Pre-aquecer " PREHEAT_1_LABEL)
#define MSG_PREHEAT_1_ALL                   _UxGT("Pre-aq. " PREHEAT_1_LABEL " Tudo")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Pre-aq. " PREHEAT_1_LABEL " ") LCD_STR_THERMOMETER _UxGT("Base")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Definições " PREHEAT_1_LABEL)
#define MSG_PREHEAT_2                       _UxGT("Pre-aquecer " PREHEAT_2_LABEL)
#define MSG_PREHEAT_2_N                     _UxGT("Pre-aquecer " PREHEAT_2_LABEL " ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Pre-aq. " PREHEAT_2_LABEL " Tudo")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Pre-aq. " PREHEAT_2_LABEL " ") LCD_STR_THERMOMETER _UxGT("Base")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Definições " PREHEAT_2_LABEL)
#define MSG_COOLDOWN                        _UxGT("Arrefecer")
#define MSG_SWITCH_PS_ON                    _UxGT("Ligar")
#define MSG_SWITCH_PS_OFF                   _UxGT("Desligar")
#define MSG_EXTRUDE                         _UxGT("Extrudir")
#define MSG_RETRACT                         _UxGT("Retrair")
#define MSG_MOVE_AXIS                       _UxGT("Mover eixo")
#define MSG_MOVE_X                          _UxGT("Mover X")
#define MSG_MOVE_Y                          _UxGT("Mover Y")
#define MSG_MOVE_Z                          _UxGT("Mover Z")
#define MSG_MOVE_E                          _UxGT("Mover Extrusor")
#define MSG_MOVE_Z_DIST                     _UxGT("Mover %smm")
#define MSG_MOVE_01MM                       _UxGT("Mover 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mover 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mover 10mm")
#define MSG_SPEED                           _UxGT("Velocidade")
#define MSG_BED_Z                           _UxGT("Base Z")
#define MSG_NOZZLE                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Bico")
#define MSG_BED                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Base")
#define MSG_FAN_SPEED                       _UxGT("Vel. ventoinha")
#define MSG_FLOW                            _UxGT("Fluxo")
#define MSG_CONTROL                         _UxGT("Controlo")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Temp. Automática")
#define MSG_LCD_ON                          _UxGT("On")
#define MSG_LCD_OFF                         _UxGT("Off")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Acc")
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
#define MSG_VMAX                            _UxGT(" Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-retracção")
#define MSG_A_TRAVEL                        _UxGT("A-movimento")
#define MSG_STEPS_PER_MM                    _UxGT("Passo/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("A passo/mm")
  #define MSG_BSTEPS                        _UxGT("B passo/mm")
  #define MSG_CSTEPS                        _UxGT("C passo/mm")
#else
  #define MSG_ASTEPS                        _UxGT("X passo/mm")
  #define MSG_BSTEPS                        _UxGT("Y passo/mm")
  #define MSG_CSTEPS                        _UxGT("Z passo/mm")
#endif
#define MSG_ESTEPS                          _UxGT("E passo/mm")
#define MSG_E1STEPS                         _UxGT("E1 passo/mm")
#define MSG_E2STEPS                         _UxGT("E2 passo/mm")
#define MSG_E3STEPS                         _UxGT("E3 passo/mm")
#define MSG_E4STEPS                         _UxGT("E4 passo/mm")
#define MSG_E5STEPS                         _UxGT("E5 passo/mm")
#define MSG_E6STEPS                         _UxGT("E6 passo/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movimento")
#define MSG_FILAMENT                        _UxGT("Filamento")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E em mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Diam.")
#define MSG_CONTRAST                        _UxGT("Contraste")
#define MSG_STORE_EEPROM                    _UxGT("Guardar na memoria")
#define MSG_LOAD_EEPROM                     _UxGT("Carregar da memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Rest. de emergen.")
#define MSG_REFRESH                         LCD_STR_REFRESH _UxGT(" Recarregar")
#define MSG_WATCH                           _UxGT("Monitorizar")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Afinar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausar impressão")
#define MSG_RESUME_PRINT                    _UxGT("Retomar impressão")
#define MSG_STOP_PRINT                      _UxGT("Parar impressão")
#define MSG_MEDIA_MENU                      _UxGT("Imprimir do SD")
#define MSG_NO_MEDIA                        _UxGT("Sem cartão SD")
#define MSG_DWELL                           _UxGT("Em espera...")
#define MSG_USERWAIT                        _UxGT("Á espera de ordem")
#define MSG_PRINT_ABORTED                   _UxGT("Impressão cancelada")
#define MSG_NO_MOVE                         _UxGT("Sem movimento")
#define MSG_KILLED                          _UxGT("EMERGÊNCIA. ")
#define MSG_STOPPED                         _UxGT("PARADO. ")
#define MSG_CONTROL_RETRACT                 _UxGT(" Retrair mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Troca Retrair mm")
#define MSG_CONTROL_RETRACTF                _UxGT(" Retrair  V")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT(" Levantar mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT(" DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Troca DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT(" DesRet  V")
#define MSG_AUTORETRACT                     _UxGT(" AutoRetr.")
#define MSG_FILAMENTCHANGE                  _UxGT("Trocar filamento")
#define MSG_INIT_MEDIA                      _UxGT("Inici. cartão SD")
#define MSG_CHANGE_MEDIA                    _UxGT("Trocar cartão SD")
#define MSG_ZPROBE_OUT                      _UxGT("Sensor fora/base")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Desvio Z")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Fim de curso")
#define MSG_HEATING_FAILED_LCD              _UxGT("Aquecimento falhou")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: T Máxima")
#define MSG_ERR_MINTEMP                     _UxGT("Err: T Mínima")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: T Base Máxima")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: T Base Mínima")
#define MSG_HEATING                         _UxGT("Aquecendo...")
#define MSG_BED_HEATING                     _UxGT("Aquecendo base...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibração Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrar X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrar Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrar Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrar Centro")

#define MSG_LCD_ENDSTOPS                    _UxGT("Fim de curso")

#define MSG_EXPECTED_PRINTER                _UxGT("Impressora Incorreta")

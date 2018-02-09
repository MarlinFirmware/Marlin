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
 * Portuguese (Brazil)
 * UTF-8 for Graphical Display
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_PT_BR_UTF_H
#define LANGUAGE_PT_BR_UTF_H

#define DISPLAY_CHARSET_ISO10646_1
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" pronto.")
#define MSG_SD_INSERTED                     _UxGT("Cartão inserido")
#define MSG_SD_REMOVED                      _UxGT("Cartão removido")
#define MSG_MAIN                            _UxGT("Menu principal")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Desabi. motores")
#define MSG_AUTO_HOME                       _UxGT("Ir para origen")
#define MSG_LEVEL_BED_HOMING                _UxGT("Indo para origem")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Ajustar Jogo")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#define MSG_SET_ORIGIN                      _UxGT("Ajustar orig.")
#define MSG_PREHEAT_1                       _UxGT("Pre-aquecer PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Pre-aquecer PLA")
#define MSG_PREHEAT_1_ALL                   _UxGT("Pre-aq.Todo PLA")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Pre-aq. PLA ") LCD_STR_THERMOMETER _UxGT("Base")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Ajustar PLA")
#define MSG_PREHEAT_2                       _UxGT("Pre-aquecer ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Pre-aquecer ABS")
#define MSG_PREHEAT_2_ALL                   _UxGT("Pre-aq.Todo ABS")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Pre-aq. ABS ") LCD_STR_THERMOMETER _UxGT("Base")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Ajustar ABS")
#define MSG_COOLDOWN                        _UxGT("Esfriar")
#define MSG_SWITCH_PS_ON                    _UxGT("Ligar")
#define MSG_SWITCH_PS_OFF                   _UxGT("Desligar")
#define MSG_EXTRUDE                         _UxGT("Extrudar")
#define MSG_RETRACT                         _UxGT("Retrair")
#define MSG_MOVE_AXIS                       _UxGT("Mover eixo")
#define MSG_MOVE_X                          _UxGT("Mover X")
#define MSG_MOVE_Y                          _UxGT("Mover Y")
#define MSG_MOVE_Z                          _UxGT("Mover Z")
#define MSG_MOVE_E                          _UxGT("Mover Extrusor")
#define MSG_MOVE_01MM                       _UxGT("Mover 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mover 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mover 10mm")
#define MSG_SPEED                           _UxGT("Velocidade")
#define MSG_BED_Z                           _UxGT("Base Z")
#define MSG_NOZZLE                          LCD_STR_THERMOMETER _UxGT(" Bocal")
#define MSG_BED                             LCD_STR_THERMOMETER _UxGT(" Base")
#define MSG_FAN_SPEED                       _UxGT("Vel. Ventoinha")
#define MSG_FLOW                            _UxGT("Fluxo")
#define MSG_CONTROL                         _UxGT("Controle")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Temp. Automática")
#define MSG_ON                              _UxGT("Ligado ")
#define MSG_OFF                             _UxGT("Desligado")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Acc")
#define MSG_JERK                            _UxGT("Jogo")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("jogo VA")
  #define MSG_VB_JERK                       _UxGT("jogo VB")
  #define MSG_VC_JERK                       _UxGT("jogo VC")
#else
  #define MSG_VA_JERK                       _UxGT("jogo VX")
  #define MSG_VB_JERK                       _UxGT("jogo VY")
  #define MSG_VC_JERK                       _UxGT("jogo VZ")
#endif
#define MSG_VE_JERK                         _UxGT("jogo VE")
#define MSG_VMAX                            _UxGT(" Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("Retrair A")
#define MSG_A_TRAVEL                        _UxGT("A-movimento")
#define MSG_STEPS_PER_MM                    _UxGT("Passo/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("A/mm")
  #define MSG_BSTEPS                        _UxGT("B/mm")
  #define MSG_CSTEPS                        _UxGT("C/mm")
#else
  #define MSG_ASTEPS                        _UxGT("X/mm")
  #define MSG_BSTEPS                        _UxGT("Y/mm")
  #define MSG_CSTEPS                        _UxGT("Z/mm")
#endif
#define MSG_ESTEPS                          _UxGT("E/mm")
#define MSG_E1STEPS                         _UxGT("E1/mm")
#define MSG_E2STEPS                         _UxGT("E2/mm")
#define MSG_E3STEPS                         _UxGT("E3/mm")
#define MSG_E4STEPS                         _UxGT("E4/mm")
#define MSG_E5STEPS                         _UxGT("E5/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movimento")
#define MSG_FILAMENT                        _UxGT("Filamento")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("Extr. em mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Diametro Fil.")
#define MSG_CONTRAST                        _UxGT("Contraste")
#define MSG_STORE_EEPROM                    _UxGT("Salvar")
#define MSG_LOAD_EEPROM                     _UxGT("Ler")
#define MSG_RESTORE_FAILSAFE                _UxGT("Rest. de emerg.")
#define MSG_REFRESH                         LCD_STR_REFRESH _UxGT(" Restaurar")
#define MSG_WATCH                           _UxGT("Monitorar")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Afinar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausar impressão")
#define MSG_RESUME_PRINT                    _UxGT("Resumir impressão")
#define MSG_STOP_PRINT                      _UxGT("Parar impressão")
#define MSG_CARD_MENU                       _UxGT("Imprimir do SD")
#define MSG_NO_CARD                         _UxGT("Sem cartão SD")
#define MSG_DWELL                           _UxGT("Repouso...")
#define MSG_USERWAIT                        _UxGT("Esperando ordem")
#define MSG_RESUMING                        _UxGT("Resumindo Impres.")
#define MSG_PRINT_ABORTED                   _UxGT("Impres. Abortada.")
#define MSG_NO_MOVE                         _UxGT("Sem movimento")
#define MSG_KILLED                          _UxGT("PARADA DE EMERG.")
#define MSG_STOPPED                         _UxGT("PARADA. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Retrair mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Retrair Troca mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retrair  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Levantar mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Des Retrair mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Des RetTroca mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Des Retrair  V")
#define MSG_AUTORETRACT                     _UxGT("Retração Autom.")
#define MSG_FILAMENTCHANGE                  _UxGT("Trocar Filamento")
#define MSG_INIT_SDCARD                     _UxGT("Iniciar SD")
#define MSG_CNG_SDCARD                      _UxGT("Trocar SD")
#define MSG_ZPROBE_OUT                      _UxGT("Son. fora da mesa")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Deslocamento no Z")
#define MSG_BABYSTEP_X                      _UxGT("Passinho X")
#define MSG_BABYSTEP_Y                      _UxGT("Passinho Y")
#define MSG_BABYSTEP_Z                      _UxGT("Passinho Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Fim de Curso")
#define MSG_HEATING_FAILED_LCD              _UxGT("Aquecimento falhou")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: T Máxima")
#define MSG_ERR_MINTEMP                     _UxGT("Err: T Mínima")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: T Base Máxima")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: T Base Mínima")
#define MSG_HEATING                         _UxGT("Aquecendo...")
#define MSG_HEATING_COMPLETE                _UxGT("Aquecida.")
#define MSG_BED_HEATING                     _UxGT("Aquecendo base..")
#define MSG_BED_DONE                        _UxGT("Base aquecida.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibrar Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrar X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrar Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrar Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrar Centro")

#endif // LANGUAGE_PT_BR_UTF_H

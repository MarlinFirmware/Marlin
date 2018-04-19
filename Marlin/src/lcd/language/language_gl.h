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
 * Galician language (ISO "gl")
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_GL_H
#define LANGUAGE_GL_H

#define DISPLAY_CHARSET_ISO10646_1
#define NOT_EXTENDED_ISO10646_1_5X7

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" lista.")
#define MSG_SD_INSERTED                     _UxGT("Tarxeta inserida")
#define MSG_SD_REMOVED                      _UxGT("Tarxeta retirada")
#define MSG_LCD_ENDSTOPS                    _UxGT("FinCarro")
#define MSG_MAIN                            _UxGT("Menu principal")
#define MSG_AUTOSTART                       _UxGT("Autoarranque")
#define MSG_DISABLE_STEPPERS                _UxGT("Apagar motores")
#define MSG_AUTO_HOME                       _UxGT("Ir a orixe")
#define MSG_AUTO_HOME_X                     _UxGT("Ir orixe X")
#define MSG_AUTO_HOME_Y                     _UxGT("Ir orixe Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Ir orixe Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Ir orixes XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Prema pulsador")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Seguinte punto")
#define MSG_LEVEL_BED_DONE                  _UxGT("Nivelado feito")
#define MSG_SET_HOME_OFFSETS                _UxGT("Offsets na orixe")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets fixados")
#define MSG_SET_ORIGIN                      _UxGT("Fixar orixe")
#define MSG_PREHEAT_1                       _UxGT("Prequentar PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Prequentar PLA ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Preque. PLA Todo")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Preque. PLA Cama")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Preque. PLA conf")
#define MSG_PREHEAT_2                       _UxGT("Prequentar ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Prequentar ABS ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Preque. ABS Todo")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Preque. ABS Cama")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Preque. ABS conf")
#define MSG_COOLDOWN                        _UxGT("Arrefriar")
#define MSG_SWITCH_PS_ON                    _UxGT("Acender")
#define MSG_SWITCH_PS_OFF                   _UxGT("Apagar")
#define MSG_EXTRUDE                         _UxGT("Extrudir")
#define MSG_RETRACT                         _UxGT("Retraer")
#define MSG_MOVE_AXIS                       _UxGT("Mover eixe")
#define MSG_BED_LEVELING                    _UxGT("Nivelar cama")
#define MSG_LEVEL_BED                       _UxGT("Nivelar cama")
#define MSG_MOVE_X                          _UxGT("Mover X")
#define MSG_MOVE_Y                          _UxGT("Mover Y")
#define MSG_MOVE_Z                          _UxGT("Mover Z")
#define MSG_MOVE_E                          _UxGT("Extrusor")
#define MSG_MOVE_01MM                       _UxGT("Mover 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mover 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mover 10mm")
#define MSG_SPEED                           _UxGT("Velocidade")
#define MSG_BED_Z                           _UxGT("Cama Z")
#define MSG_NOZZLE                          _UxGT("Bico")
#define MSG_BED                             _UxGT("Cama")
#define MSG_FAN_SPEED                       _UxGT("Velocidade vent.")
#define MSG_FLOW                            _UxGT("Fluxo")
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
#define MSG_SELECT                          _UxGT("Escolla")
#define MSG_ACC                             _UxGT("Acel")
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
#define MSG_STEPS_PER_MM                    _UxGT("Pasos/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("A pasos/mm")
  #define MSG_BSTEPS                        _UxGT("B pasos/mm")
  #define MSG_CSTEPS                        _UxGT("C pasos/mm")
#else
  #define MSG_ASTEPS                        _UxGT("X pasos/mm")
  #define MSG_BSTEPS                        _UxGT("Y pasos/mm")
  #define MSG_CSTEPS                        _UxGT("Z pasos/mm")
#endif
#define MSG_ESTEPS                          _UxGT("E pasos/mm")
#define MSG_E1STEPS                         _UxGT("E1 pasos/mm")
#define MSG_E2STEPS                         _UxGT("E2 pasos/mm")
#define MSG_E3STEPS                         _UxGT("E3 pasos/mm")
#define MSG_E4STEPS                         _UxGT("E4 pasos/mm")
#define MSG_E5STEPS                         _UxGT("E5 pasos/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movemento")
#define MSG_FILAMENT                        _UxGT("Filamento")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E en mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Diam. fil.")
#define MSG_CONTRAST                        _UxGT("Constraste LCD")
#define MSG_STORE_EEPROM                    _UxGT("Gardar en memo.")
#define MSG_LOAD_EEPROM                     _UxGT("Cargar de memo.")
#define MSG_RESTORE_FAILSAFE                _UxGT("Cargar de firm.")
#define MSG_REFRESH                         _UxGT("Volver a cargar")
#define MSG_WATCH                           _UxGT("Monitorizacion")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Axustar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausar impres.")
#define MSG_RESUME_PRINT                    _UxGT("Seguir impres.")
#define MSG_STOP_PRINT                      _UxGT("Deter impres.")
#define MSG_CARD_MENU                       _UxGT("Tarxeta SD")
#define MSG_NO_CARD                         _UxGT("Sen tarxeta SD")
#define MSG_DWELL                           _UxGT("En repouso...")
#define MSG_USERWAIT                        _UxGT("A espera...")
#define MSG_RESUMING                        _UxGT("Imprimindo...")
#define MSG_PRINT_ABORTED                   _UxGT("Impre. cancelada")
#define MSG_NO_MOVE                         _UxGT("Sen movemento.")
#define MSG_KILLED                          _UxGT("PROGRAMA MORTO")
#define MSG_STOPPED                         _UxGT("PROGRAMA PARADO")
#define MSG_CONTROL_RETRACT                 _UxGT("Retraccion mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Cambio retra. mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retraccion V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Alzar Z mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Recup. retra. mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Cambio recup. mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Recuperacion V")
#define MSG_AUTORETRACT                     _UxGT("Retraccion auto.")
#define MSG_FILAMENTCHANGE                  _UxGT("Cambiar filamen.")
#define MSG_INIT_SDCARD                     _UxGT("Iniciando SD")
#define MSG_CNG_SDCARD                      _UxGT("Cambiar SD")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda-Z sen cama")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_BLTOUCH_SELFTEST                _UxGT("Comprobar BLTouch")
#define MSG_BLTOUCH_RESET                   _UxGT("Iniciar BLTouch")
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Offset Z")
#define MSG_BABYSTEP_X                      _UxGT("Micropaso X")
#define MSG_BABYSTEP_Y                      _UxGT("Micropaso Y")
#define MSG_BABYSTEP_Z                      _UxGT("Micropaso Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Erro fin carro")
#define MSG_HEATING_FAILED_LCD              _UxGT("Fallo quentando")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Erro temperatura")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Temp. excesiva")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: temp. max.")
#define MSG_ERR_MINTEMP                     _UxGT("Err: temp. min.")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: MAXTEMP BED")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: MINTEMP BED")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("SISTEMA MORTO")
#define MSG_PLEASE_RESET                    _UxGT("Debe reiniciar!")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Quentando...")
#define MSG_HEATING_COMPLETE                _UxGT("Xa esta quente")
#define MSG_BED_HEATING                     _UxGT("Quentando cama")
#define MSG_BED_DONE                        _UxGT("Cama esta quente")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibracion Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrar X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrar Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrar Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrar Centro")
#define MSG_INFO_MENU                       _UxGT("Acerca de...")
#define MSG_INFO_PRINTER_MENU               _UxGT("Informacion")
#define MSG_INFO_STATS_MENU                 _UxGT("Estadisticas")
#define MSG_INFO_BOARD_MENU                 _UxGT("Placa nai")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistores")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrusores")
#define MSG_INFO_BAUDRATE                   _UxGT("Baudios")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocolo")
#define MSG_CASE_LIGHT                      _UxGT("Luz")
#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Total traballos")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Total completos")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tempo impresion")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Traballo +longo")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total extruido")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Traballos")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completos")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tempo")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("O +longo")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruido")
#endif
#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Fonte alime.")
#define MSG_DRIVE_STRENGTH                  _UxGT("Potencia motor")
#define MSG_DAC_PERCENT                     _UxGT("Motor %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Garda DAC EEPROM")

#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Segue traballo")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Agarde para")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("iniciar troco")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("de filamento")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Agarde pola")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("descarga do")
  #define MSG_FILAMENT_CHANGE_UNLOAD_3        _UxGT("filamento")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Introduza o")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("filamento e")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("faga click")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Agarde pola")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("carga do")
  #define MSG_FILAMENT_CHANGE_LOAD_3          _UxGT("filamento")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Agarde para")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("seguir co")
  #define MSG_FILAMENT_CHANGE_RESUME_3        _UxGT("traballo")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Agarde...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Descargando...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Introduza e click")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Cargando...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Seguindo...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_GL_H


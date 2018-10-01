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
 * Spanish
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_ES_UTF_H
#define LANGUAGE_ES_UTF_H

#define DISPLAY_CHARSET_ISO10646_1
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" lista.")
#define MSG_BACK                            _UxGT("Atrás")
#define MSG_SD_INSERTED                     _UxGT("Tarjeta colocada")
#define MSG_SD_REMOVED                      _UxGT("Tarjeta retirada")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menú principal")
#define MSG_AUTOSTART                       _UxGT("Inicio automático")
#define MSG_DISABLE_STEPPERS                _UxGT("Apagar motores")
#define MSG_DEBUG_MENU                      _UxGT("Menú depurar")
#define MSG_PROGRESS_BAR_TEST               _UxGT("Prueba barra avance")
#define MSG_AUTO_HOME                       _UxGT("Llevar al origen")
#define MSG_AUTO_HOME_X                     _UxGT("Origen X")
#define MSG_AUTO_HOME_Y                     _UxGT("Origen Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Origen Z")
#define MSG_TMC_Z_CALIBRATION               _UxGT("Calibrar Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Origen XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Iniciar (Presione)")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Siguiente punto")
#define MSG_LEVEL_BED_DONE                  _UxGT("Nivelación lista!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Ajustar desfases")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Desfase aplicado")
#define MSG_SET_ORIGIN                      _UxGT("Establecer origen")
#define MSG_PREHEAT_1                       _UxGT("Precalentar PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Todo")
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" End")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Cama")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Config")
#define MSG_PREHEAT_2                       _UxGT("Precalentar ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Todo")
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" End")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Cama")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Config")
#define MSG_COOLDOWN                        _UxGT("Enfriar")
#define MSG_SWITCH_PS_ON                    _UxGT("Encender")
#define MSG_SWITCH_PS_OFF                   _UxGT("Apagar")
#define MSG_EXTRUDE                         _UxGT("Extruir")
#define MSG_RETRACT                         _UxGT("Retraer")
#define MSG_MOVE_AXIS                       _UxGT("Mover ejes")
#define MSG_BED_LEVELING                    _UxGT("Nivelar cama")
#define MSG_LEVEL_BED                       _UxGT("Nivelar cama")

#define MSG_MOVING                          _UxGT("Moviendo...")
#define MSG_FREE_XY                         _UxGT("Libre XY")
#define MSG_MOVE_X                          _UxGT("Mover X")
#define MSG_MOVE_Y                          _UxGT("Mover Y")
#define MSG_MOVE_Z                          _UxGT("Mover Z")
#define MSG_MOVE_E                          _UxGT("Extrusor")
#define MSG_MOVE_01MM                       _UxGT("Mover 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mover 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mover 10mm")
#define MSG_SPEED                           _UxGT("Velocidad")
#define MSG_BED_Z                           _UxGT("Cama Z")
#define MSG_NOZZLE                          _UxGT("Boquilla")
#define MSG_BED                             _UxGT("Cama")
#define MSG_FAN_SPEED                       _UxGT("Ventilador")
#define MSG_FLOW                            _UxGT("Flujo")
#define MSG_CONTROL                         _UxGT("Control")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#define MSG_AUTOTEMP                        _UxGT("Temperatura Auto.")
#define MSG_ON                              _UxGT("Encender")
#define MSG_OFF                             _UxGT("Apagar")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Seleccionar")
#define MSG_ACC                             _UxGT("Aceleración")
#define MSG_JERK                            _UxGT("Jerk")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("Vel. viaje min")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Acel. max")
#define MSG_A_RETRACT                       _UxGT("Acel. retrac.")
#define MSG_A_TRAVEL                        _UxGT("Acel. Viaje")
#define MSG_STEPS_PER_MM                    _UxGT("Pasos/mm")
#define MSG_XSTEPS                          _UxGT("X pasos/mm")
#define MSG_YSTEPS                          _UxGT("Y pasos/mm")
#define MSG_ZSTEPS                          _UxGT("Z pasos/mm")
#define MSG_ESTEPS                          _UxGT("E pasos/mm")
#define MSG_E1STEPS                         _UxGT("E1 pasos/mm")
#define MSG_E2STEPS                         _UxGT("E2 pasos/mm")
#define MSG_E3STEPS                         _UxGT("E3 pasos/mm")
#define MSG_E4STEPS                         _UxGT("E4 pasos/mm")
#define MSG_E5STEPS                         _UxGT("E5 pasos/mm")
#define MSG_E6STEPS                         _UxGT("E6 pasos/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Movimiento")
#define MSG_FILAMENT                        _UxGT("Filamento")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#define MSG_ADVANCE_K                       _UxGT("Avance K")
#define MSG_CONTRAST                        _UxGT("Contraste")
#define MSG_STORE_EEPROM                    _UxGT("Guardar memoria")
#define MSG_LOAD_EEPROM                     _UxGT("Cargar memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Restaurar memoria")
#define MSG_REFRESH                         _UxGT("Volver a cargar")
#define MSG_WATCH                           _UxGT("Información")
#define MSG_PREPARE                         _UxGT("Preparar")
#define MSG_TUNE                            _UxGT("Ajustar")
#define MSG_PAUSE_PRINT                     _UxGT("Pausar impresión")
#define MSG_RESUME_PRINT                    _UxGT("Reanudar impresión")
#define MSG_STOP_PRINT                      _UxGT("Detener impresión")
#define MSG_CARD_MENU                       _UxGT("Menú de SD")
#define MSG_NO_CARD                         _UxGT("No hay tarjeta SD")
#define MSG_DWELL                           _UxGT("Reposo...")
#define MSG_USERWAIT                        _UxGT("Esperando órdenes")
#define MSG_PRINT_ABORTED                   _UxGT("Impresión cancelada")
#define MSG_NO_MOVE                         _UxGT("Sin movimiento")
#define MSG_KILLED                          _UxGT("Parada de emergencia")
#define MSG_STOPPED                         _UxGT("Detenida")
#define MSG_CONTROL_RETRACT                 _UxGT("Retraer mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Interc. Retraer mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retraer  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Levantar mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Interc. DesRet mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("DesRet V")
#define MSG_AUTORETRACT                     _UxGT("Retracción Auto.")
#define MSG_FILAMENTCHANGE                  _UxGT("Cambiar filamento")
#define MSG_INIT_SDCARD                     _UxGT("Iniciando tarjeta")
#define MSG_CNG_SDCARD                      _UxGT("Cambiar tarjeta")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda Z fuera")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Auto-Prueba")
#define MSG_BLTOUCH_RESET                   _UxGT("Reiniciar BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("primero")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Desfase Z")
#define MSG_BABYSTEP_X                      _UxGT("Micropaso X")
#define MSG_BABYSTEP_Y                      _UxGT("Micropaso Y")
#define MSG_BABYSTEP_Z                      _UxGT("Micropaso Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Cancelado - Endstop")
#define MSG_HEATING_FAILED_LCD              _UxGT("Error: al calentar")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Error: temperatura")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Error: temperatura")
#define MSG_ERR_MAXTEMP                     _UxGT("Error: Temp Máxima")
#define MSG_ERR_MINTEMP                     _UxGT("Error: Temp Mínima")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Error: Temp Max Cama")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Error: Temp Min Cama")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("IMPRESORA PARADA")
#define MSG_PLEASE_RESET                    _UxGT("Por favor, reinicie")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Calentando...")
#define MSG_BED_HEATING                     _UxGT("Calentando Cama...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibración Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrar X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrar Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrar Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrar Centro")
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto Calibración")
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Est. Altura Delta")
#define MSG_INFO_MENU                       _UxGT("Inf. Impresora")
#define MSG_INFO_PRINTER_MENU               _UxGT("Inf. Impresora")
#define MSG_INFO_STATS_MENU                 _UxGT("Estadísticas Imp.")
#define MSG_INFO_BOARD_MENU                 _UxGT("Inf. Controlador")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistores")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrusores")
#define MSG_INFO_BAUDRATE                   _UxGT("Baudios")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocolo")
#define MSG_CASE_LIGHT                      _UxGT("Luz cabina")

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              _UxGT("Conteo de impresión")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completadas")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tiempo total de imp.")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Impresión más larga")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total de Extrusión")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Impresiones")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completadas")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Más larga")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrusión")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Temperatura mínima")
#define MSG_INFO_MAX_TEMP                   _UxGT("Temperatura máxima")
#define MSG_INFO_PSU                        _UxGT("Fuente de poder")
#define MSG_DRIVE_STRENGTH                  _UxGT("Potencia driver")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Escribe DAC EEPROM")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("IMPR. PAUSADA")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("OPC. REINICIO:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Extruir más")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Resumir imp.")

#define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Temp Mínima es ")
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Boquilla: ")

#define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Esperando iniciar")

#define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Inserte filamento")
#define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("y presione el botón")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_2        _UxGT("del filamento")
  #define MSG_FILAMENT_CHANGE_INIT_3        _UxGT("cambiar")
  #define MSG_FILAMENT_CHANGE_INSERT_3      _UxGT("para continuar...")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_2        _UxGT("del fil. cambiar")
  #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("Inserte filamento")
#endif // LCD_HEIGHT < 4

#define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Esperado por")
#define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filamento expulsado")
#define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Esperado por")
#define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("Cargar filamento")
#define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Esperado por")
#define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("Extruir filamento")
#define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Esperando imp.")
#define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("para resumir")
#define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Oprima botón para")
#define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("Calentar la boquilla")
#define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Calentando boquilla")
#define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Espere por favor")

#endif // LANGUAGE_ES_UTF_H

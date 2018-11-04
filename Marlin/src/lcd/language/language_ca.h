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
#pragma once

/**
 * Catalan
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" preparada.")
#define MSG_SD_INSERTED                     _UxGT("Targeta detectada.")
#define MSG_SD_REMOVED                      _UxGT("Targeta extreta.")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops")
#define MSG_MAIN                            _UxGT("Menú principal")
#define MSG_AUTOSTART                       _UxGT("Inici automatic")
#define MSG_DISABLE_STEPPERS                _UxGT("Desactiva motors")
#define MSG_DEBUG_MENU                      _UxGT("Menu de depuracio")
#define MSG_PROGRESS_BAR_TEST               _UxGT("Test barra progres")
#define MSG_AUTO_HOME                       _UxGT("Ves a l'origen")
#define MSG_AUTO_HOME_X                     _UxGT("X a origen")
#define MSG_AUTO_HOME_Y                     _UxGT("Y a origen")
#define MSG_AUTO_HOME_Z                     _UxGT("Z a origen")
#define MSG_TMC_Z_CALIBRATION               _UxGT("Calibra Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Origen XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Premeu per iniciar")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Següent punt")
#define MSG_LEVEL_BED_DONE                  _UxGT("Anivellament fet!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Ajusta decalatge")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Decalatge aplicat")
#define MSG_SET_ORIGIN                      _UxGT("Estableix origen")
#define MSG_PREHEAT_1                       _UxGT("Preescalfa " PREHEAT_1_LABEL)
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Tot")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Llit")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Conf.")
#define MSG_PREHEAT_2                       _UxGT("Preescalfa " PREHEAT_2_LABEL)
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Tot")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Llit")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Conf.")
#define MSG_COOLDOWN                        _UxGT("Refreda")
#define MSG_SWITCH_PS_ON                    _UxGT("Switch power on")
#define MSG_SWITCH_PS_OFF                   _UxGT("Switch power off")
#define MSG_EXTRUDE                         _UxGT("Extrudeix")
#define MSG_RETRACT                         _UxGT("Retreu")
#define MSG_MOVE_AXIS                       _UxGT("Mou eixos")
#define MSG_BED_LEVELING                    _UxGT("Anivella llit")
#define MSG_LEVEL_BED                       _UxGT("Anivella llit")

#define MSG_MOVING                          _UxGT("Movent..")
#define MSG_FREE_XY                         _UxGT("XY lliures")
#define MSG_MOVE_X                          _UxGT("Mou X")
#define MSG_MOVE_Y                          _UxGT("Mou Y")
#define MSG_MOVE_Z                          _UxGT("Mou Z")
#define MSG_MOVE_E                          _UxGT("Extrusor")
#define MSG_MOVE_01MM                       _UxGT("Mou 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Mou 1mm")
#define MSG_MOVE_10MM                       _UxGT("Mou 10mm")
#define MSG_SPEED                           _UxGT("Velocitat")
#define MSG_BED_Z                           _UxGT("Llit Z")
#define MSG_NOZZLE                          _UxGT("Nozzle")
#define MSG_BED                             _UxGT("Llit")
#define MSG_FAN_SPEED                       _UxGT("Vel. Ventilador")
#define MSG_FLOW                            _UxGT("Flux")
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
#define MSG_SELECT                          _UxGT("Select")
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
#define MSG_VTRAV_MIN                       _UxGT("VViatge min")
#define MSG_AMAX                            _UxGT("Accel. max ")
#define MSG_A_RETRACT                       _UxGT("Accel. retracc")
#define MSG_A_TRAVEL                        _UxGT("Accel. Viatge")
#define MSG_STEPS_PER_MM                    _UxGT("Passos/mm")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("Apassos/mm")
  #define MSG_BSTEPS                        _UxGT("Bpassos/mm")
  #define MSG_CSTEPS                        _UxGT("Cpassos/mm")
#else
  #define MSG_ASTEPS                        _UxGT("Xpassos/mm")
  #define MSG_BSTEPS                        _UxGT("Ypassos/mm")
  #define MSG_CSTEPS                        _UxGT("Zpassos/mm")
#endif
#define MSG_ESTEPS                          _UxGT("Epassos/mm")
#define MSG_E1STEPS                         _UxGT("E1passos/mm")
#define MSG_E2STEPS                         _UxGT("E2passos/mm")
#define MSG_E3STEPS                         _UxGT("E3passos/mm")
#define MSG_E4STEPS                         _UxGT("E4passos/mm")
#define MSG_E5STEPS                         _UxGT("E5passos/mm")
#define MSG_E6STEPS                         _UxGT("E6passos/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Moviment")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E en mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Diam. Fil.")
#define MSG_CONTRAST                        _UxGT("Contrast de LCD")
#define MSG_STORE_EEPROM                    _UxGT("Desa memoria")
#define MSG_LOAD_EEPROM                     _UxGT("Carrega memoria")
#define MSG_RESTORE_FAILSAFE                _UxGT("Restaura valors")
#define MSG_REFRESH                         _UxGT("Actualitza")
#define MSG_WATCH                           _UxGT("Pantalla Info.")
#define MSG_PREPARE                         _UxGT("Prepara")
#define MSG_TUNE                            _UxGT("Ajusta")
#define MSG_PAUSE_PRINT                     _UxGT("Pausa impressio")
#define MSG_RESUME_PRINT                    _UxGT("Repren impressio")
#define MSG_STOP_PRINT                      _UxGT("Atura impressio.")
#define MSG_CARD_MENU                       _UxGT("Imprimeix de SD")
#define MSG_NO_CARD                         _UxGT("No hi ha targeta")
#define MSG_DWELL                           _UxGT("En repos...")
#define MSG_USERWAIT                        _UxGT("Esperant usuari..")
#define MSG_PRINT_ABORTED                   _UxGT("Imp. cancelada")
#define MSG_NO_MOVE                         _UxGT("Sense moviment.")
#define MSG_KILLED                          _UxGT("MATAT.")
#define MSG_STOPPED                         _UxGT("ATURADA.")
#define MSG_CONTROL_RETRACT                 _UxGT("Retreu mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Retreure mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Retreu V")
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Aixeca mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("DesRet +mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Swap DesRet +mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("DesRet V")
#define MSG_AUTORETRACT                     _UxGT("Auto retraccio")
#define MSG_FILAMENTCHANGE                  _UxGT("Canvia filament")
#define MSG_INIT_SDCARD                     _UxGT("Inicialitza SD")
#define MSG_CHANGE_SDCARD                   _UxGT("Canvia SD")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda Z fora")
#define MSG_BLTOUCH_RESET                   _UxGT("Reinicia BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("primer")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Decalatge Z")
#define MSG_BABYSTEP_X                      _UxGT("Micropas X")
#define MSG_BABYSTEP_Y                      _UxGT("Micropas Y")
#define MSG_BABYSTEP_Z                      _UxGT("Micropas Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Cancel. Endstop")
#define MSG_HEATING_FAILED_LCD              _UxGT("Error al escalfar")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: TEMP REDUNDANT")
#define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: TEMP MAXIMA")
#define MSG_ERR_MINTEMP                     _UxGT("Err: TEMP MINIMA")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: TEMPMAX LLIT")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: TEMPMIN LLIT")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("IMPRESSORA PARADA")
#define MSG_PLEASE_RESET                    _UxGT("Reinicieu")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Escalfant...")
#define MSG_BED_HEATING                     _UxGT("Escalfant llit...")
#define MSG_DELTA_CALIBRATE                 _UxGT("Calibratge Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Calibra X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibra Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibra Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibra el centre")

#define MSG_INFO_MENU                       _UxGT("Quant a la impr.")
#define MSG_INFO_PRINTER_MENU               _UxGT("Info Impressora")
#define MSG_INFO_STATS_MENU                 _UxGT("Estadistiques")
#define MSG_INFO_BOARD_MENU                 _UxGT("Info placa")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrusors")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocol")
#define MSG_CASE_LIGHT                      _UxGT("Llum")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Total impressions")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Acabades")
  #define MSG_INFO_PRINT_TIME               _UxGT("Temps imprimint")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Treball mes llarg")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total extrudit")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Impressions")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Acabades")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Mes llarg")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrudit")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Temp. mínima")
#define MSG_INFO_MAX_TEMP                   _UxGT("Temp. màxima")
#define MSG_INFO_PSU                        _UxGT("Font alimentacio")

#define MSG_DRIVE_STRENGTH                  _UxGT("Força motor")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM Write")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Repren impressió")

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Esperant per")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("iniciar el canvi")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("de filament")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Esperant per")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("treure filament")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Poseu filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("i premeu el boto")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("per continuar...")
  #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Premeu boto per")
  #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("escalfar nozzle.")
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Escalfant nozzle")
  #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Espereu...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Esperant carrega")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("de filament")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Esperant per")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("reprendre")
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Espereu...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Expulsant...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insereix i prem")
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Escalfant...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Carregant...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Reprenent...")
#endif // LCD_HEIGHT < 4

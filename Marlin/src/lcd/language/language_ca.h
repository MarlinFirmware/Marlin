/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Catalan
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */
namespace Language_ca {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Catalan");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" preparada.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Targeta detectada.");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Targeta extreta.");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menú principal");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Inici automatic");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Desactiva motors");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Menu de depuracio");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Test barra progres");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Ves a l'origen");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("X a origen");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Y a origen");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Z a origen");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Origen XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Premeu per iniciar");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Següent punt");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Anivellament fet!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Ajusta decalatge");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Decalatge aplicat");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Estableix origen");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Preescalfa ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Preescalfa ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" End");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" End ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" Tot");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" Llit");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Preescalfa ") PREHEAT_1_LABEL _UxGT(" Conf.");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Preescalfa ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Preescalfa ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Preescalfa ") PREHEAT_2_LABEL _UxGT(" End");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Preescalfa ") PREHEAT_2_LABEL _UxGT(" End ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Preescalfa ") PREHEAT_2_LABEL _UxGT(" Tot");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Preescalfa ") PREHEAT_2_LABEL _UxGT(" Llit");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Preescalfa ") PREHEAT_2_LABEL _UxGT(" Conf.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Refreda");

  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudeix");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retreu");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Mou eixos");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Anivella llit");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Anivella llit");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Movent..");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("XY lliures");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mou X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mou Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mou Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extrusor");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extrusor *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mou %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mou 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mou 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mou 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocitat");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Llit Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Nozzle");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Nozzle ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Llit");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Vel. Ventilador");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Vel. Ventilador ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flux");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flux ~");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VViatge min");

  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Accel. retracc");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Accel. Viatge");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Passos/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("passos/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("passos/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("passos/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Epassos/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*passos/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Moviment");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E en mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Diam. Fil.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Diam. Fil. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Contrast de LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Desa memoria");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Carrega memoria");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Restaura valors");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Actualitza");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Pantalla Info.");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Prepara");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Ajusta");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausa impressio");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Repren impressio");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Atura impressio.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Imprimeix de SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("No hi ha targeta");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("En repos...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Esperant usuari..");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Imp. cancelada");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Sense moviment.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("MATAT.");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ATURADA.");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retreu mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Swap Retreure mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retreu V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Aixeca mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("DesRet +mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Swap DesRet +mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("DesRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Auto retraccio");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Canvia filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Canvia filament *");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Inicialitza SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Canvia SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda Z fora");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reinicia BLTouch");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s primer");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Decalatge Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Micropas X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Micropas Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Micropas Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Cancel. Endstop");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Error al escalfar");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Err: TEMP REDUNDANT");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("THERMAL RUNAWAY");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err: TEMP MAXIMA");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err: TEMP MINIMA");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("IMPRESSORA PARADA");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Reinicieu");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Escalfant...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Escalfant llit...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibratge Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibra X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibra Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibra Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibra el centre");

  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Quant a la impr.");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info Impressora");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Estadistiques");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Info placa");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistors");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrusors");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baud");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocol");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Llum");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Total impressions");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Acabades");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Temps imprimint");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Treball mes llarg");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Total extrudit");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Impressions");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Acabades");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Mes llarg");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extrudit");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Temp. mínima");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Temp. màxima");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Font alimentacio");

  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Força motor");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Repren impressió");

  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Impressora incorrecta");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Esperant per", "iniciar el canvi", "de filament"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Esperant per", "treure filament"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Poseu filament", "i premeu el boto", "per continuar..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Premeu boto per", "escalfar nozzle."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Escalfant nozzle", "Espereu..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Esperant carrega", "de filament"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Esperant per", "reprendre"));
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Espereu..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Expulsant..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Insereix i prem"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Escalfant..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Carregant..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Reprenent..."));
  #endif // LCD_HEIGHT < 4
}

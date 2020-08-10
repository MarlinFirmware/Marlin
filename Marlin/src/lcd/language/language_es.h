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
 * Spanish
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_es {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Spanish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" lista");
  PROGMEM Language_Str MSG_YES                             = _UxGT("SI");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NO");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Atrás");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Cancelando...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("SD/USB insertado");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("SD/USB retirado");
  PROGMEM Language_Str MSG_MEDIA_RELEASED                  = _UxGT("SD/USB lanzado");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Esperando al SD/USB");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Error lectura SD/USB");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("Disp. USB retirado");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Inicio USB fallido");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Soft Endstops");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menú principal");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Ajustes avanzados");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Configuración");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Inicio automático");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Apagar motores");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Menú depuración");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Prob. barra progreso");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Llevar al origen");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Origen X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Origen Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Origen Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto alineado Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Origen XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Pulsar para comenzar");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Siguiente punto");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("¡Nivelación lista!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Compensación Altura");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Ajustar desfases");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Desfase aplicada");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Establecer origen");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Precalentar ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Precalentar ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Fin");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Fin ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Todo");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Cama");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Precalentar ") PREHEAT_1_LABEL _UxGT(" Ajuste");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Precalentar ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Precalentar ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Fin");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Fin ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Todo");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Cama");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Precalentar ") PREHEAT_2_LABEL _UxGT(" Ajuste");
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Precalen. Personali.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Enfriar");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Control Láser");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Láser Apagado");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Láser Encendido");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Potencia Láser");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Control Mandrino");
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Mandrino Apagado");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Mandrino Encendido");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Potencia Mandrino");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Invertir giro");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Encender Fuente");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Apagar Fuente");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extruir");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retraer");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Mover ejes");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Nivelando Cama");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Nivelar Cama");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Nivelar Esquinas");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Siguente Esquina");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Editor Mallado");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Editar Mallado");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Ed. Mallado parada");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Sondear Punto");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Índice X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Índice Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Valor Z");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Comandos Personaliz.");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Probar Sonda");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Punto");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Desviación");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("Modo IDEX");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Desfase Boquillas");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Aparcado");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplicar");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Copia Reflejada");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Control Total");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2ª Boquilla X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2ª Boquilla Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2ª Boquilla Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Hacer G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Herramientas UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Nivel.Cama.Uni.(UBL)");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Punto de inclinación");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Crear Mallado man.");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Colocar cuña y Medir");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Medir");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Retirar y Medir Cama");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Mover al Siguente");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Activar UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Desactivar UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Temp. Cama");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT ("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Editar Mallado");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Edit. Mallado perso.");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Ajuste fino Mallado");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Term. edici. Mallado");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Crear Mallado Perso.");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Crear Mallado");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = _UxGT("Crear Mallado (") PREHEAT_1_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = _UxGT("Crear Mallado (") PREHEAT_2_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Crear Mallado Frío");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Ajustar alt. Mallado");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Cantidad de altura");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Validar Mallado");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = _UxGT("Validar Mallado (") PREHEAT_1_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = _UxGT("Validar Mallado (") PREHEAT_2_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Vali. Mallado perso.");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 Calentando Cama");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Calent. Boquilla");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Imprimado manual...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Impri. longit. fija");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Imprimación Lista");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Cancelado");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Dejando G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Contin. Mallado cama");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Nivelando Mallado");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("Nivelando 3Puntos");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Nivel. Mallado cuad.");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Nivel de Mallado");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Puntos Laterales");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Tipo de mapa ");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Salida Mapa mallado");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Salida para el host");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Salida para CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Off Printer Backup");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Salida Info. UBL");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Cantidad de relleno");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Relleno manual");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Relleno inteligente");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Mallado de relleno");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Invalidar todo");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Invalidar proximos");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Ajustar Fino Todo");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Ajustar Fino proxi.");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Almacen de Mallado");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Huecos de memoria");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Cargar Mallado cama");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Guardar Mallado cama");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Mallado %i Cargado");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Mallado %i Guardado");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Sin guardar");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Error: Guardar UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Error: Restaurar UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Desfase de Z: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Desfase de Z Parado");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("UBL Paso a Paso");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Crear Mallado Frío");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Relleno inteligente");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Validar Mallado");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Ajustar Fino Todo");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Validar Mallado");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Ajustar Fino Todo");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Guardar Mallado cama");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Control LED");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Luzes");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Luz predefinida");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Rojo");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Naranja");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Amarillo");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Verde");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Azul");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Índigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Violeta");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Blanco");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Por defecto");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Luces personalizadas");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Intensidad Rojo");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Intensidad Verde");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Intensidad Azul");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Intensidad Blanco");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Brillo");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Moviendo...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Libre XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Mover X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Mover Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Mover Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extrusor");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extrusor *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Hotend muy frio");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Mover %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Mover 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Mover 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Mover 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocidad");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Cama Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Boquilla");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Boquilla ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Cama");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Recinto");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Ventilador");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Ventilador ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Vel. Ext. ventilador");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Vel. Ext. ventilador ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flujo");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flujo ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Control");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Temperatura Auto.");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Encender");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Apagar");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Seleccionar");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Seleccionar *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Aceleración");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Velocidad");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vel. viaje min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Accel");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Acel. max") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Acel. max") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Acel. max") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Acel. max") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Acel. max *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Acel. retrac.");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Acel. Viaje");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Pasos/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" pasos/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" pasos/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" pasos/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E pasos/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* pasos/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Movimiento");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filamento");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E en mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Fil. Dia. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Descarga mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Carga mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Avance K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Avance K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Contraste LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Guardar EEPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Cargar EEPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Rest. fábrica");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Inicializar EEPROM");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Actualizar SD/USB");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Resetear Impresora");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Recargar");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Pantalla de Inf.");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Preparar");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Ajustar");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Iniciar impresión");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Siguinte");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Iniciar");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Parar");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Imprimir");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Reiniciar");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Cancelar");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Listo");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausar impresión");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Reanudar impresión");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Detener impresión");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Recuper. por interr.");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Imprim. desde SD/USB");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("SD/USB no presente");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Reposo...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Pulsar para Reanudar");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Impresión Pausada");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Imprimiendo...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Impresión cancelada");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Sin movimiento");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("MUERTA");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("DETENIDA");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retraer mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Interc. Retraer mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retraer  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Levantar mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("DesRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Interc. DesRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("DesRet V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Retracción Auto.");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Inter. longitud");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Purgar longitud");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Cambiar Herramienta");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Aumentar Z");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPD          = _UxGT("Prime Speed");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPD        = _UxGT("Vel. de retracción");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Colocar boquilla");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Cambiar filamento");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Cambiar filamento *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Cargar filamento");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Cargar filamento *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Descargar filamento");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Descargar fil. *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Descargar todo");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Iniciar SD/USB");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Cambiar SD/USB");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Lanzar SD/USB");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Sonda Z fuera cama");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Factor de desviación");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Cmd: Auto-Prueba");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Cmd: Reiniciar");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Cmd: Subir pistón");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Cmd: Bajar pistón");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("Cmd: Modo Software");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("Cmd: Modo 5V");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("Cmd: Modo OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Cmd: Modo almacenar");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Poner BLTouch a 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Poner BLTouch a OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Informe de drenaje");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("PELIGRO: ¡Una mala configuración puede producir daños! ¿Proceder igualmente?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Iniciar TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Test de desfase Z");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Guardar");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Subir TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Subir Sonda Z");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Bajar Sonda Z");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Origen %s%s%s Primero");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Desfase Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Micropaso X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Micropaso Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Micropaso Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Total");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Cancelado - Endstop");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Calent. fallido");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Calent. cama fallido");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_CHAMBER      = _UxGT("Calent. Cám. fallido");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Err: TEMP. REDUN.");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("FUGA TÉRMICA");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("FUGA TÉRMICA CAMA");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("FUGA TÉRMICA CAMARA");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err:TEMP. MÁX");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err:TEMP. MIN");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Err:TEMP. MÁX CAMA");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Err:TEMP. MIN CAMA");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_CHAMBER             = _UxGT("Err:TEMP. MÁX CÁMARA");
  PROGMEM Language_Str MSG_ERR_MINTEMP_CHAMBER             = _UxGT("Err:TEMP. MIN CÁMARA");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Origen XY Primero");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("IMPRESORA DETENIDA");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Por favor, reinicie");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Calentando...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Enfriando...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Calentando Cama...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Enfriando Cama...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Calentando Cámara...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Enfriando Cámara...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibración Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibrar X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibrar Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibrar Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibrar Centro");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Configuración Delta");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto Calibración");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Est. Altura Delta");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Ajustar Sonda Z");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Barra Diagonal");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Altura");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Radio");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Acerca de Impresora");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info. Impresora");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("Nivelando 3puntos");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Nivelando Lineal");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Nivelando Bilineal");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Nivelando UBL");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Nivelando en Mallado");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Estadísticas Imp.");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Info. Controlador");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistores");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrusores");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baudios");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocolo");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Luz cabina");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Brillo cabina");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Impresora incorrecta");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Cont. de impresión");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completadas");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tiempo total de imp.");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Impresión más larga");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Total Extruido");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Impresiones");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completadas");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Total");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Más larga");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Extruido");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Temp. Mínima");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Temp. Máxima");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Fuente alimentación");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Fuerza de empuje");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Driver %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("ERROR CONEX. TMC");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Escribe DAC EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("CAMBIAR FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("IMPRESIÓN PAUSADA");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("CARGAR FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("DESCARGAR FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("OPC. REINICIO:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Purgar más");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Continuar imp.");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Boquilla: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Sensor de sección");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Dist de secc. mm");
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("Ir a origen Fallado");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Sondeo Fallado");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Muy Frio");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("ELIJE FILAMENTO");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_MMU2_WRONG_FIRMWARE             = _UxGT("¡Actu. MMU Firmware!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU Necesita Cuidado");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Continuar imp.");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Resumiendo...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Cargar Filamento");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Cargar Todo");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Cargar hasta boqui.");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Expulsar Filamento");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Expulsar Filamento ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Descargar Filamento");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Cargando Fil. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Expulsando Fil. ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Descargando Fil....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Todo");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Filamento ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Reiniciar MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("Reiniciando MMU...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Retirar, y pulsar");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Mezcla");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Componente =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Miezclador");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Degradado");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Degradado Total");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Mezcla Conmutada");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Mezcla Cíclica");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Mezcla de Degradado");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Degradado inverso");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("Activar Herr.V");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("Inicio Herr.V");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT("   Fin Herr.V");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("Alias Herr.V");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("Reiniciar Herr.V");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("Cometer mezc. Herr.V");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("Herr.V reiniciados");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Inicio Z:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT("   Fin Z:");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Games");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Invaders");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Sn4k3");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Maze");

  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Pulsar el botón para", "reanudar impresión"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_INIT              = _UxGT(MSG_1_LINE("Aparcando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Esperando para", "iniciar el cambio", "de filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Inserte el filamento", "y pulse el botón", "para continuar..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Pulse el botón para", "calentar la boquilla"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Calentando boquilla", "Espere por favor..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Espere para", "liberar el filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Espere para", "purgar el filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Pulse para finalizar", "la purga de filamen."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Esperando impresora", "para reanudar..."));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Pulse para continuar"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_INIT              = _UxGT(MSG_1_LINE("Aparcando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Por Favor espere..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Inserte y Pulse"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Pulse para Calentar"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Calentando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Liberando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Cargando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Purgando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Pulse para finalizar"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Reanudando..."));
  #endif

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Controladores TMC");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Amperaje Controlador");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Límite Hibrido");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Origen sin sensores");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Modo de pasos");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("StealthChop Habilit.");

  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Reiniciar");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" dentro:");

  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Correction");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Suavizado");
}

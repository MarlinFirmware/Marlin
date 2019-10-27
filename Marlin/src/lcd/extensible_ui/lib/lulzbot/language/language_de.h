/*****************
 * language_de.h *
 *****************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

namespace Language_de {
  using namespace Language_en; // Inherit undefined strings from English

  PROGMEM Language_Str YES                      = u8"JA";
  PROGMEM Language_Str NO                       = u8"NEIN";
  PROGMEM Language_Str BACK                     = u8"Zurück";

  PROGMEM Language_Str MOVE_AXIS                = u8"Achsen bewegen";
  PROGMEM Language_Str MOTORS_OFF               = u8"Motoren deaktivieren";
  PROGMEM Language_Str TEMPERATURE              = u8"Temperatur";
  PROGMEM Language_Str CHANGE_FILAMENT          = u8"Filament wechseln";
  PROGMEM Language_Str ADVANCED_SETTINGS        = u8"Erw. Einstellungen";
  PROGMEM Language_Str ABOUT_PRINTER            = u8"Über den Drucker";
  PROGMEM Language_Str PRINTER_STATISTICS       = u8"Drucker-Statistik";

  PROGMEM Language_Str ZPROBE_ZOFFSET           = u8"Sondenversatz Z";
  PROGMEM Language_Str TOOL_OFFSETS             = u8"Werkzeugversätze";
  PROGMEM Language_Str VELOCITY                 = u8"Geschwindigkeit";
  PROGMEM Language_Str ACCELERATION             = u8"Beschleunigung";
  PROGMEM Language_Str ACCEL_PRINTING           = u8"Beschleunigung";
  PROGMEM Language_Str ACCEL_TRAVEL             = u8"A Einzug";
  PROGMEM Language_Str ACCEL_RETRACT            = u8"A Leerfahrt";
  PROGMEM Language_Str BACKLASH                 = u8"Spiel";
  PROGMEM Language_Str SMOOTHING                = u8"Glätten";
  PROGMEM Language_Str CORRECTION               = u8"Korrektur";
  PROGMEM Language_Str ENDSTOPS                 = u8"Endstopp";
  PROGMEM Language_Str SOFT_ENDSTOPS            = u8"Software-Endstopp";
  PROGMEM Language_Str RESTORE_DEFAULTS         = u8"Standardwerte laden";


  PROGMEM Language_Str HOTEND                   = u8"Düse";
  PROGMEM Language_Str HOTEND1                  = u8"Düse 1";
  PROGMEM Language_Str HOTEND2                  = u8"Düse 2";
  PROGMEM Language_Str HOTEND3                  = u8"Düse 3";
  PROGMEM Language_Str HOTEND4                  = u8"Düse 4";
  PROGMEM Language_Str BED                      = u8"Bett";
  PROGMEM Language_Str AXIS_ALL                 = u8"Alle";

  PROGMEM Language_Str FAN_SPEED                = u8"Lüfter";

  PROGMEM Language_Str PRINT_FILE               = u8"Drucken";

  PROGMEM Language_Str RESUME_PRINT             = u8"SD-Druck fortsetzen";
  PROGMEM Language_Str PAUSE_PRINT              = u8"SD-Druck pausieren";
  PROGMEM Language_Str STOP_PRINT               = u8"SD-Druck abbrechen";

  PROGMEM Language_Str INFO_PRINT_COUNT         = u8"Gesamte Drucke";
  PROGMEM Language_Str INFO_COMPLETED_PRINTS    = u8"Komplette Drucke";
  PROGMEM Language_Str INFO_PRINT_TIME          = u8"Gesamte Druckzeit";
  PROGMEM Language_Str INFO_PRINT_LONGEST       = u8"Längste Druckzeit";
  PROGMEM Language_Str INFO_PRINT_FILAMENT      = u8"Gesamt Extrudiert";

  PROGMEM Language_Str PRINTER_HALTED           = u8"DRUCKER GESTOPPT";
  PROGMEM Language_Str PLEASE_RESET             = u8"Bitte neustarten";
}; // namespace Language_de

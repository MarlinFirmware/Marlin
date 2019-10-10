/*****************
 * language_fr.h *
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

namespace Language_fr {
  using namespace Language_en; // Inherit undefined strings from English

  PROGMEM Language_Str YES                      = u8"oui";
  PROGMEM Language_Str NO                       = u8"non";
  PROGMEM Language_Str BACK                     = u8"Retour";

  PROGMEM Language_Str AUTO_HOME                = u8"Origine auto";
  //PROGMEM Language_Str CLEAN_NOZZLE             = u8"Clean Nozzle";
  PROGMEM Language_Str MOVE_AXIS                = u8"Déplacer un axe";
  PROGMEM Language_Str MOTORS_OFF               = u8"Arrêter moteurs";
  PROGMEM Language_Str TEMPERATURE              = u8"Température";
  PROGMEM Language_Str CHANGE_FILAMENT          = u8"Changer filament";
  PROGMEM Language_Str ADVANCED_SETTINGS        = u8"Config. avancée";
  PROGMEM Language_Str ABOUT_PRINTER            = u8"Infos imprimante";
  PROGMEM Language_Str PRINTER_STATISTICS       = u8"Stats. imprimante";

  PROGMEM Language_Str ZPROBE_ZOFFSET           = u8"Décalage Z";
  PROGMEM Language_Str STEPS_PER_MM             = u8"Pas/mm";
  PROGMEM Language_Str TOOL_OFFSETS             = u8"Offsets Outil";
  PROGMEM Language_Str VELOCITY                 = u8"Vélocité";
  PROGMEM Language_Str ACCELERATION             = u8"Accélération";
  PROGMEM Language_Str ACCEL_PRINTING           = u8"A impr.";
  PROGMEM Language_Str ACCEL_TRAVEL             = u8"A dépl.";
  PROGMEM Language_Str ACCEL_RETRACT            = u8"A retrait";
  PROGMEM Language_Str JUNC_DEVIATION           = u8"Déviat. jonct.";
  //PROGMEM Language_Str BACKLASH                 = u8"Backlash";
  PROGMEM Language_Str SMOOTHING                = u8"Lissage";
  PROGMEM Language_Str MOTOR_CURRENT            = u8"Courant";
  PROGMEM Language_Str ENDSTOPS                 = u8"Butées";
  PROGMEM Language_Str SOFT_ENDSTOPS            = u8"Butées SW";
  PROGMEM Language_Str RESTORE_DEFAULTS         = u8"Restaurer défauts";


  PROGMEM Language_Str HOTEND                   = u8"Buse";
  PROGMEM Language_Str HOTEND1                  = u8"Buse 1";
  PROGMEM Language_Str HOTEND2                  = u8"Buse 2";
  PROGMEM Language_Str HOTEND3                  = u8"Buse 3";
  PROGMEM Language_Str HOTEND4                  = u8"Buse 4";
  PROGMEM Language_Str BED                      = u8"Lit";
  PROGMEM Language_Str AXIS_ALL                 = u8"Tous";
  PROGMEM Language_Str HOME                     = u8"Origine";

  PROGMEM Language_Str FAN_SPEED                = u8"Vitesse ventil.";
  PROGMEM Language_Str RUNOUT_SENSOR            = u8"Capteur fil.";

  PROGMEM Language_Str PRINT_FILE               = u8"Imprimer";

  PROGMEM Language_Str RESUME_PRINT             = u8"Reprendre impr.";
  PROGMEM Language_Str PAUSE_PRINT              = u8"Pause impression";
  PROGMEM Language_Str STOP_PRINT               = u8"Arrêter impr.";

  PROGMEM Language_Str INFO_PRINT_COUNT         = u8"Nbre impressions";
  PROGMEM Language_Str INFO_COMPLETED_PRINTS    = u8"Terminées";
  PROGMEM Language_Str INFO_PRINT_TIME          = u8"Tps impr. total";
  PROGMEM Language_Str INFO_PRINT_LONGEST       = u8"Impr. la + longue";
  PROGMEM Language_Str INFO_PRINT_FILAMENT      = u8"Total filament";

  PROGMEM Language_Str PRINTER_HALTED           = u8"IMPR. STOPPÉE";
  PROGMEM Language_Str PLEASE_RESET             = u8"Redémarrer SVP";
}; // namespace Language_fr

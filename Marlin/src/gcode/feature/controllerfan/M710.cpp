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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(CONTROLLER_FAN_EDITABLE)

#include "../../gcode.h"
#include "../../../feature/controllerfan.h"

/**
 * M710: Set controller fan settings
 *
 *  R         : Reset to defaults
 *  S[0-255]  : Fan speed when motors are active
 *  I[0-255]  : Fan speed when motors are idle
 *  A[0|1]    : Turn auto mode on or off
 *  D         : Set auto mode idle duration
 *
 * Examples:
 *   M710                   ; Report current Settings
 *   M710 R                 ; Reset SIAD to defaults
 *   M710 I64               ; Set controller fan Idle Speed to 25%
 *   M710 S255              ; Set controller fan Active Speed to 100%
 *   M710 S0                ; Set controller fan Active Speed to OFF
 *   M710 I255 A0           ; Set controller fan Idle Speed to 100% with Auto Mode OFF
 *   M710 I127 A1 S255 D160 ; Set controller fan idle speed 50%, AutoMode On, Fan speed 100%, duration to 160 Secs
 */
void GcodeSuite::M710() {

  const bool seenR = parser.seen('R');
  if (seenR) controllerFan.reset();

  const bool seenS = parser.seenval('S');
  if (seenS) controllerFan.settings.active_speed = parser.value_byte();

  const bool seenI = parser.seenval('I');
  if (seenI) controllerFan.settings.idle_speed = parser.value_byte();

  const bool seenA = parser.seenval('A');
  if (seenA) controllerFan.settings.auto_mode = parser.value_bool();

  const bool seenD = parser.seenval('D');
  if (seenD) controllerFan.settings.duration = parser.value_ushort();

  if (!(seenR || seenS || seenI || seenA || seenD))
    M710_report();
}

void GcodeSuite::M710_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_CONTROLLER_FAN));
  SERIAL_ECHOLNPGM("  M710"
    " S", int(controllerFan.settings.active_speed),
    " I", int(controllerFan.settings.idle_speed),
    " A", int(controllerFan.settings.auto_mode),
    " D", controllerFan.settings.duration,
    " ; (", (int(controllerFan.settings.active_speed) * 100) / 255, "%"
    " ", (int(controllerFan.settings.idle_speed) * 100) / 255, "%)"
  );
}

#endif // CONTROLLER_FAN_EDITABLE

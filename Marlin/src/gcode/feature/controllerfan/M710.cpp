/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(USE_CONTROLLER_FAN)

#include "../../gcode.h"
#include "../../../feature/controllerfan.h"

void M710_report(const bool forReplay=true) {
  if (!forReplay) SERIAL_ECHOLNPGM("; Controller Fan");
  SERIAL_ECHOPAIR("M710 "
    "S", int(controllerFan.settings.speed),
    "I", int(controllerFan.settings.idle_speed),
    "A", int(controllerFan.settings.auto_mode),
    "D", controllerFan.settings.duration / 1000UL
  );
  if (!forReplay)
    SERIAL_ECHOPAIR(" ; (",
      float(controllerFan.settings.active_speed) / 2.55f, "% ",
      float(controllerFan.settings.idle_speed) / 2.55f, "%)"
    );

  SERIAL_EOL();
}

/**
 * M710: Set controller fan settings
 * You can set all parameters separate or together.
 * Examples:
 *   M710                   ; Return current Settings
 *   M710 S0                ; Turn off Controller Fan
 *   M710 I255              ; Set Fan Idle Speed Setting
 *   M710 R                 ; Reset settings to defaults
 *   M710 S255              ; Set controller Fan speed setting to 100%
 *   M710 I255 A0           ; Set Fan idle speed setting and AutoMode off
 *   M710 S255              ; Set controller Fan speed setting to 100%
 *   M710 I127 A1 S255 D160 ; Set controller Fan idle speed 50%, AutoMode On, Fan speed 100%, duration to 160 Secs
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
  if (seenD) controllerFan.settings.duration = parser.value_ushort() * 1000UL;

  if (seenR || seenS || seenI || seenA || seenD)
    controllerFan.update();
  else
    M710_report();
}

#endif // USE_CONTROLLER_FAN

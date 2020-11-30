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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(CASE_LIGHT_ENABLE)

#include "../../../feature/caselight.h"
#include "../../gcode.h"

/**
 * M355: Turn case light on/off and set brightness
 *
 *   P<byte>  Set case light brightness (PWM pin required - ignored otherwise)
 *
 *   S<bool>  Set case light on/off
 *
 *   When S turns on the light on a PWM pin then the current brightness level is used/restored
 *
 *   M355 P200 S0 turns off the light & sets the brightness level
 *   M355 S1 turns on the light with a brightness of 200 (assuming a PWM pin)
 */
void GcodeSuite::M355() {
  bool didset = false;
  #if CASELIGHT_USES_BRIGHTNESS
    if (parser.seenval('P')) {
      didset = true;
      caselight.brightness = parser.value_byte();
    }
  #endif
  const bool sflag = parser.seenval('S');
  if (sflag) {
    didset = true;
    caselight.on = parser.value_bool();
  }
  if (didset) caselight.update(sflag);

  // Always report case light status
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Case light: ");
  if (!caselight.on)
    SERIAL_ECHOLNPGM(STR_OFF);
  else {
    #if CASELIGHT_USES_BRIGHTNESS
      if (PWM_PIN(CASE_LIGHT_PIN)) {
        SERIAL_ECHOLN(int(caselight.brightness));
        return;
      }
    #endif
    SERIAL_ECHOLNPGM(STR_ON);
  }
}

#endif // CASE_LIGHT_ENABLE

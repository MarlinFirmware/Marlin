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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(LIN_ADVANCE)

#include "../../gcode.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"

/**
 * M900: Set and/or Get advance K factor and WH/D ratio
 *
 *  K<factor>                  Set advance K factor
 *  R<ratio>                   Set ratio directly (overrides WH/D)
 *  W<width> H<height> D<diam> Set ratio from WH/D
 */
void GcodeSuite::M900() {
  stepper.synchronize();

  const float newK = parser.floatval('K', -1);
  if (newK >= 0) planner.extruder_advance_k = newK;

  float newR = parser.floatval('R', -1);
  if (newR < 0) {
    const float newD = parser.floatval('D', -1),
                newW = parser.floatval('W', -1),
                newH = parser.floatval('H', -1);
    if (newD >= 0 && newW >= 0 && newH >= 0)
      newR = newD ? (newW * newH) / CIRCLE_AREA(newD * 0.5) : 0;
  }
  if (newR >= 0) planner.advance_ed_ratio = newR;

  SERIAL_ECHO_START();
  SERIAL_ECHOPAIR("Advance K=", planner.extruder_advance_k);
  SERIAL_ECHOPGM(" E/D=");
  const float ratio = planner.advance_ed_ratio;
  if (ratio) SERIAL_ECHO(ratio); else SERIAL_ECHOPGM("Auto");
  SERIAL_EOL();
}

#endif // LIN_ADVANCE

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
 * M900: Set and/or Get advance K factor
 *
 *  K<factor>                  Set advance K factor
 */
void GcodeSuite::M900() {
    stepper.synchronize();

    const float newK = parser.floatval('K', -1);
    if (newK >= 0) planner.extruder_advance_K = newK;

    SERIAL_ECHO_START();
    SERIAL_ECHOPAIR("Advance K=", planner.extruder_advance_K);
    SERIAL_EOL();
}

#endif // LIN_ADVANCE

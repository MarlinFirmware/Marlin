/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * gcode/temp/M143_M193.cpp
 *
 * Laser Cooler target temperature control
 */

#include "../../inc/MarlinConfig.h"

#if HAS_COOLER

#include "../../feature/cooler.h"
extern Cooler cooler;

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../lcd/marlinui.h"

/**
 * M143: Set cooler temperature
 */
void GcodeSuite::M143() {
  if (DEBUGGING(DRYRUN)) return;
  if (parser.seenval('S')) {
    thermalManager.setTargetCooler(parser.value_celsius());
    parser.value_celsius() ? cooler.enable() : cooler.disable();
  }
}

/**
 * M193: Sxxx Wait for laser current temp to reach target temp. Waits only when cooling.
 */
void GcodeSuite::M193() {
  if (DEBUGGING(DRYRUN)) return;

  if (parser.seenval('S')) {
    cooler.enable();
    thermalManager.setTargetCooler(parser.value_celsius());
    if (thermalManager.isLaserCooling()) {
      ui.set_status_P(GET_TEXT(MSG_LASER_COOLING));
      thermalManager.wait_for_cooler(true);
    }
  }
}

#endif // HAS_COOLER

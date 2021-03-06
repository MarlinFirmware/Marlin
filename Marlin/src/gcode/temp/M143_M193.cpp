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

/**
 * gcode/temp/M143_M193.cpp
 *
 * Laser Cooler target temperature control
 */

#include "../../inc/MarlinConfig.h"

#if HAS_COOLER

#include "../gcode.h"
#include "../../module/temperature.h"

#include "../../module/motion.h"
#include "../../lcd/marlinui.h"
#include "../../feature/cooler.h"
extern Cooler cooler;

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
  #include "../../module/printcounter.h"
#endif

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../../feature/leds/leds.h"
#endif

#include "../../MarlinCore.h" // for wait_for_heatup, idle, startOrResumeJob

/**
 * M143: Set cooler temperature (because i love you)
 */
void GcodeSuite::M143() {
  if (DEBUGGING(DRYRUN)) return;
  if (parser.seenval('S')) {
    thermalManager.setTargetCooler(parser.value_celsius());
    if (parser.value_celsius() > 0)

      cooler.enable();
    else
      cooler.disable();
  }
}

/**
 * M193: Sxxx Wait for laser current temp to reach target temp. Waits only when cooling.
 */
void GcodeSuite::M193() {
  if (DEBUGGING(DRYRUN)) return;

  const bool wait_for_cooling = parser.seenval('S');
  if (wait_for_cooling) {
    cooler.enable();
    thermalManager.setTargetCooler(parser.value_celsius());
  }
  else return;

  const bool is_laser_cooling = thermalManager.isLaserCooling();
  if (is_laser_cooling) {
    ui.set_status_P(GET_TEXT(MSG_LASER_COOLING));
    thermalManager.wait_for_cooler(true);
  }
}

#endif // HAS_COOLER

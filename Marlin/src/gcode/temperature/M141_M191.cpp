/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if HAS_HEATED_CHAMBER

#include "../gcode.h"
#include "../../module/temperature.h"

/*
#include "../../module/motion.h"
#include "../../lcd/ultralcd.h"

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
  #include "../../module/printcounter.h"
#endif

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../../feature/leds/leds.h"
#endif

#include "../../Marlin.h" // for wait_for_heatup and idle()
*/

/**
 * M141: Set chamber temperature
 */
void GcodeSuite::M141() {
  if (DEBUGGING(DRYRUN)) return;
  if (parser.seenval('S')) thermalManager.setTargetChamber(parser.value_celsius());
}

/**
 * M191: Sxxx Wait for chamber current temp to reach target temp. Waits only when heating
 *       Rxxx Wait for chamber current temp to reach target temp. Waits when heating and cooling
 */
/*
void GcodeSuite::M191() {
  if (DEBUGGING(DRYRUN)) return;

  const bool no_wait_for_cooling = parser.seenval('S');
  if (no_wait_for_cooling || parser.seenval('R')) {
    thermalManager.setTargetChamber(parser.value_celsius());
    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      if (parser.value_celsius() > BED_MINTEMP)
        print_job_timer.start();
    #endif
  }
  else return;

  lcd_setstatusPGM(thermalManager.isHeatingChamber() ? PSTR(MSG_CHAMBER_HEATING) : PSTR(MSG_CHAMBER_COOLING));

  thermalManager.wait_for_chamber(no_wait_for_cooling);
}
*/

#endif // HAS_HEATED_CHAMBER

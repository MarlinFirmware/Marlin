/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * gcode/temp/M86-M87.cpp
 *
 * Hotend Idle Timeout
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(HOTEND_IDLE_TIMEOUT)

#include "../gcode.h"
#include "../../feature/hotend_idle.h"

void GcodeSuite::M86_report(const bool forReplay/*=true*/) {
  hotend_idle_settings_t &c = hotend_idle.cfg;
  report_heading(forReplay, F("Hotend Idle Timeout"));
  SERIAL_ECHOLNPGM("  M86"
    #if HAS_HEATED_BED
      " B", c.bed_target,
    #endif
    " E", c.nozzle_target,
    " S", c.timeout,
    " T", c.trigger
  );
}

/**
 * M86: Set / Report Hotend Idle Timeout
 *
 * Parameters
 *  S<seconds>  : Idle timeout. Set to 0 to disable.
 *  E<temp>     : Extruder idle temperature to set on timeout
 *  B<temp>     : Bed idle temperature to set on timeout
 *  T<temp>     : Minimum extruder temperature to consider for timeout (> idle temperature)
 */
void GcodeSuite::M86() {
  if (!parser.seen_any()) return M86_report();
  hotend_idle_settings_t &c = hotend_idle.cfg;
  if (parser.seenval('S')) c.timeout = parser.value_ushort();
  if (parser.seenval('T')) c.trigger = parser.value_celsius();
  if (parser.seenval('E')) c.nozzle_target = parser.value_celsius();
  #if HAS_HEATED_BED
    if (parser.seenval('B')) c.bed_target = parser.value_celsius();
  #endif
  const celsius_t min_trigger = c.nozzle_target + TEMP_HYSTERESIS;
  if (c.trigger <= min_trigger)
    SERIAL_ECHOLNPGM("?Idle Timeout (T) trigger temperature should be over ", min_trigger, "C.");
}

/**
 * M86: Cancel Hotend Idle Timeout (by setting the timeout period to 0)
 */
void GcodeSuite::M87() {
  hotend_idle.cfg.timeout = 0;
}

#endif // HOTEND_IDLE_TIMEOUT

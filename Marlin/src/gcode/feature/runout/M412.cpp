/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * M412 is retained as a backward-compatibility alias for M591.
 * New code should use M591 directly.
 *
 * M412 S<bool>   => M591 S<bool>  (enable/disable for motion.extruder)
 * M412 D<mm>     => M591 D<mm>    (runout distance for motion.extruder)
 * M412 R         => M591 R        (reset sensor)
 * M412 H<bool>   => M591 H<bool>  (host handling)
 * M412 (no args) => M591 (report active extruder state)
 *
 * NOTE: M412 L<mm> (motion distance for FILAMENT_SWITCH_AND_MOTION) is
 * still handled here because M591 uses L as an alias for D, not motion
 * distance. That parameter remains M412-only until a separate M591 param
 * is decided upon.
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_FILAMENT_SENSOR

#include "../../gcode.h"
#include "../../../feature/runout.h"

/**
 * M412: Enable / Disable filament runout detection (backward-compat shim for M591)
 *
 * Parameters
 *  R         : Reset the runout sensor
 *  S<bool>   : Enable/disable runout detection for motion.extruder
 *  H<bool>   : Enable/disable host handling of filament runout
 *              (Requires HOST_ACTION_COMMANDS)
 *  D<linear> : Extra distance to continue after runout is triggered
 *              (Requires HAS_FILAMENT_RUNOUT_DISTANCE)
 *
 * With FILAMENT_SWITCH_AND_MOTION:
 *  L<linear> : Missing motion length to consider a jam
 */
void GcodeSuite::M412() {
  if (parser.seen("RS"
    TERN_(HAS_FILAMENT_RUNOUT_DISTANCE, "D")
    TERN_(FILAMENT_SWITCH_AND_MOTION,   "L")
    TERN_(HOST_ACTION_COMMANDS,         "H")
  )) {
    #if ENABLED(HOST_ACTION_COMMANDS)
      if (parser.seen('H')) runout.host_handling = parser.value_bool();
    #endif

    const bool seenR = parser.seen_test('R'), seenS = parser.seen('S');
    if (seenR || seenS) runout.reset();
    if (seenS) runout.set_enabled(TERN0(MULTI_FILAMENT_SENSOR, motion.extruder), parser.value_bool());

    #if HAS_FILAMENT_RUNOUT_DISTANCE
      if (parser.seenval('D'))
        runout.set_runout_distance(parser.value_linear_units(), motion.extruder);
    #endif
    #if ENABLED(FILAMENT_SWITCH_AND_MOTION)
      if (parser.seenval('L')) runout.set_motion_distance(parser.value_linear_units());
    #endif
  }
  else
    M591_report(false);
}

/**
 * M412_report: Emit M591 lines (one per sensor) for M503 replay.
 * Delegates entirely to M591_report so the saved config is always
 * in M591 form and can be replayed whether or not M412 remains.
 */
void GcodeSuite::M412_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);
  M591_report(forReplay);
  #if ENABLED(FILAMENT_SWITCH_AND_MOTION)
    report_heading_etc(forReplay, F("Filament Motion Distance"));
    SERIAL_ECHOLNPGM("  M412 L", LINEAR_UNIT(runout.motion_distance()));
  #endif
}

#endif // HAS_FILAMENT_SENSOR

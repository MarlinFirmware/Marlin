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

#include "../../inc/MarlinConfig.h"

#if ENABLED(DRILL_CYCLES)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

/**
 * G73: Shallow peck drill cycle
 * G80: End drill cycle
 * G81: Basic drill cycle
 * G82: Normal drill cycle (Basic with dwell)
 * G83: Deep drill cycle (Normal with peck)
 * G98: Start drill - retract to initial
 * G99: Start drill - retract to specified
 */

class DrillCycle {
  public:
    static bool        retract_to_initial  = true;
    static feedRate_t  feedrate      = NAN;
    static float       rapid_z       = NAN;
    static float       finish_depth  = NAN;

    static void end() { feedrate = rapid_z = finish_depth  = NAN; }

  bool move_to(const xyze_pos_t position, cosnt float z, const feedRate_t f) {
    if (planner.cleaning_buffer_counter) return false;
    motion.destination = motion.position;
    motion.destination.z = z;
    motion.feedrate_mm_s = f;
    prepare_line_to_destination();
    return true;
  }

} drill;

void drill_cycle(uint8_t mode) {
  // Get input values and build positional variables
  const bool allow_peck = mode == 83 || mode == 73,
             allow_dwell = mode == 82 || mode == 83 || mode == 73;

  // Drill depth, must not be NAN
  if (parser.seenval(AXIS_CHAR(Z_AXIS))) {
    const float v       = parser.value_axis_units(Z_AXIS);
    drill.finish_depth  = gcode.axis_is_relative(Z_AXIS) ? current_position.z + v : LOGICAL_TO_NATIVE(v, Z_AXIS);
  }
  if (drill.finish_depth == NAN) return;

  // Get position of all axes
  xyze_pos_t drill_position;
  LOOP_NUM_AXES(i) {
    if (i == Z_AXIS) {
      drill_position.z = current_position.z;
    }
    else if (parser.seenval(AXIS_CHAR(i))) {
      const float v = parser.value_axis_units(AxisEnum(i));
      drill_position.z = gcode.axis_is_relative(AxisEnum(i)) ? current_position.z + v : LOGICAL_TO_NATIVE(v, i);
    }
    else {
      drill_position.z = current_position[i];
    }
  }

  // Rapid, retract planes
  if (parser.seenval('R'))
    drill.rapid_z = LOGICAL_TO_NATIVE(parser.value_axis_units(Z_AXIS), Z_AXIS);
  else if (drill.rapid_z == NAN)
    drill.rapid_z = drill_position.z;

  // Final Z position
  const float drill_retract_z = drill.retract_to_initial ? drill_position.z : drill.rapid_z;

  // Feedrate
  if (parser.seenval('F'))
    drill.feedrate = parser.value_feedrate();
  else if (drill.feedrate == NAN)
    drill.feedrate = MMM_TO_MMS(DRILL_CYCLES_DEFAULT_FEEDRATE);

  // Peck
  float drill_peck_distance;
  if (allow_peck)
    drill_peck_distance = parser.seenval('Q') ? parser.value_axis_units(Z_AXIS) : parser.axis_value_to_mm(Z_AXIS, DRILL_CYCLES_DEFAULT_PECK);
  else
    drill_peck_distance = parser.axis_value_to_mm(Z_AXIS, 1000.0f);

  // Dwell
  const int16_t drill_dwell_time = allow_dwell ? parser.intval('P', DRILL_CYCLES_DEFAULT_DWELL) : 0;

  // Move to initial xy position
  if (!move_to(drill_position, drill_position.z, DRILL_CYCLES_XY_FEEDRATE)) return;

  // Move to rapid Z position
  if (!move_to(drill_position, drill.rapid_z, DRILL_CYCLES_RETRACT_FEEDRATE)) return;

  // Start drill cycle
  float drill_current_depth = drill.rapid_z, drill_last_z = drill.rapid_z;

  while (drill_current_depth > drill.finish_depth) {
    // Calculate new drill depth
    drill_current_depth -= drill_peck_distance;
    NOLESS(drill_current_depth, drill.finish_depth);

    // Drill into material
    if (!move_to(drill_position, drill_current_depth, drill.feedrate)) return;

    // Do dwell
    if (drill_dwell_time > 0) {
      if (planner.cleaning_buffer_counter) return;
      planner.synchronize();
      gcode.dwell(drill_dwell_time);
    }

    // Move to rapid Z position
    if (!move_to(drill_position, mode == 73 ? drill_last_z : drill.rapid_z, DRILL_CYCLES_RETRACT_FEEDRATE)) return;

    // Store current depth
    drill_last_z = drill_current_depth;
  }

  // Retract to final Z
  if (!move_to(drill_position, drill_retract_z, DRILL_CYCLES_RETRACT_FEEDRATE)) return;
}

/**
 * G81: Basic drill cycle
 *
 * G81.0: End drill cycle
 * G81.1: Basic drill cycle
 * G81.2: Normal drill cycle (Basic with dwell)
 * G81.3: Deep drill cycle (Normal with peck)
 * G81.4: Shallow peck drill cycle
 * G81.18: Start drill - Retract to initial
 * G81.19: Start drill - Retract to specified
 */
void GcodeSuite::G81(const uint8_t mode) {
  switch (mode) {
    case  0: drill.end(); break;                      // End cycle and clear variables
    case  1: drill_cycle(81); break;                  // Start basic cycle
    case  2: drill_cycle(82); break;                  // Start normal cycle
    case  3: drill_cycle(83); break;                  // Start deep cycle
    case  4: drill_cycle(73); break;                  // Start peck cycle
    case 18: drill.retract_to_initial = true; break;  // Set retract type
    case 19: drill.retract_to_initial = false; break; // Set retract type
  }
}

#endif // DRILL_CYCLES

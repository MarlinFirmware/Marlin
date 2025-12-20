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

#include "../../inc/MarlinConfig.h"

#if ENABLED(DRILL_CYCLES)

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * G73: Shallow peck drill cycle
 * G80: End drill cycle
 * G81: Basic drill cycle
 * G82: Normal drill cycle (Basic with dwell)
 * G83: Deep drill cycle (Normal with peck)
 * G98: Start drill - retract to initial
 * G99: Start drill - retract to specified
 */

bool        retract_to_initial  = true;
feedRate_t  drill_feedrate      = NAN;
float       drill_rapid_z       = NAN;
float       drill_finish_depth  = NAN;

void move_to(xyze_pos_t position, float z, feedRate_t f) {
  LOOP_NUM_AXES(i) {
    destination[i] = position[i];
  }

  destination[Z_AXIS] = z;
  feedrate_mm_s = f;

  prepare_line_to_destination();
 }

void drill_cycle(uint8_t mode) {
  //get input values and build positional variables
  bool        allow_peck          = mode == 83 || mode == 73;
  bool        allow_dwell         = mode == 82 || mode == 83 || mode == 73;

  //drill depth, must not be NAN
  if(parser.seenval(AXIS_CHAR(Z_AXIS))) {
    const float v       = parser.value_axis_units(Z_AXIS);
    drill_finish_depth  = gcode.axis_is_relative(AxisEnum(Z_AXIS)) ? current_position[Z_AXIS] + v : LOGICAL_TO_NATIVE(v, Z_AXIS);
  }
  if(drill_finish_depth == NAN) return;

  //get position of all axes
  xyze_pos_t  drill_position;
  LOOP_NUM_AXES(i) {
    if(i == Z_AXIS) {
      drill_position[i]   = current_position.z;
    } else if (parser.seenval(AXIS_CHAR(i))) {
      const float v = parser.value_axis_units((AxisEnum)i);
      drill_position[i] = gcode.axis_is_relative(AxisEnum(i)) ? current_position[i] + v : LOGICAL_TO_NATIVE(v, i);
    } else {
      drill_position[i] = current_position[i];
    }
  }

  //rapid, retract planes
  if(parser.seenval('R')) drill_rapid_z = LOGICAL_TO_NATIVE(parser.value_axis_units(Z_AXIS), Z_AXIS);
  else if(drill_rapid_z == NAN) drill_rapid_z = drill_position[Z_AXIS];
  float drill_retract_z = retract_to_initial ? drill_position[Z_AXIS] : drill_rapid_z;

  //feedrate
  if(parser.seenval('F')) drill_feedrate = parser.value_feedrate();
  else if(drill_feedrate == NAN) drill_feedrate = MMM_TO_MMS(DRILL_CYCLES_DEFAULT_FEEDRATE);

  //peck
  float drill_peck_distance = parser.axis_value_to_mm(Z_AXIS, 1000.0f);
  if(allow_peck) {
    drill_peck_distance = parser.seenval('Q') ? parser.value_axis_units(Z_AXIS) : parser.axis_value_to_mm(Z_AXIS, DRILL_CYCLES_DEFAULT_PECK);
  }

  //dwell
  int16_t drill_dwell_time = 0;
  if(allow_dwell) {
    drill_dwell_time = parser.seenval('P') ? parser.value_int() : DRILL_CYCLES_DEFAULT_DWELL;
  }

  //move to initial xy position
  move_to(drill_position, 
            drill_position[Z_AXIS],
            DRILL_CYCLES_XY_FEEDRATE);

  //move to rapid z position
  move_to(drill_position, 
            drill_rapid_z,
            DRILL_CYCLES_RETRACT_FEEDRATE);

  //start drill cycle
  float drill_current_depth = drill_rapid_z;
  float drill_last_z        = drill_current_depth;
  while(drill_current_depth > drill_finish_depth) {
    //calculate new drill depth
    drill_current_depth -= drill_peck_distance;
    if(drill_current_depth < drill_finish_depth) drill_current_depth = drill_finish_depth;

    //drill into material
    move_to(drill_position, 
            drill_current_depth,
            drill_feedrate);

    //do dwell
    if(drill_dwell_time > 0) {
      char gcode_str[15];
      sprintf_P(gcode_str, PSTR("G4 P%d"), drill_dwell_time);
      gcode.process_subcommands_now(gcode_str);
    }

    //move to rapid z position
    move_to(drill_position, 
            mode == 73 ? drill_last_z : drill_rapid_z,
            DRILL_CYCLES_RETRACT_FEEDRATE);

    //store current depth
    drill_last_z = drill_current_depth;
  }

  //retract to final z
  move_to(drill_position,
            drill_retract_z,
            DRILL_CYCLES_RETRACT_FEEDRATE);
}

void GcodeSuite::G81(uint8_t mode) {
  switch(mode) {
    case 0:         //End cycle and clear variables
      drill_feedrate      = NAN;
      drill_rapid_z       = NAN;
      drill_finish_depth  = NAN;
      break;

    case 1:       //Start basic cycle
      drill_cycle(81);
      break;

    case 2:       //Start normal cycle
      drill_cycle(82);
      break;

    case 3:       //Start deep cycle
      drill_cycle(83);
      break;

    case 4:       //Start peck cycle
      drill_cycle(73);
      break;

    case 18:      //Set retract type
      retract_to_initial = true;
      break;

    case 19:      //Set retract type
      retract_to_initial = false;
      break;
  }
}

#endif // DRILL_CYCLES

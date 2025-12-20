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
 * G80: Cancel drill cycle
 * G81: Basic drill cycle
 * G82: Normal drill cycle (Basic with dwell)
 * G83: Deep drill cycle (Normal with peck)
 * G98: Start drill - retract to initial
 * G99: Start drill - retract to specified
 */

 //#define DRILL_CYCLE_DEBUG

bool  retract_to_initial  = true;
bool  drill_cycle_started = false;

void move_to_XYZF(float x, float y, float z, uint16_t f) {
  char gcode_str[50], x_str[10], y_str[10], z_str[10];
  dtostrf(x, 1, 3, x_str);
  dtostrf(y, 1, 3, y_str);
  dtostrf(z, 1, 3, z_str);
  
  sprintf_P(gcode_str, PSTR("G1 X%s Y%s Z%s F%d"), x_str, y_str, z_str, f);
#if ENABLED(DRILL_CYCLE_DEBUG)
  SERIAL_ECHOPGM("DEBUG: ", gcode_str);
#endif
  gcode.process_subcommands_now(gcode_str);
 }

 void drill_start(bool initial) {
  if(!drill_cycle_started) {
    drill_cycle_started = true;
    retract_to_initial = initial;
  }
}

void drill_stop() {
  drill_cycle_started = false;
}

void drill_cycle(uint8_t mode) {
  if(!drill_cycle_started) return;

  bool      allow_peck          = mode == 83 || mode == 73;
  bool      allow_dwell         = mode == 82 || mode == 83 || mode == 73;

  float     drill_x_position    = parser.seenval(AXIS_CHAR(X_AXIS)) ? parser.value_float() : NATIVE_TO_LOGICAL(current_position.x, X_AXIS);
  float     drill_y_position    = parser.seenval(AXIS_CHAR(Y_AXIS)) ? parser.value_float() : NATIVE_TO_LOGICAL(current_position.y, Y_AXIS);
  float     drill_initial_z     = NATIVE_TO_LOGICAL(current_position.z, Z_AXIS);

  if(!parser.seenval(AXIS_CHAR(Z_AXIS))) return;
  float     drill_finish_depth  = parser.value_float();

  float     drill_rapid_z       = parser.seenval('R') ? parser.value_float() : drill_initial_z;
  float     drill_retract_z     = retract_to_initial ? drill_initial_z : drill_rapid_z;
  float     drill_current_depth = drill_rapid_z;

  uint16_t  drill_feedrate      = parser.seenval('F') ? parser.value_int() : (drill_feedrate > 0 ? drill_feedrate : DRILL_CYCLES_DEFAULT_FEEDRATE);
  float     drill_peck_distance = allow_peck ? (parser.seenval('Q') ? parser.value_float() : (DRILL_CYCLES_DEFAULT_PECK > 0 ? DRILL_CYCLES_DEFAULT_PECK : 10000)) : 10000;
  uint16_t  drill_dwell_time    = allow_dwell ? (parser.seenval('P') ? parser.value_int() : DRILL_CYCLES_DEFAULT_DWELL) : 0;

  //move to initial xy position
  move_to_XYZF(drill_x_position, 
            drill_y_position,
            drill_initial_z,
            DRILL_CYCLES_XY_FEEDRATE);

  //move to rapid z position
  move_to_XYZF(drill_x_position,
            drill_y_position,
            drill_rapid_z,
            DRILL_CYCLES_RETRACT_FEEDRATE);

  //start drill cycle
  float drill_last_z = drill_rapid_z;
  while(drill_current_depth > drill_finish_depth) {
    //calculate new drill depth
    drill_current_depth -= drill_peck_distance;
    if(drill_current_depth < drill_finish_depth) drill_current_depth = drill_finish_depth;

    //drill into material
    move_to_XYZF(drill_x_position, 
            drill_y_position,
            drill_current_depth,
            drill_feedrate);

    //do dwell
    if(drill_dwell_time > 0) {
      char gcode_str[15];
      sprintf_P(gcode_str, PSTR("G4 P%d"), drill_dwell_time);
    #if ENABLED(DRILL_CYCLE_DEBUG)
      SERIAL_ECHOPGM("DEBUG: ", gcode_str);
    #endif
      gcode.process_subcommands_now(gcode_str);
    }

    //move to rapid z position
    move_to_XYZF(drill_x_position,
            drill_y_position,
            mode == 73 ? drill_last_z : drill_rapid_z,
            DRILL_CYCLES_RETRACT_FEEDRATE);

    //store current depth
    drill_last_z = drill_current_depth;
  }

  //retract to final z
  move_to_XYZF(drill_x_position, 
            drill_y_position,
            drill_retract_z,
            DRILL_CYCLES_RETRACT_FEEDRATE);
}

void GcodeSuite::G81(uint8_t mode) {
  switch(mode) {
    case 0:
      drill_stop();
      break;

    case 1:
      drill_cycle(81);
      break;

    case 2:
      drill_cycle(82);
      break;

    case 3:
      drill_cycle(83);
      break;

    case 4:
      drill_cycle(73);
      break;

    case 18:
      drill_start(true);
      break;

    case 19:
      drill_start(false);
      break;
  }
}

#endif // DRILL_CYCLES

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

#include "../gcode.h"
#include "../../module/motion.h"

#include "../../MarlinCore.h"
#include "../../module/stepper.h"

#include "../../module/planner.h"

void GcodeSuite::G13() {
  if (parser.seenval('Z') && parser.seenval('W') /*&& parser.seenval('V')*/) {
    const float z_val = parser.floatval('Z'),
                v_val = parser.floatval('V'),
                w_val = parser.floatval('W');
    //if(parser.seenval('R')){
    
     set_relative_mode(true);
    //}
    
    planner.synchronize();
    stepper.set_samostatny_pohyb(true);
    
    wv_pos[0] = w_val;

    current_position[Z_AXIS] += z_val;
    current_position[I_AXIS] += wv_pos[0] - w_val;
    //current_position[J_AXIS] += w_val;
    planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
    planner.synchronize();
    stepper.set_samostatny_pohyb(false);
    //if(parser.seenval('R')){
      set_relative_mode(false);
    //}
    SERIAL_ECHOLNPGM("", current_position);
  }
}

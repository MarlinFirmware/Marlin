/**
 * Marlin2ForPipetBot
 * Copyright 2022 DerAndere
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
 * @file xyzbc_head_table.cpp
 * @author DerAndere
 * @brief Kinematics for 5 axis CNC machine in head-table configuration.
 *
 * This machine has a tilting head (B axis parallel to the Y axis) and a horizontal rotary
 * table (C axis parallel to the Z axis). The user can use axes U, V, W to move in the rotated coordinate system.
 *
 * Inspired by: 
 * LinuxCNC maxkins.c (https://github.com/LinuxCNC/linuxcnc/blob/master/src/emc/kinematics/maxkins.c)
 * Copyright (c) 2007 Chris Radek
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(XYZBC_HEAD_TABLE)

#include "xyzbc_head_table.h"
#include "motion.h"


/**
 * xyzbc inverse kinematics
 *
 * Calculate the joints positions for a given position, storing the result in the global delta[] array.
 * if joints control mode is active (default), input position is interpreted as joints position.
 * If rotational tool center point control mode is active, input position is interpreted as machine position using ik_calc.
 */
void inverse_kinematics(const xyz_pos_t &raw) {
  #if HAS_HOTEND_OFFSET
    // Delta hotend offsets must be applied in Cartesian space with no "spoofing"
    xyz_pos_t pos = NUM_AXIS_ARRAY(raw.x - hotend_offset[active_extruder].x,
                      raw.y - hotend_offset[active_extruder].y,
                      raw.z,
                      raw.i,
                      raw.j
                    );
    if (!tool_centerpoint_control)
      delta = pos;
    else
      ik_calc(pos);
  #else
    if (!tool_centerpoint_control)
      delta = raw;
    else
      ik_calc(raw);
  #endif
}

/**
 * Calculate the joints positions for a given machine
 * position, storing the result in the global delta[] array.
 *
 * This is an expensive calculation.
 */
void ik_calc(const xyz_pos_t &pos) {
  const float pivot_length = tool_length_offset + mrzp_z_offset

  // B correction
  const float zb = (pivot_length + pos.w) * cos(RADIANS(pos.i));
  const float xb = (pivot_length + pos.w) * sin(RADIANS(pos.i));

  // C correction
  const float xyr = HYPOT2(pos.x, pos.y);
  const float xytheta = ATAN2(pos.y, pos.x) - RADIANS(pos.j);

  delta.set(xyr * cos(xytheta) - xb, xyr * sin(xytheta), pos.z + zb - pivot_length, pos.i, pos.j);
}

#endif //XYZBC_HEAD_TABLE
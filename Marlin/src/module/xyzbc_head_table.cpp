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


// Distance between the center of rotation of the rotary axis and home position.
// Machine rotary zero point Z offset is the distance from the center of rotation of the B axis to the gauge line.
constexpr float mrzp_z_offset = DEFAULT_MRZP_Z_OFFSET_MM;

/**
 * xyzbc inverse kinematics
 *
 * Calculate the joints positions for a given machine
 * position, storing the result in the global delta[] array.
 *
 * This is an expensive calculation.
 */

void inverse_kinematics(const xyz_pos_t &pos) {
  if (!tool_centerpoint_control && !tool_Length_compensation)
    delta = pos;
  else if (tool_Length_compensation) {
    delta.set(pos.x, pos.y, pos.z + tool_length_offsets[active_extruder], pos.i, pos.j)
  }
  else {

    const float pivot_length = tool_length_offset + mrzp_z_offset

    // B correction
    const float zb = (pivot_length + pos.w) * cos(RADIANS(pos.i));
    const float xb = (pivot_length + pos.w) * sin(RADIANS(pos.i));
        
    // C correction
    const float xyr = HYPOT2(pos.x, pos.y);
    const float xytheta = ATAN2(pos.y, pos.x) - RADIANS(pos.j);

    delta.set(xyr * cos(xytheta) - xb, xyr * sin(xytheta), pos.z + zb - pivot_length, pos.i, pos.j);
  }
}

#endif //XYZBC_HEAD_TABLE
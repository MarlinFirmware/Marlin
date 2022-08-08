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


// Initialized by settings.load()
float segments_per_second;
float mrzp_offset;

/**
 * xyzbc head table inverse kinematics
 *
 * Calculate the joints positions for a given position, storing the result in the global delta[] array.
 * if joints control mode is active (default), input position is interpreted as joints position.
 * If rotational tool center point control mode is active, input position is interpreted as machine position using native_to_joint().
 */
void inverse_kinematics(const xyz_pos_t &raw) {
    delta = native_to_joint(raw);
}

/**
 * Calculate the joints positions for a given position.
 *
 * This is an expensive calculation.
 */
xyz_pos_t native_to_joint(const xyz_pos_t &native) {
  if (!tool_centerpoint_control) return native;

  // X and Y hotend offsets must be applied in Cartesian space with no "spoofing"
  xyz_pos_t pos = NUM_AXIS_ARRAY(
                    native.x - tool_offsets[active_extruder].x TERN_(HAS_HOTEND_OFFSET, - hotend_offset[active_extruder].x),
                    native.y - tool_offsets[active_extruder].y TERN_(HAS_HOTEND_OFFSET, - hotend_offset[active_extruder].y),
                    native.z,
                    native.i,
                    native.j
                  );

  const float offset = tool_offsets[active_extruder].z TERN_(HAS_HOTEND_OFFSET, + hotend_offset[active_extruder].z);
  const float pivot_length = mrzp_offset + offset;

  #if HAS_J_AXIS || AXIS4_NAME == 'B'
    // B correction
    const float zb = pivot_length * cos(RADIANS(pos.i)) - mrzp_offset;
    const float xb = pivot_length * sin(RADIANS(pos.i));

    #if HAS_J_AXIS
      // C correction
      const float xyr = HYPOT2(pos.x, pos.y);
      const float xytheta = ATAN2(pos.y, pos.x) - RADIANS(pos.j);
    #endif

    const xyz_pos_t joints_pos = NUM_AXIS_ARRAY(
      TERN(HAS_J_AXIS, xyr * cos(xytheta) - xb, pos.x - xb),
      TERN(HAS_J_AXIS, xyr * sin(xytheta) - xb, pos.y),
      pos.z + zb,
      pos.i,
      pos.j
    );

  #elif (!HAS_J_AXIS) && (AXIS4_NAME == 'C')
    // C correction
    const float xyr = HYPOT2(pos.x, pos.y);
    const float xytheta = ATAN2(pos.y, pos.x) - RADIANS(pos.i);

    const xyz_pos_t joints_pos = NUM_AXIS_ARRAY(
      xyr * cos(xytheta),
      xyr * sin(xytheta),
      pos.z + offset,
      pos.i,
      pos.j
    );
  #endif

  return joints_pos;
}

#endif //XYZBC_HEAD_TABLE
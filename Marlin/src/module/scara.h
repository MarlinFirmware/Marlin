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
#pragma once

/**
 * scara.h - SCARA-specific functions
 */

#include "../core/macros.h"

extern float segments_per_second;

#if ENABLED(AXEL_TPARA)

  float constexpr L1 = TPARA_LINKAGE_1, L2 = TPARA_LINKAGE_2,   // Float constants for Robot arm calculations
                  L1_2 = FLOAT_SQ(L1), L1_2_2 = 2.0 * L1_2,
                  L2_2 = FLOAT_SQ(L2);

  void forward_kinematics(const float a, const float b, const float c);
  void home_TPARA();

#else

  float constexpr L1 = SCARA_LINKAGE_1, L2 = SCARA_LINKAGE_2,   // Float constants for SCARA calculations
                  L1_2 = FLOAT_SQ(L1), L1_2_2 = 2.0 * L1_2,
                  L2_2 = FLOAT_SQ(L2);

  void forward_kinematics(const float a, const float b);

  /**
   * Tool offsets on a SCARA are bolted to the forearm, so they turn with it. The offset of a
   * tool (hotend or probe) is in the forearm's frame: X to the right of the arm's direction,
   * Y toward its tip, so it is the plain offset when the forearm points along +Y.
   * The tool is then at the end of a "virtual forearm" from the elbow: a different length at an
   * angle from the real forearm. The tool point is at L2 along the arm for a tool with no offset.
   */
  struct scara_tool_t {
    float length;   // Elbow to tool (mm)
    float angle;    // Direction of the tool from the forearm (degrees, positive is counter-clockwise)
  };
  scara_tool_t scara_tool(const xy_pos_t &offset);    // The virtual forearm for a tool offset from the L2 tool point
  scara_tool_t scara_hotend_tool();                   // ...for the active hotend (relative to T0)
  scara_tool_t scara_active_tool();                   // ...for the tool the kinematics use: the hotend, or the probe in a ScaraProbeTool

  // Where 'to' is when 'from' is at the point: the same arm, a different tool
  xy_pos_t scara_convert(const xy_pos_t &point, const scara_tool_t &from, const scara_tool_t &to);

  // The reach of a tool is a ring around the shoulder: the elbow can't fold or straighten all the way without
  // trouble, so the ring is narrowed by 'inset' mm at both edges. It excludes a dead zone around the shoulder.
  void scara_reach_ring(const scara_tool_t &tool, const float inset, float &inner, float &outer);

  // Can the tool reach the point?
  bool scara_can_reach(const xy_pos_t &point, const scara_tool_t &tool, const float inset=0);

  // Move the point to the nearest point the tool can reach, straight toward or away from the shoulder.
  // Returns true if the point had to move.
  bool scara_clamp_reach(xy_pos_t &point, const scara_tool_t &tool, const float inset=0);

  // Which way the elbow bends. 1 = counterclockwise (right-handed), -1 = clockwise. G28 sets it from the side of the
  // home angle, M665 D changes it. The elbow can only change sides through a straight arm.
  extern int8_t scara_elbow_dir;

  // Change the side of the elbow. If the arm is homed it goes out to full extension along the line from the shoulder
  // through the tool, bends the other way there, and comes back to where it was. Returns false (and leaves the elbow as
  // it was) if that can't be done: the joint ranges don't allow the straight or the other-sided arm, or the tool is at the shoulder.
  bool scara_set_elbow_dir(const int8_t dir);

  // Move the point to where the arms put the tool when the joints are kept inside their ranges (SCARA_THETA_MIN/MAX and
  // SCARA_PSI_MIN/MAX). Returns true if it had to move. Always false without ranges.
  bool scara_clamp_joints(xy_pos_t &point, const scara_tool_t &tool);

  // Can the tool reach every point of the rectangle? The reach is a disc less a disc, so the outer edge reaches the whole
  // rectangle if it reaches the corners, and the dead zone misses it if the point of the rectangle nearest the shoulder
  // is outside it.
  bool scara_rect_reachable(const xy_pos_t &corner1, const xy_pos_t &corner2, const scara_tool_t &tool, const float inset=0);

  // Move the point to the nearest point the tool can reach inside the box (the software endstops). Returns false, with
  // the point unchanged, if no point can be both (the box is entirely in the dead zone or out of reach).
  bool scara_clamp_to_limits(xy_pos_t &point, const scara_tool_t &tool, const xy_pos_t &box_min, const xy_pos_t &box_max, const float inset=0);

  // A target is kept this far inside the edges of the reach: at full extension or fold a small move of the tool
  // is a big turn of the elbow
  constexpr float SCARA_REACH_MARGIN = 0.1f;

  #if HAS_BED_PROBE
    /**
     * While this is in scope the probe is the active tool: the kinematics place the probe itself
     * at the given position, with the probe offset (M851) in the forearm's frame. On exit the
     * hotend is the active tool again and the XY position is where the arm is now, as seen by the hotend.
     * 'use' is false for a move of the nozzle, which does nothing.
     */
    class ScaraProbeTool {
      public:
        ScaraProbeTool(const bool use=true);
        ~ScaraProbeTool();
      private:
        const bool used;
    };
  #endif

#endif

void inverse_kinematics(const xyz_pos_t &raw);
void scara_set_axis_is_at_home(const AxisEnum axis);
void scara_report_positions();

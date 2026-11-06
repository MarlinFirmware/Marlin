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

/**
 * scara.cpp
 */

#include "../inc/MarlinConfig.h"

#if IS_SCARA

#include "scara.h"
#include "motion.h"
#include "planner.h"

#if ANY(HOMING_HYBRID_THRESHOLD, HAS_HOMING_CURRENT)
  #include "../feature/tmc_util.h"
#endif

#if ENABLED(AXEL_TPARA)
  #include "endstops.h"
#endif

#if ALL(SCARA, HAS_BED_PROBE)
  #include "probe.h"
#endif

//#define SCARA_DEBUG
#define DEBUG_OUT ENABLED(SCARA_DEBUG)
#include "../core/debug_out.h"

float segments_per_second = DEFAULT_SEGMENTS_PER_SECOND;

#if ENABLED(SCARA)

  constexpr xy_pos_t scara_offset = { SCARA_OFFSET_X, SCARA_OFFSET_Y };

  int8_t scara_elbow_dir = SCARA_ELBOW_DIR;

  #if defined(SCARA_THETA_MIN) || defined(SCARA_PSI_MIN)
    #define SCARA_HAS_JOINT_RANGES 1
  #endif

  /**
   * The virtual forearm of a tool at 'offset' (forearm frame) from the L2 tool point.
   * The tool is at (offset.x across, L2 + offset.y along) the forearm from the elbow.
   */
  scara_tool_t scara_tool(const xy_pos_t &offset) {
    if (!offset.x && !offset.y) return { L2, 0 };
    const float along = L2 + offset.y;
    return { HYPOT(along, offset.x), -DEGREES(ATAN2(offset.x, along)) };
  }

  /**
   * The virtual forearm of the active tool, whose offset is relative to T0.
   * The factors depend only on the offset, so they are kept until a hotend XY offset changes
   * (M218, M501, M502, the LCD, ...), which makes every change take effect without a hook.
   */
  // Set while the probe is the active tool: its offset from T0 (in the forearm's frame) replaces the hotend's
  static const xy_pos_t *scara_offset_override = nullptr;

  // The factors depend only on the offset, so they are kept until it changes
  static scara_tool_t scara_cached_tool(const xy_pos_t &offset, xy_pos_t &last_offset, scara_tool_t &tool) {
    if (offset.x != last_offset.x || offset.y != last_offset.y) {
      tool = scara_tool(offset);
      last_offset = offset;
    }
    return tool;
  }

  scara_tool_t scara_hotend_tool() {
    static xy_pos_t last_offset = { NAN, NAN };
    static scara_tool_t tool = { L2, 0 };
    xy_pos_t offset = { 0, 0 };
    #if HAS_HOTEND_OFFSET
      offset.set(motion.active_hotend_offset().x - motion.hotend_offset[0].x,
                 motion.active_hotend_offset().y - motion.hotend_offset[0].y);
    #endif
    return scara_cached_tool(offset, last_offset, tool);
  }

  scara_tool_t scara_active_tool() {
    if (!scara_offset_override) return scara_hotend_tool();
    static xy_pos_t last_offset = { NAN, NAN };
    static scara_tool_t tool = { L2, 0 };
    return scara_cached_tool(*scara_offset_override, last_offset, tool);
  }

  void scara_reach_ring(const scara_tool_t &tool, const float inset, float &inner, float &outer) {
    outer = L1 + tool.length - inset;
    inner = ABS(L1 - tool.length);
    #if MIDDLE_DEAD_ZONE_R > 0
      NOLESS(inner, float(MIDDLE_DEAD_ZONE_R));
    #endif
    inner += inset;
  }

  static bool scara_angles_allowed(const xy_pos_t &point, const scara_tool_t &tool, const int8_t dir);

  bool scara_can_reach(const xy_pos_t &point, const scara_tool_t &tool, const float inset/*=0*/) {
    float inner, outer;
    scara_reach_ring(tool, inset, inner, outer);
    const float R2 = HYPOT2(point.x - scara_offset.x, point.y - scara_offset.y);
    return outer > 0 && R2 <= sq(outer) && R2 >= sq(inner)
        && scara_angles_allowed(point, tool, scara_elbow_dir);   // ...and the joints can turn that far (always, without ranges)
  }

  bool scara_rect_reachable(const xy_pos_t &corner1, const xy_pos_t &corner2, const scara_tool_t &tool, const float inset/*=0*/) {
    float inner, outer;
    scara_reach_ring(tool, inset, inner, outer);
    if (outer <= 0) return false;
    const float x1 = _MIN(corner1.x, corner2.x), x2 = _MAX(corner1.x, corner2.x),
                y1 = _MIN(corner1.y, corner2.y), y2 = _MAX(corner1.y, corner2.y),
                dx1 = x1 - scara_offset.x, dx2 = x2 - scara_offset.x,
                dy1 = y1 - scara_offset.y, dy2 = y2 - scara_offset.y,
                nx = _MAX(x1, _MIN(x2, scara_offset.x)) - scara_offset.x,   // The nearest point of the rectangle to the shoulder
                ny = _MAX(y1, _MIN(y2, scara_offset.y)) - scara_offset.y;
    const float far2 = _MAX(_MAX(HYPOT2(dx1, dy1), HYPOT2(dx2, dy1)), _MAX(HYPOT2(dx1, dy2), HYPOT2(dx2, dy2)));
    return far2 <= sq(outer) && HYPOT2(nx, ny) >= sq(inner);
  }

  bool scara_clamp_reach(xy_pos_t &point, const scara_tool_t &tool, const float inset/*=0*/) {
    float inner, outer;
    scara_reach_ring(tool, inset, inner, outer);
    NOLESS(outer, 0.0f);
    if (inner > outer) inner = outer = (inner + outer) * 0.5f;  // Too narrow to have a ring: the middle of it

    const float dx = point.x - scara_offset.x, dy = point.y - scara_offset.y, R2 = HYPOT2(dx, dy);
    if (R2 <= sq(outer) && R2 >= sq(inner)) return false;

    // Straight toward or away from the shoulder is the nearest. At the shoulder itself any direction is, so use +Y
    const float R = SQRT(R2), r = R2 > sq(outer) ? outer : inner;
    const bool at_center = R < 0.001f;
    point.x = scara_offset.x + (at_center ? 0.0f : dx / R * r);
    point.y = scara_offset.y + (at_center ? r : dy / R * r);
    return true;
  }

  bool scara_clamp_to_limits(xy_pos_t &point, const scara_tool_t &tool, const xy_pos_t &box_min, const xy_pos_t &box_max, const float inset/*=0*/) {
    float inner, outer;
    scara_reach_ring(tool, inset, inner, outer);
    if (outer <= 0 || inner > outer) return false;

    // The nearest point of the region (the box less the dead zone and past the reach) is on its edge: along a side of the box,
    // or on an arc of one of the two circles. So try each of these, and the corners and crossings where they meet.
    constexpr float eps = 0.001f;
    const xy_pos_t c = scara_offset;
    float best_d2 = 1e30f;
    xy_pos_t best = point;
    auto try_point = [&](const float x, const float y) {
      const float R = HYPOT(x - c.x, y - c.y);
      if (x < box_min.x - eps || x > box_max.x + eps || y < box_min.y - eps || y > box_max.y + eps) return;
      if (R < inner - eps || R > outer + eps) return;
      const float d2 = HYPOT2(x - point.x, y - point.y);
      if (d2 < best_d2) { best_d2 = d2; best.set(x, y); }
    };

    const float xc = _MAX(box_min.x, _MIN(box_max.x, point.x)), yc = _MAX(box_min.y, _MIN(box_max.y, point.y));
    try_point(xc, yc);                                               // The box's nearest point
    try_point(box_min.x, yc); try_point(box_max.x, yc);              // The nearest point on each side
    try_point(xc, box_min.y); try_point(xc, box_max.y);
    const float xs[] = { box_min.x, box_max.x }, ys[] = { box_min.y, box_max.y }, radii[] = { inner, outer };
    for (uint8_t i = 0; i < 2; ++i) for (uint8_t j = 0; j < 2; ++j)
      try_point(xs[i], ys[j]);                                       // The corners

    const float R = HYPOT(point.x - c.x, point.y - c.y);
    for (uint8_t k = 0; k < 2; ++k) {
      const float r = radii[k];
      if (R > 0.001f) try_point(c.x + (point.x - c.x) / R * r, c.y + (point.y - c.y) / R * r); // The nearest point on the circle
      for (uint8_t i = 0; i < 2; ++i) {                              // Where the circle crosses each side
        const float dx = xs[i] - c.x, dy = ys[i] - c.y;
        if (ABS(dx) <= r) { const float h = SQRT(sq(r) - sq(dx)); try_point(xs[i], c.y + h); try_point(xs[i], c.y - h); }
        if (ABS(dy) <= r) { const float w = SQRT(sq(r) - sq(dy)); try_point(c.x + w, ys[i]); try_point(c.x - w, ys[i]); }
      }
    }

    if (best_d2 >= 1e30f) return false;
    point = best;
    return true;
  }

  #if HAS_BED_PROBE

    ScaraProbeTool::ScaraProbeTool(const bool use) : used(use) {
      if (used) scara_offset_override = &probe.offset_xy;
    }

    ScaraProbeTool::~ScaraProbeTool() {
      if (!used) return;
      scara_offset_override = nullptr;
      // The arm hasn't moved, but the hotend is the tool again, so its XY position is where the arm puts it now
      planner.synchronize();
      motion.get_cartesian_from_steppers();
      motion.position.x = motion.cartes.x;
      motion.position.y = motion.cartes.y;
      motion.sync_plan_position();
    }

  #endif // HAS_BED_PROBE

  /**
   * Morgan SCARA Forward Kinematics. Results in 'cartes'.
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   * Modified by dekutree64 to take elbow psi rather than distal arm relative to cartesian X axis.
   */
  // Where the tool is for the given joint angles (degrees: theta of the upper arm, and psi of the real forearm)
  static xy_pos_t scara_tip(const float theta, const float psi, const scara_tool_t &tool) {
    const float a = RADIANS(theta), b = RADIANS(theta + psi + tool.angle),
                a_sin = sin(a) * L1, a_cos = cos(a) * L1,
                b_sin = sin(b) * tool.length, b_cos = cos(b) * tool.length;
    return { a_cos + b_cos + scara_offset.x, a_sin + b_sin + scara_offset.y };
  }

  // The joint angles (radians) that put the tool at the point. 'psi' is for the real forearm, and the tool's
  // angle from it is taken out. With 'clamp' a point out of reach gets the nearest straight or folded arm.
  static void scara_angles(const xy_pos_t &point, const scara_tool_t &tool, float &theta, float &psi, const bool clamp=false) {
    const float x = point.x - scara_offset.x, y = point.y - scara_offset.y,
      cosPsi0 = (HYPOT2(x, y) - HYPOT2(L1, tool.length)) / (2.0f * L1 * tool.length),
      cosPsi = clamp ? _MAX(-1.0f, _MIN(1.0f, cosPsi0)) : cosPsi0,
      virtual_psi = ACOS(cosPsi) * scara_elbow_dir, K1 = L1 + tool.length * cosPsi, K2 = tool.length * sin(virtual_psi);
    theta = ATAN2(K1 * y - K2 * x, K1 * x + K2 * y);
    psi = virtual_psi - RADIANS(tool.angle);
  }

  xy_pos_t scara_convert(const xy_pos_t &point, const scara_tool_t &from, const scara_tool_t &to) {
    float theta, psi;
    scara_angles(point, from, theta, psi, true);
    return scara_tip(DEGREES(theta), DEGREES(psi), to);
  }

  #if SCARA_HAS_JOINT_RANGES

    /**
     * Put the joint angles (degrees) inside their ranges. The shoulder is an angle, so it is first brought into the
     * 360-degree window that starts at its minimum (the same direction, so the tool doesn't move). Returns true if
     * an angle was past an end of its range and had to be stopped there, which moves the tool.
     */
    static bool scara_limit_angles(float &theta, float &psi) {
      constexpr float eps = 0.001f;   // A point exactly at an end of a range is inside it, whatever the float math says
      bool clamped = false;
      #ifdef SCARA_THETA_MIN
        constexpr float span = float(SCARA_THETA_MAX) - float(SCARA_THETA_MIN);
        float d = theta - float(SCARA_THETA_MIN);
        d -= 360.0f * FLOOR(d / 360.0f);
        if (d > span) {                       // In the gap of the window: the nearer end of the range
          if (d - span > eps && d < 360.0f - eps) clamped = true;
          d = (d - span) <= (360.0f - d) ? span : 0.0f;
        }
        theta = float(SCARA_THETA_MIN) + d;
      #endif
      #ifdef SCARA_PSI_MIN
        if (psi > 180.0f) psi -= 360.0f; else if (psi <= -180.0f) psi += 360.0f;
        if (psi < float(SCARA_PSI_MIN))      { clamped |= (float(SCARA_PSI_MIN) - psi > eps); psi = float(SCARA_PSI_MIN); }
        else if (psi > float(SCARA_PSI_MAX)) { clamped |= (psi - float(SCARA_PSI_MAX) > eps); psi = float(SCARA_PSI_MAX); }
      #endif
      return clamped;
    }

  #endif // SCARA_HAS_JOINT_RANGES

  bool scara_clamp_joints(xy_pos_t &point, const scara_tool_t &tool) {
    #if SCARA_HAS_JOINT_RANGES
      float theta, psi;
      scara_angles(point, tool, theta, psi, true);
      theta = DEGREES(theta); psi = DEGREES(psi);
      if (!scara_limit_angles(theta, psi)) return false;
      point = scara_tip(theta, psi, tool);
      return true;
    #else
      UNUSED(point); UNUSED(tool);
      return false;
    #endif
  }

  // Do the joint ranges allow the tool at the point, with the elbow bent the given way?
  // (Declared above, where scara_can_reach uses it.)
  static bool scara_angles_allowed(const xy_pos_t &point, const scara_tool_t &tool, const int8_t dir) {
    #if SCARA_HAS_JOINT_RANGES
      const int8_t was = scara_elbow_dir;
      scara_elbow_dir = dir;
      xy_pos_t p = point;
      const bool ok = !scara_clamp_joints(p, tool);
      scara_elbow_dir = was;
      return ok;
    #else
      UNUSED(point); UNUSED(tool); UNUSED(dir);
      return true;
    #endif
  }

  bool scara_set_elbow_dir(const int8_t dir) {
    const int8_t want = dir < 0 ? -1 : 1;
    if (want == scara_elbow_dir) return true;

    // Not homed: the position isn't known, so there is nothing to move. Homing sets the side from the home angle.
    if (!motion.axis_was_homed(X_AXIS) || !motion.axis_was_homed(Y_AXIS)) {
      scara_elbow_dir = want;
      return true;
    }

    // The elbow can only change sides through a straight arm. So go out to full extension along the line from the
    // shoulder through the tool, bend the other way there, and come back. Check that the arm can do all of it first.
    const scara_tool_t tool = scara_active_tool();
    float inner, outer;
    scara_reach_ring(tool, 0, inner, outer);
    const xy_pos_t here = { motion.position.x, motion.position.y };
    const float dx = here.x - scara_offset.x, dy = here.y - scara_offset.y, R = HYPOT(dx, dy);
    if (R < 0.001f) {
      SERIAL_ERROR_MSG("The tool is at the shoulder. Move it away before changing the elbow.");
      return false;
    }
    const float k = (outer - 0.001f) / R;     // Just short of straight, which is within reach
    const xy_pos_t out = { scara_offset.x + dx * k, scara_offset.y + dy * k };

    if (!scara_angles_allowed(out, tool, scara_elbow_dir) || !scara_angles_allowed(out, tool, want) || !scara_angles_allowed(here, tool, want)) {
      SERIAL_ERROR_MSG("The joint ranges (SCARA_THETA_MIN/MAX, SCARA_PSI_MIN/MAX) don't allow the elbow to change sides here.");
      return false;
    }

    motion.blocking_move_xy(out);
    scara_elbow_dir = want;
    motion.blocking_move_xy(here);
    return true;
  }

  void forward_kinematics(const float theta, const float psi) {
    const xy_pos_t tip = scara_tip(theta, psi, scara_active_tool());
    motion.cartes.x = tip.x;
    motion.cartes.y = tip.y;

    /*
      DEBUG_ECHOLNPGM(
        "SCARA FK Angle a=", theta,
        " b=", theta + psi,
        " a_sin=", a_sin,
        " a_cos=", a_cos,
        " b_sin=", b_sin,
        " b_cos=", b_cos
      );
      DEBUG_ECHOLNPGM(" cartes (X,Y) = "(motion.cartes.x, ", ", motion.cartes.y, ")");
    //*/
  }

  void scara_set_axis_is_at_home(const AxisEnum axis) {
    if (axis == Z_AXIS) {
      motion.position[axis] = SUM_TERN(HAS_SCARA_OFFSET, motion.base_home_pos(axis), motion.scara_home_offset.c);
    }
    else {
      #ifdef SCARA_HOME_THETA
        motion.delta.a = SCARA_HOME_THETA;
        motion.delta.b = SCARA_HOME_PSI;
        if (SCARA_HOME_PSI) scara_elbow_dir = SCARA_HOME_PSI > 0 ? 1 : -1;   // The arm is bent the way it was homed
      #else
        motion.position[X_AXIS] = motion.base_home_pos(X_AXIS);
        motion.position[Y_AXIS] = motion.base_home_pos(Y_AXIS);
        inverse_kinematics(motion.position);
        motion.delta.b -= motion.delta.a * SCARA_CROSSTALK_FACTOR; // Un-apply crosstalk while adding home offset
      #endif

      #if HAS_SCARA_OFFSET
        motion.delta.a += motion.scara_home_offset.a;
        motion.delta.b += motion.scara_home_offset.b;
      #endif
      forward_kinematics(motion.delta.a, motion.delta.b);
      motion.position[axis] = motion.cartes[axis];
      // The home offset (M665) can move the home position out of bounds after the compile-time check
      if (axis == X_AXIS && (!WITHIN(motion.cartes.x, X_MIN_POS - 0.01f, X_MAX_POS + 0.01f) || !WITHIN(motion.cartes.y, Y_MIN_POS - 0.01f, Y_MAX_POS + 0.01f)))
        SERIAL_WARN_MSG("SCARA home position is outside the movement limits.");
      //DEBUG_ECHOLNPGM("homeposition (theta,psi):", motion.delta.a, ",", motion.delta.b);
      //DEBUG_ECHOLNPGM("  (x,y)", motion.position.x, ",", motion.position.y);
      motion.delta.b += motion.delta.a * SCARA_CROSSTALK_FACTOR;
    }
    motion.update_software_endstops(axis);
  }

  void inverse_kinematics(const xyz_pos_t &raw) {
    // Math adapted from RepRepFirmware ScaraKinematics.cpp https://github.com/Duet3D/RepRapFirmware
    // Solve for the active tool's virtual forearm, which is at an angle from the real one. Every segment of a
    // move comes through here, so a point the arm can't reach (past full extension, or the dead zone that a
    // straight line between reachable points can cross) goes to the nearest one that it can: the path bends
    // around the dead zone instead of the arm getting a NaN.
    const scara_tool_t tool = scara_active_tool();
    xy_pos_t point = { raw.x, raw.y };
    scara_clamp_reach(point, tool);
    float theta, psi;
    scara_angles(point, tool, theta, psi, true);
    theta = DEGREES(theta); psi = DEGREES(psi);

    // Keep each joint inside its range, which is also what turns the shoulder into the right window. A joint that is
    // stopped at the end of its range leaves the tool where the arms put it.
    TERN_(SCARA_HAS_JOINT_RANGES, scara_limit_angles(theta, psi));

    // With no range for the shoulder, atan2 gives its angle from -180 to 180, so a tool crossing the line behind the
    // shoulder would turn the arm 360 degrees the long way around. The same direction nearest the last angle is the short way.
    #ifndef SCARA_THETA_MIN
      theta += 360.0f * roundf((motion.delta.a - theta) / 360.0f);
    #endif

    motion.delta.set(theta, psi + theta * SCARA_CROSSTALK_FACTOR, raw.z);

    /*
      DEBUG_ECHOLNPGM("SCARA IK");
      DEBUG_ECHOLNPGM("  (x,y) raw ", raw.x, ",", raw.y, "  offset ", x, ",", y);
      DEBUG_ECHOLNPGM("  (theta,psi)", DEGREES(theta), ",", DEGREES(psi));
      DEBUG_ECHOLNPGM("  (a,b)", motion.delta.a, ",", motion.delta.b);
    //*/
  }

#elif ENABLED(AXEL_TPARA)

  // TPARA offset relative to the origin of the robot
  constexpr xyz_pos_t robot_shoulder_offset = { 0, 0, TPARA_SHOULDER_AXIS_HEIGHT };
  // Workspace offset relative to the origin of the robot
  constexpr xyz_pos_t robot_workspace_offset = { TPARA_OFFSET_X, TPARA_OFFSET_Y, TPARA_OFFSET_Z };
  // Tool offset relative to the tool center point of the robot
  constexpr xyz_pos_t tool_offset = { TPARA_TCP_OFFSET_X, TPARA_TCP_OFFSET_Y, TPARA_TCP_OFFSET_Z };
  // Tool offset in cylindrical coordinates (r, phi, z)
  constexpr xyz_pos_t tool_offset_cyl = { SQRT(sq(TPARA_TCP_OFFSET_X) + sq(TPARA_TCP_OFFSET_Y)) , ATAN2(TPARA_TCP_OFFSET_Y, TPARA_TCP_OFFSET_X), TPARA_TCP_OFFSET_Z };

  //xyz_pos_t home_t_w_offset = tool_offset - robot_workspace_offset;

  // Remove offset for calculation with trigonometric
  // Tool offset coordinates are recalculated for each angle
  xyz_pos_t remove_W_T_offset(const xyz_pos_t &raw) {
    // Remove workspace offset first, so we can use trigonometrics relative to robot reference
    // frame (otherwise a negative raw position would "mirror/invert" the tool offset)
    xyz_pos_t toolhead_absolute = raw + robot_workspace_offset;

    // We should apply a rotation matrix, but is too costly to calculate sin and cos
    const float r2 = HYPOT2(toolhead_absolute.x, toolhead_absolute.y);
    xyz_pos_t tool_offset_rotated;
    if (UNEAR_ZERO(r2)) {
      // avoid zero div
      tool_offset_rotated.x = tool_offset_cyl.x;
      tool_offset_rotated.y = 0.0f;
      tool_offset_rotated.z = tool_offset_cyl.z;
    }
    else {
      const float inv_r = RSQRT(r2);
      tool_offset_rotated.x = tool_offset_cyl.x * toolhead_absolute.x * inv_r;  // Equivalent to tool_offset_cyl.x * cos(atan2(toolhead_absolute.y, toolhead_absolute.x))
      tool_offset_rotated.y = tool_offset_cyl.x * toolhead_absolute.y * inv_r;  // Equivalent to tool_offset_cyl.x * sin(atan2(toolhead_absolute.y, toolhead_absolute.x))
      tool_offset_rotated.z = tool_offset_cyl.z;
    }

    return toolhead_absolute - tool_offset_rotated; // Returns the real robot position without tool or workspace offset
  }

  // Apply tool and workspace offset to robot flange position, accounting for the rotated tool offset
  constexpr xyz_pos_t apply_T_W_offset(const xyz_pos_t &rpos) {
    // We should apply a rotation matrix, but it's too costly
    const float r2 = rpos.x * rpos.x + rpos.y * rpos.y;
    if (UNEAR_ZERO(r2)) {
      // avoid zero div
      xyz_pos_t tool_offset_rotated = { tool_offset_cyl.x, 0.0f, tool_offset_cyl.z };
      return rpos + tool_offset_rotated - robot_workspace_offset;
    }
    else {
      const float inv_r = RSQRT(r2);
      xyz_pos_t tool_offset_rotated = {
        tool_offset_cyl.x * rpos.x * inv_r,
        tool_offset_cyl.x * rpos.y * inv_r,
        tool_offset_cyl.z
      };
      //DEBUG_ECHOLNPGM(" Tool_offset_rotated(x,y,z) ", tool_offset_rotated.x, ",", tool_offset_rotated.y, ",", tool_offset_rotated.z );
      return rpos + tool_offset_rotated - robot_workspace_offset;
    }
  }

  /**
   * Set an axis' current position to its home position (after homing).
   *
   * TPARA must wait for YZ homing before setting motion.position.Y/Z to home.
   * Neither Y nor Z is home until both are at home.
   */
  void scara_set_axis_is_at_home(const AxisEnum axis) {
    // Home position should be arm end position -+ offsets (+ tool offset - workspace offset), measured at home robot position
    xyz_pos_t homeposition = { X_HOME_POS, Y_HOME_POS, Z_HOME_POS };

    //DEBUG_ECHOLNPGM("TPARA Set axis is at home: ", C(iaxis_codes[axis]));
    //DEBUG_XYZ("Home: ", homeposition);
    //DEBUG_XYZ("Pos before IK: ", motion.position);
    //DEBUG_ECHOLNPGM("Angles Before: Theta: ", motion.delta.a, " Phi: ", motion.delta.b, " Psi: ", motion.delta.c);

    inverse_kinematics(homeposition);

    //DEBUG_ECHOLNPGM("Angles After IK: Theta: ", motion.delta.a, " Phi: ", motion.delta.b, " Psi: ", motion.delta.c);

    forward_kinematics(motion.delta.a, motion.delta.b, motion.delta.c);
    motion.position[axis] = motion.cartes[axis];

    //DEBUG_XYZ("'position' after FK: ", motion.position);
    //DEBUG_XYZ("'cartes' after FK: ", motion.cartes);

    motion.update_software_endstops(axis);

    //DEBUG_ECHOLNPGM("Final Angles: Theta: ", motion.delta.a, " Phi: ", motion.delta.b, " Psi: ", motion.delta.c);
    //DEBUG_XYZ("Final Pos: ", motion.position);
    //DEBUG_XYZ("Robot Offsets Shoulder:", robot_shoulder_offset);
    //DEBUG_XYZ("Robot Offsets Tool:", tool_offset);
    //DEBUG_XYZ("Robot Offsets Workspace:", robot_workspace_offset);
    //DEBUG_EOL();
  }

  // Convert ABC inputs in degrees to XYZ outputs in mm
  void forward_kinematics(const float a, const float b, const float c) {
    const float w = c - b,
                r = L1 * cos(RADIANS(b)) + L2 * sin(RADIANS(w - (90 - b))),
                x = r  * cos(RADIANS(a)),
                y = r  * sin(RADIANS(a)),
                rho2 = L1_2 + L2_2 - 2.0f * L1 * L2 * cos(RADIANS(w));

    const xyz_pos_t calculated_fk = xyz_pos_t({ x, y, SQRT(rho2 - sq(x) - sq(y)) }) ;
    motion.cartes = calculated_fk + robot_shoulder_offset + tool_offset - robot_workspace_offset;

    //DEBUG_ECHOPGM("TPARA FK Theta:", a, " Phi: ", b, " Psi: ", c);
    //DEBUG_ECHOPGM(" Calculated X':", calculated_fk.x, " Y':", calculated_fk.y, " Z':", calculated_fk.z);
    //DEBUG_XYZ(" Workspace", motion.cartes);
    //DEBUG_EOL();
  }

  // Home YZ together, then X (or all at once). Based on quick_home_xy & home_delta
  void home_TPARA() {
    // First Init the current position of all carriages to 0,0,0
    motion.position.reset();
    motion.destination.reset();
    motion.sync_plan_position();

    //DEBUG_ECHOLNPGM("Reset and sync position to the assumed start position");
    // Set the assumed start position of the robot for homing, so it home ZY axis at same time preserving the B and C motor angle
    constexpr xyz_pos_t init_w_offset = apply_T_W_offset(xyz_pos_t({ L2, 0, 0 }));

    motion.position.set(init_w_offset.x, init_w_offset.y, init_w_offset.z);
    motion.destination.set(init_w_offset.x, init_w_offset.y, init_w_offset.z);
    motion.sync_plan_position();

    // Disable stealthChop if used. Enable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      #if X_SENSORLESS
        sensorless_t stealth_states_x = motion.sensorless_axis_homing_start(X_AXIS);
      #endif
      #if Y_SENSORLESS
        sensorless_t stealth_states_y = motion.sensorless_axis_homing_start(Y_AXIS);
      #endif
      #if Z_SENSORLESS
        sensorless_t stealth_states_z = motion.sensorless_axis_homing_start(Z_AXIS);
      #endif
    #endif

    {
      // Use the homing current and Hybrid Threshold for this move only
      TERN_(HAS_HOMING_CURRENT, const TemporaryHomingCurrent temp_current(Z_AXIS));
      TERN_(HOMING_HYBRID_THRESHOLD, const TemporaryHomingHybridThreshold temp_hybrid_thrs(Z_AXIS));

      // Move to home, should move Z, Y, then X. Move X to near 0 (to avoid div by zero
      // and sign/angle stability around 0 for trigonometric functions), Y to 0 and Z to Z_MAX_LENGTH
      constexpr xyz_pos_t homing_pos_dir = apply_T_W_offset(xyz_pos_t({ 1, 0, Z_MAX_LENGTH }));
      motion.position.set(homing_pos_dir.x, homing_pos_dir.y, homing_pos_dir.z);

      motion.goto_current_position(motion.homing_feedrate(Z_AXIS));
      planner.synchronize();
    } // Restore homing current and Hybrid Threshold

    // Re-enable stealthChop if used. Disable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      TERN_(X_SENSORLESS, motion.sensorless_axis_homing_end(X_AXIS, stealth_states_x));
      TERN_(Y_SENSORLESS, motion.sensorless_axis_homing_end(Y_AXIS, stealth_states_y));
      TERN_(Z_SENSORLESS, motion.sensorless_axis_homing_end(Z_AXIS, stealth_states_z));
    #endif

    endstops.validate_homing_move();

    // At least one motor has reached its endstop.
    // Now re-home each motor separately.
    TERN_(HOME_Z_FIRST, motion.homeaxis(C_AXIS));
    motion.homeaxis(TERN(HOME_Y_BEFORE_X, B_AXIS, A_AXIS));
    motion.homeaxis(TERN(HOME_Y_BEFORE_X, A_AXIS, B_AXIS));
    IF_DISABLED(HOME_Z_FIRST, motion.homeaxis(C_AXIS));

    //DEBUG_XYZ("Position after homeaxis: ", motion.position);

    // Set all carriages to their home positions
    // Do this here all at once for Delta, because
    // XYZ isn't ABC. Applying this per-tower would
    // give the impression that they are the same.
    LOOP_NUM_AXES(i) motion.set_axis_is_at_home((AxisEnum)i);

    //DEBUG_ECHOLNPGM("Sync_plan_position after home");
    motion.sync_plan_position();
  }

  void inverse_kinematics(const xyz_pos_t &raw) {
    // Remove offsets to calculate with trigonometric
    const xyz_pos_t tpos = remove_W_T_offset(raw) - robot_shoulder_offset; //raw + robot_workspace_offset  - tool_offset_rotated - robot_shoulder_offset;
    // IK, Refer to TPARA analysis
    const float RXY = SQRT(HYPOT2(tpos.x, tpos.y)),
                RHO_2 = NORMSQ(tpos.x, tpos.y, tpos.z),
                //RHO = SQRT(RHO2),
                LSS = L1_2 + L2_2,    // L1^2 + L2^2 , LSS : Lenght square sum
                LM = 2.0f * L1 * L2,  // Length multiplication and doubled

                // Method 2
                CG = (LSS - RHO_2) / LM,    // Cosine of gamma
                SG = SQRT(1 - POW(CG, 2)),  // Sine of gamma
                K1 = L1 - L2 * CG,          // K1 projection
                K2 = L2 * SG,               // K2 projection

                // Angle of Body/base Joint
                THETA = ATAN2(tpos.y, tpos.x),

                // Angle of Elbow Joint, between L1 and L2
                //GAMMA = ACOS(CG),         // Method 1
                GAMMA = ATAN2(SG, CG),      // Method 2

                // Angle of Shoulder Joint, elevation angle measured from horizontal plane XY (r+)
                //PHI = asin(tpos.z/RHO) + asin(L2 * sin(GAMMA) / RHO), // Method 1
                PHI = ATAN2(tpos.z, RXY) + ATAN2(K2, K1),               // Method 2

                // Elbow motor angle measured from horizontal, same reference frame as shoulder angle (r+)
                PSI = PHI + GAMMA;

    motion.delta.set(DEGREES(THETA), DEGREES(PHI), DEGREES(PSI));

    //DEBUG_ECHOLNPGM(" TPARA IK raw(x,y,z) ", raw.x, ",", raw.y, ",", raw.z, " Robot pos(x,y,z) ", tpos.x, ",", tpos.y, ",", tpos.z + robot_shoulder_offset.z, " Rho^2=", RHO_2, " Theta=", DEGREES(THETA), " Phi=", DEGREES(PHI), " Psi=", DEGREES(PSI), " Gamma=", DEGREES(GAMMA));
  }

#endif // AXEL_TPARA

void scara_report_positions() {
  SERIAL_ECHOLNPGM(
    #if ENABLED(AXEL_TPARA)
        "TPARA Theta: ", planner.get_axis_position_degrees(A_AXIS)
      , " Phi: ",        planner.get_axis_position_degrees(B_AXIS)
      , " Psi: ",        planner.get_axis_position_degrees(C_AXIS)
    #else
        "SCARA Theta: ", planner.get_axis_position_degrees(A_AXIS)
      , " Psi: ",        planner.get_axis_position_degrees(B_AXIS)
    #endif
  );
  SERIAL_EOL();
}

#endif // IS_SCARA

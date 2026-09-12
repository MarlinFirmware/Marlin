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

#if HAS_HOME_OFFSET

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../lcd/marlinui.h"
#include "../../libs/buzzer.h"

/**
 * M206: Set Additional Homing Offset (X Y Z). SCARA aliases T=X, P=Y
 *
 * *** TODO: Deprecate M206 for SCARA in favor of M665.
 */
void GcodeSuite::M206() {
  if (!parser.seen_any()) return M206_report();
  LOOP_NUM_AXES(a)
    if (parser.seenval(AXIS_CHAR(a))) motion.set_home_offset((AxisEnum)a, parser.value_axis_units((AxisEnum)a));
  #if ENABLED(SCARA)
    if (parser.seenval('T')) motion.set_home_offset(A_AXIS, parser.value_float()); // Theta
    if (parser.seenval('P')) motion.set_home_offset(B_AXIS, parser.value_float()); // Psi
  #endif

  motion.report_position();
}

void GcodeSuite::M206_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_HOME_OFFSET));
  #if IS_CARTESIAN
    SERIAL_ECHOLNPGM_P(NUM_AXIS_PAIRED_LIST(
      PSTR("  M206 X"), LINEAR_UNIT(motion.home_offset.x),
      SP_Y_STR, LINEAR_UNIT(motion.home_offset.y),
      SP_Z_STR, LINEAR_UNIT(motion.home_offset.z),
      SP_I_STR, I_AXIS_UNIT(motion.home_offset.i),
      SP_J_STR, J_AXIS_UNIT(motion.home_offset.j),
      SP_K_STR, K_AXIS_UNIT(motion.home_offset.k),
      SP_U_STR, U_AXIS_UNIT(motion.home_offset.u),
      SP_V_STR, V_AXIS_UNIT(motion.home_offset.v),
      SP_W_STR, W_AXIS_UNIT(motion.home_offset.w)
    ));
  #else
    SERIAL_ECHOLNPGM_P(PSTR("  M206 Z"), LINEAR_UNIT(motion.home_offset.z));
  #endif
}

/**
 * M428: Set home_offset so that the current position becomes a known
 *       "reference point." Park the tool where the reference point
 *       should be, then send M428.
 *
 *       Where the tool is parked selects between two distinct operations:
 *
 *       - Parked near 0 this means "the tool is at 0" (e.g., park the
 *         nozzle on the bed to make that spot Z0). This is the usual
 *         case, and the only possible one for an axis homing to min,
 *         whose endstop is at (or near) 0 already.
 *
 *       - Parked near the endstop of an axis homing to max this instead
 *         means "the tool is at [XYZ]_MAX_POS". 0 is a whole axis length
 *         away, so setting 0 can't be the intent. What gets calibrated
 *         is where MAX_POS sits, not where 0 sits.
 *
 *       home_offset is applied by G28, not to the current position, so a
 *       new offset takes effect on the next homing.
 *
 *       M428 can't be used more than 2cm away from 0 or an endstop.
 *
 *       Use M206 to set these values directly.
 */
void GcodeSuite::M428() {
  if (motion.homing_needed_error()) return;

  xyz_float_t diff;
  LOOP_NUM_AXES(i) {
    // How far the coordinate system has to shift to put the reference point
    // under the tool. G28 sets the position to (base_home_pos + home_offset),
    // so the offset in effect is baked into the current position and has to be
    // carried over. Without it each M428 would measure from the result of the
    // previous one and undo it.
    diff[i] = motion.home_offset[i] - motion.position[i];       // "The tool is at 0"

    // Out of range on an axis homing to max means the tool is parked near the
    // endstop, a whole axis length away from 0. That's a request for the other
    // reference point: not "the tool is at 0" but "the tool is at MAX_POS",
    // setting where MAX_POS sits instead of where 0 sits. Measure from there.
    if (!WITHIN(diff[i], -20, 20) && motion.home_dir((AxisEnum)i) > 0)
      diff[i] += motion.base_home_pos((AxisEnum)i);             // "The tool is at MAX_POS"

    // Still out of range? The tool isn't parked near either reference point.
    if (!WITHIN(diff[i], -20, 20)) {
      SERIAL_ERROR_MSG(STR_ERR_M428_TOO_FAR);
      LCD_ALERTMESSAGE(MSG_ERR_M428_TOO_FAR);
      ERR_BUZZ();
      return;
    }
  }

  LOOP_NUM_AXES(i) motion.set_home_offset((AxisEnum)i, diff[i]);
  motion.report_position();
  LCD_MESSAGE(MSG_HOME_OFFSETS_APPLIED);
  OKAY_BUZZ();
}

#endif // HAS_HOME_OFFSET

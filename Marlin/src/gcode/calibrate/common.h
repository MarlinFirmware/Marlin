/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CALIBRATE_COMMON_H
#define CALIBRATE_COMMON_H

#if ENABLED(DELTA)

  /**
   * A delta can only safely home all axes at the same time
   * This is like quick_home_xy() but for 3 towers.
   */
  inline bool home_delta() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS(">>> home_delta", current_position);
    #endif
    // Init the current position of all carriages to 0,0,0
    ZERO(current_position);
    sync_plan_position();

    // Move all carriages together linearly until an endstop is hit.
    current_position[X_AXIS] = current_position[Y_AXIS] = current_position[Z_AXIS] = (DELTA_HEIGHT + home_offset[Z_AXIS] + 10);
    feedrate_mm_s = homing_feedrate(X_AXIS);
    line_to_current_position();
    stepper.synchronize();

    // If an endstop was not hit, then damage can occur if homing is continued.
    // This can occur if the delta height (DELTA_HEIGHT + home_offset[Z_AXIS]) is
    // not set correctly.
    if (!(Endstops::endstop_hit_bits & (_BV(X_MAX) | _BV(Y_MAX) | _BV(Z_MAX)))) {
      LCD_MESSAGEPGM(MSG_ERR_HOMING_FAILED);
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_HOMING_FAILED);
      return false;
    }

    endstops.hit_on_purpose(); // clear endstop hit flags

    // At least one carriage has reached the top.
    // Now re-home each carriage separately.
    HOMEAXIS(A);
    HOMEAXIS(B);
    HOMEAXIS(C);

    // Set all carriages to their home positions
    // Do this here all at once for Delta, because
    // XYZ isn't ABC. Applying this per-tower would
    // give the impression that they are the same.
    LOOP_XYZ(i) set_axis_is_at_home((AxisEnum)i);

    SYNC_PLAN_POSITION_KINEMATIC();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("<<< home_delta", current_position);
    #endif

    return true;
  }

#endif // DELTA

#endif // CALIBRATE_COMMON_H
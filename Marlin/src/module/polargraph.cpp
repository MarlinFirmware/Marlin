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
 * polargraph.cpp
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(POLARGRAPH)

#include "polargraph.h"
#include "motion.h"

// For homing:
#include "planner.h"
#include "endstops.h"
#include "../lcd/marlinui.h"
#include "../MarlinCore.h"

float segments_per_second; // Initialized by settings.load()

void inverse_kinematics(const xyz_pos_t &raw) {
  const float x1 = raw.x - (X_MIN_POS), x2 = (X_MAX_POS) - raw.x, y = raw.y - (Y_MAX_POS);
  delta.set(HYPOT(x1, y), HYPOT(x2, y), raw.z);
  /* // debug for one machine that is always going to 0,0
  SERIAL_ECHO(x1);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(x2);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(y);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(raw.x);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(raw.y);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(X_MIN_POS);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(X_MAX_POS);
  SERIAL_ECHO("\t");
  SERIAL_ECHO(Y_MAX_POS);
  SERIAL_ECHO("\n");*/
}

void home_polargraph() {
  // Init the current position of all carriages to 0,0,0
  current_position.reset();
  destination.reset();
  sync_plan_position();

  // Disable stealthChop if used. Enable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    TERN_(X_SENSORLESS, sensorless_t stealth_states_x = start_sensorless_homing_per_axis(X_AXIS));
    TERN_(Y_SENSORLESS, sensorless_t stealth_states_y = start_sensorless_homing_per_axis(Y_AXIS));
  #endif

  // Move all carriages together linearly until an endstop is hit.
  current_position.x=0;
  current_position.y=-Y_BED_SIZE;  // can't be reached, 
  line_to_current_position(homing_feedrate(X_AXIS));
  planner.synchronize();
  TERN_(HAS_DELTA_SENSORLESS_PROBING, endstops.report_states());

  // Re-enable stealthChop if used. Disable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING) && DISABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
    TERN_(X_SENSORLESS, end_sensorless_homing_per_axis(X_AXIS, stealth_states_x));
    TERN_(Y_SENSORLESS, end_sensorless_homing_per_axis(Y_AXIS, stealth_states_y));
  #endif

  endstops.validate_homing_move();

  // At least one carriage has reached the top.
  // Now re-home each carriage separately.
  homeaxis(A_AXIS);
  homeaxis(B_AXIS);

  // Set all motors to their home positions
  // Do this here all at once for polargraph, because
  // XY isn't AB. Applying this per-belt would
  // give the impression that they are the same.
  LOOP_ABC(i) set_axis_is_at_home((AxisEnum)i);

  sync_plan_position();
}
#endif // POLARGRAPH

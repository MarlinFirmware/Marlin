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
 * gcode.h + fwretract.h + ultralcd.cpp + language en + fr + runout.h pause.h gcode.cpp
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DYNAMIC_TOOL_MIGRATION)

#include "../../gcode.h"
#include "../../../feature/pause.h"
#include "../../../module/motion.h"
#include "../../../module/tool_change.h"
#include "../../../feature/fwretract.h"
#include "../../../module/temperature.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"

#include "../../../lcd/ultralcd.h"

uint8_t dtm_last_usable_extruder = 0;

/**
 * M606: Tool Migration
 *
 *  L[count] - Last target tool - Set the last extruder for migration : -1 to disable
 *  T[count] - Desired target tool - Force the migration to desired extruder
 *
 *  With no parameters, migrate to the next available extruder.
 */
void GcodeSuite::M606() {
  const int old_extruder = parser.intval('L', -1), new_extruder = parser.intval('T', -1);

  // Exit for invalid parameters
  if (!WITHIN(old_extruder, -1, EXTRUDERS - 1) || !WITHIN(new_extruder, -1, EXTRUDERS - 1) || new_extruder == active_extruder) return;

  // Default to the next tool
  const bool auto_migrate = (old_extruder < 0 && new_extruder < 0);

  // Set the wanted last target
  if (old_extruder >= 0) {
    dtm_last_usable_extruder = old_extruder;
    if (new_extruder < 0) return; // If just a setting then return
  }

  // Past last extruder index?
  if (auto_migrate && active_extruder >= dtm_last_usable_extruder) return;

  // Require homing before swap
  if (axis_unhomed_error()) home_all_axes();

  // Save current position
  float resume_position[XYZE];
  COPY(resume_position, current_position);

  const uint8_t selected_target = auto_migrate ? active_extruder + 1 : new_extruder;

  planner.synchronize();

  // Migrate temperature and flow
  thermalManager.setTargetHotend(thermalManager.degHotend(active_extruder), selected_target);
  planner.flow_percentage[selected_target] = planner.flow_percentage[active_extruder];

  // Migrate FWRETRACT
  #if ENABLED(FWRETRACT)
    fwretract.migrate(active_extruder, selected_target);
  #endif

  // Without human intervention it must be perfect, no wasted distances
  // If negative position or FWRETRACT retracted adjust the unload length
  // Can just be retracted, not swapped, because no dual extrusion allowed with tool migration
  float resume_e = 0;
  #if ENABLED(FWRETRACT)
    if (fwretract.retracted[active_extruder])
      resume_e = -(DTM_UNLOAD_LENGTH) + fwretract.retract_length;
    else
  #endif
      resume_e = -(DTM_UNLOAD_LENGTH) + (resume_position[E_AXIS] < 0 ? -resume_position[E_AXIS] : 0);

  if (resume_e) unscaled_relative_e_move(resume_e, MMM_TO_MMS(DTM_UNLOAD_FEEDRATE));

  // Park the nozzle by moving up by z_lift and then moving to (x_pos, y_pos)
  #if ENABLED(DTM_NOZZLE_PARK) && ENABLED(NOZZLE_PARK_FEATURE)
    Nozzle::park(2, NOZZLE_PARK_POINT);
  #endif

  // Set the new active extruder
  active_extruder = selected_target;

  // Wait for the new extruder to reach temperature
  LCD_MESSAGEPGM(MSG_FILAMENT_CHANGE_HEATING_1);
  thermalManager.wait_for_hotend(active_extruder);
  lcd_reset_status();

  // Load filament
  unscaled_relative_e_move(DTM_LOAD_LENGTH, MMM_TO_MMS(DTM_LOAD_FEEDRATE));

  // Start fan
  #if defined(DTM_FAN) && DTM_FAN < FAN_COUNT && DTM_FANSPEED > 0
    int16_t fansp = fan_speed[DTM_FAN];
    fan_speed[DTM_FAN] = DTM_FANSPEED;
  #endif

  // Purge
  unscaled_relative_e_move(DTM_PURGE_LENGTH, MMM_TO_MMS(DTM_PURGE_FEEDRATE));

  // Dwell
  #if DTM_DWELL > 0
    safe_delay(DTM_DWELL);
  #endif

  // Resume fan
  #if defined(DTM_FAN) && DTM_FAN < FAN_COUNT && DTM_FANSPEED > 0
    fan_speed[DTM_FAN] = fansp;
  #endif

  #if ENABLED(FWRETRACT)
    // Restore previous retraction
    if (fwretract.retracted[active_extruder])
      unscaled_relative_e_move(-fwretract.retract_length, fwretract.retract_feedrate_mm_s);
  #endif

  // Extruder purged and full & FwRetracted positioned, now the extruder position = 0
  planner.set_e_position_mm(0); current_position[E_AXIS] = destination[E_AXIS] = 0;

  // Position the gear for negative resume_position
  if (resume_position[E_AXIS] < 0) unscaled_relative_e_move(resume_position[E_AXIS], MMM_TO_MMS(DTM_UNLOAD_FEEDRATE));

  // Move XY to starting position, then Z
  do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

  // Set Z_AXIS to saved position
  do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);

  // Extrusion positions are resumed and ready to be confirmed
  // Set extruder to saved position
  planner.set_e_position_mm(destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS]);
}

#endif // DYNAMIC_TOOL_MIGRATION

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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)

#include "../../gcode.h"
#include "../../../feature/pause.h"
#include "../../../module/motion.h"
#include "../../../module/printcounter.h"

#if EXTRUDERS > 1
  #include "../../../module/tool_change.h"
#endif

#if HAS_LCD_MENU
  #include "../../../lcd/ultralcd.h"
#endif

/**
 * M600: Pause for filament change
 *
 *  E[distance] - Retract the filament this far
 *  Z[distance] - Move the Z axis by this distance
 *  X[position] - Move to this X position, with Y
 *  Y[position] - Move to this Y position, with X
 *  U[distance] - Retract distance for removal (manual reload)
 *  L[distance] - Extrude distance for insertion (manual reload)
 *  B[count]    - Number of times to beep, -1 for indefinite (if equipped with a buzzer)
 *  T[toolhead] - Select extruder for filament change
 *
 *  Default values are used for omitted arguments.
 */
void GcodeSuite::M600() {
  point_t park_point = NOZZLE_PARK_POINT;

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  #if ENABLED(DUAL_X_CARRIAGE)
    int8_t DXC_ext = target_extruder;
    if (!parser.seen('T')) {  // If no tool index is specified, M600 was (probably) sent in response to filament runout.
                              // In this case, for duplicating modes set DXC_ext to the extruder that ran out.
      #if ENABLED(FILAMENT_RUNOUT_SENSOR) && NUM_RUNOUT_SENSORS > 1
        if (dxc_is_duplicating())
          DXC_ext = (READ(FIL_RUNOUT2_PIN) == FIL_RUNOUT_INVERTING) ? 1 : 0;
      #else
        DXC_ext = active_extruder;
      #endif
    }
  #endif

  // Show initial "wait for start" message
  #if HAS_LCD_MENU
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INIT, ADVANCED_PAUSE_MODE_PAUSE_PRINT, target_extruder);
  #endif

  #if ENABLED(HOME_BEFORE_FILAMENT_CHANGE)
    // Don't allow filament change without homing first
    if (axis_unhomed_error()) gcode.home_all_axes();
  #endif

  #if EXTRUDERS > 1
    // Change toolhead if specified
    const uint8_t active_extruder_before_filament_change = active_extruder;
    if (
      active_extruder != target_extruder
      #if ENABLED(DUAL_X_CARRIAGE)
        && dual_x_carriage_mode != DXC_DUPLICATION_MODE && dual_x_carriage_mode != DXC_SCALED_DUPLICATION_MODE
      #endif
    ) tool_change(target_extruder, 0, false);
  #endif

  // Initial retract before move to filament change position
  const float retract = -ABS(parser.seen('E') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_PARK_RETRACT_LENGTH
      + (PAUSE_PARK_RETRACT_LENGTH)
    #endif
  );

  // Lift Z axis
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  // Move XY axes to filament change position or given position
  if (parser.seenval('X')) park_point.x = parser.linearval('X');
  if (parser.seenval('Y')) park_point.y = parser.linearval('Y');

  #if HAS_HOTEND_OFFSET && DISABLED(DUAL_X_CARRIAGE) && DISABLED(DELTA)
    park_point.x += (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0);
    park_point.y += (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0);
  #endif

  // Unload filament
  const float unload_length = -ABS(parser.seen('U') ? parser.value_axis_units(E_AXIS)
                                                     : fc_settings[active_extruder].unload_length);

  // Slow load filament
  constexpr float slow_load_length = FILAMENT_CHANGE_SLOW_LOAD_LENGTH;

  // Fast load filament
  const float fast_load_length = ABS(parser.seen('L') ? parser.value_axis_units(E_AXIS)
                                                       : fc_settings[active_extruder].load_length);

  const int beep_count = parser.intval('B',
    #ifdef FILAMENT_CHANGE_ALERT_BEEPS
      FILAMENT_CHANGE_ALERT_BEEPS
    #else
      -1
    #endif
  );

  if (pause_print(retract, park_point, unload_length, true DXC_PASS)) {
    wait_for_confirmation(true, beep_count DXC_PASS);
    resume_print(slow_load_length, fast_load_length, ADVANCED_PAUSE_PURGE_LENGTH, beep_count DXC_PASS);
  }

  #if EXTRUDERS > 1
    // Restore toolhead if it was changed
    if (active_extruder_before_filament_change != active_extruder)
      tool_change(active_extruder_before_filament_change, 0, false);
  #endif
}

#endif // ADVANCED_PAUSE_FEATURE

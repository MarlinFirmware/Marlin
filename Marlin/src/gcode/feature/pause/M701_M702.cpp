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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)

#include "../../gcode.h"
#include "../../../Marlin.h"
#include "../../../module/motion.h"
#include "../../../module/temperature.h"
#include "../../../libs/point_t.h"

#if EXTRUDERS > 1
  #include "../../../module/tool_change.h"
#endif

#if ENABLED(ULTIPANEL)
  #include "../../../lcd/ultralcd.h"
#endif

/**
 * M701: Load filament
 *
 *  T<extruder> - Optional extruder number. Current extruder if omitted.
 *  Z<distance> - Move the Z axis by this distance
 *  L<distance> - Extrude distance for insertion (positive value) (manual reload)
 *
 *  Default values are used for omitted arguments.
 */
void GcodeSuite::M701() {
  point_t park_point = NOZZLE_PARK_POINT;

  #if ENABLED(NO_MOTION_BEFORE_HOMING)
    // Only raise Z if the machine is homed
    if (axis_unhomed_error()) park_point.z = 0;
  #endif

  if (get_target_extruder_from_command()) return;

  // Z axis lift
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  // Show initial "wait for load" message
  #if ENABLED(ULTIPANEL)
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_LOAD, ADVANCED_PAUSE_MODE_LOAD_FILAMENT, target_extruder);
  #endif

  #if EXTRUDERS > 1
    // Change toolhead if specified
    uint8_t active_extruder_before_filament_change = active_extruder;
    if (active_extruder != target_extruder)
      tool_change(target_extruder, 0, true);
  #endif

  // Lift Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(MIN(current_position[Z_AXIS] + park_point.z, Z_MAX_POS), NOZZLE_PARK_Z_FEEDRATE);

  // Load filament
  constexpr float slow_load_length = FILAMENT_CHANGE_SLOW_LOAD_LENGTH;
  const float fast_load_length = ABS(parser.seen('L') ? parser.value_axis_units(E_AXIS)
                                                       : filament_change_load_length[active_extruder]);
  load_filament(slow_load_length, fast_load_length, ADVANCED_PAUSE_PURGE_LENGTH, FILAMENT_CHANGE_ALERT_BEEPS,
                true, thermalManager.still_heating(target_extruder), ADVANCED_PAUSE_MODE_LOAD_FILAMENT
                #if ENABLED(DUAL_X_CARRIAGE)
                  , target_extruder
                #endif
              );

  // Restore Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(MAX(current_position[Z_AXIS] - park_point.z, 0), NOZZLE_PARK_Z_FEEDRATE);

  #if EXTRUDERS > 1
    // Restore toolhead if it was changed
    if (active_extruder_before_filament_change != active_extruder)
      tool_change(active_extruder_before_filament_change, 0, true);
  #endif

  // Show status screen
  #if ENABLED(ULTIPANEL)
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
  #endif
}

/**
 * M702: Unload filament
 *
 *  T<extruder> - Optional extruder number. If omitted, current extruder
 *                (or ALL extruders with FILAMENT_UNLOAD_ALL_EXTRUDERS).
 *  Z<distance> - Move the Z axis by this distance
 *  U<distance> - Retract distance for removal (manual reload)
 *
 *  Default values are used for omitted arguments.
 */
void GcodeSuite::M702() {
  point_t park_point = NOZZLE_PARK_POINT;

  #if ENABLED(NO_MOTION_BEFORE_HOMING)
    // Only raise Z if the machine is homed
    if (axis_unhomed_error()) park_point.z = 0;
  #endif

  if (get_target_extruder_from_command()) return;

  // Z axis lift
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  // Show initial "wait for unload" message
  #if ENABLED(ULTIPANEL)
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_UNLOAD, ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, target_extruder);
  #endif

  #if EXTRUDERS > 1
    // Change toolhead if specified
    uint8_t active_extruder_before_filament_change = active_extruder;
    if (active_extruder != target_extruder)
      tool_change(target_extruder, 0, true);
  #endif

  // Lift Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(MIN(current_position[Z_AXIS] + park_point.z, Z_MAX_POS), NOZZLE_PARK_Z_FEEDRATE);

  // Unload filament
  #if EXTRUDERS > 1 && ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
    if (!parser.seenval('T')) {
      HOTEND_LOOP() {
        if (e != active_extruder) tool_change(e, 0, true);
        unload_filament(-filament_change_unload_length[e], true, ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT);
      }
    }
    else
  #endif
  {
    // Unload length
    const float unload_length = -ABS(parser.seen('U') ? parser.value_axis_units(E_AXIS) :
                                                        filament_change_unload_length[target_extruder]);

    unload_filament(unload_length, true, ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT);
  }

  // Restore Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(MAX(current_position[Z_AXIS] - park_point.z, 0), NOZZLE_PARK_Z_FEEDRATE);

  #if EXTRUDERS > 1
    // Restore toolhead if it was changed
    if (active_extruder_before_filament_change != active_extruder)
      tool_change(active_extruder_before_filament_change, 0, true);
  #endif

  // Show status screen
  #if ENABLED(ULTIPANEL)
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
  #endif
}

#endif // ADVANCED_PAUSE_FEATURE

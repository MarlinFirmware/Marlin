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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(SCREWS_TILT_ADJUST)

  #include "../gcode.h"
  #include "../../module/planner.h"
  #include "../../module/probe.h"
  #include "../../feature/bedlevel/bedlevel.h"

  #define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
  #include "../../core/debug_out.h"

  //
  // Sanity check
  //
  constexpr xy_pos_t screws_tilt_adjust_pos[] = SCREWS_TILT_ADJUST_PROBE_XY;
  constexpr const char * const screws_tilt_adjust_names[] = SCREWS_TILT_ADJUST_PROBE_NAMES;

  #define G35_PROBE_COUNT COUNT(screws_tilt_adjust_pos)
  #define G35_PROBE_COUNT_NAMES COUNT(screws_tilt_adjust_names)

  static_assert((SCREWS_TILT_ADJUST_THREAD >= 0) && 
                (SCREWS_TILT_ADJUST_THREAD <= 5) && 
                (trunc(SCREWS_TILT_ADJUST_THREAD) == SCREWS_TILT_ADJUST_THREAD),
    "SCREWS_TILT_ADJUST_THREAD must have a integer number between 0 and 5."
  );

  static_assert(G35_PROBE_COUNT > 2,
    "SCREWS_TILT_ADJUST_PROBE_XY requires at least 3 {X,Y} positions."
  );

  static_assert(G35_PROBE_COUNT == G35_PROBE_COUNT_NAMES,
    "SCREWS_TILT_ADJUST_PROBE_XY and SCREWS_TILT_ADJUST_PROBE_NAMES must have the same number of elements."
  );

  /**
   * G35: Read bed corners to help adjust bed screws
   * 
   *   T<screw_thread>
   * 
   * screw_thread: 0 - Clockwise M3
   *               1 - Counter-Clockwise M3
   *               2 - Clockwise M4
   *               3 - Counter-Clockwise M4
   *               4 - Clockwise M5
   *               5 - Counter-Clockwise M5
   **/
  void GcodeSuite::G35() {
    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOLNPGM(">>> G35");
      log_machine_info();
    }

    bool err_break = false;
    float z_measured[G35_PROBE_COUNT] = { 0 };

    const int8_t screws_tilt_adjust_thread = parser.intval('T', SCREWS_TILT_ADJUST_THREAD);
    if (!WITHIN(screws_tilt_adjust_thread, 0, 5)) {
      SERIAL_ECHOLNPGM("?(T)hread out of bounds (0-5).");
      err_break = true;
    }

    if (!err_break)
    {
      // Wait for planner moves to finish!
      planner.synchronize();

      // Disable the leveling matrix before auto-aligning
      #if HAS_LEVELING
        #if ENABLED(RESTORE_LEVELING_AFTER_G35)
          const bool leveling_was_active = planner.leveling_active;
        #endif
        set_bed_leveling_enabled(false);
      #endif

      #if ENABLED(CNC_WORKSPACE_PLANES)
        workspace_plane = PLANE_XY;
      #endif

      // Always home with tool 0 active
      #if HOTENDS > 1
        const uint8_t old_tool_index = active_extruder;
        tool_change(0, true);
      #endif

      #if HAS_DUPLICATION_MODE
        extruder_duplication_enabled = false;
      #endif

      // Home before the probe procedure
      home_all_axes();

      #if BOTH(BLTOUCH, BLTOUCH_HS_MODE)
          // In BLTOUCH HS mode, the probe travels in a deployed state.
          // Users of G35 might have a badly misaligned bed, so raise Z by the
          // length of the deployed pin (BLTOUCH stroke < 7mm)
        #define Z_BASIC_CLEARANCE Z_CLEARANCE_BETWEEN_PROBES + 7.0f
      #else
        #define Z_BASIC_CLEARANCE Z_CLEARANCE_BETWEEN_PROBES
      #endif

      // Probe all positions
      for (uint8_t i = 0; i < G35_PROBE_COUNT; ++i) {

        current_position.z = Z_BASIC_CLEARANCE;

        // Probe a Z height.
        const float z_probed_height = probe.probe_at_point(screws_tilt_adjust_pos[i], PROBE_PT_RAISE, 0, true);

        if (isnan(z_probed_height)) {
          SERIAL_ECHOLNPAIR("Probing failed: Point (",screws_tilt_adjust_names[i],") ",i,
                                                " X=", screws_tilt_adjust_pos[i].x, 
                                                " Y=", screws_tilt_adjust_pos[i].y);
          err_break = true;
          break;
        }

        if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPAIR("Probing: Point (",screws_tilt_adjust_names[i],") ",i,
                                           " X=", screws_tilt_adjust_pos[i].x, 
                                           " Y=", screws_tilt_adjust_pos[i].y, 
                                           " Z=", z_probed_height);
        }

        // Add height to each value
        z_measured[i] = z_probed_height;

      } // for

      if (!err_break)
      {
        float z_base = z_measured[0];
        float threads_factor[] = {0.5, 0.5, 0.7, 0.7, 0.8, 0.8};
        float adjust;

        // Calculate adjusts
        for (uint8_t i = 1; i < G35_PROBE_COUNT; ++i) {

          float z = z_measured[i];
          float diff = z_base - z;

          if (abs(diff) < 0.001f)
          {
            adjust = 0;
          }
          else
          {
            adjust = diff / threads_factor[SCREWS_TILT_ADJUST_THREAD];
          }

          long full_turns = trunc(adjust);
          float decimal_part = adjust - (float)full_turns;
          long minutes = trunc((float)(decimal_part * 60.0f));

          if ((SCREWS_TILT_ADJUST_THREAD % 2) == 0)
          {
            SERIAL_ECHOLNPAIR("Adjust ",screws_tilt_adjust_names[i],
                   "\n    Direction = ", ((adjust < 0) ? "Counter-Clockwise" : "Clockwise"), 
                   "\n   Full turns = ", abs(full_turns), 
                   "\n      Minutes = ", abs(minutes));
          }
          else 
          {
            SERIAL_ECHOLNPAIR("Adjust ",screws_tilt_adjust_names[i],
                   "\n    Direction = ", ((adjust < 0) ? "Clockwise" : "Counter-Clockwise"), 
                   "\n   Full turns = ", abs(full_turns), 
                   "\n      Minutes = ", abs(minutes));
          } 
        } // for
      }
      else
      {
        SERIAL_ECHOLNPGM("G35 aborted.");
      }   

      // Restore the active tool after homing
      #if HOTENDS > 1     
        tool_change(old_tool_index, (
          #if ENABLED(PARKING_EXTRUDER)
            false // Fetch the previous toolhead
          #else
            true
          #endif
        ));
      #endif

      #if HAS_LEVELING && ENABLED(RESTORE_LEVELING_AFTER_G35)
        set_bed_leveling_enabled(leveling_was_active);
      #endif
      
      // Stow the probe, as the last call to probe.probe_at_point(...) left
      // the probe deployed if it was successful.
      probe.stow();

      // After this operation the z position needs correction
      set_axis_not_trusted(Z_AXIS);

      // Home Z after the alignment procedure
      process_subcommands_now_P(PSTR("G28Z"));  
    }

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G35");
}

#endif // SCREWS_TILT_ADJUST

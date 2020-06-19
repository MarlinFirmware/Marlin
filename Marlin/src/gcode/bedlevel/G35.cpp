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

#if ENABLED(ASSISTED_TRAMMING)

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

constexpr xy_pos_t screws_tilt_adjust_pos[] = TRAMMING_POINT_XY;

static PGMSTR(point_name_1, TRAMMING_POINT_NAME_1);
static PGMSTR(point_name_2, TRAMMING_POINT_NAME_2);
static PGMSTR(point_name_3, TRAMMING_POINT_NAME_3);
#ifdef TRAMMING_POINT_NAME_4
  static PGMSTR(point_name_4, TRAMMING_POINT_NAME_4);
  #ifdef TRAMMING_POINT_NAME_5
    static PGMSTR(point_name_5, TRAMMING_POINT_NAME_5);
  #endif
#endif

static PGM_P const tramming_point_name[] PROGMEM = {
  point_name_1, point_name_2, point_name_3
  #ifdef TRAMMING_POINT_NAME_4
    , point_name_4
    #ifdef TRAMMING_POINT_NAME_5
      , point_name_5
    #endif
  #endif
};

#define G35_PROBE_COUNT COUNT(screws_tilt_adjust_pos)

#if !WITHIN(TRAMMING_SCREW_THREAD, 0, 5)
  #error "TRAMMING_SCREW_THREAD must be an integer from 0 to 5."
#endif

static_assert(G35_PROBE_COUNT > 2, "TRAMMING_POINT_XY requires at least 3 XY positions.");

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

  const int8_t screws_tilt_adjust_thread = parser.intval('T', TRAMMING_SCREW_THREAD);
  if (!WITHIN(screws_tilt_adjust_thread, 0, 5)) {
    SERIAL_ECHOLNPGM("?(T)hread out of bounds (0-5).");
    err_break = true;
  }

  if (!err_break) {
    // Wait for planner moves to finish!
    planner.synchronize();

    // Disable the leveling matrix before auto-aligning
    #if HAS_LEVELING
      TERN_(RESTORE_LEVELING_AFTER_G35, const bool leveling_was_active = planner.leveling_active);
      set_bed_leveling_enabled(false);
    #endif

    #if ENABLED(CNC_WORKSPACE_PLANES)
      workspace_plane = PLANE_XY;
    #endif

    // Always home with tool 0 active
    #if HAS_MULTI_HOTEND
      const uint8_t old_tool_index = active_extruder;
      tool_change(0, true);
    #endif

    #if HAS_DUPLICATION_MODE
      extruder_duplication_enabled = false;
    #endif

    // Home all before this procedure
    home_all_axes();

    // Probe all positions
    for (uint8_t i = 0; i < G35_PROBE_COUNT; ++i) {

      // In BLTOUCH HS mode, the probe travels in a deployed state.
      // Users of G35 might have a badly misaligned bed, so raise Z by the
      // length of the deployed pin (BLTOUCH stroke < 7mm)
      current_position.z = (Z_CLEARANCE_BETWEEN_PROBES) + (7 * ENABLED(BLTOUCH_HS_MODE));

      const float z_probed_height = probe.probe_at_point(screws_tilt_adjust_pos[i], PROBE_PT_RAISE, 0, true);

      if (isnan(z_probed_height)) {
        SERIAL_ECHOLNPAIR("G35 failed at point ", i, " (", tramming_point_name[i], ")"
                          " X", screws_tilt_adjust_pos[i].x,
                          " Y", screws_tilt_adjust_pos[i].y);
        err_break = true;
        break;
      }

      if (DEBUGGING(LEVELING))
        DEBUG_ECHOLNPAIR("Probing point ", int(i), " (", tramming_point_name[i], ")"
                         " X", screws_tilt_adjust_pos[i].x,
                         " Y", screws_tilt_adjust_pos[i].y,
                         " Z", z_probed_height);

      z_measured[i] = z_probed_height;
    }

    if (!err_break) {
      float z_base = z_measured[0];
      float threads_factor[] = {0.5, 0.5, 0.7, 0.7, 0.8, 0.8};
      float adjust;

      // Calculate adjusts
      LOOP_S_L_N(i, 1, G35_PROBE_COUNT) {
        const float diff = z_base - z_measured[i];
        adjust = abs(diff) < 0.001f ? 0 : diff / threads_factor[screws_tilt_adjust_thread];

        const int full_turns = trunc(adjust);
        const float decimal_part = adjust - float(full_turns);
        const int minutes = trunc(decimal_part * 60.0f);

        SERIAL_ECHOPAIR("Turn ", tramming_point_name[i],
               " ", ((!(screws_tilt_adjust_thread & 1)) == (adjust < 0)) ? "Counter-Clockwise" : "Clockwise",
               "by ", abs(full_turns), " turns");
        if (minutes) SERIAL_ECHOPAIR(" and ", abs(minutes), " minutes");
        SERIAL_EOL();
      }
    }
    else
      SERIAL_ECHOLNPGM("G35 aborted.");

    // Restore the active tool after homing
    #if HAS_MULTI_HOTEND
      tool_change(old_tool_index, DISABLED(PARKING_EXTRUDER)); // Fetch previous toolhead if not PARKING_EXTRUDER
    #endif

    #if BOTH(HAS_LEVELING, RESTORE_LEVELING_AFTER_G35)
      set_bed_leveling_enabled(leveling_was_active);
    #endif

    // Stow the probe, as the last call to probe.probe_at_point(...) left
    // the probe deployed if it was successful.
    probe.stow();

    // After this operation the Z position needs correction
    set_axis_not_trusted(Z_AXIS);

    // Home Z after the alignment procedure
    process_subcommands_now_P(PSTR("G28Z"));
  }

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G35");
}

#endif // ASSISTED_TRAMMING

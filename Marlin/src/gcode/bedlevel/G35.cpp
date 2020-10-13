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

#if ENABLED(ASSISTED_TRAMMING)

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"

#if HAS_MULTI_HOTEND
  #include "../../module/tool_change.h"
#endif

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

#if !WITHIN(TRAMMING_SCREW_THREAD, 30, 51) || TRAMMING_SCREW_THREAD % 10 > 1
  #error "TRAMMING_SCREW_THREAD must be equal to 30, 31, 40, 41, 50, or 51."
#endif

static_assert(G35_PROBE_COUNT > 2, "TRAMMING_POINT_XY requires at least 3 XY positions.");

/**
 * G35: Read bed corners to help adjust bed screws
 *
 *   S<screw_thread>
 *
 * Screw thread: 30 - Clockwise M3
 *               31 - Counter-Clockwise M3
 *               40 - Clockwise M4
 *               41 - Counter-Clockwise M4
 *               50 - Clockwise M5
 *               51 - Counter-Clockwise M5
 **/
void GcodeSuite::G35() {
  DEBUG_SECTION(log_G35, "G35", DEBUGGING(LEVELING));

  if (DEBUGGING(LEVELING)) log_machine_info();

  float z_measured[G35_PROBE_COUNT] = { 0 };

  const uint8_t screw_thread = parser.byteval('S', TRAMMING_SCREW_THREAD);
  if (!WITHIN(screw_thread, 30, 51) || screw_thread % 10 > 1) {
    SERIAL_ECHOLNPGM("?(S)crew thread must be 30, 31, 40, 41, 50, or 51.");
    return;
  }

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

  bool err_break = false;

  // Probe all positions
  LOOP_L_N(i, G35_PROBE_COUNT) {

    // In BLTOUCH HS mode, the probe travels in a deployed state.
    // Users of G35 might have a badly misaligned bed, so raise Z by the
    // length of the deployed pin (BLTOUCH stroke < 7mm)
    current_position.z = (Z_CLEARANCE_BETWEEN_PROBES) + (7 * ENABLED(BLTOUCH_HS_MODE));

    const float z_probed_height = probe.probe_at_point(screws_tilt_adjust_pos[i], PROBE_PT_RAISE, 0, true);

    if (isnan(z_probed_height)) {
      SERIAL_ECHOPAIR("G35 failed at point ", int(i), " (", tramming_point_name[i], ")");
      SERIAL_ECHOLNPAIR_P(SP_X_STR, screws_tilt_adjust_pos[i].x, SP_Y_STR, screws_tilt_adjust_pos[i].y);
      err_break = true;
      break;
    }

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPAIR("Probing point ", int(i), " (", tramming_point_name[i], ")");
      SERIAL_ECHOLNPAIR_P(SP_X_STR, screws_tilt_adjust_pos[i].x, SP_Y_STR, screws_tilt_adjust_pos[i].y, SP_Z_STR, z_probed_height);
    }

    z_measured[i] = z_probed_height;
  }

  if (!err_break) {
    const float threads_factor[] = { 0.5, 0.7, 0.8 };

    // Calculate adjusts
    LOOP_S_L_N(i, 1, G35_PROBE_COUNT) {
      const float diff = z_measured[0] - z_measured[i],
                  adjust = abs(diff) < 0.001f ? 0 : diff / threads_factor[(screw_thread - 30) / 10];

      const int full_turns = trunc(adjust);
      const float decimal_part = adjust - float(full_turns);
      const int minutes = trunc(decimal_part * 60.0f);

      SERIAL_ECHOPAIR("Turn ", tramming_point_name[i],
             " ", (screw_thread & 1) == (adjust > 0) ? "CCW" : "CW",
             " by ", abs(full_turns), " turns");
      if (minutes) SERIAL_ECHOPAIR(" and ", abs(minutes), " minutes");
      if (ENABLED(REPORT_TRAMMING_MM)) SERIAL_ECHOPAIR(" (", -diff, "mm)");
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
  set_axis_never_homed(Z_AXIS);

  // Home Z after the alignment procedure
  process_subcommands_now_P(PSTR("G28Z"));
}

#endif // ASSISTED_TRAMMING

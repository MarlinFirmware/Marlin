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

#if HAS_LEVELING

#include "bedlevel.h"
#include "../../module/planner.h"

#if EITHER(MESH_BED_LEVELING, PROBE_MANUALLY)
  #include "../../module/motion.h"
#endif

#if ENABLED(PROBE_MANUALLY)
  bool g29_in_progress = false;
#endif

#if ENABLED(LCD_BED_LEVELING)
  #include "../../lcd/marlinui.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

bool leveling_is_valid() {
  return TERN1(MESH_BED_LEVELING,          mbl.has_mesh())
      && TERN1(AUTO_BED_LEVELING_BILINEAR, bbl.has_mesh())
      && TERN1(AUTO_BED_LEVELING_UBL,      ubl.mesh_is_valid());
}

/**
 * Turn bed leveling on or off, fixing the current
 * position as-needed.
 *
 * Disable: Current position = physical position
 *  Enable: Current position = "unleveled" physical position
 */
void set_bed_leveling_enabled(const bool enable/*=true*/) {

  const bool can_change = TERN1(AUTO_BED_LEVELING_BILINEAR, !enable || leveling_is_valid());

  if (can_change && enable != planner.leveling_active) {

    planner.synchronize();

    if (planner.leveling_active) {      // leveling from on to off
      if (DEBUGGING(LEVELING)) DEBUG_POS("Leveling ON", current_position);
      // change unleveled current_position to physical current_position without moving steppers.
      planner.apply_leveling(current_position);
      planner.leveling_active = false;  // disable only AFTER calling apply_leveling
      if (DEBUGGING(LEVELING)) DEBUG_POS("...Now OFF", current_position);
    }
    else {                              // leveling from off to on
      if (DEBUGGING(LEVELING)) DEBUG_POS("Leveling OFF", current_position);
      planner.leveling_active = true;   // enable BEFORE calling unapply_leveling, otherwise ignored
      // change physical current_position to unleveled current_position without moving steppers.
      planner.unapply_leveling(current_position);
      if (DEBUGGING(LEVELING)) DEBUG_POS("...Now ON", current_position);
    }

    sync_plan_position();
  }
}

TemporaryBedLevelingState::TemporaryBedLevelingState(const bool enable) : saved(planner.leveling_active) {
  set_bed_leveling_enabled(enable);
}

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

  void set_z_fade_height(const_float_t zfh, const bool do_report/*=true*/) {

    if (planner.z_fade_height == zfh) return;

    const bool leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);

    planner.set_z_fade_height(zfh);

    if (leveling_was_active) {
      const xyz_pos_t oldpos = current_position;
      set_bed_leveling_enabled(true);
      if (do_report && oldpos != current_position)
        report_current_position();
    }
  }

#endif // ENABLE_LEVELING_FADE_HEIGHT

/**
 * Reset calibration results to zero.
 */
void reset_bed_level() {
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("reset_bed_level");
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    ubl.reset();
  #else
    set_bed_leveling_enabled(false);
    #if ENABLED(MESH_BED_LEVELING)
      mbl.reset();
    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
      bbl.reset();
    #elif ABL_PLANAR
      planner.bed_level_matrix.set_to_identity();
    #endif
  #endif
}

#if EITHER(AUTO_BED_LEVELING_BILINEAR, MESH_BED_LEVELING)

  /**
   * Enable to produce output in JSON format suitable
   * for SCAD or JavaScript mesh visualizers.
   *
   * Visualize meshes in OpenSCAD using the included script.
   *
   *   buildroot/shared/scripts/MarlinMesh.scad
   */
  //#define SCAD_MESH_OUTPUT

  /**
   * Print calibration results for plotting or manual frame adjustment.
   */
  void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, const float *values) {
    #ifndef SCAD_MESH_OUTPUT
      LOOP_L_N(x, sx) {
        serial_spaces(precision + (x < 10 ? 3 : 2));
        SERIAL_ECHO(x);
      }
      SERIAL_EOL();
    #endif
    #ifdef SCAD_MESH_OUTPUT
      SERIAL_ECHOLNPGM("measured_z = ["); // open 2D array
    #endif
    LOOP_L_N(y, sy) {
      #ifdef SCAD_MESH_OUTPUT
        SERIAL_ECHOPGM(" [");             // open sub-array
      #else
        if (y < 10) SERIAL_CHAR(' ');
        SERIAL_ECHO(y);
      #endif
      LOOP_L_N(x, sx) {
        SERIAL_CHAR(' ');
        const float offset = values[x * sx + y];
        if (!isnan(offset)) {
          if (offset >= 0) SERIAL_CHAR('+');
          SERIAL_ECHO_F(offset, int(precision));
        }
        else {
          #ifdef SCAD_MESH_OUTPUT
            for (uint8_t i = 3; i < precision + 3; i++)
              SERIAL_CHAR(' ');
            SERIAL_ECHOPGM("NAN");
          #else
            LOOP_L_N(i, precision + 3)
              SERIAL_CHAR(i ? '=' : ' ');
          #endif
        }
        #ifdef SCAD_MESH_OUTPUT
          if (x < sx - 1) SERIAL_CHAR(',');
        #endif
      }
      #ifdef SCAD_MESH_OUTPUT
        SERIAL_ECHOPGM(" ]");            // close sub-array
        if (y < sy - 1) SERIAL_CHAR(',');
      #endif
      SERIAL_EOL();
    }
    #ifdef SCAD_MESH_OUTPUT
      SERIAL_ECHOPGM("];");               // close 2D array
    #endif
    SERIAL_EOL();
  }

#endif // AUTO_BED_LEVELING_BILINEAR || MESH_BED_LEVELING

#if EITHER(MESH_BED_LEVELING, PROBE_MANUALLY)

  void _manual_goto_xy(const xy_pos_t &pos) {

    // Get the resting Z position for after the XY move
    #ifdef MANUAL_PROBE_START_Z
      constexpr float finalz = _MAX(0, MANUAL_PROBE_START_Z); // If a MANUAL_PROBE_START_Z value is set, always respect it
    #else
      #warning "It's recommended to set some MANUAL_PROBE_START_Z value for manual leveling."
    #endif
    #if Z_CLEARANCE_BETWEEN_MANUAL_PROBES > 0     // A probe/obstacle clearance exists so there is a raise:
      #ifndef MANUAL_PROBE_START_Z
        const float finalz = current_position.z;  // - Use the current Z for starting-Z if no MANUAL_PROBE_START_Z was provided
      #endif
      do_blocking_move_to_xy_z(pos, Z_CLEARANCE_BETWEEN_MANUAL_PROBES); // - Raise Z, then move to the new XY
      do_blocking_move_to_z(finalz);              // - Lower down to the starting Z height, ready for adjustment!
    #elif defined(MANUAL_PROBE_START_Z)           // A starting-Z was provided, but there's no raise:
      do_blocking_move_to_xy_z(pos, finalz);      // - Move in XY then down to the starting Z height, ready for adjustment!
    #else                                         // Zero raise and no starting Z height either:
      do_blocking_move_to_xy(pos);                // - Move over with no raise, ready for adjustment!
    #endif

    TERN_(LCD_BED_LEVELING, ui.wait_for_move = false);
  }

#endif // MESH_BED_LEVELING || PROBE_MANUALLY

#endif // HAS_LEVELING

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

#include "../../inc/MarlinConfig.h"

#if HAS_LEVELING

#include "bedlevel.h"

#if ENABLED(MESH_BED_LEVELING) || ENABLED(PROBE_MANUALLY)
  #include "../../module/motion.h"
#endif

#if PLANNER_LEVELING
  #include "../../module/planner.h"
#endif

#if ENABLED(PROBE_MANUALLY)
  bool g29_in_progress = false;
#endif

#if ENABLED(LCD_BED_LEVELING)
  #include "../../lcd/ultralcd.h"
#endif

#if ENABLED(G26_MESH_VALIDATION)
  bool g26_debug_flag; // = false
#endif

bool leveling_is_valid() {
  return
    #if ENABLED(MESH_BED_LEVELING)
      mbl.has_mesh()
    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
      !!bilinear_grid_spacing[X_AXIS]
    #elif ENABLED(AUTO_BED_LEVELING_UBL)
      true
    #else // 3POINT, LINEAR
      true
    #endif
  ;
}

/**
 * Turn bed leveling on or off, fixing the current
 * position as-needed.
 *
 * Disable: Current position = physical position
 *  Enable: Current position = "unleveled" physical position
 */
void set_bed_leveling_enabled(const bool enable/*=true*/) {

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    const bool can_change = (!enable || leveling_is_valid());
  #else
    constexpr bool can_change = true;
  #endif

  if (can_change && enable != planner.leveling_active) {

    #if ENABLED(MESH_BED_LEVELING)

      if (!enable)
        planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);

      const bool enabling = enable && leveling_is_valid();
      planner.leveling_active = enabling;
      if (enabling) planner.unapply_leveling(current_position);

    #elif ENABLED(AUTO_BED_LEVELING_UBL)
      #if PLANNER_LEVELING
        if (planner.leveling_active) {                   // leveling from on to off
          // change unleveled current_position to physical current_position without moving steppers.
          planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);
          planner.leveling_active = false;                   // disable only AFTER calling apply_leveling
        }
        else {                                        // leveling from off to on
          planner.leveling_active = true;                    // enable BEFORE calling unapply_leveling, otherwise ignored
          // change physical current_position to unleveled current_position without moving steppers.
          planner.unapply_leveling(current_position);
        }
      #else
        planner.leveling_active = enable;                    // just flip the bit, current_position will be wrong until next move.
      #endif

    #else // OLDSCHOOL_ABL

      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        // Force bilinear_z_offset to re-calculate next time
        const float reset[XYZ] = { -9999.999, -9999.999, 0 };
        (void)bilinear_z_offset(reset);
      #endif

      // Enable or disable leveling compensation in the planner
      planner.leveling_active = enable;

      if (!enable)
        // When disabling just get the current position from the steppers.
        // This will yield the smallest error when first converted back to steps.
        set_current_from_steppers_for_axis(
          #if ABL_PLANAR
            ALL_AXES
          #else
            Z_AXIS
          #endif
        );
      else
        // When enabling, remove compensation from the current position,
        // so compensation will give the right stepper counts.
        planner.unapply_leveling(current_position);

      SYNC_PLAN_POSITION_KINEMATIC();

    #endif // OLDSCHOOL_ABL
  }
}

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

  void set_z_fade_height(const float zfh, const bool do_report/*=true*/) {

    if (planner.z_fade_height == zfh) return; // do nothing if no change

    const bool level_active = planner.leveling_active;

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      if (level_active) set_bed_leveling_enabled(false);  // turn off before changing fade height for proper apply/unapply leveling to maintain current_position
    #endif

    planner.set_z_fade_height(zfh);

    if (level_active) {
      const float oldpos[] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        set_bed_leveling_enabled(true);  // turn back on after changing fade height
      #else
        set_current_from_steppers_for_axis(
          #if ABL_PLANAR
            ALL_AXES
          #else
            Z_AXIS
          #endif
        );
        SYNC_PLAN_POSITION_KINEMATIC();
      #endif
      if (do_report && memcmp(oldpos, current_position, sizeof(oldpos)))
        report_current_position();
    }
  }

#endif // ENABLE_LEVELING_FADE_HEIGHT

/**
 * Reset calibration results to zero.
 */
void reset_bed_level() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("reset_bed_level");
  #endif
  set_bed_leveling_enabled(false);
  #if ENABLED(MESH_BED_LEVELING)
    mbl.reset();
  #elif ENABLED(AUTO_BED_LEVELING_UBL)
    ubl.reset();
  #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
    bilinear_start[X_AXIS] = bilinear_start[Y_AXIS] =
    bilinear_grid_spacing[X_AXIS] = bilinear_grid_spacing[Y_AXIS] = 0;
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        z_values[x][y] = NAN;
  #elif ABL_PLANAR
    planner.bed_level_matrix.set_to_identity();
  #endif
}

#if ENABLED(AUTO_BED_LEVELING_BILINEAR) || ENABLED(MESH_BED_LEVELING)

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
  void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, element_2d_fn fn) {
    #ifndef SCAD_MESH_OUTPUT
      for (uint8_t x = 0; x < sx; x++) {
        for (uint8_t i = 0; i < precision + 2 + (x < 10 ? 1 : 0); i++)
          SERIAL_PROTOCOLCHAR(' ');
        SERIAL_PROTOCOL((int)x);
      }
      SERIAL_EOL();
    #endif
    #ifdef SCAD_MESH_OUTPUT
      SERIAL_PROTOCOLLNPGM("measured_z = ["); // open 2D array
    #endif
    for (uint8_t y = 0; y < sy; y++) {
      #ifdef SCAD_MESH_OUTPUT
        SERIAL_PROTOCOLPGM(" [");           // open sub-array
      #else
        if (y < 10) SERIAL_PROTOCOLCHAR(' ');
        SERIAL_PROTOCOL((int)y);
      #endif
      for (uint8_t x = 0; x < sx; x++) {
        SERIAL_PROTOCOLCHAR(' ');
        const float offset = fn(x, y);
        if (!isnan(offset)) {
          if (offset >= 0) SERIAL_PROTOCOLCHAR('+');
          SERIAL_PROTOCOL_F(offset, precision);
        }
        else {
          #ifdef SCAD_MESH_OUTPUT
            for (uint8_t i = 3; i < precision + 3; i++)
              SERIAL_PROTOCOLCHAR(' ');
            SERIAL_PROTOCOLPGM("NAN");
          #else
            for (uint8_t i = 0; i < precision + 3; i++)
              SERIAL_PROTOCOLCHAR(i ? '=' : ' ');
          #endif
        }
        #ifdef SCAD_MESH_OUTPUT
          if (x < sx - 1) SERIAL_PROTOCOLCHAR(',');
        #endif
      }
      #ifdef SCAD_MESH_OUTPUT
        SERIAL_PROTOCOLCHAR(' ');
        SERIAL_PROTOCOLCHAR(']');                     // close sub-array
        if (y < sy - 1) SERIAL_PROTOCOLCHAR(',');
      #endif
      SERIAL_EOL();
    }
    #ifdef SCAD_MESH_OUTPUT
      SERIAL_PROTOCOLPGM("];");                       // close 2D array
    #endif
    SERIAL_EOL();
  }

#endif // AUTO_BED_LEVELING_BILINEAR || MESH_BED_LEVELING

#if ENABLED(MESH_BED_LEVELING) || ENABLED(PROBE_MANUALLY)

  void _manual_goto_xy(const float &rx, const float &ry) {

    #if MANUAL_PROBE_HEIGHT > 0
      const float prev_z = current_position[Z_AXIS];
      do_blocking_move_to(rx, ry, MANUAL_PROBE_HEIGHT);
      do_blocking_move_to_z(prev_z);
    #else
      do_blocking_move_to_xy(rx, ry);
    #endif

    current_position[X_AXIS] = rx;
    current_position[Y_AXIS] = ry;

    #if ENABLED(LCD_BED_LEVELING)
      lcd_wait_for_move = false;
    #endif
  }

#endif

#if HAS_PROBING_PROCEDURE
  void out_of_range_error(const char* p_edge) {
    SERIAL_PROTOCOLPGM("?Probe ");
    serialprintPGM(p_edge);
    SERIAL_PROTOCOLLNPGM(" position out of range.");
  }
#endif

#endif // HAS_LEVELING

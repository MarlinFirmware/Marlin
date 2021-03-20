/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../gcode.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/planner.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../module/configuration_store.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extensible_ui/ui_api.h"
#endif

//#define M420_C_USE_MEAN

/**
 * M420: Enable/Disable Bed Leveling and/or set the Z fade height.
 *
 *   S[bool]   Turns leveling on or off
 *   Z[height] Sets the Z fade height (0 or none to disable)
 *   V[bool]   Verbose - Print the leveling grid
 *
 * With AUTO_BED_LEVELING_UBL only:
 *
 *   L[index]  Load UBL mesh from index (0 is default)
 *   T[map]    0:Human-readable 1:CSV 2:"LCD" 4:Compact
 *
 * With mesh-based leveling only:
 *
 *   C         Center mesh on the mean of the lowest and highest
 *
 * With MARLIN_DEV_MODE:
 *   S2        Create a simple random mesh and enable
 */
void GcodeSuite::M420() {
  const bool seen_S = parser.seen('S'),
             to_enable = seen_S ? parser.value_bool() : planner.leveling_active;

  #if ENABLED(MARLIN_DEV_MODE)
    if (parser.intval('S') == 2) {
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        bilinear_start[X_AXIS] = MIN_PROBE_X;
        bilinear_start[Y_AXIS] = MIN_PROBE_Y;
        bilinear_grid_spacing[X_AXIS] = (MAX_PROBE_X - (MIN_PROBE_X)) / (GRID_MAX_POINTS_X - 1);
        bilinear_grid_spacing[Y_AXIS] = (MAX_PROBE_Y - (MIN_PROBE_Y)) / (GRID_MAX_POINTS_Y - 1);
      #endif
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) {
          Z_VALUES(x, y) = 0.001 * random(-200, 200);
          #if ENABLED(EXTENSIBLE_UI)
            ExtUI::onMeshUpdate(x, y, Z_VALUES(x, y));
          #endif
        }
      SERIAL_ECHOPGM("Simulated " STRINGIFY(GRID_MAX_POINTS_X) "x" STRINGIFY(GRID_MAX_POINTS_X) " mesh ");
      SERIAL_ECHOPAIR(" (", MIN_PROBE_X);
      SERIAL_CHAR(','); SERIAL_ECHO(MIN_PROBE_Y);
      SERIAL_ECHOPAIR(")-(", MAX_PROBE_X);
      SERIAL_CHAR(','); SERIAL_ECHO(MAX_PROBE_Y);
      SERIAL_ECHOLNPGM(")");
    }
  #endif

  // If disabling leveling do it right away
  // (Don't disable for just M420 or M420 V)
  if (seen_S && !to_enable) set_bed_leveling_enabled(false);

  const float oldpos[] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    // L to load a mesh from the EEPROM
    if (parser.seen('L')) {

      set_bed_leveling_enabled(false);

      #if ENABLED(EEPROM_SETTINGS)
        const int8_t storage_slot = parser.has_value() ? parser.value_int() : ubl.storage_slot;
        const int16_t a = settings.calc_num_meshes();

        if (!a) {
          SERIAL_ECHOLNPGM("?EEPROM storage not available.");
          return;
        }

        if (!WITHIN(storage_slot, 0, a - 1)) {
          SERIAL_ECHOLNPGM("?Invalid storage slot.");
          SERIAL_ECHOLNPAIR("?Use 0 to ", a - 1);
          return;
        }

        settings.load_mesh(storage_slot);
        ubl.storage_slot = storage_slot;

      #else

        SERIAL_ECHOLNPGM("?EEPROM storage not available.");
        return;

      #endif
    }

    // L or V display the map info
    if (parser.seen('L') || parser.seen('V')) {
      ubl.display_map(parser.byteval('T'));
      SERIAL_ECHOPGM("Mesh is ");
      if (!ubl.mesh_is_valid()) SERIAL_ECHOPGM("in");
      SERIAL_ECHOLNPAIR("valid\nStorage slot: ", ubl.storage_slot);
    }

  #endif // AUTO_BED_LEVELING_UBL

  const bool seenV = parser.seen('V');

  #if HAS_MESH

    if (leveling_is_valid()) {

      // Subtract the given value or the mean from all mesh values
      if (parser.seen('C')) {
        const float cval = parser.value_float();
        #if ENABLED(AUTO_BED_LEVELING_UBL)

          set_bed_leveling_enabled(false);
          ubl.adjust_mesh_to_mean(true, cval);

        #else

          #if ENABLED(M420_C_USE_MEAN)

            // Get the sum and average of all mesh values
            float mesh_sum = 0;
            for (uint8_t x = GRID_MAX_POINTS_X; x--;)
              for (uint8_t y = GRID_MAX_POINTS_Y; y--;)
                mesh_sum += Z_VALUES(x, y);
            const float zmean = mesh_sum / float(GRID_MAX_POINTS);

          #else

            // Find the low and high mesh values
            float lo_val = 100, hi_val = -100;
            for (uint8_t x = GRID_MAX_POINTS_X; x--;)
              for (uint8_t y = GRID_MAX_POINTS_Y; y--;) {
                const float z = Z_VALUES(x, y);
                NOMORE(lo_val, z);
                NOLESS(hi_val, z);
              }
            // Take the mean of the lowest and highest
            const float zmean = (lo_val + hi_val) / 2.0 + cval;

          #endif

          // If not very close to 0, adjust the mesh
          if (!NEAR_ZERO(zmean)) {
            set_bed_leveling_enabled(false);
            // Subtract the mean from all values
            for (uint8_t x = GRID_MAX_POINTS_X; x--;)
              for (uint8_t y = GRID_MAX_POINTS_Y; y--;) {
                Z_VALUES(x, y) -= zmean;
                #if ENABLED(EXTENSIBLE_UI)
                  ExtUI::onMeshUpdate(x, y, Z_VALUES(x, y));
                #endif
              }
            #if ENABLED(ABL_BILINEAR_SUBDIVISION)
              bed_level_virt_interpolate();
            #endif
          }

        #endif
      }

    }
    else if (to_enable || seenV) {
      SERIAL_ECHO_MSG("Invalid mesh.");
      goto EXIT_M420;
    }

  #endif // HAS_MESH

  // V to print the matrix or mesh
  if (seenV) {
    #if ABL_PLANAR
      planner.bed_level_matrix.debug(PSTR("Bed Level Correction Matrix:"));
    #else
      if (leveling_is_valid()) {
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          print_bilinear_leveling_grid();
          #if ENABLED(ABL_BILINEAR_SUBDIVISION)
            print_bilinear_leveling_grid_virt();
          #endif
        #elif ENABLED(MESH_BED_LEVELING)
          SERIAL_ECHOLNPGM("Mesh Bed Level data:");
          mbl.report_mesh();
        #endif
      }
    #endif
  }

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    if (parser.seen('Z')) set_z_fade_height(parser.value_linear_units(), false);
  #endif

  // Enable leveling if specified, or if previously active
  set_bed_leveling_enabled(to_enable);

  #if HAS_MESH
    EXIT_M420:
  #endif

  // Error if leveling failed to enable or reenable
  if (to_enable && !planner.leveling_active)
    SERIAL_ERROR_MSG(MSG_ERR_M420_FAILED);

  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Bed Leveling ");
  serialprintln_onoff(planner.leveling_active);

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Fade Height ");
    if (planner.z_fade_height > 0.0)
      SERIAL_ECHOLN(planner.z_fade_height);
    else
      SERIAL_ECHOLNPGM(MSG_OFF);
  #endif

  // Report change in position
  if (memcmp(oldpos, current_position, sizeof(oldpos)))
    report_current_position();
}

#endif // HAS_LEVELING

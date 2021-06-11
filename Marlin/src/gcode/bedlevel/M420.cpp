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

#include "../gcode.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/planner.h"
#include "../../module/probe.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../module/settings.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
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
      const float x_min = probe.min_x(), x_max = probe.max_x(),
                  y_min = probe.min_y(), y_max = probe.max_y();
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        bilinear_start.set(x_min, y_min);
        bilinear_grid_spacing.set((x_max - x_min) / (GRID_MAX_CELLS_X),
                                  (y_max - y_min) / (GRID_MAX_CELLS_Y));
      #endif
      GRID_LOOP(x, y) {
        Z_VALUES(x, y) = 0.001 * random(-200, 200);
        TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(x, y, Z_VALUES(x, y)));
      }
      SERIAL_ECHOPGM("Simulated " STRINGIFY(GRID_MAX_POINTS_X) "x" STRINGIFY(GRID_MAX_POINTS_Y) " mesh ");
      SERIAL_ECHOPAIR(" (", x_min);
      SERIAL_CHAR(','); SERIAL_ECHO(y_min);
      SERIAL_ECHOPAIR(")-(", x_max);
      SERIAL_CHAR(','); SERIAL_ECHO(y_max);
      SERIAL_ECHOLNPGM(")");
    }
  #endif

  xyz_pos_t oldpos = current_position;

  // If disabling leveling do it right away
  // (Don't disable for just M420 or M420 V)
  if (seen_S && !to_enable) set_bed_leveling_enabled(false);

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
    if (parser.seen("LV")) {
      ubl.display_map(parser.byteval('T'));
      SERIAL_ECHOPGM("Mesh is ");
      if (!ubl.mesh_is_valid()) SERIAL_ECHOPGM("in");
      SERIAL_ECHOLNPAIR("valid\nStorage slot: ", ubl.storage_slot);
    }

  #endif // AUTO_BED_LEVELING_UBL

  const bool seenV = parser.seen_test('V');

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
            GRID_LOOP(x, y) mesh_sum += Z_VALUES(x, y);
            const float zmean = mesh_sum / float(GRID_MAX_POINTS);

          #else // midrange

            // Find the low and high mesh values.
            float lo_val = 100, hi_val = -100;
            GRID_LOOP(x, y) {
              const float z = Z_VALUES(x, y);
              NOMORE(lo_val, z);
              NOLESS(hi_val, z);
            }
            // Get the midrange plus C value. (The median may be better.)
            const float zmean = (lo_val + hi_val) / 2.0 + cval;

          #endif

          // If not very close to 0, adjust the mesh
          if (!NEAR_ZERO(zmean)) {
            set_bed_leveling_enabled(false);
            // Subtract the mean from all values
            GRID_LOOP(x, y) {
              Z_VALUES(x, y) -= zmean;
              TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(x, y, Z_VALUES(x, y)));
            }
            TERN_(ABL_BILINEAR_SUBDIVISION, bed_level_virt_interpolate());
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
          TERN_(ABL_BILINEAR_SUBDIVISION, print_bilinear_leveling_grid_virt());
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
    SERIAL_ERROR_MSG(STR_ERR_M420_FAILED);

  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Bed Leveling ");
  serialprintln_onoff(planner.leveling_active);

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Fade Height ");
    if (planner.z_fade_height > 0.0)
      SERIAL_ECHOLN(planner.z_fade_height);
    else
      SERIAL_ECHOLNPGM(STR_OFF);
  #endif

  // Report change in position
  if (oldpos != current_position)
    report_current_position();
}

#endif // HAS_LEVELING

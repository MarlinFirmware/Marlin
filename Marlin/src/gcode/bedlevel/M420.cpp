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

#include "../gcode.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/planner.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../module/configuration_store.h"
#endif

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
 */
void GcodeSuite::M420() {

  const float oldpos[] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    // L to load a mesh from the EEPROM
    if (parser.seen('L')) {

      #if ENABLED(EEPROM_SETTINGS)
        const int8_t storage_slot = parser.has_value() ? parser.value_int() : ubl.storage_slot;
        const int16_t a = settings.calc_num_meshes();

        if (!a) {
          SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
          return;
        }

        if (!WITHIN(storage_slot, 0, a - 1)) {
          SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
          SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
          return;
        }

        settings.load_mesh(storage_slot);
        ubl.storage_slot = storage_slot;

      #else

        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
        return;

      #endif
    }

    // L or V display the map info
    if (parser.seen('L') || parser.seen('V')) {
      ubl.display_map(parser.byteval('T'));
      SERIAL_ECHOLNPAIR("ubl.mesh_is_valid = ", ubl.mesh_is_valid());
      SERIAL_ECHOLNPAIR("ubl.storage_slot = ", ubl.storage_slot);
    }

  #endif // AUTO_BED_LEVELING_UBL

  // V to print the matrix or mesh
  if (parser.seen('V')) {
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

  bool to_enable = false;
  if (parser.seen('S')) {
    to_enable = parser.value_bool();
    set_bed_leveling_enabled(to_enable);
  }

  const bool new_status = planner.leveling_active;

  if (to_enable && !new_status) {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_M420_FAILED);
  }

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPAIR("Bed Leveling ", new_status ? MSG_ON : MSG_OFF);

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
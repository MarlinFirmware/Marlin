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

#include "Marlin.h"
#include "math.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "UBL.h"
  #include "hex_print_routines.h"

  /**
   * These variables used to be declared inside the bed_leveling class.  We are going to still declare
   * them within the .cpp file for bed leveling.   But there is only one instance of the bed leveling
   * object and we can get rid of a level of inderection by not making them 'member data'.  So, in the
   * interest of speed, we do it this way.    When we move to a 32-Bit processor, they can be moved
   * back inside the bed leveling class.
   */
  float last_specified_z,
        fade_scaling_factor_for_current_height,
        z_values[UBL_MESH_NUM_X_POINTS][UBL_MESH_NUM_Y_POINTS],
        mesh_index_to_X_location[UBL_MESH_NUM_X_POINTS + 1], // +1 just because of paranoia that we might end up on the
        mesh_index_to_Y_location[UBL_MESH_NUM_Y_POINTS + 1]; // the last Mesh Line and that is the start of a whole new cell

  bed_leveling::bed_leveling() {
    for (uint8_t i = 0; i <= UBL_MESH_NUM_X_POINTS; i++)  // We go one past what we expect to ever need for safety
      mesh_index_to_X_location[i] = double(UBL_MESH_MIN_X) + double(MESH_X_DIST) * double(i);

    for (uint8_t i = 0; i <= UBL_MESH_NUM_Y_POINTS; i++)  // We go one past what we expect to ever need for safety
      mesh_index_to_Y_location[i] = double(UBL_MESH_MIN_Y) + double(MESH_Y_DIST) * double(i);

    reset();
  }

  void bed_leveling::store_state() {
    int k = E2END - sizeof(blm.state);
    eeprom_write_block((void *)&blm.state, (void *)k, sizeof(blm.state));
  }

  void bed_leveling::load_state() {
    int k = E2END - sizeof(blm.state);
    eeprom_read_block((void *)&blm.state, (void *)k, sizeof(blm.state));

    if (sanity_check())
      SERIAL_PROTOCOLLNPGM("?In load_state() sanity_check() failed.\n");

    // These lines can go away in a few weeks.  They are just
    // to make sure people updating thier firmware won't be using
    if (blm.state.G29_Fade_Height_Multiplier != 1.0 / blm.state.G29_Correction_Fade_Height) { // an incomplete Bed_Leveling.state structure. For speed
      blm.state.G29_Fade_Height_Multiplier = 1.0 / blm.state.G29_Correction_Fade_Height;      // we now multiply by the inverse of the Fade Height instead of
      store_state();   // dividing by it. Soon... all of the old structures will be
    }                  // updated, but until then, we try to ease the transition
                       // for our Beta testers.
  }

  void bed_leveling::load_mesh(int m) {
    int k = E2END - sizeof(blm.state),
        j = (k - Unified_Bed_Leveling_EEPROM_start) / sizeof(z_values);

    if (m == -1) {
      SERIAL_PROTOCOLLNPGM("?No mesh saved in EEPROM. Zeroing mesh in memory.\n");
      reset();
      return;
    }

    if (m < 0 || m >= j || Unified_Bed_Leveling_EEPROM_start <= 0) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available to load mesh.\n");
      return;
    }

    j = k - (m + 1) * sizeof(z_values);
    eeprom_read_block((void *)&z_values , (void *)j, sizeof(z_values));

    SERIAL_PROTOCOLPGM("Mesh loaded from slot ");
    SERIAL_PROTOCOL(m);
    SERIAL_PROTOCOLPGM("  at offset 0x");
    prt_hex_word(j);
    SERIAL_EOL;
  }

  void bed_leveling:: store_mesh(int m) {
    int k = E2END - sizeof(state),
        j = (k - Unified_Bed_Leveling_EEPROM_start) / sizeof(z_values);

    if (m < 0 || m >= j || Unified_Bed_Leveling_EEPROM_start <= 0) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available to load mesh.\n");
      SERIAL_PROTOCOL(m);
      SERIAL_PROTOCOLLNPGM(" mesh slots available.\n");
      SERIAL_PROTOCOLLNPAIR("E2END     : ", E2END);
      SERIAL_PROTOCOLLNPAIR("k         : ", k);
      SERIAL_PROTOCOLLNPAIR("j         : ", j);
      SERIAL_PROTOCOLLNPAIR("m         : ", m);
      SERIAL_EOL;
      return;
    }

    j = k - (m + 1) * sizeof(z_values);
    eeprom_write_block((const void *)&z_values, (void *)j, sizeof(z_values));

    SERIAL_PROTOCOLPGM("Mesh saved in slot ");
    SERIAL_PROTOCOL(m);
    SERIAL_PROTOCOLPGM("  at offset 0x");
    prt_hex_word(j);
    SERIAL_EOL;
  }

  void bed_leveling::reset() {
    state.active = false;
    state.z_offset = 0;
    state.EEPROM_storage_slot = -1;

    ZERO(z_values);

    last_specified_z = -999.9;        // We can't pre-initialize these values in the declaration
    fade_scaling_factor_for_current_height = 0.0; // due to C++11 constraints
  }

  void bed_leveling::invalidate() {
    prt_hex_word((unsigned int)this);
    SERIAL_EOL;

    state.active = false;
    state.z_offset = 0;
    for (int x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
      for (int y = 0; y < UBL_MESH_NUM_Y_POINTS; y++)
        z_values[x][y] = NAN;
  }

  void bed_leveling::display_map(int map_type) {
    float f, current_xi, current_yi;
    int8_t i, j;
    UNUSED(map_type);

    SERIAL_PROTOCOLLNPGM("\nBed Topography Report:\n");

    SERIAL_ECHOPAIR("(", 0);
    SERIAL_ECHOPAIR(", ", UBL_MESH_NUM_Y_POINTS - 1);
    SERIAL_ECHOPGM(")    ");

    current_xi = blm.get_cell_index_x(current_position[X_AXIS] + (MESH_X_DIST) / 2.0);
    current_yi = blm.get_cell_index_y(current_position[Y_AXIS] + (MESH_Y_DIST) / 2.0);

    for (i = 0; i < UBL_MESH_NUM_X_POINTS - 1; i++)
      SERIAL_ECHOPGM("                 ");

    SERIAL_ECHOPAIR("(", UBL_MESH_NUM_X_POINTS - 1);
    SERIAL_ECHOPAIR(",", UBL_MESH_NUM_Y_POINTS - 1);
    SERIAL_ECHOLNPGM(")");

    //  if (map_type || 1) {
    SERIAL_ECHOPAIR("(", UBL_MESH_MIN_X);
    SERIAL_ECHOPAIR(",", UBL_MESH_MAX_Y);
    SERIAL_CHAR(')');

    for (i = 0; i < UBL_MESH_NUM_X_POINTS - 1; i++)
      SERIAL_ECHOPGM("                 ");

    SERIAL_ECHOPAIR("(", UBL_MESH_MAX_X);
    SERIAL_ECHOPAIR(",", UBL_MESH_MAX_Y);
    SERIAL_ECHOLNPGM(")");
    //  }

    for (j = UBL_MESH_NUM_Y_POINTS - 1; j >= 0; j--) {
      for (i = 0; i < UBL_MESH_NUM_X_POINTS; i++) {
        f = z_values[i][j];

        // is the nozzle here?  if so, mark the number
        SERIAL_CHAR(i == current_xi && j == current_yi ? '[' : ' ');

        if (isnan(f))
          SERIAL_PROTOCOLPGM("      .       ");
        else {
          // if we don't do this, the columns won't line up nicely
          if (f >= 0.0) SERIAL_CHAR(' ');
          SERIAL_PROTOCOL_F(f, 5);
          idle();
        }
        if (i == current_xi && j == current_yi) // is the nozzle here? if so, finish marking the number
          SERIAL_CHAR(']');
        else
          SERIAL_PROTOCOL("  ");

        SERIAL_CHAR(' ');
      }
      SERIAL_EOL;
      if (j) { // we want the (0,0) up tight against the block of numbers
        SERIAL_CHAR(' ');
        SERIAL_EOL;
      }
    }

    //  if (map_type) {
    SERIAL_ECHOPAIR("(", int(UBL_MESH_MIN_X));
    SERIAL_ECHOPAIR(",", int(UBL_MESH_MIN_Y));
    SERIAL_ECHOPGM(")    ");

    for (i = 0; i < UBL_MESH_NUM_X_POINTS - 1; i++)
      SERIAL_ECHOPGM("                 ");

    SERIAL_ECHOPAIR("(", int(UBL_MESH_MAX_X));
    SERIAL_ECHOPAIR(",", int(UBL_MESH_MIN_Y));
    SERIAL_CHAR(')');
    //  }

    SERIAL_ECHOPAIR("(", 0);
    SERIAL_ECHOPAIR(",", 0);
    SERIAL_ECHOPGM(")       ");

    for (i = 0; i < UBL_MESH_NUM_X_POINTS - 1; i++)
      SERIAL_ECHOPGM("                 ");

    SERIAL_ECHOPAIR("(", UBL_MESH_NUM_X_POINTS-1);
    SERIAL_ECHOPAIR(",", 0);
    SERIAL_CHAR(')');

    SERIAL_CHAR(' ');
    SERIAL_EOL;
  }

  bool bed_leveling::sanity_check() {
    uint8_t error_flag = 0;

    if (state.n_x !=  UBL_MESH_NUM_X_POINTS)  {
      SERIAL_PROTOCOLLNPGM("?UBL_MESH_NUM_X_POINTS set wrong\n");
      error_flag++;
    }

    if (state.n_y !=  UBL_MESH_NUM_Y_POINTS)  {
      SERIAL_PROTOCOLLNPGM("?UBL_MESH_NUM_Y_POINTS set wrong\n");
      error_flag++;
    }

    if (state.mesh_x_min !=  UBL_MESH_MIN_X)  {
      SERIAL_PROTOCOLLNPGM("?UBL_MESH_MIN_X set wrong\n");
      error_flag++;
    }

    if (state.mesh_y_min !=  UBL_MESH_MIN_Y)  {
      SERIAL_PROTOCOLLNPGM("?UBL_MESH_MIN_Y set wrong\n");
      error_flag++;
    }

    if (state.mesh_x_max !=  UBL_MESH_MAX_X)  {
      SERIAL_PROTOCOLLNPGM("?UBL_MESH_MAX_X set wrong\n");
      error_flag++;
    }

    if (state.mesh_y_max !=  UBL_MESH_MAX_Y)  {
      SERIAL_PROTOCOLLNPGM("?UBL_MESH_MAX_Y set wrong\n");
      error_flag++;
    }

    if (state.mesh_x_dist !=  MESH_X_DIST)  {
      SERIAL_PROTOCOLLNPGM("?MESH_X_DIST set wrong\n");
      error_flag++;
    }

    if (state.mesh_y_dist !=  MESH_Y_DIST)  {
      SERIAL_PROTOCOLLNPGM("?MESH_Y_DIST set wrong\n");
      error_flag++;
    }

    int k = E2END - sizeof(blm.state),
        j = (k - Unified_Bed_Leveling_EEPROM_start) / sizeof(z_values);

    if (j < 1) {
      SERIAL_PROTOCOLLNPGM("?No EEPROM storage available for a mesh of this size.\n");
      error_flag++;
    }

    //  SERIAL_PROTOCOLPGM("?sanity_check() return value: ");
    //  SERIAL_PROTOCOL(error_flag);
    //  SERIAL_EOL;

    return !!error_flag;
  }

#endif // AUTO_BED_LEVELING_UBL

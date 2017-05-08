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

  #include "ubl.h"
  #include "hex_print_routines.h"
  #include "temperature.h"

  /**
   * These support functions allow the use of large bit arrays of flags that take very
   * little RAM. Currently they are limited to being 16x16 in size. Changing the declaration
   * to unsigned long will allow us to go to 32x32 if higher resolution Mesh's are needed
   * in the future.
   */
  void bit_clear(uint16_t bits[16], uint8_t x, uint8_t y) { CBI(bits[y], x); }
  void bit_set(uint16_t bits[16], uint8_t x, uint8_t y) { SBI(bits[y], x); }
  bool is_bit_set(uint16_t bits[16], uint8_t x, uint8_t y) { return TEST(bits[y], x); }

  uint8_t ubl_cnt = 0;

  static void serial_echo_xy(const uint16_t x, const uint16_t y) {
    SERIAL_CHAR('(');
    SERIAL_ECHO(x);
    SERIAL_CHAR(',');
    SERIAL_ECHO(y);
    SERIAL_CHAR(')');
    safe_delay(10);
  }

  ubl_state unified_bed_leveling::state;

  float unified_bed_leveling::z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y],
        unified_bed_leveling::last_specified_z;

  // 15 is the maximum nubmer of grid points supported + 1 safety margin for now,
  // until determinism prevails
  constexpr float unified_bed_leveling::mesh_index_to_xpos[16],
                  unified_bed_leveling::mesh_index_to_ypos[16];

  bool unified_bed_leveling::g26_debug_flag = false,
       unified_bed_leveling::has_control_of_lcd_panel = false;

  int16_t unified_bed_leveling::eeprom_start = -1;  // Please stop changing this to 8 bits in size
                                                    // It needs to hold values bigger than this.

  volatile int unified_bed_leveling::encoder_diff;

  unified_bed_leveling::unified_bed_leveling() {
    ubl_cnt++;  // Debug counter to insure we only have one UBL object present in memory.
    reset();
  }

  void unified_bed_leveling::load_mesh(const int16_t slot) {
    int16_t j = (UBL_LAST_EEPROM_INDEX - eeprom_start) / sizeof(z_values);

    if (slot == -1) {
      SERIAL_PROTOCOLLNPGM("?No mesh saved in EEPROM. Zeroing mesh in memory.\n");
      reset();
      return;
    }

    if (!WITHIN(slot, 0, j - 1) || eeprom_start <= 0) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available to load mesh.\n");
      return;
    }

    j = UBL_LAST_EEPROM_INDEX - (slot + 1) * sizeof(z_values);
    eeprom_read_block((void *)&z_values, (void *)j, sizeof(z_values));

    SERIAL_PROTOCOLPAIR("Mesh loaded from slot ", slot);
    SERIAL_PROTOCOLLNPAIR(" at offset ", hex_address((void*)j));
  }

  void unified_bed_leveling::store_mesh(const int16_t slot) {
    int16_t j = (UBL_LAST_EEPROM_INDEX - eeprom_start) / sizeof(z_values);

    if (!WITHIN(slot, 0, j - 1) || eeprom_start <= 0) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available to load mesh.\n");
      SERIAL_PROTOCOL(slot);
      SERIAL_PROTOCOLLNPGM(" mesh slots available.\n");
      SERIAL_PROTOCOLLNPAIR("E2END     : ", E2END);
      SERIAL_PROTOCOLLNPAIR("k         : ", (int)UBL_LAST_EEPROM_INDEX);
      SERIAL_PROTOCOLLNPAIR("j         : ", j);
      SERIAL_PROTOCOLLNPAIR("m         : ", slot);
      SERIAL_EOL;
      return;
    }

    j = UBL_LAST_EEPROM_INDEX - (slot + 1) * sizeof(z_values);
    eeprom_write_block((const void *)&z_values, (void *)j, sizeof(z_values));

    SERIAL_PROTOCOLPAIR("Mesh saved in slot ", slot);
    SERIAL_PROTOCOLLNPAIR(" at offset ", hex_address((void*)j));
  }

  void unified_bed_leveling::reset() {
    state.active = false;
    state.z_offset = 0;
    state.eeprom_storage_slot = -1;

    ZERO(z_values);

    last_specified_z = -999.9;
  }

  void unified_bed_leveling::invalidate() {
    state.active = false;
    state.z_offset = 0;
    for (int x = 0; x < GRID_MAX_POINTS_X; x++)
      for (int y = 0; y < GRID_MAX_POINTS_Y; y++)
        z_values[x][y] = NAN;
  }

  void unified_bed_leveling::display_map(const int map_type) {
    const bool map0 = map_type == 0;
    constexpr uint8_t spaces = 11 * (GRID_MAX_POINTS_X - 2);

    if (map0) {
      SERIAL_PROTOCOLLNPGM("\nBed Topography Report:\n");
      serial_echo_xy(0, GRID_MAX_POINTS_Y - 1);
      SERIAL_ECHO_SP(spaces + 3);
      serial_echo_xy(GRID_MAX_POINTS_X - 1, GRID_MAX_POINTS_Y - 1);
      SERIAL_EOL;
      serial_echo_xy(UBL_MESH_MIN_X, UBL_MESH_MAX_Y);
      SERIAL_ECHO_SP(spaces - 3);
      serial_echo_xy(UBL_MESH_MAX_X, UBL_MESH_MAX_Y);
      SERIAL_EOL;
    }

    const float current_xi = ubl.get_cell_index_x(current_position[X_AXIS] + (MESH_X_DIST) / 2.0),
                current_yi = ubl.get_cell_index_y(current_position[Y_AXIS] + (MESH_Y_DIST) / 2.0);

    for (int8_t j = GRID_MAX_POINTS_Y - 1; j >= 0; j--) {
      for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
        const bool is_current = i == current_xi && j == current_yi;

        // is the nozzle here? then mark the number
        if (map0) SERIAL_CHAR(is_current ? '[' : ' ');

        const float f = z_values[i][j];
        if (isnan(f)) {
          serialprintPGM(map0 ? PSTR("   .  ") : PSTR("NAN"));
        }
        else {
          // if we don't do this, the columns won't line up nicely
          if (map0 && f >= 0.0) SERIAL_CHAR(' ');
          SERIAL_PROTOCOL_F(f, 3);
          idle();
        }
        if (!map0 && i < GRID_MAX_POINTS_X - 1) SERIAL_CHAR(',');

        #if TX_BUFFER_SIZE > 0
          MYSERIAL.flushTX();
        #endif
        safe_delay(15);
        if (map0) {
          SERIAL_CHAR(is_current ? ']' : ' ');
          SERIAL_CHAR(' ');
        }
      }
      SERIAL_EOL;
      if (j && map0) { // we want the (0,0) up tight against the block of numbers
        SERIAL_CHAR(' ');
        SERIAL_EOL;
      }
    }

    if (map0) {
      serial_echo_xy(UBL_MESH_MIN_X, UBL_MESH_MIN_Y);
      SERIAL_ECHO_SP(spaces + 1);
      serial_echo_xy(UBL_MESH_MAX_X, UBL_MESH_MIN_Y);
      SERIAL_EOL;
      serial_echo_xy(0, 0);
      SERIAL_ECHO_SP(spaces + 5);
      serial_echo_xy(GRID_MAX_POINTS_X - 1, 0);
      SERIAL_EOL;
    }
  }

  bool unified_bed_leveling::sanity_check() {
    uint8_t error_flag = 0;

    const int j = (UBL_LAST_EEPROM_INDEX - eeprom_start) / sizeof(z_values);
    if (j < 1) {
      SERIAL_PROTOCOLLNPGM("?No EEPROM storage available for a mesh of this size.\n");
      error_flag++;
    }

    return !!error_flag;
  }

#endif // AUTO_BED_LEVELING_UBL

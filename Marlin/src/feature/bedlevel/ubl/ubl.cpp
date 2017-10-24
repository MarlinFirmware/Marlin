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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)

  #include "ubl.h"
  unified_bed_leveling ubl;

  #include "../../../module/configuration_store.h"
  #include "../../../module/planner.h"
  #include "../../../module/motion.h"
  #include "../../bedlevel/bedlevel.h"

  #include "math.h"

  /**
   * These support functions allow the use of large bit arrays of flags that take very
   * little RAM. Currently they are limited to being 16x16 in size. Changing the declaration
   * to unsigned long will allow us to go to 32x32 if higher resolution Mesh's are needed
   * in the future.
   */
  void bit_clear(uint16_t bits[16], const uint8_t x, const uint8_t y) { CBI(bits[y], x); }
  void bit_set(uint16_t bits[16], const uint8_t x, const uint8_t y) { SBI(bits[y], x); }
  bool is_bit_set(uint16_t bits[16], const uint8_t x, const uint8_t y) { return TEST(bits[y], x); }

  uint8_t ubl_cnt = 0;

  void unified_bed_leveling::echo_name() { SERIAL_PROTOCOLPGM("Unified Bed Leveling"); }

  void unified_bed_leveling::report_state() {
    echo_name();
    SERIAL_PROTOCOLPGM(" System v" UBL_VERSION " ");
    if (!planner.leveling_active) SERIAL_PROTOCOLPGM("in");
    SERIAL_PROTOCOLLNPGM("active.");
    safe_delay(50);
  }

  static void serial_echo_xy(const int16_t x, const int16_t y) {
    SERIAL_CHAR('(');
    SERIAL_ECHO(x);
    SERIAL_CHAR(',');
    SERIAL_ECHO(y);
    SERIAL_CHAR(')');
    safe_delay(10);
  }

  int8_t unified_bed_leveling::storage_slot;

  float unified_bed_leveling::z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  // 15 is the maximum nubmer of grid points supported + 1 safety margin for now,
  // until determinism prevails
  constexpr float unified_bed_leveling::_mesh_index_to_xpos[16],
                  unified_bed_leveling::_mesh_index_to_ypos[16];

  bool unified_bed_leveling::g26_debug_flag = false,
       unified_bed_leveling::has_control_of_lcd_panel = false;

  #if ENABLED(ULTRA_LCD)
    bool unified_bed_leveling::lcd_map_control = false;
  #endif

  volatile int unified_bed_leveling::encoder_diff;

  unified_bed_leveling::unified_bed_leveling() {
    ubl_cnt++;  // Debug counter to insure we only have one UBL object present in memory.  We can eliminate this (and all references to ubl_cnt) very soon.
    reset();
  }

  void unified_bed_leveling::reset() {
    set_bed_leveling_enabled(false);
    storage_slot = -1;
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      planner.set_z_fade_height(10.0);
    #endif
    ZERO(z_values);
  }

  void unified_bed_leveling::invalidate() {
    set_bed_leveling_enabled(false);
    set_all_mesh_points_to_value(NAN);
  }

  void unified_bed_leveling::set_all_mesh_points_to_value(const float value) {
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) {
        z_values[x][y] = value;
      }
    }
  }

  // display_map() currently produces three different mesh map types
  // 0 : suitable for PronterFace and Repetier's serial console
  // 1 : .CSV file suitable for importation into various spread sheets
  // 2 : disply of the map data on a RepRap Graphical LCD Panel

  void unified_bed_leveling::display_map(const int map_type) {
    constexpr uint8_t spaces = 8 * (GRID_MAX_POINTS_X - 2);

    SERIAL_PROTOCOLPGM("\nBed Topography Report");
    if (map_type == 0) {
      SERIAL_PROTOCOLPGM(":\n\n");
      serial_echo_xy(0, GRID_MAX_POINTS_Y - 1);
      SERIAL_ECHO_SP(spaces + 3);
      serial_echo_xy(GRID_MAX_POINTS_X - 1, GRID_MAX_POINTS_Y - 1);
      SERIAL_EOL();
      serial_echo_xy(MESH_MIN_X, MESH_MAX_Y);
      SERIAL_ECHO_SP(spaces);
      serial_echo_xy(MESH_MAX_X, MESH_MAX_Y);
      SERIAL_EOL();
    }
    else {
      SERIAL_PROTOCOLPGM(" for ");
      serialprintPGM(map_type == 1 ? PSTR("CSV:\n\n") : PSTR("LCD:\n\n"));
    }

    const float current_xi = get_cell_index_x(current_position[X_AXIS] + (MESH_X_DIST) / 2.0),
                current_yi = get_cell_index_y(current_position[Y_AXIS] + (MESH_Y_DIST) / 2.0);

    for (int8_t j = GRID_MAX_POINTS_Y - 1; j >= 0; j--) {
      for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
        const bool is_current = i == current_xi && j == current_yi;

        // is the nozzle here? then mark the number
        if (map_type == 0) SERIAL_CHAR(is_current ? '[' : ' ');

        const float f = z_values[i][j];
        if (isnan(f)) {
          serialprintPGM(map_type == 0 ? PSTR("    .   ") : PSTR("NAN"));
        }
        else if (map_type <= 1) {
          // if we don't do this, the columns won't line up nicely
          if (map_type == 0 && f >= 0.0) SERIAL_CHAR(' ');
          SERIAL_PROTOCOL_F(f, 3);
        }
        idle();
        if (map_type == 1 && i < GRID_MAX_POINTS_X - 1) SERIAL_CHAR(',');

        #if TX_BUFFER_SIZE > 0
          MYSERIAL.flushTX();
        #endif
        safe_delay(15);
        if (map_type == 0) {
          SERIAL_CHAR(is_current ? ']' : ' ');
          SERIAL_CHAR(' ');
        }
      }
      SERIAL_EOL();
      if (j && map_type == 0) { // we want the (0,0) up tight against the block of numbers
        SERIAL_CHAR(' ');
        SERIAL_EOL();
      }
    }

    if (map_type == 0) {
      serial_echo_xy(MESH_MIN_X, MESH_MIN_Y);
      SERIAL_ECHO_SP(spaces + 4);
      serial_echo_xy(MESH_MAX_X, MESH_MIN_Y);
      SERIAL_EOL();
      serial_echo_xy(0, 0);
      SERIAL_ECHO_SP(spaces + 5);
      serial_echo_xy(GRID_MAX_POINTS_X - 1, 0);
      SERIAL_EOL();
    }
  }

  bool unified_bed_leveling::sanity_check() {
    uint8_t error_flag = 0;

    if (settings.calc_num_meshes() < 1) {
      SERIAL_PROTOCOLLNPGM("?Insufficient EEPROM storage for a mesh of this size.");
      error_flag++;
    }

    return !!error_flag;
  }

#endif // AUTO_BED_LEVELING_UBL

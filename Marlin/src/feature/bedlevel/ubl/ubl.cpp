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

  uint8_t ubl_cnt = 0;

  void unified_bed_leveling::echo_name(
    #if NUM_SERIAL > 1
      const int8_t port/*= -1*/
    #endif
  ) {
    SERIAL_PROTOCOLPGM_P(port, "Unified Bed Leveling");
  }

  void unified_bed_leveling::report_current_mesh(
    #if NUM_SERIAL > 1
      const int8_t port/*= -1*/
    #endif
  ) {
    if (!leveling_is_valid()) return;
    SERIAL_ECHO_START_P(port);
    SERIAL_ECHOLNPGM_P(port, "  G29 I99");
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0;  y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(z_values[x][y])) {
          SERIAL_ECHO_START_P(port);
          SERIAL_ECHOPAIR_P(port, "  M421 I", x);
          SERIAL_ECHOPAIR_P(port, " J", y);
          SERIAL_ECHOPGM_P(port, " Z");
          SERIAL_ECHO_F_P(port, z_values[x][y], 2);
          SERIAL_EOL_P(port);
          safe_delay(75); // Prevent Printrun from exploding
        }
  }

  void unified_bed_leveling::report_state(
    #if NUM_SERIAL > 1
      const int8_t port/*= -1*/
    #endif
  ) {
    echo_name(
      #if NUM_SERIAL > 1
        port
      #endif
    );
    SERIAL_PROTOCOLPGM_P(port, " System v" UBL_VERSION " ");
    if (!planner.leveling_active) SERIAL_PROTOCOLPGM_P(port, "in");
    SERIAL_PROTOCOLLNPGM_P(port, "active.");
    safe_delay(50);
  }

  #if ENABLED(UBL_DEVEL_DEBUGGING)

    static void debug_echo_axis(const AxisEnum axis) {
      if (current_position[axis] == destination[axis])
        SERIAL_ECHOPGM("-------------");
      else
        SERIAL_ECHO_F(destination[X_AXIS], 6);
    }

    void debug_current_and_destination(PGM_P title) {

      // if the title message starts with a '!' it is so important, we are going to
      // ignore the status of the g26_debug_flag
      if (*title != '!' && !g26_debug_flag) return;

      const float de = destination[E_AXIS] - current_position[E_AXIS];

      if (de == 0.0) return; // Printing moves only

      const float dx = destination[X_AXIS] - current_position[X_AXIS],
                  dy = destination[Y_AXIS] - current_position[Y_AXIS],
                  xy_dist = HYPOT(dx, dy);

      if (xy_dist == 0.0) return;

      SERIAL_ECHOPGM("   fpmm=");
      const float fpmm = de / xy_dist;
      SERIAL_ECHO_F(fpmm, 6);

      SERIAL_ECHOPGM("    current=( ");
      SERIAL_ECHO_F(current_position[X_AXIS], 6);
      SERIAL_ECHOPGM(", ");
      SERIAL_ECHO_F(current_position[Y_AXIS], 6);
      SERIAL_ECHOPGM(", ");
      SERIAL_ECHO_F(current_position[Z_AXIS], 6);
      SERIAL_ECHOPGM(", ");
      SERIAL_ECHO_F(current_position[E_AXIS], 6);
      SERIAL_ECHOPGM(" )   destination=( ");
      debug_echo_axis(X_AXIS);
      SERIAL_ECHOPGM(", ");
      debug_echo_axis(Y_AXIS);
      SERIAL_ECHOPGM(", ");
      debug_echo_axis(Z_AXIS);
      SERIAL_ECHOPGM(", ");
      debug_echo_axis(E_AXIS);
      SERIAL_ECHOPGM(" )   ");
      serialprintPGM(title);
      SERIAL_EOL();

    }

  #endif // UBL_DEVEL_DEBUGGING

  int8_t unified_bed_leveling::storage_slot;

  float unified_bed_leveling::z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  // 15 is the maximum nubmer of grid points supported + 1 safety margin for now,
  // until determinism prevails
  constexpr float unified_bed_leveling::_mesh_index_to_xpos[16],
                  unified_bed_leveling::_mesh_index_to_ypos[16];

  #if HAS_LCD_MENU
    bool unified_bed_leveling::lcd_map_control = false;
  #endif

  volatile int unified_bed_leveling::encoder_diff;

  unified_bed_leveling::unified_bed_leveling() {
    ubl_cnt++;  // Debug counter to ensure we only have one UBL object present in memory.  We can eliminate this (and all references to ubl_cnt) very soon.
    reset();
  }

  void unified_bed_leveling::reset() {
    const bool was_enabled = planner.leveling_active;
    set_bed_leveling_enabled(false);
    storage_slot = -1;
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      planner.set_z_fade_height(10.0);
    #endif
    ZERO(z_values);
    if (was_enabled) report_current_position();
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

  static void serial_echo_xy(const uint8_t sp, const int16_t x, const int16_t y) {
    SERIAL_ECHO_SP(sp);
    SERIAL_CHAR('(');
    if (x < 100) { SERIAL_CHAR(' '); if (x < 10) SERIAL_CHAR(' '); }
    SERIAL_ECHO(x);
    SERIAL_CHAR(',');
    if (y < 100) { SERIAL_CHAR(' '); if (y < 10) SERIAL_CHAR(' '); }
    SERIAL_ECHO(y);
    SERIAL_CHAR(')');
    safe_delay(5);
  }

  static void serial_echo_column_labels(const uint8_t sp) {
    SERIAL_ECHO_SP(7);
    for (int8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      if (i < 10) SERIAL_CHAR(' ');
      SERIAL_ECHO(i);
      SERIAL_ECHO_SP(sp);
    }
    safe_delay(10);
  }

  /**
   * Produce one of these mesh maps:
   *   0: Human-readable
   *   1: CSV format for spreadsheet import
   *   2: TODO: Display on Graphical LCD
   *   4: Compact Human-Readable
   */
  void unified_bed_leveling::display_map(const int map_type) {
    #if HAS_AUTO_REPORTING || ENABLED(HOST_KEEPALIVE_FEATURE)
      suspend_auto_report = true;
    #endif

    constexpr uint8_t eachsp = 1 + 6 + 1,                           // [-3.567]
                      twixt = eachsp * (GRID_MAX_POINTS_X) - 9 * 2; // Leading 4sp, Coordinates 9sp each

    const bool human = !(map_type & 0x3), csv = map_type == 1, lcd = map_type == 2, comp = map_type & 0x4;

    SERIAL_ECHOPGM("\nBed Topography Report");
    if (human) {
      SERIAL_ECHOPGM(":\n\n");
      serial_echo_xy(4, MESH_MIN_X, MESH_MAX_Y);
      serial_echo_xy(twixt, MESH_MAX_X, MESH_MAX_Y);
      SERIAL_EOL();
      serial_echo_column_labels(eachsp - 2);
    }
    else {
      SERIAL_ECHOPGM(" for ");
      serialprintPGM(csv ? PSTR("CSV:\n") : PSTR("LCD:\n"));
    }

    const float current_xi = get_cell_index_x(current_position[X_AXIS] + (MESH_X_DIST) / 2.0),
                current_yi = get_cell_index_y(current_position[Y_AXIS] + (MESH_Y_DIST) / 2.0);

    if (!lcd) SERIAL_EOL();
    for (int8_t j = GRID_MAX_POINTS_Y - 1; j >= 0; j--) {

      // Row Label (J index)
      if (human) {
        if (j < 10) SERIAL_CHAR(' ');
        SERIAL_ECHO(j);
        SERIAL_ECHOPGM(" |");
      }

      // Row Values (I indexes)
      for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {

        // Opening Brace or Space
        const bool is_current = i == current_xi && j == current_yi;
        if (human) SERIAL_CHAR(is_current ? '[' : ' ');

        // Z Value at current I, J
        const float f = z_values[i][j];
        if (lcd) {
          // TODO: Display on Graphical LCD
        }
        else if (isnan(f))
          serialprintPGM(human ? PSTR("  .   ") : PSTR("NAN"));
        else if (human || csv) {
          if (human && f >= 0.0) SERIAL_CHAR(f > 0 ? '+' : ' ');  // Space for positive ('-' for negative)
          SERIAL_ECHO_F(f, 3);                                    // Positive: 5 digits, Negative: 6 digits
        }
        if (csv && i < GRID_MAX_POINTS_X - 1) SERIAL_CHAR('\t');

        // Closing Brace or Space
        if (human) SERIAL_CHAR(is_current ? ']' : ' ');

        SERIAL_FLUSHTX();
        idle();
      }
      if (!lcd) SERIAL_EOL();

      // A blank line between rows (unless compact)
      if (j && human && !comp) SERIAL_ECHOLNPGM("   |");
    }

    if (human) {
      serial_echo_column_labels(eachsp - 2);
      SERIAL_EOL();
      serial_echo_xy(4, MESH_MIN_X, MESH_MIN_Y);
      serial_echo_xy(twixt, MESH_MAX_X, MESH_MIN_Y);
      SERIAL_EOL();
      SERIAL_EOL();
    }

    #if HAS_AUTO_REPORTING || ENABLED(HOST_KEEPALIVE_FEATURE)
      suspend_auto_report = false;
    #endif
  }

  bool unified_bed_leveling::sanity_check() {
    uint8_t error_flag = 0;

    if (settings.calc_num_meshes() < 1) {
      SERIAL_PROTOCOLLNPGM("?Mesh too big for EEPROM.");
      error_flag++;
    }

    return !!error_flag;
  }

#endif // AUTO_BED_LEVELING_UBL

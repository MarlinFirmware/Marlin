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
    SERIAL_ECHOLNPGM_P(port, "  G29 I 999");
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0;  y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(z_values[x][y])) {
          SERIAL_ECHO_START_P(port);
          SERIAL_ECHOPAIR_P(port, "  M421 I", x);
          SERIAL_ECHOPAIR_P(port, " J", y);
          SERIAL_ECHOPGM_P(port, " Z");
          SERIAL_ECHO_F_P(port, z_values[x][y], 6);
          SERIAL_ECHOPAIR_P(port, " ; X", LOGICAL_X_POSITION(mesh_index_to_xpos(x)));
          SERIAL_ECHOPAIR_P(port, ", Y", LOGICAL_Y_POSITION(mesh_index_to_ypos(y)));
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

  static void serial_echo_xy(const int16_t x, const int16_t y) {
    SERIAL_CHAR('(');
    SERIAL_ECHO(x);
    SERIAL_CHAR(',');
    SERIAL_ECHO(y);
    SERIAL_CHAR(')');
    safe_delay(10);
  }

  #if ENABLED(UBL_DEVEL_DEBUGGING)

    static void debug_echo_axis(const AxisEnum axis) {
      if (current_position[axis] == destination[axis])
        SERIAL_ECHOPGM("-------------");
      else
        SERIAL_ECHO_F(destination[X_AXIS], 6);
    }

    void debug_current_and_destination(const char *title) {

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
      SERIAL_ECHO(title);
      SERIAL_EOL();

    }

  #endif // UBL_DEVEL_DEBUGGING

  int8_t unified_bed_leveling::storage_slot;

  float unified_bed_leveling::z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  // 15 is the maximum nubmer of grid points supported + 1 safety margin for now,
  // until determinism prevails
  constexpr float unified_bed_leveling::_mesh_index_to_xpos[16],
                  unified_bed_leveling::_mesh_index_to_ypos[16];

  #if ENABLED(ULTIPANEL)
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

  // display_map() currently produces three different mesh map types
  // 0 : suitable for PronterFace and Repetier's serial console
  // 1 : .CSV file suitable for importation into various spread sheets
  // 2 : disply of the map data on a RepRap Graphical LCD Panel

  void unified_bed_leveling::display_map(const int map_type) {
    #if HAS_AUTO_REPORTING
      suspend_auto_report = true;
    #endif

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
        SERIAL_FLUSHTX();
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

    #if HAS_AUTO_REPORTING
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

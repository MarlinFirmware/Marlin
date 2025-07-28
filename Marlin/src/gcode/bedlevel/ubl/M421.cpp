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

/**
 * M421.cpp - Unified Bed Leveling
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)

#include "../../gcode.h"
#include "../../../feature/bedlevel/bedlevel.h"
#if ENABLED(DYNAMIC_MARGINS)
  #include "../../../module/probe.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../../lcd/extui/ui_api.h"
#endif

/**
 * M421: Set a single Mesh Bed Leveling Z coordinate
 *
 * Usage:
 *   M421 I<xindex> J<yindex> Z<linear>  : Set the Mesh Point IJ to the Z value
 *   M421 I<xindex> J<yindex> Q<offset>  : Add the Q value to the Mesh Point IJ
 *   M421 I<xindex> J<yindex> N          : Set the Mesh Point IJ to NAN (not set)
 *   M421 C Z<linear>                    : Set the closest Mesh Point to the Z value
 *   M421 C Q<offset>                    : Add the Q value to the closest Mesh Point
 * 
 * M421: Set one or more PROBING_MARGINS if ENABLED(DYNAMIC_MARGINS) in mm
 *
 * Usage:
 * 
 *  - set single margin (l):
 *    M421 L10
 *  - set multiple margins (lf):
 *    M421 L10 F10
 *  - set all margins to one value (lrfb):
 *    M421 L50 R50 F50 B50
 *  - recalculate all min margins based on probe offsets and reachable area (lrfb):
 *    M421 L0 R0 F0 B0
 *  - reset margins to the defaults
 *    M421 D
 *
 */
void GcodeSuite::M421() {
  xy_int8_t ij = { int8_t(parser.intval('I', -1)), int8_t(parser.intval('J', -1)) };
  const bool hasI = ij.x >= 0,
             hasJ = ij.y >= 0,
             hasC = parser.seen_test('C'),
             hasN = parser.seen_test('N'),
             hasZ = parser.seen('Z'),
             hasQ = !hasZ && parser.seen('Q');

  const bool mesh_command = hasZ || hasQ || hasN;
  if (hasC) ij = bedlevel.find_closest_mesh_point_of_type(CLOSEST, current_position);
  bool did_something = false;
  // Mesh point modification
  if (mesh_command) {
    // Bad parameter combination: Must have exactly one of (C or both I+J)
    if (int(hasC) + int(hasI && hasJ) != 1) {
      SERIAL_ERROR_MSG(STR_ERR_M421_PARAMETERS);
      return;
    }
    // Test for I J out of range
    if (!WITHIN(ij.x, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(ij.y, 0, GRID_MAX_POINTS_Y - 1)) {
      SERIAL_ERROR_MSG(STR_ERR_MESH_XY);
      return;
    }
    // Apply Z/Q/N value
    float &zval = bedlevel.z_values[ij.x][ij.y];
    zval = hasN ? NAN : parser.value_linear_units() + (hasQ ? zval : 0);
    TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(ij.x, ij.y, zval));
    did_something = true;
  }
  #if ENABLED(DYNAMIC_MARGINS)
    auto safe_margin = [](int user_val, int min_required, const char axis_char) {
      const int m = constrain(user_val, 0, 1000);
      if (m < min_required) {
        #if DISABLED(MARLIN_SMALL_BUILD)
          SERIAL_ECHOPGM("  ! ");
          SERIAL_CHAR(axis_char);
          SERIAL_ECHOPGM(" margin too small (");
          SERIAL_ECHO(m);
          SERIAL_ECHOPGM(" < ");
          SERIAL_ECHO(min_required);
          SERIAL_ECHOPGM("). ");
          SERIAL_CHAR(axis_char);
          SERIAL_ECHOPGM(" set to: ");
          SERIAL_ECHOLN(min_required);
        #endif
        return min_required;
      }
      return m;
    };
    if (parser.seen('D')) {
      // 'D' with no value resets all margins
      bedlevel.margin_l = PROBING_MARGIN_LEFT;
      bedlevel.margin_f = PROBING_MARGIN_RIGHT;
      bedlevel.margin_r = PROBING_MARGIN_FRONT;
      bedlevel.margin_b = PROBING_MARGIN_BACK;
      #if DISABLED(MARLIN_SMALL_BUILD)
        SERIAL_ECHOPGM("  Margins reset: L");
        SERIAL_ECHO(bedlevel.margin_l);
        SERIAL_ECHOPGM(" R");
        SERIAL_ECHO(bedlevel.margin_r);
        SERIAL_ECHOPGM(" F");
        SERIAL_ECHO(bedlevel.margin_f);
        SERIAL_ECHOPGM(" B");
        SERIAL_ECHO(bedlevel.margin_b);
        SERIAL_EOL();
      #endif
      did_something = true;
    }
    if (parser.seen('L')) {
      // Left margin: X_MIN_POS + probe_offset must reach >= 0, but min 10
      bedlevel.margin_l = safe_margin(parser.value_int(), constrain(_MAX(10, TERN(HAS_HOME_OFFSET, home_offset.x, X_MIN_POS) - ceilf(fabs(probe.offset_xy.x))), 10, X_BED_SIZE), 'L');
      did_something = true;
    }
    if (parser.seen('F')) {
      // Front margin: Y_MIN_POS + probe_offset must reach >= 0, but min 10
      bedlevel.margin_f = safe_margin(parser.value_int(), constrain(_MAX(10, TERN(HAS_HOME_OFFSET, home_offset.y, Y_MIN_POS) - ceilf(fabs(probe.offset_xy.y))), 10, Y_BED_SIZE), 'F');
      did_something = true;
    }
    if (parser.seen('R')) {
      // Right margin: The probe must not exceed X_BED_SIZE, but min 10
      bedlevel.margin_r = safe_margin(parser.value_int(), constrain(_MAX(10, X_BED_SIZE - (X_MAX_POS - ceilf(fabs(probe.offset_xy.x)))), 10, X_BED_SIZE), 'R');
      did_something = true;
    }
    if (parser.seen('B')) {
      // Back margin: The probe must not exceed Y_BED_SIZE, but min 10
      bedlevel.margin_b = safe_margin(parser.value_int(), constrain(_MAX(10, Y_BED_SIZE - (Y_MAX_POS - ceilf(fabs(probe.offset_xy.y)))), 10, Y_BED_SIZE), 'B');
      did_something = true;
    }
  #endif
  if (!did_something) {
    #if ENABLED(DYNAMIC_MARGINS)
      M421_report();
    #else
      SERIAL_ERROR_MSG(STR_ERR_M421_PARAMETERS);
    #endif
  }
}
#if ENABLED(DYNAMIC_MARGINS)
  void GcodeSuite::M421_report(const bool forReplay/*=true*/) {
    TERN_(MARLIN_SMALL_BUILD, return);
    report_heading_etc(forReplay, F("Dynamic Margins"));
    SERIAL_ECHOPGM("  M421 L");
    SERIAL_ECHO(LINEAR_UNIT(bedlevel.margin_l));
    SERIAL_ECHOPGM(" R");
    SERIAL_ECHO(LINEAR_UNIT(bedlevel.margin_r));
    SERIAL_ECHOPGM(" F");
    SERIAL_ECHO(LINEAR_UNIT(bedlevel.margin_f));
    SERIAL_ECHOPGM(" B");
    SERIAL_ECHO(LINEAR_UNIT(bedlevel.margin_b));
    SERIAL_ECHOLNPGM(" ; Margins in mm");
  }
#endif

#endif // AUTO_BED_LEVELING_UBL

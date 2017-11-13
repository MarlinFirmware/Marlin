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

#include "../gcode.h"
#include "../../module/motion.h"
//#include "../../module/stepper.h"

#if ENABLED(CNC_COORDINATE_SYSTEMS)

  /**
   * Select a coordinate system and update the workspace offset.
   * System index -1 is used to specify machine-native.
   */
  bool GCodeSuite::select_coordinate_system(const int8_t _new) {
    if (active_coordinate_system == _new) return false;
    stepper.synchronize();
    float old_offset[XYZ] = { 0 }, new_offset[XYZ] = { 0 };
    if (WITHIN(active_coordinate_system, 0, MAX_COORDINATE_SYSTEMS - 1))
      COPY(old_offset, coordinate_system[active_coordinate_system]);
    if (WITHIN(_new, 0, MAX_COORDINATE_SYSTEMS - 1))
      COPY(new_offset, coordinate_system[_new]);
    active_coordinate_system = _new;
    LOOP_XYZ(i) {
      const float diff = new_offset[i] - old_offset[i];
      if (diff) {
        position_shift[i] += diff;
        update_software_endstops((AxisEnum)i);
      }
    }
    return true;
  }

  /**
   * In CNC G-code G53 is like a modifier
   * It precedes a movement command (or other modifiers) on the same line.
   * This is the first command to use parser.chain() to make this possible.
   */
  void GCodeSuite::G53() {
    // If this command has more following...
    if (parser.chain()) {
      const int8_t _system = active_coordinate_system;
      active_coordinate_system = -1;
      process_parsed_command();
      active_coordinate_system = _system;
    }
  }

  /**
   * G54-G59.3: Select a new workspace
   *
   * A workspace is an XYZ offset to the machine native space.
   * All workspaces default to 0,0,0 at start, or with EEPROM
   * support they may be restored from a previous session.
   *
   * G92 is used to set the current workspace's offset.
   */
  void G54_59(uint8_t subcode=0) {
    const int8_t _space = parser.codenum - 54 + subcode;
    if (gcode.select_coordinate_system(_space)) {
      SERIAL_PROTOCOLLNPAIR("Select workspace ", _space);
      report_current_position();
    }
  }
  void GCodeSuite::G54() { G54_59(); }
  void GCodeSuite::G55() { G54_59(); }
  void GCodeSuite::G56() { G54_59(); }
  void GCodeSuite::G57() { G54_59(); }
  void GCodeSuite::G58() { G54_59(); }
  void GCodeSuite::G59() { G54_59(parser.subcode); }

#endif // CNC_COORDINATE_SYSTEMS

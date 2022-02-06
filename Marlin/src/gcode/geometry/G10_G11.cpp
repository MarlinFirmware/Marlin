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

#if ENABLED(FWRETRACT)
  #include "../../feature/fwretract.h"
#endif

#if EITHER(FWRETRACT, CNC_COORDINATE_SYSTEMS)
  #include "../gcode.h"
  #include "../../module/motion.h"
  #include "../../module/stepper.h"
  // #define DEBUG_G10 // Show debug messages

/**
 * G10 S#         - Retract filament according to settings of M207
 *     L2/L20 P# X# Y# Z#... - Sets the origin of the coordinate system, specified by the P parameter (1 to 9)
 *
 * Beta L2/L20 implementation based on https://github.com/MarlinFirmware/Marlin/issues/14734
 * and http://linuxcnc.org/docs/2.6/html/gcode/gcode.html#sec:G10-L1_
 *
 * *Supported CNC_COORDINATE_SYSTEMS parameters: L2 and L20
 * *Unsupported parameters: L1
 * !Beta - Use with caution.
 */
void GcodeSuite::G10() {
  #if ENABLED(FWRETRACT)
    fwretract.retract(true E_OPTARG(parser.boolval('S')));
  #endif

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    const int8_t target_system = parser.intval('P') - 1, // Subtract 1 because P1 is G54, which is Marlin coordinate_system 0
                offset_type   = parser.intval('L');

    if (WITHIN(target_system, 0, MAX_COORDINATE_SYSTEMS - 1)) {
      const int8_t current_system = gcode.active_coordinate_system; // Store current coord system
      if (current_system != target_system) {
        gcode.select_coordinate_system(target_system); // Select New Coordinate System If Needed
        #if ENABLED(DEBUG_G10)
          SERIAL_ECHOLNPGM("Switching to workspace ", target_system);
          report_current_position();
        #endif
      }
      switch (offset_type) {
        case 1: // Sets the tool offset, as if the L parameter was not present
          SERIAL_ECHOLN("Error: L1 not supported, only L2 and L20");
          break;
        // Sets the work offsets of the specified (P[1-9]) coordinate system, by subtracting the specified X, Y, Z... values
        // from the current machine coordinate X, Y, Z... values (Works the same as LinuxCNC)
        // eg: If G53's X=50, Y=100 then G10 P1 L2 X10 Y2 will set G54's X=40, Y=98
        case 2:
          LOOP_LOGICAL_AXES(i) {
            if (parser.seen(axis_codes[i])) {
              if (TERN1(HAS_EXTRUDERS, i != E_AXIS)) {
                const float axis_shift = parser.axis_value_to_mm((AxisEnum)i, parser.value_float());
                position_shift[i]      = -axis_shift;
                update_workspace_offset((AxisEnum)i);
              }
            }
          }
          break;
        // Sets the work coordinates of the specified (P[1-9]) coordinate system, by matching the specified X, Y, Z... values
        // Works similar to G92, except you can specifiy a coordinate system directly (Works the same as LinuxCNC)
        // eg: G10 P2 L20 X10 Y50 will set G55's X=10, Y=50 no matter which coordinate system is currently selected
        case 20:
          LOOP_LOGICAL_AXES(i) {
            if (parser.seen(axis_codes[i])) {
              if (TERN1(HAS_EXTRUDERS, i != E_AXIS)) {
                const float axis_value = parser.axis_value_to_mm((AxisEnum)i, parser.value_float());
                position_shift[i]       = -current_position[i] + axis_value;
                update_workspace_offset((AxisEnum)i);
              }
            }
          }
          break;
      }
      gcode.coordinate_system[gcode.active_coordinate_system] = position_shift;
    #if ENABLED(DEBUG_G10)
      SERIAL_ECHOLNPGM("New position for workspace ", target_system);
      report_current_position();
    #endif
      if (current_system != target_system) {
        gcode.select_coordinate_system(current_system);
      }
    }
  #endif // ENABLED(CNC_COORDINATE_SYSTEMS)
}
#endif   // EITHER(FWRETRACT, CNC_COORDINATE_SYSTEMS)

#if ENABLED(FWRETRACT)
  /**
   * G11 - Recover filament according to settings of M208
   */
  void GcodeSuite::G11() { fwretract.retract(false); }
#endif

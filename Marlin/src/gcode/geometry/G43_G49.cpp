/**
 * Marlin2ForPipetBot
 * Copyright 2022 DerAndere
 * 
 * Based on:
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
 * @file G43_G49.cpp
 * @author DerAndere
 * @brief G-codes related to Tool Length Compensation and Rotational Tool Center Point Control
 */

#include "../../inc/MarlinConfig.h"

#if HAS_TOOL_LENGTH_COMPENSATION

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * G43: Enable Tool Length Compensation.
 * 
 * G43: Enable Simple Tool Length Compensation.
 * G43 Tool Length Compensation can be canceled with G49.
 *
 * G43.4: Enable Rotational Tool Center Point Control Mode.
 * G43.4 Rotational Tool Center Point Control Mode can be canceled with G49.
 * 
 * Only one can be active at any time
 */
void GcodeSuite::G43() {

  #if USE_GCODE_SUBCODES
    const uint8_t subcode_G43 = parser.subcode();
  #else
    constexpr uint8_t subcode_G43 = 0;                           
  #endif

  switch (subcode_G43) {
    default: return;                                              // Ignore unknown G43.x

    case 0:                                                       // G43 - Tool Length Compensation.
      TERN_(HAS_TOOL_CENTERPOINT_CONTROL, tool_centerpoint_control = false);
      simple_tool_length_compensation = true
      tool_length_offset = tool_length_offsets[active_extruder];
      update_workspace_offset(Z_AXIS);
      break;

    #if HAS_TOOL_CENTERPOINT_CONTROL
      case 4:                                                     // G43.4 - Rotational Tool Center Point Control Mode.
        simple_tool_length_compensation = false;
        tool_centerpoint_control = true;
        tool_length_offset = tool_length_offsets[active_extruder];
        break;
    #endif
  }
}


/**
 * G49: Cancel Rotational Tool Center Point Control Mode.
 * 
 * Rotational Tool Center Point Control Mode can be enabled with G43.3
 */
void GcodeSuite::G49() {
    tool_centerpoint_control = false;
    TERN_(HAS_TOOL_CENTERPOINT_CONTROL, tool_centerpoint_control = false);
  }

#endif
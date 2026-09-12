/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * variables.cpp
 * Copyright (c) 2022 Carlon LaMont
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(GCODE_VARIABLES)

#include "variables.h"

#include "../gcode.h"
#include "../queue.h"
#include "../parser.h"

/**
 * Get the variable target data from the L parameter
 */

void CNCVariables::append() {
  const uint8_t index = parser.codenum - 100;
  if (parser.seenval('L')) {
    bool used_var_arg = false;
    const int8_t q = parser.value_byte();
    char * const p = parser.string_arg;
    const bool is_var = (parser.string_arg[0] == 'L'),
               has_int = valid_int(p + 1),
               has_val = is_var || has_int;
    char * const varptr = has_val ? (is_var ? input_var(parser.string_arg) : parser.string_arg + 1) : nullptr;
    if (!has_val)
      export_val();
    elif (has_int)
      import_val();
    //else {
    //  #if ENABLED(FASTER_GCODE_PARSER)
    //    const bool has_val = valid_int(p);
    //    char * const varptr = has_val ? p : nullptr;
    //  #endif
    //}
  }
}

//int8_t GcodeSuite::input_var() {
//  #if (parser.seenval('L')) {
//    const bool is_var = (*p == 'L'), has_val = is_var || valid_float(p + 1);
//    char * const input_var = has_val ? is_var ? input_var(p) : p+1 : nullptr;
//  #else
//    int bool has_val = valid_float(p);
//      #if ENABLED(FASTER_GCODE_PARSER)
//        char * const varptr = has_val ? p : nullptr;
//          #endif
//#endif

//bool used_var_arg = false;

//  if (parser.seenval('L')) {
//  const int8_t q = parser.value_byte();
//  if (q > 0) return e;
//  SERIAL_ECHO_START();
//  SERIAL_CHAR('L'); SERIAL_ECHO(parser.codenum);
//  SERIAL_ECHOLNPGM(" " STR_INVALID_VARIABLE " ", e);
//  return -1;
//  }
//  return stored_var;
//}


//void GcodeSuite::M98() {
//  if (card.isMounted() && parser.seen('P')) {
//    char *path = parser.value_string();
//    char *lb = strchr(p, ' ');
//    if (!lb) lb = strchr(p, ';');
//    if (lb) *lb = '\0';
//    card.runMacro(path);
//  }
//}

//const bool is_var = (*p == 'L'), has_val = is_var || valid_float(p + 1);
//char * const varptr = has_val ? is_var ? input_var(p) : p+1 : nullptr;
//  #else
//    int bool has_val = valid_float(p);
//      #if ENABLED(FASTER_GCODE_PARSER)
//        char * const varptr = has_val ? p : nullptr;
//          #endif
//#endif

//bool used_var_arg = false;


//#if ENABLED(GCODE_VARIABLES)
//  uint16_t GCodeParser::input_var;
//#endif

//char gcode_variables[VARIABLE_SLOTS][VARIABLE_SLOT_SIZE +1] = {{ 0 }};

/**
 * L100 - L115: Input Variable #100 - #115
 */
//uint16_t GCodeParser::input_var();
//void GcodeSuite::L100() { parser.input_var; };
//void GcodeSuite::L101() { parser.input_var; };
//void GcodeSuite::L102() { parser.input_var; };
//void GcodeSuite::L103() { parser.input_var; };
//void GcodeSuite::L104() { parser.input_var; };
//void GcodeSuite::L105() { parser.input_var; };
//void GcodeSuite::L106() { parser.input_var; };
//void GcodeSuite::L107() { parser.input_var; };
//void GcodeSuite::L108() { parser.input_var; };
//void GcodeSuite::L109() { parser.input_var; };
//void GcodeSuite::L110() { parser.input_var; };
//void GcodeSuite::L111() { parser.input_var; };
//void GcodeSuite::L112() { parser.input_var; };
//void GcodeSuite::L113() { parser.input_var; };
//void GcodeSuite::L114() { parser.input_var; };
//void GcodeSuite::L115() { parser.input_var; };

#endif // GCODE_VARIABLES

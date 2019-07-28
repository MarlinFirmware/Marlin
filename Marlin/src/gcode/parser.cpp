/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * parser.cpp - Parser for a GCode line, providing a parameter interface.
 */

#include "parser.h"

#include "../Marlin.h"

#if NUM_SERIAL > 1
  #include "queue.h"
#endif

// Must be declared for allocation and to satisfy the linker
// Zero values need no initialization.

bool GCodeParser::volumetric_enabled;

#if ENABLED(INCH_MODE_SUPPORT)
  float GCodeParser::linear_unit_factor, GCodeParser::volumetric_unit_factor;
#endif

#if ENABLED(TEMPERATURE_UNITS_SUPPORT)
  TempUnit GCodeParser::input_temp_units = TEMPUNIT_C;
#endif

char *GCodeParser::command_ptr,
     *GCodeParser::string_arg,
     *GCodeParser::value_ptr;
char GCodeParser::command_letter;
int GCodeParser::codenum;

#if USE_GCODE_SUBCODES
  uint8_t GCodeParser::subcode;
#endif

#if ENABLED(GCODE_MOTION_MODES)
  int16_t GCodeParser::motion_mode_codenum = -1;
  #if USE_GCODE_SUBCODES
    uint8_t GCodeParser::motion_mode_subcode;
  #endif
#endif

#if ENABLED(FASTER_GCODE_PARSER)
  // Optimized Parameters
  uint32_t GCodeParser::codebits;  // found bits
  uint8_t GCodeParser::param[26];  // parameter offsets from command_ptr
#else
  char *GCodeParser::command_args; // start of parameters
#endif

// Create a global instance of the GCode parser singleton
GCodeParser parser;

/**
 * Clear all code-seen (and value pointers)
 *
 * Since each param is set/cleared on seen codes,
 * this may be optimized by commenting out ZERO(param)
 */
void GCodeParser::reset() {
  string_arg = nullptr;                 // No whole line argument
  command_letter = '?';                 // No command letter
  codenum = 0;                          // No command code
  #if USE_GCODE_SUBCODES
    subcode = 0;                        // No command sub-code
  #endif
  #if ENABLED(FASTER_GCODE_PARSER)
    codebits = 0;                       // No codes yet
    //ZERO(param);                      // No parameters (should be safe to comment out this line)
  #endif
}

// Populate all fields by parsing a single line of GCode
// 58 bytes of SRAM are used to speed up seen/value
void GCodeParser::parse(char *p) {

  reset(); // No codes to report

  // Skip spaces
  while (*p == ' ') ++p;

  // Skip N[-0-9] if included in the command line
  if (*p == 'N' && NUMERIC_SIGNED(p[1])) {
    #if ENABLED(FASTER_GCODE_PARSER)
      //set('N', p + 1);     // (optional) Set the 'N' parameter value
    #endif
    p += 2;                  // skip N[-0-9]
    while (NUMERIC(*p)) ++p; // skip [0-9]*
    while (*p == ' ')   ++p; // skip [ ]*
  }

  // *p now points to the current command, which should be G, M, or T
  command_ptr = p;

  // Get the command letter, which must be G, M, or T
  const char letter = *p++;

  // Nullify asterisk and trailing whitespace
  char *starpos = strchr(p, '*');
  if (starpos) {
    --starpos;                          // *
    while (*starpos == ' ') --starpos;  // spaces...
    starpos[1] = '\0';
  }

  #if ENABLED(GCODE_MOTION_MODES)
    #if ENABLED(ARC_SUPPORT)
      #define GTOP 3
    #else
      #define GTOP 1
    #endif
  #endif

  // Bail if the letter is not G, M, or T
  // (or a valid parameter for the current motion mode)
  switch (letter) {

    case 'G': case 'M': case 'T':

      // Skip spaces to get the numeric part
      while (*p == ' ') p++;

      #if ENABLED(PRUSA_MMU2)
        if (letter == 'T') {
          // check for special MMU2 T?/Tx/Tc commands
          if (*p == '?' || *p == 'x' || *p == 'c') {
            command_letter = letter;
            string_arg = p;
            return;
          }
        }
      #endif

      // Bail if there's no command code number
      if (!NUMERIC(*p)) return;

      // Save the command letter at this point
      // A '?' signifies an unknown command
      command_letter = letter;

      // Get the code number - integer digits only
      codenum = 0;
      do { codenum *= 10, codenum += *p++ - '0'; } while (NUMERIC(*p));

      // Allow for decimal point in command
      #if USE_GCODE_SUBCODES
      if (*p == '.') {
        p++;
        while (NUMERIC(*p))
        subcode *= 10, subcode += *p++ - '0';
      }
      #endif

      // Skip all spaces to get to the first argument, or nul
      while (*p == ' ') p++;

      #if ENABLED(GCODE_MOTION_MODES)
        if (letter == 'G' && (codenum <= GTOP || codenum == 5
                                #if ENABLED(G38_PROBE_TARGET)
                                  || codenum == 38
                                #endif
                             )
        ) {
          motion_mode_codenum = codenum;
          #if USE_GCODE_SUBCODES
            motion_mode_subcode = subcode;
          #endif
        }
      #endif

      break;

    #if ENABLED(GCODE_MOTION_MODES)
      #if ENABLED(ARC_SUPPORT)
        case 'I': case 'J': case 'R':
          if (motion_mode_codenum != 2 && motion_mode_codenum != 3) return;
      #endif
      case 'P': case 'Q':
        if (motion_mode_codenum != 5) return;
      case 'X': case 'Y': case 'Z': case 'E': case 'F':
        if (motion_mode_codenum < 0) return;
        command_letter = 'G';
        codenum = motion_mode_codenum;
        #if USE_GCODE_SUBCODES
          subcode = motion_mode_subcode;
        #endif
        p--; // Back up one character to use the current parameter
      break;
    #endif // GCODE_MOTION_MODES

    default: return;
  }

  // The command parameters (if any) start here, for sure!

  #if DISABLED(FASTER_GCODE_PARSER)
    command_args = p; // Scan for parameters in seen()
  #endif

  // Only use string_arg for these M codes
  if (letter == 'M') switch (codenum) {
    #if ENABLED(GCODE_MACROS)
      case 810: case 811: case 812: case 813: case 814:
      case 815: case 816: case 817: case 818: case 819:
    #endif
    case 23: case 28: case 30: case 117: case 118: case 928: string_arg = p; return;
    default: break;
  }

  #if ENABLED(DEBUG_GCODE_PARSER)
    const bool debug = codenum == 800;
  #endif

  /**
   * Find all parameters, set flags and pointers for fast parsing
   *
   * Most codes ignore 'string_arg', but those that want a string will get the right pointer.
   * The following loop assigns the first "parameter" having no numeric value to 'string_arg'.
   * This allows M0/M1 with expire time to work: "M0 S5 You Win!"
   * For 'M118' you must use 'E1' and 'A1' rather than just 'E' or 'A'
   */
  string_arg = nullptr;
  while (const char code = *p++) {                    // Get the next parameter. A NUL ends the loop

    // Special handling for M32 [P] !/path/to/file.g#
    // The path must be the last parameter
    if (code == '!' && letter == 'M' && codenum == 32) {
      string_arg = p;                           // Name starts after '!'
      char * const lb = strchr(p, '#');         // Already seen '#' as SD char (to pause buffering)
      if (lb) *lb = '\0';                       // Safe to mark the end of the filename
      return;
    }

    // Arguments MUST be uppercase for fast GCode parsing
    #if ENABLED(FASTER_GCODE_PARSER)
      #define PARAM_TEST WITHIN(code, 'A', 'Z')
    #else
      #define PARAM_TEST true
    #endif

    if (PARAM_TEST) {

      while (*p == ' ') p++;                    // Skip spaces between parameters & values

      const bool has_num = valid_float(p);

      #if ENABLED(DEBUG_GCODE_PARSER)
        if (debug) {
          SERIAL_ECHOPAIR("Got letter ", code, " at index ", (int)(p - command_ptr - 1));
          if (has_num) SERIAL_ECHOPGM(" (has_num)");
        }
      #endif

      if (!has_num && !string_arg) {            // No value? First time, keep as string_arg
        string_arg = p - 1;
        #if ENABLED(DEBUG_GCODE_PARSER)
          if (debug) SERIAL_ECHOPAIR(" string_arg: ", hex_address((void*)string_arg)); // DEBUG
        #endif
      }

      #if ENABLED(DEBUG_GCODE_PARSER)
        if (debug) SERIAL_EOL();
      #endif

      #if ENABLED(FASTER_GCODE_PARSER)
        set(code, has_num ? p : nullptr);       // Set parameter exists and pointer (nullptr for no number)
      #endif
    }
    else if (!string_arg) {                     // Not A-Z? First time, keep as the string_arg
      string_arg = p - 1;
      #if ENABLED(DEBUG_GCODE_PARSER)
        if (debug) SERIAL_ECHOPAIR(" string_arg: ", hex_address((void*)string_arg)); // DEBUG
      #endif
    }

    if (!WITHIN(*p, 'A', 'Z')) {                // Another parameter right away?
      while (*p && DECIMAL_SIGNED(*p)) p++;     // Skip over the value section of a parameter
      while (*p == ' ') p++;                    // Skip over all spaces
    }
  }
}

#if ENABLED(CNC_COORDINATE_SYSTEMS)

  // Parse the next parameter as a new command
  bool GCodeParser::chain() {
    #if ENABLED(FASTER_GCODE_PARSER)
      char *next_command = command_ptr;
      if (next_command) {
        while (*next_command && *next_command != ' ') ++next_command;
        while (*next_command == ' ') ++next_command;
        if (!*next_command) next_command = nullptr;
      }
    #else
      const char *next_command = command_args;
    #endif
    if (next_command) parse(next_command);
    return !!next_command;
  }

#endif // CNC_COORDINATE_SYSTEMS

void GCodeParser::unknown_command_error() {
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPAIR(MSG_UNKNOWN_COMMAND, command_ptr, "\"");
}

#if ENABLED(DEBUG_GCODE_PARSER)

  void GCodeParser::debug() {
    SERIAL_ECHOPAIR("Command: ", command_ptr, " (", command_letter);
    SERIAL_ECHO(codenum);
    SERIAL_ECHOLNPGM(")");
    #if ENABLED(FASTER_GCODE_PARSER)
      SERIAL_ECHOPGM(" args: { ");
      for (char c = 'A'; c <= 'Z'; ++c) if (seen(c)) { SERIAL_CHAR(c); SERIAL_CHAR(' '); }
      SERIAL_CHAR('}');
    #else
      SERIAL_ECHOPAIR(" args: { ", command_args, " }");
    #endif
    if (string_arg) SERIAL_ECHOPAIR(" string: \"", string_arg, "\"");
    SERIAL_ECHOLNPGM("\n");
    for (char c = 'A'; c <= 'Z'; ++c) {
      if (seen(c)) {
        SERIAL_ECHOPAIR("Code '", c); SERIAL_ECHOPGM("':");
        if (has_value()) {
          SERIAL_ECHOPAIR(
            "\n    float: ", value_float(),
            "\n     long: ", value_long(),
            "\n    ulong: ", value_ulong(),
            "\n   millis: ", value_millis(),
            "\n   sec-ms: ", value_millis_from_seconds(),
            "\n      int: ", value_int(),
            "\n   ushort: ", value_ushort(),
            "\n     byte: ", (int)value_byte(),
            "\n     bool: ", (int)value_bool(),
            "\n   linear: ", value_linear_units(),
            "\n  celsius: ", value_celsius()
          );
        }
        else
          SERIAL_ECHOPGM(" (no value)");
        SERIAL_ECHOLNPGM("\n");
      }
    }
  }

#endif // DEBUG_GCODE_PARSER

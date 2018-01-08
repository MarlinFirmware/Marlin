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

/**
 * parser.cpp - Parser for a GCode line, providing a parameter interface.
 */

#include "parser.h"

#include "../Marlin.h"

#if ENABLED(DEBUG_GCODE_PARSER)
  #include "../libs/hex_print_routines.h"
#endif

// Must be declared for allocation and to satisfy the linker
// Zero values need no initialization.

bool GCodeParser::volumetric_enabled;

#if ENABLED(INCH_MODE_SUPPORT)
  float GCodeParser::linear_unit_factor, GCodeParser::volumetric_unit_factor;
#endif

#if ENABLED(TEMPERATURE_UNITS_SUPPORT)
  TempUnit GCodeParser::input_temp_units;
#endif

char *GCodeParser::command_ptr,
     *GCodeParser::string_arg,
     *GCodeParser::value_ptr;
char GCodeParser::command_letter;
int GCodeParser::codenum;
#if USE_GCODE_SUBCODES
  uint8_t GCodeParser::subcode;
#endif

#if ENABLED(FASTER_GCODE_PARSER)
  // Optimized Parameters
  byte GCodeParser::codebits[4];   // found bits
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
  string_arg = NULL;                    // No whole line argument
  command_letter = '?';                 // No command letter
  codenum = 0;                          // No command code
  #if USE_GCODE_SUBCODES
    subcode = 0;                        // No command sub-code
  #endif
  #if ENABLED(FASTER_GCODE_PARSER)
    ZERO(codebits);                     // No codes yet
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

  // Bail if the letter is not G, M, or T
  switch (letter) { case 'G': case 'M': case 'T': break; default: return; }

  // Skip spaces to get the numeric part
  while (*p == ' ') p++;

  // Bail if there's no command code number
  if (!NUMERIC(*p)) return;

  // Save the command letter at this point
  // A '?' signifies an unknown command
  command_letter = letter;

  // Get the code number - integer digits only
  codenum = 0;
  do {
    codenum *= 10, codenum += *p++ - '0';
  } while (NUMERIC(*p));

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

  // The command parameters (if any) start here, for sure!

  #if DISABLED(FASTER_GCODE_PARSER)
    command_args = p; // Scan for parameters in seen()
  #endif

  // Only use string_arg for these M codes
  if (letter == 'M') switch (codenum) { case 23: case 28: case 30: case 117: case 928: string_arg = p; return; default: break; }

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
  string_arg = NULL;
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

      const bool has_num = NUMERIC(p[0])                            // [0-9]
                        || (p[0] == '.' && NUMERIC(p[1]))           // .[0-9]
                        || (
                              (p[0] == '-' || p[0] == '+') && (     // [-+]
                                NUMERIC(p[1])                       //     [0-9]
                                || (p[1] == '.' && NUMERIC(p[2]))   //     .[0-9]
                              )
                            );

      #if ENABLED(DEBUG_GCODE_PARSER)
        if (debug) {
          SERIAL_ECHOPAIR("Got letter ", code);
          SERIAL_ECHOPAIR(" at index ", (int)(p - command_ptr - 1));
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
      {
        set(code, has_num ? p : NULL            // Set parameter exists and pointer (NULL for no number)
          #if ENABLED(DEBUG_GCODE_PARSER)
            , debug
          #endif
        );
      }
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
        if (!*next_command) next_command = NULL;
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
  SERIAL_ECHOPAIR(MSG_UNKNOWN_COMMAND, command_ptr);
  SERIAL_CHAR('"');
  SERIAL_EOL();
}

#if ENABLED(DEBUG_GCODE_PARSER)

  void GCodeParser::debug() {
    SERIAL_ECHOPAIR("Command: ", command_ptr);
    SERIAL_ECHOPAIR(" (", command_letter);
    SERIAL_ECHO(codenum);
    SERIAL_ECHOLNPGM(")");
    #if ENABLED(FASTER_GCODE_PARSER)
      SERIAL_ECHO(" args: \"");
      for (char c = 'A'; c <= 'Z'; ++c)
        if (seen(c)) { SERIAL_CHAR(c); SERIAL_CHAR(' '); }
    #else
      SERIAL_ECHOPAIR(" args: \"", command_args);
    #endif
    SERIAL_ECHOPGM("\"");
    if (string_arg) {
      SERIAL_ECHOPGM(" string: \"");
      SERIAL_ECHO(string_arg);
      SERIAL_CHAR('"');
    }
    SERIAL_ECHOPGM("\n\n");
    for (char c = 'A'; c <= 'Z'; ++c) {
      if (seen(c)) {
        SERIAL_ECHOPAIR("Code '", c); SERIAL_ECHOPGM("':");
        if (has_value()) {
          SERIAL_ECHOPAIR("\n    float: ", value_float());
          SERIAL_ECHOPAIR("\n     long: ", value_long());
          SERIAL_ECHOPAIR("\n    ulong: ", value_ulong());
          SERIAL_ECHOPAIR("\n   millis: ", value_millis());
          SERIAL_ECHOPAIR("\n   sec-ms: ", value_millis_from_seconds());
          SERIAL_ECHOPAIR("\n      int: ", value_int());
          SERIAL_ECHOPAIR("\n   ushort: ", value_ushort());
          SERIAL_ECHOPAIR("\n     byte: ", (int)value_byte());
          SERIAL_ECHOPAIR("\n     bool: ", (int)value_bool());
          SERIAL_ECHOPAIR("\n   linear: ", value_linear_units());
          SERIAL_ECHOPAIR("\n  celsius: ", value_celsius());
        }
        else
          SERIAL_ECHOPGM(" (no value)");
        SERIAL_ECHOPGM("\n\n");
      }
    }
  }

#endif // DEBUG_GCODE_PARSER

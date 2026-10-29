/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../test/unit_tests.h"
#include "src/gcode/gcode.h"
#include "src/gcode/parser.h"

MARLIN_TEST(gcode, process_parsed_command) {
  GcodeSuite suite;
  parser.command_letter = 'G';
  parser.codenum = 0;
  suite.process_parsed_command(false);
}

MARLIN_TEST(gcode, parse_g_code_number_limit) {
  char max_command[] = "G65535";
  parser.parse(max_command);
  TEST_ASSERT_TRUE(parser.is_command('G', UINT16_MAX));

  char overflow_command[] = "G65536";
  parser.parse(overflow_command);
  TEST_ASSERT_EQUAL('?', parser.command_letter);
}

MARLIN_TEST(gcode, reject_m_code_overflow_alias) {
  char current_command[] = "M65648";
  parser.parse(current_command);
  TEST_ASSERT_EQUAL('?', parser.command_letter);
  TEST_ASSERT_FALSE(parser.is_command('M', 112));
}

MARLIN_TEST(gcode, parse_g1_xz) {
  char current_command[] = "G0 X10 Z30";
  parser.command_letter = -128;
  parser.codenum = -1;
  parser.parse(current_command);
  TEST_ASSERT_EQUAL('G', parser.command_letter);
  TEST_ASSERT_EQUAL(0, parser.codenum);
  TEST_ASSERT_TRUE(parser.seen('X'));
  TEST_ASSERT_FALSE(parser.seen('Y'));
  TEST_ASSERT_TRUE(parser.seen('Z'));
  TEST_ASSERT_FALSE(parser.seen('E'));
}

MARLIN_TEST(gcode, parse_g1_nxz) {
  char current_command[] = "N123 G0 X10 Z30";
  parser.command_letter = -128;
  parser.codenum = -1;
  parser.parse(current_command);
  TEST_ASSERT_EQUAL('G', parser.command_letter);
  TEST_ASSERT_EQUAL(0, parser.codenum);
  TEST_ASSERT_TRUE(parser.seen('X'));
  TEST_ASSERT_FALSE(parser.seen('Y'));
  TEST_ASSERT_TRUE(parser.seen('Z'));
  TEST_ASSERT_FALSE(parser.seen('E'));
}

MARLIN_TEST(gcode, dec_to_long) {
  TEST_ASSERT_EQUAL_INT32(0, GCodeParser::dec_to_long(""));
  TEST_ASSERT_EQUAL_INT32(0, GCodeParser::dec_to_long("abc"));
  TEST_ASSERT_EQUAL_INT32(12, GCodeParser::dec_to_long("12"));
  TEST_ASSERT_EQUAL_INT32(12, GCodeParser::dec_to_long("  12"));
  TEST_ASSERT_EQUAL_INT32(7, GCodeParser::dec_to_long("+7"));
  TEST_ASSERT_EQUAL_INT32(-45, GCodeParser::dec_to_long("-45"));
  TEST_ASSERT_EQUAL_INT32(3, GCodeParser::dec_to_long("3.99"));        // Stops at the point
  TEST_ASSERT_EQUAL_INT32(1, GCodeParser::dec_to_long("1e5"));         // No exponent
  TEST_ASSERT_EQUAL_INT32(0, GCodeParser::dec_to_long("0x10"));        // No hex
  TEST_ASSERT_EQUAL_INT32(0, GCodeParser::dec_to_long("--5"));
  TEST_ASSERT_EQUAL_INT32(0, GCodeParser::dec_to_long("-"));
  TEST_ASSERT_EQUAL_INT32(INT32_MAX, GCodeParser::dec_to_long("2147483647"));
  TEST_ASSERT_EQUAL_INT32(INT32_MAX, GCodeParser::dec_to_long("2147483648"));
  TEST_ASSERT_EQUAL_INT32(INT32_MAX, GCodeParser::dec_to_long("99999999999999"));
  TEST_ASSERT_EQUAL_INT32(INT32_MIN, GCodeParser::dec_to_long("-2147483648"));
  TEST_ASSERT_EQUAL_INT32(INT32_MIN, GCodeParser::dec_to_long("-2147483649"));
  TEST_ASSERT_EQUAL_INT32(INT32_MIN, GCodeParser::dec_to_long("-99999999999999"));
}

MARLIN_TEST(gcode, dec_to_ulong) {
  TEST_ASSERT_EQUAL_UINT32(0, GCodeParser::dec_to_ulong(""));
  TEST_ASSERT_EQUAL_UINT32(1234, GCodeParser::dec_to_ulong("1234"));
  TEST_ASSERT_EQUAL_UINT32(2147483648UL, GCodeParser::dec_to_ulong("2147483648"));
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, GCodeParser::dec_to_ulong("4294967295"));
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, GCodeParser::dec_to_ulong("4294967296"));
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, GCodeParser::dec_to_ulong("99999999999999"));
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, GCodeParser::dec_to_ulong("-1"));  // A minus sign negates, as strtoul does
}

MARLIN_TEST(gcode, dec_to_float) {
  TEST_ASSERT_EQUAL_FLOAT(0, GCodeParser::dec_to_float(""));
  TEST_ASSERT_EQUAL_FLOAT(0, GCodeParser::dec_to_float("abc"));
  TEST_ASSERT_EQUAL_FLOAT(0, GCodeParser::dec_to_float("nan"));
  TEST_ASSERT_EQUAL_FLOAT(0, GCodeParser::dec_to_float("inf"));
  TEST_ASSERT_EQUAL_FLOAT(10, GCodeParser::dec_to_float("10"));
  TEST_ASSERT_EQUAL_FLOAT(0.5f, GCodeParser::dec_to_float(".5"));
  TEST_ASSERT_EQUAL_FLOAT(5, GCodeParser::dec_to_float("5."));
  TEST_ASSERT_EQUAL_FLOAT(-0.5f, GCodeParser::dec_to_float("-.5"));
  TEST_ASSERT_EQUAL_FLOAT(12.34f, GCodeParser::dec_to_float(" +12.34"));
  TEST_ASSERT_EQUAL_FLOAT(0.001f, GCodeParser::dec_to_float("0.001"));
  TEST_ASSERT_EQUAL_FLOAT(0.0001f, GCodeParser::dec_to_float("0.0001"));
  TEST_ASSERT_EQUAL_FLOAT(123.456f, GCodeParser::dec_to_float("123.456"));
  TEST_ASSERT_EQUAL_FLOAT(-1234.5678f, GCodeParser::dec_to_float("-1234.5678"));
  TEST_ASSERT_EQUAL_FLOAT(8388607, GCodeParser::dec_to_float("8388607"));
  TEST_ASSERT_EQUAL_FLOAT(1, GCodeParser::dec_to_float("1E5"));        // No exponent
  TEST_ASSERT_EQUAL_FLOAT(0, GCodeParser::dec_to_float("0x10"));       // No hex
  TEST_ASSERT_EQUAL_FLOAT(1.5f, GCodeParser::dec_to_float("1.5.7"));   // Stops at the second point
  TEST_ASSERT_EQUAL_FLOAT(10000000000.0f, GCodeParser::dec_to_float("10000000000"));  // Digits past the 9th scale the value
}

MARLIN_TEST(gcode, value_of_parameters) {
  char current_command[] = "G1 X-12.5 Y0.25 F3000 E1e3 S4294967295";
  parser.parse(current_command);
  TEST_ASSERT_EQUAL_FLOAT(-12.5f, parser.floatval('X'));
  TEST_ASSERT_EQUAL_FLOAT(0.25f, parser.floatval('Y'));
  TEST_ASSERT_EQUAL_INT32(3000, parser.longval('F'));
  TEST_ASSERT_EQUAL_FLOAT(1, parser.floatval('E'));                    // 'E' ends a number, so no scientific notation
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, parser.ulongval('S'));
}

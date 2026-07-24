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

#if ANY(SWITCHING_TOOLHEAD, MAGNETIC_SWITCHING_TOOLHEAD, ELECTROMAGNETIC_SWITCHING_TOOLHEAD)

MARLIN_TEST(gcode, parse_explicit_positive_signed_code) {
  char explicit_positive[] = "T+1";
  parser.parse(explicit_positive);
  TEST_ASSERT_EQUAL('T', parser.command_letter);
  TEST_ASSERT_EQUAL(1, int16_t(parser.codenum));
}

MARLIN_TEST(gcode, parse_negative_signed_code) {
  char negative[] = "T-1";
  parser.parse(negative);
  TEST_ASSERT_EQUAL('T', parser.command_letter);
  TEST_ASSERT_EQUAL(-1, int16_t(parser.codenum));
}

MARLIN_TEST(gcode, reject_negative_sign_without_digits) {
  char missing_digits[] = "M-";
  parser.parse(missing_digits);
  TEST_ASSERT_EQUAL('?', parser.command_letter);
}

MARLIN_TEST(gcode, reject_positive_sign_without_digits) {
  char missing_digits[] = "M+";
  parser.parse(missing_digits);
  TEST_ASSERT_EQUAL('?', parser.command_letter);
}

#endif

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

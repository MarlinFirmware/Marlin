#include "../test/marlin_tests.h"
#include <src/gcode/gcode.h>
#include <src/gcode/parser.h>

MARLIN_TEST(gcode, process_parsed_command) {
  GcodeSuite suite;
  parser.command_letter = 'G';
  parser.codenum = 0;
  suite.process_parsed_command(false);
}

MARLIN_TEST(gcode, parse_g1_xz)
{
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

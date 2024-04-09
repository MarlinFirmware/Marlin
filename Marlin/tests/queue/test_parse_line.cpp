#include "../test/marlin_tests.h"
#include <unity.h>
#include <src/gcode/queue.h>

static void test_parse_line(const char *command, const char *expected) {
  char buffer[MAX_CMD_SIZE];
  int buffer_index = 0;
  uint8_t input_state = PS_NORMAL;

  for (int position = 0 ; command[position] ; position++) {
    process_stream_char(command[position], input_state, buffer, buffer_index);
  }
  buffer[buffer_index] = 0;

  TEST_ASSERT_EQUAL_STRING(expected, buffer);
}

MARLIN_TEST(parse_line, g1_x_and_parenthesis_comment) {
  #if ENABLED(PAREN_COMMENTS)
    test_parse_line("G0 X10 (Z30)", "G0 X10 ");
  #else
    test_parse_line("G0 X10 (Z30)", "G0 X10 (Z30)");
  #endif
}

MARLIN_TEST(parse_line, g1_x_and_parenthesis_inline_comment) {
  #if ENABLED(PAREN_COMMENTS)
    test_parse_line("G0 X10 (Y20) Z30", "G0 X10  Z30");
  #else
    test_parse_line("G0 X10 (Y20) Z30", "G0 X10 (Y20) Z30");
  #endif
}

MARLIN_TEST(parse_line, g1_xz) {
  test_parse_line("G0 X10 Z30", "G0 X10 Z30");
}

MARLIN_TEST(parse_line, g1_x_and_comment) {
  test_parse_line("G0 X10 ; Z30", "G0 X10 ");
}

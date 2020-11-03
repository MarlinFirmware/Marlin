#include <unity.h>
#include <src/gcode/queue.h>

#ifndef TESTS_QUEUE_BASE
#define TESTS_QUEUE_BASE

void test_parse_line(const char *command, const char *expected) {
  char buffer[MAX_CMD_SIZE];
  int buffer_index = 0;
  uint8_t input_state = PS_NORMAL;

  for (int position = 0 ; command[position] ; position++) {
    process_stream_char(command[position], input_state, buffer, buffer_index);
  }
  buffer[buffer_index] = 0;

  TEST_ASSERT_EQUAL_STRING(expected, buffer);
}

#endif

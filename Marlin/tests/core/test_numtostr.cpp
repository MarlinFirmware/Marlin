/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "src/core/types.h"
#include "src/libs/numtostr.h"

#include <math.h>

static const char * F2S(const float v, const int8_t width, const uint8_t prec) {
  static char buf[FTOSTRF_BUFSIZE];
  return ftostrf(buf, v, width, prec);
}

MARLIN_TEST(numtostr, ftostrf_precision) {
  TEST_ASSERT_EQUAL_STRING("123",     F2S(123.456f, 1, 0));
  TEST_ASSERT_EQUAL_STRING("123.5",   F2S(123.456f, 1, 1));
  TEST_ASSERT_EQUAL_STRING("123.46",  F2S(123.456f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("123.456", F2S(123.456f, 1, 3));
}

MARLIN_TEST(numtostr, ftostrf_rounds_up_to_the_next_digit) {
  TEST_ASSERT_EQUAL_STRING("2.00",   F2S(1.999f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("10.0",   F2S(9.96f, 1, 1));
  TEST_ASSERT_EQUAL_STRING("100",    F2S(99.5f, 1, 0));
  TEST_ASSERT_EQUAL_STRING("0.50",   F2S(0.499999f, 1, 2));
}

MARLIN_TEST(numtostr, ftostrf_small_values_and_zero) {
  TEST_ASSERT_EQUAL_STRING("0",       F2S(0.0f, 1, 0));
  TEST_ASSERT_EQUAL_STRING("0.00",    F2S(0.0f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("0.05",    F2S(0.05f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("0.001",   F2S(0.001f, 1, 3));
  TEST_ASSERT_EQUAL_STRING("0.000",   F2S(0.0004f, 1, 3));
  TEST_ASSERT_EQUAL_STRING("0.000001", F2S(0.000001f, 1, 6));
}

MARLIN_TEST(numtostr, ftostrf_negative) {
  TEST_ASSERT_EQUAL_STRING("-1.50",   F2S(-1.5f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("-0.50",   F2S(-0.5f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("-123.46", F2S(-123.456f, 1, 2));
}

MARLIN_TEST(numtostr, ftostrf_temperatures_and_positions) {
  TEST_ASSERT_EQUAL_STRING("20.00",   F2S(20.0f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("210.12",  F2S(210.12f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("37.62",   F2S(37.62f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("299.99",  F2S(299.99f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("-12.345", F2S(-12.345f, 1, 3));
  TEST_ASSERT_EQUAL_STRING("0.025",   F2S(0.025f, 1, 3));
}

MARLIN_TEST(numtostr, ftostrf_width) {
  TEST_ASSERT_EQUAL_STRING("  1.50",  F2S(1.5f, 6, 2));
  TEST_ASSERT_EQUAL_STRING("-1.50",   F2S(-1.5f, 5, 2));
  TEST_ASSERT_EQUAL_STRING(" -1.50",  F2S(-1.5f, 6, 2));
  TEST_ASSERT_EQUAL_STRING("1.50  ",  F2S(1.5f, -6, 2));
  TEST_ASSERT_EQUAL_STRING("-1.50 ",  F2S(-1.5f, -6, 2));
  TEST_ASSERT_EQUAL_STRING("123.456", F2S(123.456f, 3, 3)); // Wider than the field: not truncated
}

MARLIN_TEST(numtostr, ftostrf_width_is_limited_to_the_buffer) {
  const char * const s = F2S(1.0f, 100, 1);
  TEST_ASSERT_EQUAL(FTOSTRF_BUFSIZE - 1, strlen(s));
  TEST_ASSERT_EQUAL_STRING("1.0", s + (FTOSTRF_BUFSIZE - 1 - 3));
  const char * const t = F2S(1.0f, -100, 1);
  TEST_ASSERT_EQUAL(FTOSTRF_BUFSIZE - 1, strlen(t));
  TEST_ASSERT_EQUAL_CHAR('1', t[0]);
}

MARLIN_TEST(numtostr, ftostrf_precision_is_limited) {
  TEST_ASSERT_EQUAL_STRING("0.500000", F2S(0.5f, 1, 9));
}

MARLIN_TEST(numtostr, ftostrf_large_values) {
  TEST_ASSERT_EQUAL_STRING("1234567.00", F2S(1234567.0f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("100000000.0000", F2S(100000000.0f, 1, 4));
  TEST_ASSERT_EQUAL_STRING("ovf",    F2S(1.0e10f, 1, 2));
  TEST_ASSERT_EQUAL_STRING("-ovf",   F2S(-1.0e10f, 1, 2));
}

MARLIN_TEST(numtostr, ftostrf_not_a_number_and_infinity) {
  TEST_ASSERT_EQUAL_STRING("nan",  F2S(NAN, 1, 2));
  TEST_ASSERT_EQUAL_STRING("ovf",  F2S(INFINITY, 1, 2));
  TEST_ASSERT_EQUAL_STRING("-ovf", F2S(-INFINITY, 1, 2));
}

MARLIN_TEST(numtostr, ftostrf_in_mstring) {
  MString<20> str;
  str.append(F("T:"), p_float_t(20.0f, 2), F(" /"), p_float_t(0.0f, 2));
  TEST_ASSERT_EQUAL_STRING("T:20.00 /0.00", &str);

  str.set(w_float_t(1.5f, 6, 1));
  TEST_ASSERT_EQUAL_STRING("   1.5", &str);

  str.set(p_float_t(-0.25f, 2));
  TEST_ASSERT_EQUAL_STRING("-0.25", &str);
}

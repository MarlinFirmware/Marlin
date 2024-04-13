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
#include <src/core/macros.h>

MARLIN_TEST(macros_bitwise_8, TEST) {
  uint8_t odd_set = 0xAA;
  uint8_t even_set = 0x55;
  for (uint8_t b = 0; b < 8; ++b) {
    TEST_ASSERT_EQUAL((b % 2) != 0, TEST(odd_set, b));
    TEST_ASSERT_EQUAL((b % 2) == 0, TEST(even_set, b));
  }
}

MARLIN_TEST(macros_bitwise_8, SET_BIT_TO) {
  uint8_t n = 0x00;

  // Test LSB
  SET_BIT_TO(n, 0, true);
  TEST_ASSERT_EQUAL(0x01, n);
  SET_BIT_TO(n, 0, false);
  TEST_ASSERT_EQUAL(0x00, n);

  // Test MSB
  SET_BIT_TO(n, 7, true);
  TEST_ASSERT_EQUAL(0x80, n);
  SET_BIT_TO(n, 7, false);
  TEST_ASSERT_EQUAL(0x00, n);

  // Test a bit in the middle
  SET_BIT_TO(n, 3, true);
  TEST_ASSERT_EQUAL(0x08, n);
  SET_BIT_TO(n, 3, false);
  TEST_ASSERT_EQUAL(0x00, n);
}

MARLIN_TEST(macros_bitwise_8, SBI) {
  uint8_t n;

  // Test LSB
  n = 0x00;
  SBI(n, 0);
  TEST_ASSERT_EQUAL(0x01, n);

  // Test MSB
  n = 0x00;
  SBI(n, 7);
  TEST_ASSERT_EQUAL(0x80, n);

  // Test a bit in the middle
  n = 0x00;
  SBI(n, 3);
  TEST_ASSERT_EQUAL(0x08, n);
}

MARLIN_TEST(macros_bitwise_8, CBI) {
  uint8_t n;

  // Test LSB
  n = 0xFF;
  CBI(n, 0);
  TEST_ASSERT_EQUAL(0xFE, n);

  // Test MSB
  n = 0xFF;
  CBI(n, 7);
  TEST_ASSERT_EQUAL(0x7F, n);

  // Test a bit in the middle
  n = 0xFF;
  CBI(n, 3);
  TEST_ASSERT_EQUAL(0xF7, n);
}

MARLIN_TEST(macros_bitwise_8, TBI) {
  uint8_t n;

  // Test LSB
  n = 0xAA;
  TBI(n, 0);
  TEST_ASSERT_EQUAL(0xAB, n);

  // Test MSB
  n = 0xAA;
  TBI(n, 7);
  TEST_ASSERT_EQUAL(0x2A, n);

  // Test a bit in the middle
  n = 0xAA;
  TBI(n, 3);
  TEST_ASSERT_EQUAL(0xA2, n);
}

// 32-bit BIT operation tests
// These verify the above macros, but specifically with the MSB of a uint32_t.
// This ensures that the macros are not limited to 8-bit operations.

MARLIN_TEST(macros_bitwise_32, TEST_32bit) {
  uint32_t odd_set = 0x80000000;
  uint32_t even_set = 0x00000000;
  TEST_ASSERT_EQUAL(true, TEST(odd_set, 31));
  TEST_ASSERT_EQUAL(false, TEST(even_set, 31));
}

MARLIN_TEST(macros_bitwise_32, SET_BIT_TO_32bit) {
  uint32_t n = 0x00000000;

  // Test MSB
  SET_BIT_TO(n, 31, true);
  TEST_ASSERT_EQUAL(0x80000000, n);
  SET_BIT_TO(n, 31, false);
  TEST_ASSERT_EQUAL(0x00000000, n);
}

MARLIN_TEST(macros_bitwise_32, SBI_32bit) {
  uint32_t n = 0x00000000;

  // Test MSB
  SBI(n, 31);
  TEST_ASSERT_EQUAL(0x80000000, n);
}

MARLIN_TEST(macros_bitwise_32, CBI_32bit) {
  uint32_t n = 0xFFFFFFFF;

  // Test MSB
  CBI(n, 31);
  TEST_ASSERT_EQUAL(0x7FFFFFFF, n);
}

MARLIN_TEST(macros_bitwise_32, TBI_32bit) {
  uint32_t n = 0x7FFFFFFF;

  // Test MSB
  TBI(n, 31);
  TEST_ASSERT_EQUAL(0xFFFFFFFF, n);
}

// Geometry macros
MARLIN_TEST(macros_geometry, cu_int) {
  TEST_ASSERT_EQUAL(8, cu(2));
  TEST_ASSERT_EQUAL(27, cu(3));
}

MARLIN_TEST(macros_geometry, cu_float) {
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 8.615f, cu(2.05f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 28.094f, cu(3.04f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 13.998f, cu(2.41f));
}

MARLIN_TEST(macros_geometry, RADIANS) {
  TEST_ASSERT_FLOAT_WITHIN(0.001f, float(M_PI), RADIANS(180.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, RADIANS(0.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, float(M_PI) / 4, RADIANS(45.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, float(M_PI) / 2, RADIANS(90.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 3 * float(M_PI) / 2, RADIANS(270.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 4 * float(M_PI), RADIANS(720.0f));
}

MARLIN_TEST(macros_geometry, DEGREES) {
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, DEGREES(float(M_PI)));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, DEGREES(0.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 45.0f, DEGREES(float(M_PI) / 4));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, DEGREES(float(M_PI) / 2));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 270.0f, DEGREES(3 * float(M_PI) / 2));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 720.0f, DEGREES(4 * float(M_PI)));
}

MARLIN_TEST(macros_geometry, HYPOT2) {
  TEST_ASSERT_EQUAL(25, HYPOT2(3, 4));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 13.0f, HYPOT2(2.0f, 3.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 18.72f, HYPOT2(2.4f, 3.6f));
}

MARLIN_TEST(macros_geometry, NORMSQ) {
  TEST_ASSERT_EQUAL(14, NORMSQ(1, 2, 3));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 14.0f, NORMSQ(1.0f, 2.0f, 3.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 20.16f, NORMSQ(1.2f, 2.4f, 3.6f));
}

MARLIN_TEST(macros_geometry, CIRCLE_AREA) {
  TEST_ASSERT_EQUAL(float(M_PI) * 4, CIRCLE_AREA(2));
}

MARLIN_TEST(macros_geometry, CIRCLE_CIRC) {
  TEST_ASSERT_EQUAL(2 * float(M_PI) * 3, CIRCLE_CIRC(3));
}

MARLIN_TEST(macros_numeric, SIGN) {
  TEST_ASSERT_EQUAL(1, SIGN(100));
  TEST_ASSERT_EQUAL(-1, SIGN(-100));
  TEST_ASSERT_EQUAL(0, SIGN(0));
}

MARLIN_TEST(macros_numeric, IS_POWER_OF_2) {
  TEST_ASSERT_EQUAL(false, IS_POWER_OF_2(0));
  TEST_ASSERT_EQUAL(true, IS_POWER_OF_2(1));
  TEST_ASSERT_EQUAL(true, IS_POWER_OF_2(4));
  TEST_ASSERT_EQUAL(false, IS_POWER_OF_2(5));
  TEST_ASSERT_EQUAL(false, IS_POWER_OF_2(0x80000001));
  TEST_ASSERT_EQUAL(true, IS_POWER_OF_2(0x80000000));
}

// Numeric constraints
MARLIN_TEST(macros_numeric, NOLESS_int) {
  // Scenario 1: Input was already acceptable
  int a = 8;
  NOLESS(a, 5);
  TEST_ASSERT_EQUAL(8, a);

  // Original scenario: Input was less than the limit
  a = 5;
  NOLESS(a, 10);
  TEST_ASSERT_EQUAL(10, a);

  // Scenario 2: Input is negative, and coerces to a positive number
  a = -5;
  NOLESS(a, 0);
  TEST_ASSERT_EQUAL(0, a);

  // Scenario 3: Input is negative, and coerces to another negative number
  a = -10;
  NOLESS(a, -5);
  TEST_ASSERT_EQUAL(-5, a);
}

MARLIN_TEST(macros_numeric, NOLESS_uint) {
  // Scenario 1: Input was already acceptable
  unsigned int b = 8u;
  NOLESS(b, 5u);
  TEST_ASSERT_EQUAL(8u, b);

  // Original scenario: Input was less than the limit
  b = 5u;
  NOLESS(b, 10u);
  TEST_ASSERT_EQUAL(10u, b);
}

MARLIN_TEST(macros_numeric, NOLESS_float) {
  // Scenario 1: Input was already acceptable
  float c = 8.5f;
  NOLESS(c, 5.5f);
  TEST_ASSERT_EQUAL_FLOAT(8.5f, c);

  // Original scenario: Input was less than the limit
  c = 5.5f;
  NOLESS(c, 10.5f);
  TEST_ASSERT_EQUAL_FLOAT(10.5f, c);

  // Scenario 2: Input is negative, and coerces to a positive number
  c = -5.5f;
  NOLESS(c, 5.0f);
  TEST_ASSERT_EQUAL_FLOAT(5.0f, c);

  // Scenario 3: Input is negative, and coerces to another negative number
  c = -10.5f;
  NOLESS(c, -5.5f);
  TEST_ASSERT_EQUAL_FLOAT(-5.5f, c);
  c = -5.5f;
  NOLESS(c, -10.5f);
  TEST_ASSERT_EQUAL_FLOAT(-5.5f, c);
}

MARLIN_TEST(macros_numeric, NOMORE_int) {
  // Scenario 1: Input was already acceptable
  int a = 8;
  NOMORE(a, 10);
  TEST_ASSERT_EQUAL(8, a);

  // Original scenario: Input was more than the limit
  a = 15;
  NOMORE(a, 10);
  TEST_ASSERT_EQUAL(10, a);

  // Scenario 2: Input is positive, and coerces to a negative number
  a = 5;
  NOMORE(a, -2);
  TEST_ASSERT_EQUAL(-2, a);

  // Scenario 3: Input is negative, and coerces to another negative number
  a = -5;
  NOMORE(a, -10);
  TEST_ASSERT_EQUAL(-10, a);
}

MARLIN_TEST(macros_numeric, NOMORE_uint) {
  // Scenario 1: Input was already acceptable
  unsigned int b = 8u;
  NOMORE(b, 10u);
  TEST_ASSERT_EQUAL(8u, b);

  // Original scenario: Input was more than the limit
  b = 15u;
  NOMORE(b, 10u);
  TEST_ASSERT_EQUAL(10u, b);
}

MARLIN_TEST(macros_numeric, NOMORE_float) {
  // Scenario 1: Input was already acceptable
  float c = 8.5f;
  NOMORE(c, 10.5f);
  TEST_ASSERT_EQUAL_FLOAT(8.5f, c);

  // Original scenario: Input was more than the limit
  c = 15.5f;
  NOMORE(c, 10.5f);
  TEST_ASSERT_EQUAL_FLOAT(10.5f, c);

  // Scenario 2: Input is positive, and coerces to a negative number
  c = 5.5f;
  NOMORE(c, -1.7f);
  TEST_ASSERT_EQUAL_FLOAT(-1.7f, c);

  // Scenario 3: Input is negative, and coerces to another negative number
  c = -5.5f;
  NOMORE(c, -10.5f);
  TEST_ASSERT_EQUAL_FLOAT(-10.5f, c);
}

MARLIN_TEST(macros_numeric, LIMIT_int) {
  int a = 15;
  LIMIT(a, 10, 20);
  TEST_ASSERT_EQUAL(15, a);

  a = 5;
  LIMIT(a, 10, 20);
  TEST_ASSERT_EQUAL(10, a);

  a = 25;
  LIMIT(a, 10, 20);
  TEST_ASSERT_EQUAL(20, a);

  // Scenario: Range is [-10, -5]
  a = -8;
  LIMIT(a, -10, -5);
  TEST_ASSERT_EQUAL(-8, a);

  a = -12;
  LIMIT(a, -10, -5);
  TEST_ASSERT_EQUAL(-10, a);

  a = -3;
  LIMIT(a, -10, -5);
  TEST_ASSERT_EQUAL(-5, a);

  // Scenario: Range is [-10, 5]
  a = 0;
  LIMIT(a, -10, 5);
  TEST_ASSERT_EQUAL(0, a);

  a = -12;
  LIMIT(a, -10, 5);
  TEST_ASSERT_EQUAL(-10, a);

  a = 6;
  LIMIT(a, -10, 5);
  TEST_ASSERT_EQUAL(5, a);
}

MARLIN_TEST(macros_numeric, LIMIT_uint) {
  unsigned int b = 15u;
  LIMIT(b, 10u, 20u);
  TEST_ASSERT_EQUAL(15u, b);

  b = 5u;
  LIMIT(b, 10u, 20u);
  TEST_ASSERT_EQUAL(10u, b);

  b = 25u;
  LIMIT(b, 10u, 20u);
  TEST_ASSERT_EQUAL(20u, b);
}

MARLIN_TEST(macros_numeric, LIMIT_float) {
  float c = 15.5f;
  LIMIT(c, 10.5f, 20.5f);
  TEST_ASSERT_EQUAL_FLOAT(15.5f, c);

  c = 5.5f;
  LIMIT(c, 10.5f, 20.5f);
  TEST_ASSERT_EQUAL_FLOAT(10.5f, c);

  c = 25.5f;
  LIMIT(c, 10.5f, 20.5f);
  TEST_ASSERT_EQUAL_FLOAT(20.5f, c);

  // Scenario: Range is [-10.5, -5.5]
  c = -8.5f;
  LIMIT(c, -10.5f, -5.5f);
  TEST_ASSERT_EQUAL_FLOAT(-8.5f, c);

  c = -12.5f;
  LIMIT(c, -10.5f, -5.5f);
  TEST_ASSERT_EQUAL_FLOAT(-10.5f, c);

  c = -3.5f;
  LIMIT(c, -10.5f, -5.5f);
  TEST_ASSERT_EQUAL_FLOAT(-5.5f, c);

  // Scenario: Range is [-10.5, 5.5]
  c = 0.0f;
  LIMIT(c, -10.5f, 5.5f);
  TEST_ASSERT_EQUAL_FLOAT(0.0f, c);

  c = -12.5f;
  LIMIT(c, -10.5f, 5.5f);
  TEST_ASSERT_EQUAL_FLOAT(-10.5f, c);

  c = 6.5f;
  LIMIT(c, -10.5f, 5.5f);
  TEST_ASSERT_EQUAL_FLOAT(5.5f, c);
}


// Looping macros
MARLIN_TEST(macros_looping, DO_macro) {
  #define _M_1(A) (A)
  int sum = DO(M, +, 1, 2, 3, 4, 5);
  TEST_ASSERT_EQUAL(15, sum);

  // Test with maximum number of arguments
  sum = DO(M, +, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
           21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40);
  TEST_ASSERT_EQUAL(820, sum);
  #undef _M_1
}

// Configuration Options
MARLIN_TEST(macros_options, ENABLED_DISABLED) {
  #define OPTION_A
  #define OPTION_B 1
  #define OPTION_C true
  #define OPTION_D 0
  #define OPTION_E false
  // #define OPTION_F

  // Test ENABLED macro
  TEST_ASSERT_TRUE(ENABLED(OPTION_A));
  TEST_ASSERT_TRUE(ENABLED(OPTION_B));
  TEST_ASSERT_TRUE(ENABLED(OPTION_C));
  TEST_ASSERT_FALSE(ENABLED(OPTION_D));
  TEST_ASSERT_FALSE(ENABLED(OPTION_E));
  TEST_ASSERT_FALSE(ENABLED(OPTION_F));

  // Test DISABLED macro
  TEST_ASSERT_FALSE(DISABLED(OPTION_A));
  TEST_ASSERT_FALSE(DISABLED(OPTION_B));
  TEST_ASSERT_FALSE(DISABLED(OPTION_C));
  TEST_ASSERT_TRUE(DISABLED(OPTION_D));
  TEST_ASSERT_TRUE(DISABLED(OPTION_E));
  TEST_ASSERT_TRUE(DISABLED(OPTION_F));

  #undef OPTION_A
  #undef OPTION_B
  #undef OPTION_C
  #undef OPTION_D
  #undef OPTION_E
}

MARLIN_TEST(macros_options, ANY) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test ANY macro
  TEST_ASSERT_TRUE(ANY(OPTION_DISABLED, OPTION_ENABLED, OPTION_DISABLED)); // Enabled option in the middle
  TEST_ASSERT_TRUE(ANY(OPTION_ENABLED, OPTION_DISABLED, OPTION_DISABLED)); // Enabled option at the beginning
  TEST_ASSERT_TRUE(ANY(OPTION_DISABLED, OPTION_DISABLED, OPTION_ENABLED)); // Enabled option at the end
  TEST_ASSERT_FALSE(ANY(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, ALL) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test ALL macro
  TEST_ASSERT_TRUE(ALL(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_FALSE(ALL(OPTION_ENABLED, OPTION_DISABLED, OPTION_ENABLED)); // Disabled option in the middle
  TEST_ASSERT_FALSE(ALL(OPTION_DISABLED, OPTION_ENABLED, OPTION_ENABLED)); // Disabled option at the beginning
  TEST_ASSERT_FALSE(ALL(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Disabled option at the end
  TEST_ASSERT_FALSE(ALL(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, NONE) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test NONE macro
  TEST_ASSERT_FALSE(NONE(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_FALSE(NONE(OPTION_ENABLED, OPTION_DISABLED, OPTION_ENABLED)); // Disabled option in the middle
  TEST_ASSERT_FALSE(NONE(OPTION_DISABLED, OPTION_ENABLED, OPTION_ENABLED)); // Disabled option at the beginning
  TEST_ASSERT_FALSE(NONE(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Disabled option at the end
  TEST_ASSERT_TRUE(NONE(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, COUNT_ENABLED) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test COUNT_ENABLED macro
  TEST_ASSERT_EQUAL(3, COUNT_ENABLED(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_EQUAL(2, COUNT_ENABLED(OPTION_ENABLED, OPTION_DISABLED, OPTION_ENABLED)); // Disabled option in the middle
  TEST_ASSERT_EQUAL(2, COUNT_ENABLED(OPTION_DISABLED, OPTION_ENABLED, OPTION_ENABLED)); // Disabled option at the beginning
  TEST_ASSERT_EQUAL(2, COUNT_ENABLED(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Disabled option at the end
  TEST_ASSERT_EQUAL(0, COUNT_ENABLED(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, MANY) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test MANY macro
  TEST_ASSERT_FALSE(MANY(OPTION_ENABLED, OPTION_DISABLED, OPTION_DISABLED)); // Only one option enabled
  TEST_ASSERT_TRUE(MANY(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Two options enabled
  TEST_ASSERT_TRUE(MANY(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_FALSE(MANY(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // No options enabled

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}


// Ternary macros
MARLIN_TEST(macros_options, TERN) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test TERN macro
  TEST_ASSERT_EQUAL(1, TERN(OPTION_ENABLED, 1, 0)); // OPTION_ENABLED is enabled, so it should return '1'
  TEST_ASSERT_EQUAL(0, TERN(OPTION_DISABLED, 1, 0)); // OPTION_DISABLED is disabled, so it should return '0'

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, TERN0) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test TERN0 macro
  TEST_ASSERT_EQUAL(1, TERN0(OPTION_ENABLED, 1)); // OPTION_ENABLED is enabled, so it should return '1'
  TEST_ASSERT_EQUAL(0, TERN0(OPTION_DISABLED, 1)); // OPTION_DISABLED is disabled, so it should return '0'

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, TERN1) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test TERN1 macro
  TEST_ASSERT_EQUAL(0, TERN1(OPTION_ENABLED, 0)); // OPTION_ENABLED is enabled, so it should return '0'
  TEST_ASSERT_EQUAL(1, TERN1(OPTION_DISABLED, 0)); // OPTION_DISABLED is disabled, so it should return '1'

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, TERN_) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test TERN_ macro by conditionally inverting the input
  TEST_ASSERT_EQUAL(-1, TERN_(OPTION_ENABLED, -)1); // OPTION_ENABLED is enabled, so it should return '1'
  TEST_ASSERT_EQUAL(1, TERN_(OPTION_DISABLED, -)1); // OPTION_DISABLED is disabled, so it should return nothing

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

MARLIN_TEST(macros_options, IF_DISABLED) {
  #define OPTION_ENABLED 1
  #define OPTION_DISABLED 0

  // Test IF_DISABLED macro by conditionally inverting the input
  TEST_ASSERT_EQUAL(1, IF_DISABLED(OPTION_ENABLED, -)1); // OPTION_ENABLED is enabled, so it should return nothing
  TEST_ASSERT_EQUAL(-1, IF_DISABLED(OPTION_DISABLED, -)1); // OPTION_DISABLED is disabled, so it should return '1'

  #undef OPTION_ENABLED
  #undef OPTION_DISABLED
}

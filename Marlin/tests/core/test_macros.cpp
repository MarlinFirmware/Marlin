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

// These represent enabled and disabled configuration options for testing.
// They will be used by multiple tests.
#define OPTION_ENABLED 1
#define OPTION_DISABLED 0

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
  TEST_ASSERT_TRUE(ANY(OPTION_DISABLED, OPTION_ENABLED, OPTION_DISABLED)); // Enabled option in the middle
  TEST_ASSERT_TRUE(ANY(OPTION_ENABLED, OPTION_DISABLED, OPTION_DISABLED)); // Enabled option at the beginning
  TEST_ASSERT_TRUE(ANY(OPTION_DISABLED, OPTION_DISABLED, OPTION_ENABLED)); // Enabled option at the end
  TEST_ASSERT_FALSE(ANY(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled
}

MARLIN_TEST(macros_options, ALL) {
  TEST_ASSERT_TRUE(ALL(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_FALSE(ALL(OPTION_ENABLED, OPTION_DISABLED, OPTION_ENABLED)); // Disabled option in the middle
  TEST_ASSERT_FALSE(ALL(OPTION_DISABLED, OPTION_ENABLED, OPTION_ENABLED)); // Disabled option at the beginning
  TEST_ASSERT_FALSE(ALL(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Disabled option at the end
  TEST_ASSERT_FALSE(ALL(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled
}

MARLIN_TEST(macros_options, NONE) {
  TEST_ASSERT_FALSE(NONE(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_FALSE(NONE(OPTION_ENABLED, OPTION_DISABLED, OPTION_ENABLED)); // Disabled option in the middle
  TEST_ASSERT_FALSE(NONE(OPTION_DISABLED, OPTION_ENABLED, OPTION_ENABLED)); // Disabled option at the beginning
  TEST_ASSERT_FALSE(NONE(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Disabled option at the end
  TEST_ASSERT_TRUE(NONE(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled
}

MARLIN_TEST(macros_options, COUNT_ENABLED) {
  TEST_ASSERT_EQUAL(3, COUNT_ENABLED(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_EQUAL(2, COUNT_ENABLED(OPTION_ENABLED, OPTION_DISABLED, OPTION_ENABLED)); // Disabled option in the middle
  TEST_ASSERT_EQUAL(2, COUNT_ENABLED(OPTION_DISABLED, OPTION_ENABLED, OPTION_ENABLED)); // Disabled option at the beginning
  TEST_ASSERT_EQUAL(2, COUNT_ENABLED(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Disabled option at the end
  TEST_ASSERT_EQUAL(0, COUNT_ENABLED(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // All options disabled
}

MARLIN_TEST(macros_options, MANY) {
  TEST_ASSERT_FALSE(MANY(OPTION_ENABLED, OPTION_DISABLED, OPTION_DISABLED)); // Only one option enabled
  TEST_ASSERT_TRUE(MANY(OPTION_ENABLED, OPTION_ENABLED, OPTION_DISABLED)); // Two options enabled
  TEST_ASSERT_TRUE(MANY(OPTION_ENABLED, OPTION_ENABLED, OPTION_ENABLED)); // All options enabled
  TEST_ASSERT_FALSE(MANY(OPTION_DISABLED, OPTION_DISABLED, OPTION_DISABLED)); // No options enabled
}


// Ternary macros
MARLIN_TEST(macros_options, TERN) {
  TEST_ASSERT_EQUAL(1, TERN(OPTION_ENABLED, 1, 0)); // OPTION_ENABLED is enabled, so it should return '1'
  TEST_ASSERT_EQUAL(0, TERN(OPTION_DISABLED, 1, 0)); // OPTION_DISABLED is disabled, so it should return '0'
}

MARLIN_TEST(macros_options, TERN0) {
  TEST_ASSERT_EQUAL(1, TERN0(OPTION_ENABLED, 1)); // OPTION_ENABLED is enabled, so it should return '1'
  TEST_ASSERT_EQUAL(0, TERN0(OPTION_DISABLED, 1)); // OPTION_DISABLED is disabled, so it should return '0'
}

MARLIN_TEST(macros_options, TERN1) {
  TEST_ASSERT_EQUAL(0, TERN1(OPTION_ENABLED, 0)); // OPTION_ENABLED is enabled, so it should return '0'
  TEST_ASSERT_EQUAL(1, TERN1(OPTION_DISABLED, 0)); // OPTION_DISABLED is disabled, so it should return '1'
}

MARLIN_TEST(macros_options, TERN_) {
  TEST_ASSERT_EQUAL(-1, TERN_(OPTION_ENABLED, -)1); // OPTION_ENABLED is enabled, so it should return '1'
  TEST_ASSERT_EQUAL(1, TERN_(OPTION_DISABLED, -)1); // OPTION_DISABLED is disabled, so it should return nothing
}

MARLIN_TEST(macros_options, IF_DISABLED) {
  TEST_ASSERT_EQUAL(1, IF_DISABLED(OPTION_ENABLED, -)1); // OPTION_ENABLED is enabled, so it should return nothing
  TEST_ASSERT_EQUAL(-1, IF_DISABLED(OPTION_DISABLED, -)1); // OPTION_DISABLED is disabled, so it should return '1'
}

MARLIN_TEST(macros_options, OPTITEM) {
  int enabledArray[] = {OPTITEM(OPTION_ENABLED, 1, 2)};
  int disabledArray[] = {OPTITEM(OPTION_DISABLED, 1, 2)};
  TEST_ASSERT_EQUAL(2, sizeof(enabledArray) / sizeof(int)); // OPTION_ENABLED is enabled, so it should return an array of size 2
  TEST_ASSERT_EQUAL(0, sizeof(disabledArray) / sizeof(int)); // OPTION_DISABLED is disabled, so it should return an array of size 0
}

MARLIN_TEST(macros_options, OPTARG) {
  int enabledArgs[] = {0 OPTARG(OPTION_ENABLED, 1, 2)};
  int disabledArgs[] = {0 OPTARG(OPTION_DISABLED, 1, 2)};

  int sumEnabledArgs = 0;
  for (const auto& arg : enabledArgs) {
    sumEnabledArgs += arg;
  }

  int sumDisabledArgs = 0;
  for (const auto& arg : disabledArgs) {
    sumDisabledArgs += arg;
  }

  TEST_ASSERT_EQUAL(3, sumEnabledArgs); // OPTION_ENABLED is enabled, so it should return 3
  TEST_ASSERT_EQUAL(0, sumDisabledArgs); // OPTION_DISABLED is disabled, so it should return 0
}

MARLIN_TEST(macros_options, OPTCODE) {
  int enabledCode = 0; OPTCODE(OPTION_ENABLED, enabledCode = 1);
  int disabledCode = 0; OPTCODE(OPTION_DISABLED, disabledCode = 1);
  TEST_ASSERT_EQUAL(1, enabledCode); // OPTION_ENABLED is enabled, so it should return 1
  TEST_ASSERT_EQUAL(0, disabledCode); // OPTION_DISABLED is disabled, so it should return 0
}

MARLIN_TEST(macros_optional_math, PLUS_TERN0) {
  int enabledPlus = 5 PLUS_TERN0(OPTION_ENABLED, 2);
  int disabledPlus = 5 PLUS_TERN0(OPTION_DISABLED, 2);
  TEST_ASSERT_EQUAL(7, enabledPlus); // OPTION_ENABLED is enabled, so it should return 7
  TEST_ASSERT_EQUAL(5, disabledPlus); // OPTION_DISABLED is disabled, so it should return 5
}

MARLIN_TEST(macros_optional_math, MINUS_TERN0) {
  int enabledMinus = 5 MINUS_TERN0(OPTION_ENABLED, 2);
  int disabledMinus = 5 MINUS_TERN0(OPTION_DISABLED, 2);
  TEST_ASSERT_EQUAL(3, enabledMinus); // OPTION_ENABLED is enabled, so it should return 3
  TEST_ASSERT_EQUAL(5, disabledMinus); // OPTION_DISABLED is disabled, so it should return 5
}

MARLIN_TEST(macros_optional_math, MUL_TERN1) {
  int enabledMul = 5 MUL_TERN1(OPTION_ENABLED, 2);
  int disabledMul = 5 MUL_TERN1(OPTION_DISABLED, 2);
  TEST_ASSERT_EQUAL(10, enabledMul); // OPTION_ENABLED is enabled, so it should return 10
  TEST_ASSERT_EQUAL(5, disabledMul); // OPTION_DISABLED is disabled, so it should return 5
}

MARLIN_TEST(macros_optional_math, DIV_TERN1) {
  int enabledDiv = 10 DIV_TERN1(OPTION_ENABLED, 2);
  int disabledDiv = 10 DIV_TERN1(OPTION_DISABLED, 2);
  TEST_ASSERT_EQUAL(5, enabledDiv); // OPTION_ENABLED is enabled, so it should return 5
  TEST_ASSERT_EQUAL(10, disabledDiv); // OPTION_DISABLED is disabled, so it should return 10
}

MARLIN_TEST(macros_optional_math, SUM_TERN) {
  int enabledSum = SUM_TERN(OPTION_ENABLED, 5, 2);
  int disabledSum = SUM_TERN(OPTION_DISABLED, 5, 2);
  TEST_ASSERT_EQUAL(7, enabledSum); // OPTION_ENABLED is enabled, so it should return 7
  TEST_ASSERT_EQUAL(5, disabledSum); // OPTION_DISABLED is disabled, so it should return 5
}

MARLIN_TEST(macros_optional_math, DIFF_TERN) {
  int enabledDiff = DIFF_TERN(OPTION_ENABLED, 5, 2);
  int disabledDiff = DIFF_TERN(OPTION_DISABLED, 5, 2);
  TEST_ASSERT_EQUAL(3, enabledDiff); // OPTION_ENABLED is enabled, so it should return 3
  TEST_ASSERT_EQUAL(5, disabledDiff); // OPTION_DISABLED is disabled, so it should return 5
}

MARLIN_TEST(macros_optional_math, MUL_TERN) {
  int enabledMul = MUL_TERN(OPTION_ENABLED, 5, 2);
  int disabledMul = MUL_TERN(OPTION_DISABLED, 5, 2);
  TEST_ASSERT_EQUAL(10, enabledMul); // OPTION_ENABLED is enabled, so it should return 10
  TEST_ASSERT_EQUAL(5, disabledMul); // OPTION_DISABLED is disabled, so it should return 5
}

MARLIN_TEST(macros_optional_math, DIV_TERN) {
  int enabledDiv = DIV_TERN(OPTION_ENABLED, 10, 2);
  int disabledDiv = DIV_TERN(OPTION_DISABLED, 10, 2);
  TEST_ASSERT_EQUAL(5, enabledDiv); // OPTION_ENABLED is enabled, so it should return 5
  TEST_ASSERT_EQUAL(10, disabledDiv); // OPTION_DISABLED is disabled, so it should return 10
}

// Mock pin definitions
#define PIN1_PIN 1
#define PIN2_PIN 2
#define PIN3_PIN -1

MARLIN_TEST(macros_pins, PIN_EXISTS) {
  // Test PIN_EXISTS macro
  int pin1_exists, pin2_exists, pin3_exists, pin4_exists;

  #if PIN_EXISTS(PIN1)
    pin1_exists = 1;
  #else
    pin1_exists = 0;
  #endif

  #if PIN_EXISTS(PIN2)
    pin2_exists = 1;
  #else
    pin2_exists = 0;
  #endif

  #if PIN_EXISTS(PIN3)
    pin3_exists = 1;
  #else
    pin3_exists = 0;
  #endif

  #if PIN_EXISTS(PIN4)
    pin4_exists = 1;
  #else
    pin4_exists = 0;
  #endif

  TEST_ASSERT_TRUE(pin1_exists);
  TEST_ASSERT_TRUE(pin2_exists);
  TEST_ASSERT_FALSE(pin3_exists);
  TEST_ASSERT_FALSE(pin4_exists);
}

MARLIN_TEST(macros_pins, PINS_EXIST) {
  // Test PINS_EXIST macro
  int pins1_2_exist, pins1_3_exist;

  #if PINS_EXIST(PIN1, PIN2)
    pins1_2_exist = 1;
  #else
    pins1_2_exist = 0;
  #endif

  #if PINS_EXIST(PIN1, PIN3)
    pins1_3_exist = 1;
  #else
    pins1_3_exist = 0;
  #endif

  TEST_ASSERT_TRUE(pins1_2_exist);
  TEST_ASSERT_FALSE(pins1_3_exist);
}

MARLIN_TEST(macros_pins, ANY_PIN) {
  // Test ANY_PIN macro
  int any_pin1_3, any_pin3_4;

  #if ANY_PIN(PIN1, PIN3)
    any_pin1_3 = 1;
  #else
    any_pin1_3 = 0;
  #endif

  #if ANY_PIN(PIN3, PIN4)
    any_pin3_4 = 1;
  #else
    any_pin3_4 = 0;
  #endif

  TEST_ASSERT_TRUE(any_pin1_3);
  TEST_ASSERT_FALSE(any_pin3_4);
}

// Undefine mock pin definitions
#undef PIN1_PIN
#undef PIN2_PIN
#undef PIN3_PIN


// Mock button definitions
#define BTN_BUTTON1 1
#define BTN_BUTTON2 2
#define BTN_BUTTON3 -1

MARLIN_TEST(macros_buttons, BUTTON_EXISTS) {
  // Test BUTTON_EXISTS macro
  int button1_exists, button2_exists, button3_exists, button4_exists;

  #if BUTTON_EXISTS(BUTTON1)
    button1_exists = 1;
  #else
    button1_exists = 0;
  #endif

  #if BUTTON_EXISTS(BUTTON2)
    button2_exists = 1;
  #else
    button2_exists = 0;
  #endif

  #if BUTTON_EXISTS(BUTTON3)
    button3_exists = 1;
  #else
    button3_exists = 0;
  #endif

  #if BUTTON_EXISTS(BUTTON4)
    button4_exists = 1;
  #else
    button4_exists = 0;
  #endif

  TEST_ASSERT_TRUE(button1_exists);
  TEST_ASSERT_TRUE(button2_exists);
  TEST_ASSERT_FALSE(button3_exists);
  TEST_ASSERT_FALSE(button4_exists);
}

MARLIN_TEST(macros_buttons, BUTTONS_EXIST) {
  // Test BUTTONS_EXIST macro
  int buttons1_2_exist, buttons1_3_exist;

  #if BUTTONS_EXIST(BUTTON1, BUTTON2)
    buttons1_2_exist = 1;
  #else
    buttons1_2_exist = 0;
  #endif

  #if BUTTONS_EXIST(BUTTON1, BUTTON3)
    buttons1_3_exist = 1;
  #else
    buttons1_3_exist = 0;
  #endif

  TEST_ASSERT_TRUE(buttons1_2_exist);
  TEST_ASSERT_FALSE(buttons1_3_exist);
}

MARLIN_TEST(macros_buttons, ANY_BUTTON) {
  // Test ANY_BUTTON macro
  int any_button1_3, any_button3_4;

  #if ANY_BUTTON(BUTTON1, BUTTON3)
    any_button1_3 = 1;
  #else
    any_button1_3 = 0;
  #endif

  #if ANY_BUTTON(BUTTON3, BUTTON4)
    any_button3_4 = 1;
  #else
    any_button3_4 = 0;
  #endif

  TEST_ASSERT_TRUE(any_button1_3);
  TEST_ASSERT_FALSE(any_button3_4);
}

// Undefine mock button definitions
#undef BTN_BUTTON1
#undef BTN_BUTTON2
#undef BTN_BUTTON3


MARLIN_TEST(macros_value_functions, WITHIN) {
  // Test WITHIN macro
  TEST_ASSERT_TRUE(WITHIN(5, 1, 10)); // 5 is within 1 and 10
  TEST_ASSERT_TRUE(WITHIN(1, 1, 10)); // Edge case: 1 is the lower limit
  TEST_ASSERT_TRUE(WITHIN(10, 1, 10)); // Edge case: 10 is the upper limit
  TEST_ASSERT_FALSE(WITHIN(0, 1, 10)); // Edge case: 0 is just below the lower limit
  TEST_ASSERT_FALSE(WITHIN(11, 1, 10)); // Edge case: 11 is just above the upper limit
  TEST_ASSERT_FALSE(WITHIN(15, 1, 10)); // 15 is not within 1 and 10
}

MARLIN_TEST(macros_value_functions, ISEOL) {
  // Test ISEOL macro
  TEST_ASSERT_TRUE(ISEOL('\n')); // '\n' is an end-of-line character
  TEST_ASSERT_TRUE(ISEOL('\r')); // '\r' is an end-of-line character
  TEST_ASSERT_FALSE(ISEOL('a')); // 'a' is not an end-of-line character
}

MARLIN_TEST(macros_value_functions, NUMERIC) {
  // Test NUMERIC macro
  TEST_ASSERT_TRUE(NUMERIC('0')); // Edge case: '0' is the lowest numeric character
  TEST_ASSERT_TRUE(NUMERIC('5')); // '5' is a numeric character
  TEST_ASSERT_TRUE(NUMERIC('9')); // Edge case: '9' is the highest numeric character
  TEST_ASSERT_FALSE(NUMERIC('0' - 1)); // Edge case: '/' is just before '0' in ASCII
  TEST_ASSERT_FALSE(NUMERIC('9' + 1)); // Edge case: ':' is just after '9' in ASCII
  TEST_ASSERT_FALSE(NUMERIC('a')); // 'a' is not a numeric character
}

MARLIN_TEST(macros_value_functions, DECIMAL) {
  // Test DECIMAL macro
  TEST_ASSERT_TRUE(DECIMAL('0')); // Edge case: '0' is the lowest numeric character
  TEST_ASSERT_TRUE(DECIMAL('5')); // '5' is a numeric character
  TEST_ASSERT_TRUE(DECIMAL('9')); // Edge case: '9' is the highest numeric character
  TEST_ASSERT_TRUE(DECIMAL('.')); // '.' is a decimal character
  TEST_ASSERT_FALSE(DECIMAL('0' - 1)); // Edge case: '/' is just before '0' in ASCII
  TEST_ASSERT_FALSE(DECIMAL('9' + 1)); // Edge case: ':' is just after '9' in ASCII
  TEST_ASSERT_FALSE(DECIMAL('-')); // '-' is not a decimal character, but can appear in numbers
  TEST_ASSERT_FALSE(DECIMAL('+')); // '+' is not a decimal character, but can appear in numbers
  TEST_ASSERT_FALSE(DECIMAL('e')); // 'e' is not a decimal character, but can appear in scientific notation
}

MARLIN_TEST(macros_value_functions, HEXCHR) {
  // Test HEXCHR macro
  TEST_ASSERT_EQUAL(0, HEXCHR('0')); // Edge case: '0' is the lowest numeric character
  TEST_ASSERT_EQUAL(9, HEXCHR('9')); // Edge case: '9' is the highest numeric character
  TEST_ASSERT_EQUAL(10, HEXCHR('a')); // 'a' is a hex character with value 10
  TEST_ASSERT_EQUAL(10, HEXCHR('A')); // 'A' is a hex character with value 10
  TEST_ASSERT_EQUAL(15, HEXCHR('f')); // Edge case: 'f' is the highest lowercase hex character
  TEST_ASSERT_EQUAL(15, HEXCHR('F')); // Edge case: 'F' is the highest uppercase hex character
  TEST_ASSERT_EQUAL(-1, HEXCHR('g')); // 'g' is not a hex character
}

MARLIN_TEST(macros_value_functions, NUMERIC_SIGNED) {
  // Test NUMERIC_SIGNED macro
  TEST_ASSERT_TRUE(NUMERIC_SIGNED('0')); // Edge case: '0' is the lowest numeric character
  TEST_ASSERT_TRUE(NUMERIC_SIGNED('5')); // '5' is a numeric character
  TEST_ASSERT_TRUE(NUMERIC_SIGNED('9')); // Edge case: '9' is the highest numeric character
  TEST_ASSERT_TRUE(NUMERIC_SIGNED('-')); // '-' is not a numeric character, but can appear in signed numbers
  TEST_ASSERT_TRUE(NUMERIC_SIGNED('+')); // '+' is not a numeric character, but can appear in signed numbers
  TEST_ASSERT_FALSE(NUMERIC_SIGNED('.')); // '.' is not a numeric character
  TEST_ASSERT_FALSE(NUMERIC_SIGNED('0' - 1)); // Edge case: '/' is just before '0' in ASCII
  TEST_ASSERT_FALSE(NUMERIC_SIGNED('9' + 1)); // Edge case: ':' is just after '9' in ASCII
  TEST_ASSERT_FALSE(NUMERIC_SIGNED('e')); // 'e' is not a numeric character, but can appear in scientific notation
}

MARLIN_TEST(macros_value_functions, DECIMAL_SIGNED) {
  // Test DECIMAL_SIGNED macro
  TEST_ASSERT_TRUE(DECIMAL_SIGNED('0')); // Edge case: '0' is the lowest numeric character
  TEST_ASSERT_TRUE(DECIMAL_SIGNED('5')); // '5' is a decimal character
  TEST_ASSERT_TRUE(DECIMAL_SIGNED('9')); // Edge case: '9' is the highest numeric character
  TEST_ASSERT_TRUE(DECIMAL_SIGNED('-')); // '-' is not a numeric character, but can appear in signed numbers
  TEST_ASSERT_TRUE(DECIMAL_SIGNED('+')); // '+' is not a numeric character, but can appear in signed numbers
  TEST_ASSERT_TRUE(DECIMAL_SIGNED('.')); // '.' is a decimal character
  TEST_ASSERT_FALSE(DECIMAL_SIGNED('0' - 1)); // Edge case: '/' is just before '0' in ASCII
  TEST_ASSERT_FALSE(DECIMAL_SIGNED('9' + 1)); // Edge case: ':' is just after '9' in ASCII
  TEST_ASSERT_FALSE(DECIMAL_SIGNED('e')); // 'e' is not a decimal character, but can appear in scientific notation
}

MARLIN_TEST(macros_array, COUNT) {
  // Test COUNT macro
  int array[10];
  TEST_ASSERT_EQUAL(10, COUNT(array)); // The array has 10 elements
}

MARLIN_TEST(macros_array, ZERO) {
  // Test ZERO macro
  int array[5] = {1, 2, 3, 4, 5};
  ZERO(array);
  for (auto& element : array) {
    TEST_ASSERT_EQUAL(0, element);
  }
}

MARLIN_TEST(macros_array, COPY) {
  int array1[5] = {1, 2, 3, 4, 5};
  int array2[5] = {0};
  COPY(array2, array1);
  for (const auto& element : array1) {
    TEST_ASSERT_EQUAL(element, array2[&element - &array1[0]]); // All elements should be equal
  }
}

MARLIN_TEST(macros_expansion, CODE_N) {
  int a = 0;
  CODE_N(0, a+=1, a+=2, a+=3, a+=4, a+=5, a+=6, a+=7, a+=8, a+=9, a+=10, a+=11, a+=12, a+=13, a+=14, a+=15, a+=16);
  TEST_ASSERT_EQUAL(0, a);

  a = 0;
  CODE_N(1, a+=1, a+=2, a+=3, a+=4, a+=5, a+=6, a+=7, a+=8, a+=9, a+=10, a+=11, a+=12, a+=13, a+=14, a+=15, a+=16);
  TEST_ASSERT_EQUAL(1, a);

  a = 0;
  CODE_N(2, a+=1, a+=2, a+=3, a+=4, a+=5, a+=6, a+=7, a+=8, a+=9, a+=10, a+=11, a+=12, a+=13, a+=14, a+=15, a+=16);
  TEST_ASSERT_EQUAL(3, a);

  a = 0;
  CODE_N(16, a+=1, a+=2, a+=3, a+=4, a+=5, a+=6, a+=7, a+=8, a+=9, a+=10, a+=11, a+=12, a+=13, a+=14, a+=15, a+=16);
  TEST_ASSERT_EQUAL(136, a);

  // 16 is the highest number supported by the CODE_N macro
}

MARLIN_TEST(macros_expansion, GANG_N) {
  TEST_ASSERT_EQUAL(0, 0 GANG_N(0, +1, +2, +3, +4, +5, +6, +7, +8, +9, +10, +11, +12, +13, +14, +15, +16));
  TEST_ASSERT_EQUAL(1, 0 GANG_N(1, +1, +2, +3, +4, +5, +6, +7, +8, +9, +10, +11, +12, +13, +14, +15, +16));
  TEST_ASSERT_EQUAL(3, 0 GANG_N(2, +1, +2, +3, +4, +5, +6, +7, +8, +9, +10, +11, +12, +13, +14, +15, +16));
  TEST_ASSERT_EQUAL(136, 0 GANG_N(16, +1, +2, +3, +4, +5, +6, +7, +8, +9, +10, +11, +12, +13, +14, +15, +16));

  // 16 is the highest number supported by the GANG_N macro
}

MARLIN_TEST(macros_expansion, GANG_N_1) {
  // Count by twos to be sure it can't bass by returning N
  TEST_ASSERT_EQUAL(0, 0 GANG_N_1(0, +2));
  TEST_ASSERT_EQUAL(2, 0 GANG_N_1(1, +2));
  TEST_ASSERT_EQUAL(4, 0 GANG_N_1(2, +2));
  TEST_ASSERT_EQUAL(32, 0 GANG_N_1(16, +2));
}

MARLIN_TEST(macros_expansion, LIST_N) {
  std::vector<int> expected, result;
  int compare_size;

  expected = {};
  result = {LIST_N(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }

  expected = {1};
  result = {LIST_N(1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }

  expected = {1, 2};
  result = {LIST_N(2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }

  expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  result = {LIST_N(16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }
}

MARLIN_TEST(macros_expansion, LIST_N_1) {
  std::vector<int> expected, result;
  int compare_size;

  expected = {};
  result = {LIST_N_1(0, 1)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }

  expected = {2};
  result = {LIST_N_1(1, 2)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }

  expected = {1, 1};
  result = {LIST_N_1(2, 1)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }

  expected = std::vector<int>(16, 1);
  result = {LIST_N_1(16, 1)};
  TEST_ASSERT_EQUAL(expected.size(), result.size());
  compare_size = _MIN(expected.size(), result.size());
  for (int i = 0; i < compare_size; i++) {
    TEST_ASSERT_EQUAL(expected[i], result[i]);
  }
}

MARLIN_TEST(macros_expansion, ARRAY_N) {
  // Test ARRAY_N macro
  std::array<int, 5> expected = {1, 2, 3, 4, 5};
  std::array<int, 5> result = ARRAY_N(5, 1, 2, 3, 4, 5);
  TEST_ASSERT_EQUAL(expected.size(), result.size());

  std::array<int, 3> expected2 = {1, 2, 3};
  std::array<int, 3> result2 = ARRAY_N(3, 1, 2, 3);
  TEST_ASSERT_EQUAL(expected2.size(), result2.size());
}

MARLIN_TEST(macros_expansion, ARRAY_N_1) {
  // Test ARRAY_N_1 macro
  std::array<int, 5> expected = {2, 2, 2, 2, 2};
  std::array<int, 5> result = ARRAY_N_1(5, 2);
  TEST_ASSERT_EQUAL(expected.size(), result.size());

  std::array<int, 3> expected2 = {1, 1, 1};
  std::array<int, 3> result2 = ARRAY_N_1(3, 1);
  TEST_ASSERT_EQUAL(expected2.size(), result2.size());
}

MARLIN_TEST(macros_math, CEILING) {
  TEST_ASSERT_EQUAL(2, CEILING(3, 2));
  TEST_ASSERT_EQUAL(5, CEILING(10, 2));
  TEST_ASSERT_EQUAL(0, CEILING(0, 2));
}

MARLIN_TEST(macros_math, ABS) {
  TEST_ASSERT_EQUAL(5, ABS(-5));
  TEST_ASSERT_EQUAL(5, ABS(5));
  TEST_ASSERT_EQUAL_FLOAT(5.5, ABS(-5.5));
  TEST_ASSERT_EQUAL_FLOAT(5.5, ABS(5.5));
}

MARLIN_TEST(macros_float, UNEAR_ZERO) {
  TEST_ASSERT_TRUE(UNEAR_ZERO(0.0000009f));
  TEST_ASSERT_FALSE(UNEAR_ZERO(0.000001f));
}

MARLIN_TEST(macros_float, NEAR_ZERO) {
  TEST_ASSERT_TRUE(NEAR_ZERO(0.0000001f));
  TEST_ASSERT_TRUE(NEAR_ZERO(-0.0000001f));
  TEST_ASSERT_FALSE(NEAR_ZERO(0.0000011f));
  TEST_ASSERT_FALSE(NEAR_ZERO(-0.0000011f));
}

MARLIN_TEST(macros_float, NEAR) {
  TEST_ASSERT_TRUE(NEAR(0.000001f, 0.000002f));
  TEST_ASSERT_FALSE(NEAR(0.0000009f, 0.000002f));
}

MARLIN_TEST(macros_float, RECIPROCAL) {
  TEST_ASSERT_EQUAL_FLOAT(1.0f, RECIPROCAL(1.0f));
  TEST_ASSERT_EQUAL_FLOAT(0.0f, RECIPROCAL(0.0f));
  TEST_ASSERT_EQUAL_FLOAT(2.0f, RECIPROCAL(0.5f));
  TEST_ASSERT_EQUAL_FLOAT(-2.0f, RECIPROCAL(-0.5f));
  TEST_ASSERT_EQUAL_FLOAT(0.0f, RECIPROCAL(0.0000001f));
  TEST_ASSERT_EQUAL_FLOAT(0.0f, RECIPROCAL(-0.0000001f));
}

MARLIN_TEST(macros_float, FIXFLOAT) {
  TEST_ASSERT_EQUAL(0.0000005f, FIXFLOAT(0.0f));
  TEST_ASSERT_EQUAL(-0.0000005f, FIXFLOAT(-0.0f));
}

MARLIN_TEST(macros_math, MATH_MACROS) {
  // Sanity check of macros typically mapped to compiler functions
  TEST_ASSERT_EQUAL_FLOAT(0.0f, ACOS(1.0f));
  TEST_ASSERT_EQUAL_FLOAT(0.785398f, ATAN2(1.0f, 1.0f));
  TEST_ASSERT_EQUAL_FLOAT(8.0f, POW(2.0f, 3.0f));
  TEST_ASSERT_EQUAL_FLOAT(2.0f, SQRT(4.0f));
  TEST_ASSERT_EQUAL_FLOAT(0.5f, RSQRT(4.0f));
  TEST_ASSERT_EQUAL_FLOAT(2.0f, CEIL(1.5f));
  TEST_ASSERT_EQUAL_FLOAT(1.0f, FLOOR(1.5f));
  TEST_ASSERT_EQUAL_FLOAT(1.0f, TRUNC(1.5f));
  TEST_ASSERT_EQUAL(2, LROUND(1.5f));
  TEST_ASSERT_EQUAL_FLOAT(1.0f, FMOD(5.0f, 2.0f));
  TEST_ASSERT_EQUAL_FLOAT(5.0f, HYPOT(3.0f, 4.0f));
}

MARLIN_TEST(macros_math, MIN_MAX) {
  // _MIN tests
  TEST_ASSERT_EQUAL(-1, _MIN(-1, 0));
  TEST_ASSERT_EQUAL(-1, _MIN(0, -1));
  TEST_ASSERT_EQUAL(-1, _MIN(-1, 1));
  TEST_ASSERT_EQUAL(-1, _MIN(1, -1));
  TEST_ASSERT_EQUAL(-1, _MIN(-1, -1));
  TEST_ASSERT_EQUAL(1, _MIN(1, 1));
  TEST_ASSERT_EQUAL_FLOAT(-1.5f, _MIN(-1.5f, 0.5f));
  TEST_ASSERT_EQUAL_FLOAT(-1.5f, _MIN(0.5f, -1.5f));

  // _MAX tests
  TEST_ASSERT_EQUAL(0, _MAX(-1, 0));
  TEST_ASSERT_EQUAL(0, _MAX(0, -1));
  TEST_ASSERT_EQUAL(1, _MAX(-1, 1));
  TEST_ASSERT_EQUAL(1, _MAX(1, -1));
  TEST_ASSERT_EQUAL(-1, _MAX(-1, -1));
  TEST_ASSERT_EQUAL(1, _MAX(1, 1));
  TEST_ASSERT_EQUAL_FLOAT(0.5f, _MAX(-1.5f, 0.5f));
  TEST_ASSERT_EQUAL_FLOAT(0.5f, _MAX(0.5f, -1.5f));
}

MARLIN_TEST(macros_math, INCREMENT) {
  TEST_ASSERT_EQUAL(1, INCREMENT(0));
  TEST_ASSERT_EQUAL(21, INCREMENT(20));
  // 20 is the highest number supported by the INCREMENT macro
}

MARLIN_TEST(macros_math, ADD) {
  // Test smallest add
  TEST_ASSERT_EQUAL(0, ADD0(0));
  TEST_ASSERT_EQUAL(10, ADD0(10));

  // Test largest add
  TEST_ASSERT_EQUAL(10, ADD10(0));
  TEST_ASSERT_EQUAL(20, ADD10(10));
}

MARLIN_TEST(macros_math, SUM) {
  // Test smallest sum
  TEST_ASSERT_EQUAL(3, SUM(0, 3));
  TEST_ASSERT_EQUAL(7, SUM(3, 4));

  // Test largest sum
  TEST_ASSERT_EQUAL(15, SUM(10, 5));
  TEST_ASSERT_EQUAL(19, SUM(9, 10));
}

MARLIN_TEST(macros_math, DOUBLE) {
  // Test double
  TEST_ASSERT_EQUAL(0, DOUBLE(0));
  TEST_ASSERT_EQUAL(2, DOUBLE(1));
  TEST_ASSERT_EQUAL(4, DOUBLE(2));
  TEST_ASSERT_EQUAL(20, DOUBLE(10));
}

MARLIN_TEST(macros_math, DECREMENT) {
  TEST_ASSERT_EQUAL(0, DECREMENT(1));
  TEST_ASSERT_EQUAL(14, DECREMENT(15));
}

MARLIN_TEST(macros_math, SUB) {
  // Test smallest subtraction
  TEST_ASSERT_EQUAL(0, SUB0(0));
  TEST_ASSERT_EQUAL(10, SUB0(10));

  // Test subtracting 1
  TEST_ASSERT_EQUAL(0, SUB1(1));
  TEST_ASSERT_EQUAL(5, SUB1(6));

  // Test largest subtraction
  TEST_ASSERT_EQUAL(0, SUB10(10));
  TEST_ASSERT_EQUAL(5, SUB10(15));
}


// Define a helper macro for testing
#define TEST_OP(i) ++counter;
#define TEST_OP2(i, j) counter += j;

MARLIN_TEST(macros_repeat, REPEAT) {
  int counter = 0;
  REPEAT(5, TEST_OP);
  TEST_ASSERT_EQUAL(5, counter);
}

MARLIN_TEST(macros_repeat, REPEAT_1) {
  int counter = 0;
  REPEAT_1(5, TEST_OP);
  TEST_ASSERT_EQUAL(5, counter);
}

MARLIN_TEST(macros_repeat, REPEAT2) {
  int counter = 0;
  REPEAT2(5, TEST_OP2, 1);
  TEST_ASSERT_EQUAL(5, counter);
}

MARLIN_TEST(macros_repeat, RREPEAT) {
  int counter = 0;
  RREPEAT(5, TEST_OP);
  TEST_ASSERT_EQUAL(5, counter);
}

MARLIN_TEST(macros_repeat, RREPEAT_1) {
  int counter = 0;
  RREPEAT_1(5, TEST_OP);
  TEST_ASSERT_EQUAL(5, counter);
}

MARLIN_TEST(macros_repeat, RREPEAT2) {
  int counter = 0;
  RREPEAT2(5, TEST_OP2, 1);
  TEST_ASSERT_EQUAL(5, counter);
}

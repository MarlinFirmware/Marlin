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
#include "src/core/types.h"

MARLIN_TEST(types, XYval_const_as_bools) {
  const XYval<int> xy_const_true = {1, 2};
  TEST_ASSERT_TRUE(xy_const_true);

  const XYval<int> xy_const_false = {0, 0};
  TEST_ASSERT_FALSE(xy_const_false);
}

MARLIN_TEST(types, XYval_non_const_as_bools) {
  XYval<int> xy_true = {1, 2};
  TEST_ASSERT_TRUE(xy_true);

  XYval<int> xy_false = {0, 0};
  TEST_ASSERT_FALSE(xy_false);
}

MARLIN_TEST(types, XYZval_const_as_bools) {
  const XYZval<int> xyz_const_true = {1, 2, 3};
  TEST_ASSERT_TRUE(xyz_const_true);

  const XYZval<int> xyz_const_false = {0, 0, 0};
  TEST_ASSERT_FALSE(xyz_const_false);
}

MARLIN_TEST(types, XYZval_non_const_as_bools) {
  XYZval<int> xyz_true = {1, 2, 3};
  TEST_ASSERT_TRUE(xyz_true);

  XYZval<int> xyz_false = {0, 0, 0};
  TEST_ASSERT_FALSE(xyz_false);
}

MARLIN_TEST(types, XYZEval_const_as_bools) {
  const XYZEval<int> xyze_const_true = {1, 2, 3, 4};
  TEST_ASSERT_TRUE(xyze_const_true);

  const XYZEval<int> xyze_const_false = {0, 0, 0, 0};
  TEST_ASSERT_FALSE(xyze_const_false);
}

MARLIN_TEST(types, XYZEval_non_const_as_bools) {
  XYZEval<int> xyze_true = {1, 2, 3, 4};
  TEST_ASSERT_TRUE(xyze_true);

  XYZEval<int> xyze_false = {0, 0, 0, 0};
  TEST_ASSERT_FALSE(xyze_false);
}

MARLIN_TEST(types, Flags_const_as_bools) {
  const Flags<32> flags_const_false = {0};
  TEST_ASSERT_FALSE(flags_const_false);

  const Flags<32> flags_const_true = {1};
  TEST_ASSERT_TRUE(flags_const_true);
}

MARLIN_TEST(types, Flags_non_const_as_bools) {
  Flags<32> flags_false = {0};
  TEST_ASSERT_FALSE(flags_false);

  Flags<32> flags_true = {1};
  TEST_ASSERT_TRUE(flags_true);
}

MARLIN_TEST(types, AxisFlags_const_as_bools) {
  const AxisFlags axis_flags_const_false = {0};
  TEST_ASSERT_FALSE(axis_flags_const_false);

  const AxisFlags axis_flags_const_true = {1};
  TEST_ASSERT_TRUE(axis_flags_const_true);
}

MARLIN_TEST(types, AxisFlags_non_const_as_bools) {
  AxisFlags axis_flags_false = {0};
  TEST_ASSERT_FALSE(axis_flags_false);

  AxisFlags axis_flags_true = {1};
  TEST_ASSERT_TRUE(axis_flags_true);
}

MARLIN_TEST(types, AxisBits_const_as_bools) {
  const AxisBits axis_bits_const_false = {0};
  TEST_ASSERT_FALSE(axis_bits_const_false);

  const AxisBits axis_bits_const_true = {1};
  TEST_ASSERT_TRUE(axis_bits_const_true);
}

MARLIN_TEST(types, AxisBits_non_const_as_bools) {
  AxisBits axis_bits_false = {0};
  TEST_ASSERT_FALSE(axis_bits_false);

  AxisBits axis_bits_true = {1};
  TEST_ASSERT_TRUE(axis_bits_true);
}

MARLIN_TEST(types, MString1) {
  // String with cutoff at 20 chars:
  // "F-string, 1234.50, 2"
  MString<20> str20;
  str20 = F("F-string, ");
  str20.append(1234.5f).append(',').append(' ')
       .append(2345.67).append(',').append(' ');

  TEST_ASSERT_TRUE(strcmp_P(str20, PSTR("F-string, 1234.50, 2")) == 0);

  // Truncate to "F-string"
  str20.trunc(8);

  TEST_ASSERT_FALSE(strcmp_P(&str20, PSTR("F-string")) != 0);
}

MARLIN_TEST(types, MString2) {
  // 100 dashes, but chopped down to DEFAULT_MSTRING_SIZE (20)
  TEST_ASSERT_TRUE(TSS(repchr_t('-', 100)).length() == 20);
}

MARLIN_TEST(types, SString) {
  // Hello World!-123456------   < spaces!33
  // ^ eol! ... 1234.50*2345.602 = 2895645.67
  SString<100> str(F("Hello"));
  str.append(F(" World!"));
  str += '-';
  str += uint8_t(123);
  str += F("456");
  str += repchr_t('-', 6);
  str += Spaces(3);
  str += "< spaces!";
  str += int8_t(33);
  str.eol();
  str += "^ eol!";
  str.append(" ... ", 1234.5f, '*', p_float_t(2345.602, 3), F(" = "), 1234.5 * 2345.602);

  TEST_ASSERT_TRUE(strcmp_P(str, PSTR("Hello World!-123456------   < spaces!33\n^ eol! ... 1234.50*2345.602 = 2895645.67")) == 0);
}

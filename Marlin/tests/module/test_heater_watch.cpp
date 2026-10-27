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
#include "src/module/temperature.h"

#if WATCH_HOTENDS || WATCH_BED

MARLIN_TEST(millis, next_millis_preserves_nonzero) {
  TEST_ASSERT_EQUAL_UINT32(1, next_millis(0));
  TEST_ASSERT_EQUAL_UINT32(1, next_millis(1));
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, next_millis(UINT32_MAX));
}

MARLIN_TEST(heater_watch, zero_deadline_stays_armed) {
  HeaterWatch<2, 4, 40> watch{};
  const millis_t now = millis_t(0) - SEC_TO_MS(40);
  watch.restart(20, 200, now);
  TEST_ASSERT_EQUAL_UINT32(1, watch.next_ms);
  TEST_ASSERT_FALSE(watch.elapsed(millis_t(0)));
  TEST_ASSERT_TRUE(watch.elapsed(millis_t(1)));
}

MARLIN_TEST(heater_watch, bed_zero_deadline_stays_armed) {
  HeaterWatch<2, 2, 180> watch{};
  watch.restart(20, 60, millis_t(0) - SEC_TO_MS(180));
  TEST_ASSERT_EQUAL_UINT32(1, watch.next_ms);
  TEST_ASSERT_TRUE(watch.elapsed(millis_t(1)));
}

MARLIN_TEST(heater_watch, ordinary_deadline_and_temperature_rise) {
  HeaterWatch<2, 4, 40> watch{};
  watch.restart(20, 200, 1000);
  TEST_ASSERT_EQUAL_UINT32(41000, watch.next_ms);
  TEST_ASSERT_FALSE(watch.elapsed(millis_t(40999)));
  TEST_ASSERT_TRUE(watch.elapsed(millis_t(41000)));
  TEST_ASSERT_FALSE(watch.check(21));
  TEST_ASSERT_TRUE(watch.check(22));
}

MARLIN_TEST(heater_watch, wrapped_nonzero_deadline) {
  HeaterWatch<2, 4, 40> watch{};
  watch.restart(20, 200, millis_t(0) - 1000);
  TEST_ASSERT_EQUAL_UINT32(39000, watch.next_ms);
  TEST_ASSERT_FALSE(watch.elapsed(millis_t(38999)));
  TEST_ASSERT_TRUE(watch.elapsed(millis_t(39000)));
}

MARLIN_TEST(heater_watch, off_and_near_target_disarm) {
  HeaterWatch<2, 4, 40> watch{};
  watch.restart(20, 200, 1000);
  watch.restart(20, 0, 1000);
  TEST_ASSERT_EQUAL_UINT32(0, watch.next_ms);
  TEST_ASSERT_FALSE(watch.elapsed(millis_t(41000)));
  watch.restart(196, 200, 1000);
  TEST_ASSERT_EQUAL_UINT32(0, watch.next_ms);
}

#endif

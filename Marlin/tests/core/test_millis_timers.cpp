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
#include "src/core/millis_t.h"

// Every test passes an explicit 'now' so results never depend on the real clock.

MARLIN_TEST(millis_timers, TTimeout24_disabled_is_elapsed) {
  const MTimeout24 t;
  TEST_ASSERT_FALSE(t.enabled());
  TEST_ASSERT_FALSE(t.pending(0));
  TEST_ASSERT_FALSE(t.pending(123456));
  TEST_ASSERT_TRUE(t.elapsed(0));
  TEST_ASSERT_FALSE(t.on_elapsed(0));   // Never started, so nothing has "finished"
  TEST_ASSERT_EQUAL_UINT32(0, t.remaining(123456));
}

MARLIN_TEST(millis_timers, TTimeout24_pending_then_elapsed) {
  MTimeout24 t;
  t.start(1000, 5000);
  TEST_ASSERT_TRUE(t.enabled());
  TEST_ASSERT_TRUE(t.pending(5000));
  TEST_ASSERT_TRUE(t.pending(5999));
  TEST_ASSERT_FALSE(t.pending(6000));
  TEST_ASSERT_TRUE(t.elapsed(6000));
  TEST_ASSERT_TRUE(t.on_elapsed(6000));
  TEST_ASSERT_EQUAL_UINT32(1000, t.remaining(5000));
  TEST_ASSERT_EQUAL_UINT32(1, t.remaining(5999));
  TEST_ASSERT_EQUAL_UINT32(0, t.remaining(6000));
}

MARLIN_TEST(millis_timers, TTimeout24_never_stores_zero_end_time) {
  MTimeout24 t;
  t.start(40, millis_t(0) - 40);        // Would end at exactly 0, which means "disabled"
  TEST_ASSERT_TRUE(t.enabled());
  TEST_ASSERT_EQUAL_UINT32(1, t.end_ms);
  TEST_ASSERT_TRUE(t.pending(millis_t(0) - 40));
  TEST_ASSERT_TRUE(t.pending(millis_t(0)));
  TEST_ASSERT_FALSE(t.pending(1));
}

MARLIN_TEST(millis_timers, TTimeout24_wraps_around_millis_overflow) {
  MTimeout24 t;
  t.start(100, millis_t(0) - 30);       // Ends at 70, after the counter wraps
  TEST_ASSERT_TRUE(t.pending(millis_t(0) - 1));
  TEST_ASSERT_TRUE(t.pending(69));
  TEST_ASSERT_FALSE(t.pending(70));
}

MARLIN_TEST(millis_timers, TTimeout24_works_at_any_uptime) {
  // The old "next_ms = 0" idiom saw a late first check as still pending.
  const MTimeout24 t;
  TEST_ASSERT_TRUE(t.elapsed(0x40000000UL));
  TEST_ASSERT_TRUE(t.elapsed(0x7FFFFFFFUL));
  TEST_ASSERT_TRUE(t.elapsed(0xC0000000UL));
}

MARLIN_TEST(millis_timers, TTimeout24_cancel_and_extend) {
  MTimeout24 t;
  t.extend(500);                        // Extending a disabled timer does nothing
  TEST_ASSERT_FALSE(t.enabled());
  t.start(100, 1000);
  t.extend(50);
  TEST_ASSERT_TRUE(t.pending(1149));
  TEST_ASSERT_FALSE(t.pending(1150));
  t.cancel();
  TEST_ASSERT_FALSE(t.enabled());
  TEST_ASSERT_TRUE(t.elapsed(1000));
}

MARLIN_TEST(millis_timers, TTimeout24_extend_skips_zero) {
  MTimeout24 t;
  t.start(0, millis_t(0) - 10);         // Ends at UINT32_MAX - 9
  t.extend(10);                         // Would land on 0
  TEST_ASSERT_TRUE(t.enabled());
  TEST_ASSERT_EQUAL_UINT32(1, t.end_ms);
}

MARLIN_TEST(millis_timers, TTimeout24_16bit) {
  TTimeout24<uint16_t> t;
  t.start(100, 65500);                  // Ends at 64
  TEST_ASSERT_TRUE(t.pending(65535));
  TEST_ASSERT_TRUE(t.pending(63));
  TEST_ASSERT_FALSE(t.pending(64));
}

MARLIN_TEST(millis_timers, TTimeout_disabled_and_zero_delay) {
  MTimeout t;
  TEST_ASSERT_FALSE(t.enabled());
  TEST_ASSERT_FALSE(t.on_elapsed(1000));
  t.start(0, 1000);                     // A zero delay is "disabled"
  TEST_ASSERT_FALSE(t.enabled());
  TEST_ASSERT_FALSE(t.on_elapsed(2000));
}

MARLIN_TEST(millis_timers, TTimeout_age_and_wrap) {
  MTimeout t;
  t.start(1000, millis_t(0) - 400);
  TEST_ASSERT_EQUAL_UINT32(400, t.age(0));
  TEST_ASSERT_TRUE(t.pending(599));
  TEST_ASSERT_FALSE(t.pending(600));
  TEST_ASSERT_TRUE(t.on_elapsed(600));
  TEST_ASSERT_EQUAL_UINT32(600, t.remaining(0));
  t.prime(5000);                        // Restart the same interval
  TEST_ASSERT_TRUE(t.pending(5999));
  TEST_ASSERT_TRUE(t.elapsed(6000));
}

MARLIN_TEST(millis_timers, TDelay_16bit_wrap) {
  TDelay<500> d(65500);
  TEST_ASSERT_TRUE(d.pending(65535));
  TEST_ASSERT_TRUE(d.pending(463));     // Wrapped, 499 ms later
  TEST_ASSERT_FALSE(d.pending(464));
  TEST_ASSERT_TRUE(d.elapsed(464));
  TEST_ASSERT_EQUAL_UINT32(465, d.remaining(65535));
  TEST_ASSERT_EQUAL_UINT32(1, d.remaining(463));
  TEST_ASSERT_EQUAL_UINT32(0, d.remaining(464));
  d.prime(1000);
  TEST_ASSERT_TRUE(d.pending(1499));
  TEST_ASSERT_TRUE(d.elapsed(1500));
}

MARLIN_TEST(millis_timers, MSeconds_compares_in_milliseconds) {
  MSeconds s;
  TEST_ASSERT_FALSE(s.enabled());
  TEST_ASSERT_FALSE(s.on_elapsed(99999));
  s.start(2, 10000);
  TEST_ASSERT_TRUE(s.pending(11999));
  TEST_ASSERT_FALSE(s.pending(12000));
  TEST_ASSERT_TRUE(s.on_elapsed(12000));
  TEST_ASSERT_EQUAL_UINT32(2, s.remaining(10000));
  TEST_ASSERT_EQUAL_UINT32(1, s.remaining(11000));
  s.cancel();
  TEST_ASSERT_FALSE(s.on_elapsed(99999));
}

MARLIN_TEST(millis_timers, MSeconds_full_16bit_range) {
  MSeconds s;
  s.start(65535, 0);                    // 65535 s is 18+ hours, beyond any 16-bit ms counter
  TEST_ASSERT_TRUE(s.pending(SEC_TO_MS(65535UL) - 1));
  TEST_ASSERT_TRUE(s.elapsed(SEC_TO_MS(65535UL)));
}

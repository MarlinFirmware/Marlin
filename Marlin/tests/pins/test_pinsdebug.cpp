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

#if ENABLED(PINS_DEBUGGING)

#include "src/pins/pinsDebug.h"

using Pins = PinsDebug;

// The pin table holds every pin M43 reports, so each entry must be usable
MARLIN_TEST(pinsdebug, pin_table_entries) {
  TEST_ASSERT_GREATER_THAN_UINT32(0, COUNT(pin_array));
  for (uint16_t i = 0; i < COUNT(pin_array); ++i) {
    PGM_P const name = (PGM_P)pgm_read_ptr(&pin_array[i].name);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_GREATER_THAN_UINT32(0, strlen_P(name));
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(MAX_NAME_LENGTH, strlen_P(name));
    TEST_ASSERT_EQUAL(pin_array[i].pin, getPinByIndex(i));
    TEST_ASSERT_EQUAL(pin_array[i].is_digital, pinTableIsDigital(i));
  }
}

// The HAL must supply the required hooks with usable values
MARLIN_TEST(pinsdebug, hal_constants) {
  TEST_ASSERT_GREATER_THAN_UINT32(0, Pins::count);
  TEST_ASSERT_GREATER_THAN_UINT32(0, Pins::name_pad);
}

// Valid pins are within range, and the first and last of them are valid
MARLIN_TEST(pinsdebug, is_valid_range) {
  TEST_ASSERT_TRUE(Pins::is_valid(0));
  TEST_ASSERT_TRUE(Pins::is_valid(pin_t(Pins::count - 1)));
  TEST_ASSERT_FALSE(Pins::is_valid(pin_t(-1)));
}

// get_pin maps an index to its pin, and reports an index with no valid pin
MARLIN_TEST(pinsdebug, get_pin) {
  pin_t pin = pin_t(-2);
  TEST_ASSERT_TRUE(Pins::get_pin(0, pin));
  TEST_ASSERT_EQUAL(Pins::pin_from_index(0), pin);

  const uint16_t last = Pins::count - 1;
  TEST_ASSERT_TRUE(Pins::get_pin(last, pin));
  TEST_ASSERT_EQUAL(Pins::pin_from_index(last), pin);

  // Past the end of the pin range
  TEST_ASSERT_FALSE(Pins::get_pin(Pins::count, pin));
}

// Pins in the sensitive list are untouchable unless protection is ignored,
// and other pins are touchable either way
MARLIN_TEST(pinsdebug, touchable_pins) {
  bool found_protected = false, found_free = false;
  for (uint16_t i = 0; i < Pins::count; ++i) {
    pin_t pin;
    if (!Pins::get_pin(i, pin)) continue;
    if (Pins::never_touch(i)) continue;

    pin_t got = pin_t(-2);
    if (marlin.pin_is_protected(pin)) {
      found_protected = true;
      TEST_ASSERT_TRUE(Pins::is_untouchable(i, pin, false));
      TEST_ASSERT_FALSE(Pins::is_untouchable(i, pin, true));
      TEST_ASSERT_FALSE(Pins::get_touchable_pin(i, false, got));
      TEST_ASSERT_TRUE(Pins::get_touchable_pin(i, true, got));
      TEST_ASSERT_EQUAL(pin, got);
    }
    else {
      found_free = true;
      TEST_ASSERT_FALSE(Pins::is_untouchable(i, pin, false));
      TEST_ASSERT_TRUE(Pins::get_touchable_pin(i, false, got));
      TEST_ASSERT_EQUAL(pin, got);
    }
  }
  TEST_ASSERT_TRUE(found_protected);
  TEST_ASSERT_TRUE(found_free);
}

// A pin that is not valid is never touchable
MARLIN_TEST(pinsdebug, invalid_pin_not_touchable) {
  pin_t pin = pin_t(-2);
  TEST_ASSERT_FALSE(Pins::get_touchable_pin(Pins::count, true, pin));
}

// Analog inputs have an analog index and digital pins don't
MARLIN_TEST(pinsdebug, analog_index) {
  for (uint16_t i = 0; i < COUNT(pin_array); ++i) {
    const pin_t pin = getPinByIndex(i);
    if (!Pins::is_valid(pin)) continue;
    if (Pins::is_analog(pin))
      TEST_ASSERT_GREATER_OR_EQUAL_INT(0, Pins::analog_index(pin));
    else
      TEST_ASSERT_LESS_THAN_INT(0, Pins::analog_index(pin));
  }
}

#endif // PINS_DEBUGGING

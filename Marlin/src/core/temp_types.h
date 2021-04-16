/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
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
#pragma once

#include "fp_type.h"

struct celsius_t;

// A Fahrenheit type
struct fahrenheit_t {
  float f;
  fahrenheit_t(float f = 0): f(f) {}
  fahrenheit_t(celsius_t c);
  operator float() const { return f; }
};

// A Kelvin type
struct kelvin_t {
  float f;
  kelvin_t(float f = 0): f(f) {}
  kelvin_t(celsius_t c);
  operator float() const { return f; }
};

#if ENABLED(FIXED_POINT_CELSIUS)
  //
  // celsius_t is the native unit of temperature. Signed to handle a disconnected thermistor value (-14).
  // This is using fixed point arithmetic here with 1/64th precision. The range goes from [+511. 0.015625 -512].
  // For more resolution (e.g., for a chocolate printer) this may later be changed to Celsius x 100
  //
  struct celsius_t : public fixp<true, 10, 6> {
    // The minimum value that this object can contain
    constexpr static float _minValue = -(float)(1ULL << (integral_bitwidth - 1));
    // The maximum value that this object can contain
    constexpr static float _maxValue = (float)(1ULL << (integral_bitwidth - 1)) - 1 + (float)fractional_mask / (1ULL << fractional_bitwidth);

    // Everything else comes from fixp class so it's manipulable without changing anything

    // Conversion must be explicit
    template <typename T>
    explicit constexpr celsius_t(T n) : fixp(Private::move(n)) {}

    constexpr celsius_t(int v) : fixp(Private::move(v)) {}
    constexpr celsius_t(unsigned r, bool) : fixp(r, true) {} // Raw construction with no shift

    // Default construction
    constexpr celsius_t() {}
    // Copy & move constructor
    constexpr celsius_t(const celsius_t &other) = default;
    constexpr celsius_t(celsius_t && other) = default;
    constexpr celsius_t(fixp<true, 10, 6> && other) : fixp(other) {}
    constexpr celsius_t(const fixp<true, 10, 6> & other) : fixp(other) {}

    explicit celsius_t(kelvin_t k) : fixp((float)k - 273.15f) {}
    explicit celsius_t(fahrenheit_t f) : fixp(((float)f - 32) * 0.5555555556f) {}

    celsius_t& operator=(const celsius_t &other) { data = other.data; return *this; }

    constexpr explicit operator int()    const { return (int)data; }
    constexpr explicit operator float()  const { return (float)data; }
    constexpr explicit operator double() const { return (double)data; }
  };
  constexpr celsius_t minCValue = celsius_t::_minValue;
  constexpr celsius_t maxCValue = celsius_t::_maxValue;
#elif ENABLED(FLOATING_POINT_CELSIUS)
  struct celsius_t {
    float f;
    constexpr celsius_t(float f = 0): f(f) {}
    constexpr celsius_t(unsigned f, bool): f(f) {}
    celsius_t(kelvin_t k) : f((float)k - 273.15f) {}
    celsius_t(fahrenheit_t f) : f(((float)f - 32) * 0.5555555556f) {}
    operator float() const { return f; }
    constexpr unsigned raw() const { return (unsigned)f; }
  };
  constexpr static celsius_t minCValue = -273.15f;
  constexpr static celsius_t maxCValue = 32767.0f;
#else
  struct celsius_t {
    int16_t f;
    constexpr celsius_t(int16_t f = 0): f(f) {}
    constexpr celsius_t(unsigned f, bool): f(f) {}
    celsius_t(kelvin_t k) : f(static_cast<int16_t>((float)k - 273.15f + .5f)) {}
    celsius_t(fahrenheit_t f) : f(static_cast<int16_t>(((float)f - 32) * 0.5555555556f + .5f)) {}
    operator int16_t() const { return f; }
    explicit operator float() const { return f; }
    constexpr unsigned raw() const { return (unsigned)f; }
  };

  constexpr static celsius_t minCValue = -274;
  constexpr static celsius_t maxCValue = 32767;
#endif

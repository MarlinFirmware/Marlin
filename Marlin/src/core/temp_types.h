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

//
// celsius_t is the native unit of temperature. Signed to handle a disconnected thermistor value (-14).
// This is using fixed point arithmetic here with 1/64th precision. The range goes from [+511. 0.015625 -512].
// For more resolution (e.g., for a chocolate printer) this may later be changed to Celsius x 100
//
struct celsius_t : public fixp<true, 10, 6> {
  // The minimum value that this object can contain
  constexpr static double minValue = -(double)(1ULL << (integral_bitwidth - 1));
  // The maximum value that this object can contain
  constexpr static double maxValue = (double)(1ULL << (integral_bitwidth - 1)) - 1 + (double)fractional_mask / (1ULL << fractional_bitwidth);

  // Everything else comes from fixp class so it's manipulable without changing anything

  // Conversion must be explicit
  template <typename T>
  explicit constexpr celsius_t(T n) : fixp(Private::move(n)) {}

  constexpr celsius_t(int v) : fixp(Private::move(v)) {}

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
  celsius_t& operator=(const int &val)         { data = val;        return *this; }
  celsius_t& operator=(const float &val)       { data = val;        return *this; }
  celsius_t& operator=(const double &val)      { data = val;        return *this; }

  #define C_CMP_OP(op) constexpr bool operator op(const int   &o)     const { return data op o; } \
                       constexpr bool operator op(const float &o)     const { return data op o; } \
                       constexpr bool operator op(const celsius_t &o) const { return data op (int)o; }
    C_CMP_OP(==)
    C_CMP_OP(!=)
    C_CMP_OP(<)
    C_CMP_OP(>)
    C_CMP_OP(>=)
    C_CMP_OP(<=)
  #undef C_CMP_OP

  constexpr explicit operator int()    const { return (int)data; }
  constexpr explicit operator float()  const { return (float)data; }
  constexpr explicit operator double() const { return (double)data; }
};

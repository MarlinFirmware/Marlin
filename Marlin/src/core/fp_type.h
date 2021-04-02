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

#include <stdint.h>
#include "macros.h"

// Stuff here are used to for making the type generic enough not to care about bit size and types
namespace Private
{
  // Make a catalog of types that's converting from bit width to the minimum C type capable of holding it and relation to the other types
  template <bool Signed, unsigned bitWidth> struct Size2Type {};
  template <> struct Size2Type<false, 64>  { enum { width = 64, is_signed = 0, has_next = 0 }; typedef uint64_t type; typedef uint64_t next; }; // This is wrong for next, but we avoid the code paths that would use it
  template <> struct Size2Type<false, 32>  { enum { width = 32, is_signed = 0, has_next = 1 }; typedef uint32_t type; typedef typename Size2Type<false, 64>::type next; };
  template <> struct Size2Type<false, 16>  { enum { width = 16, is_signed = 0, has_next = 1 }; typedef uint16_t type; typedef typename Size2Type<false, 32>::type next; };
  template <> struct Size2Type<false, 8>   { enum { width =  8, is_signed = 0, has_next = 1 }; typedef uint8_t  type; typedef typename Size2Type<false, 16>::type next; };

  template <> struct Size2Type<true, 64>   { enum { width = 64, is_signed = 1, has_next = 0 }; typedef int64_t type; typedef int64_t next; }; // This is wrong for next, but we avoid the code paths that would use it
  template <> struct Size2Type<true, 32>   { enum { width = 32, is_signed = 1, has_next = 1 }; typedef int32_t type; typedef typename Size2Type<true, 64>::type next; };
  template <> struct Size2Type<true, 16>   { enum { width = 16, is_signed = 1, has_next = 1 }; typedef int16_t type; typedef typename Size2Type<true, 32>::type next; };
  template <> struct Size2Type<true, 8>    { enum { width =  8, is_signed = 1, has_next = 1 }; typedef int8_t  type; typedef typename Size2Type<true, 16>::type next; };

  // Convert from one type to another. This is completely unsafe to use anywhere
  template <typename From, typename To> inline To convert(const From v) { return static_cast<To>(v); }
}

/** A fixed point template class that should work like a floating point object.
    @param Signed     If the value is signed. This uses the MSB bit of the value
    @param integral   The number of bits used for the integral value (including the sign bit)
    @param fractional The number of bits used for the fractional value */
template <bool Signed, unsigned integral, unsigned fractional>
struct fixp {
  // Type definition
  enum Sizes {
    is_signed = Signed,
    integral_bitwidth = integral,
    fractional_bitwidth = fractional,
    total_bitwidth = integral + fractional,
  };

  // The minimum resolution that this type could achieve
  static constexpr const double minimum_resolution = 1.0 / (double)(1U<<fractional);
  // What is the word type used for storing this number
  typedef typename Private::Size2Type<is_signed, total_bitwidth>::type support_type;
  // What is the intermediate type used for multiplication before normalizing
  typedef Private::Size2Type<is_signed, total_bitwidth> size2type;
  // What is the word type used for storing this unsigned number
  typedef typename Private::Size2Type<false, total_bitwidth>::type utype;
  // The next type to use when doing multiplication/division/modulo
  typedef typename size2type::next   next_type;

  // Mask required for multiply and divide
  static constexpr const utype fractional_mask = (utype)~((utype(-1)) << fractional);
  static constexpr const utype integral_mask = (utype)~fractional_mask;
  // One in fixed point
  static constexpr const support_type one = static_cast<support_type>(utype(1) << fractional);

  // Member
protected:
  support_type  data;

  // Helpers functions
private:
  // Convert any type to our fixed point value
  constexpr inline support_type from_unit(int && t) { return static_cast<support_type>(utype(t) << fractional); }
  constexpr inline support_type from_unit(unsigned int && t) { return static_cast<support_type>(utype(t) << fractional); }
  constexpr inline support_type from_unit(float && t) { return static_cast<support_type>(t * one); }
  constexpr inline support_type from_unit(double && t) { return static_cast<support_type>(t * one); }
  // Division using Egyptian algorithm, very slow, only used for large type or when DONT_USE_LARGER_FP_TYPE_FOR_MULTIPLICATION
  void divide(fixp denom, fixp & quotient, fixp & remainder) {
    int sign = 0;

    // Adjust sign so the division is always positive
    support_type n, d, x = 1, answer = 0;
    if(data < 0) { sign = !sign; n = -data; } else n = data;
    if(denom.data < 0) { sign = !sign; denom.data = -denom.data; }
    d = denom.data;

    int upshift = 32;
    utype mask = integral_mask;
    utype mod = n % d;
    // Find the best possible shift for this division without loosing resolution
    // It's not the most efficient algorithm here (it computes many modulo), but it's small and simple
    while ((mod & mask) != 0) {
        // If we left shifted from fractional bits, the remainder would overflow
        d >>= 1;
        mask <<= 1;
        upshift--;

        mod = n % d;
    }
    utype q = ((n / d) << upshift) + (mod << upshift) / d;
    quotient.data = sign ? -(support_type)q : (support_type)q;
    remainder.data = q - (quotient * denom).data;
  }


  // Construction
public:
  // Conversion must be explicit
  template <typename T>
  explicit constexpr fixp(T n)             : data(from_unit(Private::move(n))) {}
  // Default construction
  constexpr fixp() : data(0) {}
  // Copy & move constructor
  constexpr fixp(const fixp &other) = default;
  constexpr fixp(fixp && other) = default;
  constexpr fixp(const support_type & rhs, bool) : data(rhs) {}
  constexpr fixp(support_type && other, bool) : data(Private::move(other)) {}

  inline fixp& operator = (const fixp & other) { data = other.data; return *this; }

  // Comparison operators
public:
#define FP_CMP_OP(op) constexpr inline bool operator op(const fixp &o) const { return data op o.data; } \
                      constexpr inline bool operator op(const int o) const   { return data op from_unit((int &&)o); } \
                      constexpr inline bool operator op(const float o) const { return data op from_unit((float &&)o); }
    FP_CMP_OP(==)
    FP_CMP_OP(!=)
    FP_CMP_OP(<)
    FP_CMP_OP(>)
    FP_CMP_OP(>=)
    FP_CMP_OP(<=)
#undef FP_CMP_OP

  // Boolean testing operators
public:
  constexpr inline bool operator ! () const { return !data; }
  constexpr inline fixp operator ~ () const { return fixp(~data, true); } // Not sure this actually make sense
  constexpr inline fixp operator - () const { return fixp(-data, true); }
  inline fixp &operator ++ () { data += one; return *this; }
  inline fixp &operator -- () { data -= one; return *this; }
  inline fixp &operator ++ (int) { constexpr const fixp t(*this); data += one; return t; }
  inline fixp &operator -- (int) { constexpr const fixp t(*this); data -= one; return t; }

  // Bitwise and addition/subtraction operators
public:
#define FP_BIN_OP(op) inline fixp &operator op##=(const fixp &n) { data op##= n.data; return *this; } \
                      constexpr inline fixp operator op(const fixp &n) const { return fixp(data op n.data, true); } \
                      constexpr inline fixp operator op(const int n)   const { return fixp(data op from_unit((int &&)n), true); } \
                      constexpr inline fixp operator op(const float n) const { return fixp(data op from_unit((float&&)n), true); }
  FP_BIN_OP(+)
  FP_BIN_OP(-)
  // Not sure bit operator makes sense here too...
  FP_BIN_OP(&)
  FP_BIN_OP(|)
  FP_BIN_OP(^)
  // Limit code size here even if it's not conceptually a bitwise operator
  FP_BIN_OP(%)
#undef FP_BIN_OP

  // Multiplication, division and modulo operators
public:
  inline fixp &operator *= (const fixp &n) {
    if (!size2type::has_next
      #ifdef DONT_USE_LARGER_FP_TYPE_FOR_MULTIPLICATION
        || 1
      #endif
      ) {
        // Slower multiplication with 4 members
        const support_type ah = (data & integral_mask) >> fractional;
        const support_type bh = (n.data & integral_mask) >> fractional;
        const support_type al = data & fractional_mask;
        const support_type bl = n.data & fractional_mask;

        const support_type x1 = ah * bh;
        const support_type x2 = ah * bl;
        const support_type x3 = al * bh;
        const support_type x4 = al * bl;
        data = (x1 << fractional) + (x2 + x3) + (x4 >> fractional);
        return *this;
      }
    next_type t(static_cast<next_type>(data) * static_cast<next_type>(n.data));
    t >>= fractional;
    data = Private::convert<next_type, support_type>(t);
    return *this;
  }
  constexpr inline fixp operator *(const fixp &n) { return fixp(*this) *= n; }

  inline fixp &operator /=(const fixp &n) {
    if (!size2type::has_next
      #ifdef DONT_USE_LARGER_FP_TYPE_FOR_MULTIPLICATION
        || 1
      #endif
      ) { fixp q, r; divide(n, q, r); data = q.data; return *this; }
    next_type t(data);
    t <<= fractional;
    t /= n.data;
    data = Private::convert<next_type, support_type>(t);
    return *this;
  }
  constexpr inline fixp operator /(const fixp &n) { return fixp(*this) /= n; }

  // Shift operators
public:
  inline fixp &operator >>=(const fixp &n) { data >>= (int)n; return *this; }
  inline fixp &operator <<=(const fixp &n) { data <<= (int)n; return *this; }

  constexpr inline fixp operator <<(const fixp &n) { return fixp(*this) <<= n; }
  constexpr inline fixp operator >>(const fixp &n) { return fixp(*this) >>= n; }


  // Convertion operator
public:
  constexpr explicit inline operator int() const { return data >> fractional; }
  constexpr explicit inline operator float() const { return static_cast<float>(data) / one; }
  constexpr explicit inline operator double() const { return static_cast<double>(data) / one; }
  constexpr inline support_type raw() const { return data; }
};

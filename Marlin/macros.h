/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MACROS_H
#define MACROS_H

#define NUM_AXIS 4
#define ABCE 4
#define XYZE 4
#define ABC  3
#define XYZ  3

// For use in macros that take a single axis letter
#define _AXIS(A) (A##_AXIS)

#define _XMIN_ 100
#define _YMIN_ 200
#define _ZMIN_ 300
#define _XMAX_ 101
#define _YMAX_ 201
#define _ZMAX_ 301

#define FORCE_INLINE __attribute__((always_inline)) inline
#define _UNUSED      __attribute__((unused))
#define _O0          __attribute__((optimize("O0")))
#define _Os          __attribute__((optimize("Os")))
#define _O1          __attribute__((optimize("O1")))
#define _O2          __attribute__((optimize("O2")))
#define _O3          __attribute__((optimize("O3")))

// Clock speed factors
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000L) // 16 or 20
#define INT0_PRESCALER 8

// Nanoseconds per cycle
#define NANOSECONDS_PER_CYCLE (1000000000.0 / F_CPU)

// Remove compiler warning on an unused variable
#define UNUSED(x) (void) (x)

// Macros to make a string from a macro
#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define A(CODE) " " CODE "\n\t"
#define L(CODE) CODE ":\n\t"

// Macros for bit masks
#undef _BV
#define _BV(b) (1 << (b))
#define TEST(n,b) !!((n)&_BV(b))
#define SBI(n,b) (n |= _BV(b))
#define CBI(n,b) (n &= ~_BV(b))
#define SET_BIT_TO(N,B,TF) do{ if (TF) SBI(N,B); else CBI(N,B); }while(0)

#define _BV32(b) (1UL << (b))
#define TEST32(n,b) !!((n)&_BV32(b))
#define SBI32(n,b) (n |= _BV32(b))
#define CBI32(n,b) (n &= ~_BV32(b))

// Macro to check that a number if a power if 2
#define IS_POWER_OF_2(x) ((x) && !((x) & ((x) - 1)))

// Macros for maths shortcuts
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif
#define RADIANS(d) ((d)*M_PI/180.0)
#define DEGREES(r) ((r)*180.0/M_PI)
#define HYPOT2(x,y) (sq(x)+sq(y))

#define CIRCLE_AREA(R) (M_PI * sq(R))
#define CIRCLE_CIRC(R) (2.0 * M_PI * (R))

#define SIGN(a) ((a>0)-(a<0))
#define IS_POWER_OF_2(x) ((x) && !((x) & ((x) - 1)))

// Macros to contrain values
#define NOLESS(v,n) do{ if (v < n) v = n; }while(0)
#define NOMORE(v,n) do{ if (v > n) v = n; }while(0)
#define LIMIT(v,n1,n2) do{ if (v < n1) v = n1; else if (v > n2) v = n2; }while(0)

// Macros to support option testing
#define _CAT(a, ...) a ## __VA_ARGS__
#define SWITCH_ENABLED_false 0
#define SWITCH_ENABLED_true  1
#define SWITCH_ENABLED_0     0
#define SWITCH_ENABLED_1     1
#define SWITCH_ENABLED_0x0   0
#define SWITCH_ENABLED_0x1   1
#define SWITCH_ENABLED_      1
#define ENABLED(b) _CAT(SWITCH_ENABLED_, b)
#define DISABLED(b) !ENABLED(b)

#define WITHIN(V,L,H) ((V) >= (L) && (V) <= (H))
#define NUMERIC(a) WITHIN(a, '0', '9')
#define DECIMAL(a) (NUMERIC(a) || a == '.')
#define NUMERIC_SIGNED(a) (NUMERIC(a) || (a) == '-' || (a) == '+')
#define DECIMAL_SIGNED(a) (DECIMAL(a) || (a) == '-' || (a) == '+')
#define COUNT(a) (sizeof(a)/sizeof(*a))
#define ZERO(a) memset(a,0,sizeof(a))
#define COPY(a,b) memcpy(a,b,MIN(sizeof(a),sizeof(b)))

// Macros for initializing arrays
#define ARRAY_6(v1, v2, v3, v4, v5, v6, ...) { v1, v2, v3, v4, v5, v6 }
#define ARRAY_5(v1, v2, v3, v4, v5, ...)     { v1, v2, v3, v4, v5 }
#define ARRAY_4(v1, v2, v3, v4, ...)         { v1, v2, v3, v4 }
#define ARRAY_3(v1, v2, v3, ...)             { v1, v2, v3 }
#define ARRAY_2(v1, v2, ...)                 { v1, v2 }
#define ARRAY_1(v1, ...)                     { v1 }

#define _ARRAY_N(N, ...) ARRAY_ ##N(__VA_ARGS__)
#define ARRAY_N(N, ...) _ARRAY_N(N, __VA_ARGS__)

// Macros for adding
#define INC_0 1
#define INC_1 2
#define INC_2 3
#define INC_3 4
#define INC_4 5
#define INC_5 6
#define INC_6 7
#define INC_7 8
#define INC_8 9
#define INCREMENT_(n) INC_ ##n
#define INCREMENT(n) INCREMENT_(n)

// Macros for subtracting
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8
#define DECREMENT_(n) DEC_ ##n
#define DECREMENT(n) DECREMENT_(n)

#define PIN_EXISTS(PN) (defined(PN ##_PIN) && PN ##_PIN >= 0)

#define PENDING(NOW,SOON) ((long)(NOW-(SOON))<0)
#define ELAPSED(NOW,SOON) (!PENDING(NOW,SOON))

#define MMM_TO_MMS(MM_M) ((MM_M)/60.0)
#define MMS_TO_MMM(MM_S) ((MM_S)*60.0)

#define NOOP do{} while(0)

#define CEILING(x,y) (((x) + (y) - 1) / (y))

// Avoid double evaluation of arguments on MIN/MAX/ABS
#undef MIN
#undef MAX
#undef ABS
#ifdef __cplusplus

  // C++11 solution that is standards compliant. Return type is deduced automatically
  template <class L, class R> static inline constexpr auto MIN(const L lhs, const R rhs) -> decltype(lhs + rhs) {
    return lhs < rhs ? lhs : rhs;
  }
  template <class L, class R> static inline constexpr auto MAX(const L lhs, const R rhs) -> decltype(lhs + rhs){
    return lhs > rhs ? lhs : rhs;
  }
  template <class T> static inline constexpr const T ABS(const T v) {
    return v >= 0 ? v : -v;
  }
#else

  // Using GCC extensions, but Travis GCC version does not like it and gives
  //  "error: statement-expressions are not allowed outside functions nor in template-argument lists"
  #define MIN(a, b) \
    ({__typeof__(a) _a = (a); \
      __typeof__(b) _b = (b); \
      _a < _b ? _a : _b;})

  #define MAX(a, b) \
    ({__typeof__(a) _a = (a); \
      __typeof__(b) _b = (b); \
      _a > _b ? _a : _b;})

  #define ABS(a) \
    ({__typeof__(a) _a = (a); \
      _a >= 0 ? _a : -_a;})

#endif

#define MIN3(a, b, c)       MIN(MIN(a, b), c)
#define MIN4(a, b, c, d)    MIN(MIN3(a, b, c), d)
#define MIN5(a, b, c, d, e) MIN(MIN4(a, b, c, d), e)
#define MAX3(a, b, c)       MAX(MAX(a, b), c)
#define MAX4(a, b, c, d)    MAX(MAX3(a, b, c), d)
#define MAX5(a, b, c, d, e) MAX(MAX4(a, b, c, d), e)

#define UNEAR_ZERO(x) ((x) < 0.000001)
#define NEAR_ZERO(x) WITHIN(x, -0.000001, 0.000001)
#define NEAR(x,y) NEAR_ZERO((x)-(y))

#define RECIPROCAL(x) (NEAR_ZERO(x) ? 0.0 : 1.0 / (x))
#define FIXFLOAT(f) (f + (f < 0.0 ? -0.00005 : 0.00005))

//
// Maths macros that can be overridden by HAL
//
#define ATAN2(y, x) atan2(y, x)
#define POW(x, y)   pow(x, y)
#define SQRT(x)     sqrt(x)
#define CEIL(x)     ceil(x)
#define FLOOR(x)    floor(x)
#define LROUND(x)   lround(x)
#define FMOD(x, y)  fmod(x, y)
#define HYPOT(x,y)  SQRT(HYPOT2(x,y))

#endif //__MACROS_H

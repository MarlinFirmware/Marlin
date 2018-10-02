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
#pragma once

#include "config.h"

#define NUM_AXIS 4
#define ABCE 4
#define XYZE 4
#define ABC  3
#define XYZ  3

#define _AXIS(A) (A##_AXIS)

// Clock speed factors
#if !defined(CYCLES_PER_MICROSECOND) && !defined(__STM32F1__)
  #define CYCLES_PER_MICROSECOND (F_CPU / 1000000L) // 16 or 20 on AVR
#endif

// Nanoseconds per cycle
#define NANOSECONDS_PER_CYCLE (1000000000.0 / F_CPU)

// Remove compiler warning on an unused variable
#undef UNUSED
#define UNUSED(x) ((void)(x))

// Assembly wrappers for code and labels
#define A(CODE) " " CODE "\n\t"
#define L(CODE) CODE ":\n\t"

// Macros for bit masks
#define SET_BIT_TO(N,B,TF) do{ if (TF) SBI(N,B); else CBI(N,B); }while(0)

#define RADIANS(d) ((d)*float(M_PI)/180.0f)
#define DEGREES(r) ((r)*180.0f/float(M_PI))
#define HYPOT2(x,y) (sq(x)+sq(y))

#define CIRCLE_AREA(R) (float(M_PI) * sq(float(R)))
#define CIRCLE_CIRC(R) (2 * float(M_PI) * float(R))

#define SIGN(a) ((a>0)-(a<0))

// Convenience templates / macros
#undef ABS
#undef MIN
#undef MAX
#ifdef __cplusplus

  // Standards-compliant C++11 solutions

  extern "C++" {

    template <class T> static inline constexpr const T ABS(const T v) { return v >= 0 ? v : -v; }

    template <class V, class N> static inline constexpr void NOLESS(V& v, const N n) {
      if (v < n) v = n;
    }
    template <class V, class N> static inline constexpr void NOMORE(V& v, const N n) {
      if (v > n) v = n;
    }
    template <class V, class N1, class N2> static inline constexpr void LIMIT(V& v, const N1 n1, const N2 n2) {
      if (v < n1) v = n1;
      else if (v > n2) v = n2;
    }

    template <class L, class R> static inline constexpr auto MIN(const L lhs, const R rhs) -> decltype(lhs + rhs) {
      return lhs < rhs ? lhs : rhs;
    }
    template <class L, class R> static inline constexpr auto MAX(const L lhs, const R rhs) -> decltype(lhs + rhs) {
      return lhs > rhs ? lhs : rhs;
    }
    template<class T, class ... Ts> static inline constexpr const T MIN(T V, Ts... Vs) { return MIN(V, MIN(Vs...)); }
    template<class T, class ... Ts> static inline constexpr const T MAX(T V, Ts... Vs) { return MAX(V, MAX(Vs...)); }

  }

#else

  #define ABS(a) ({__typeof__(a) _a = (a); _a >= 0 ? _a : -_a;})

  // Using GCC extensions, but Travis GCC version does not like it and gives
  //  "error: statement-expressions are not allowed outside functions nor in template-argument lists"
  #define NOLESS(v, n) \
    do { \
      __typeof__(n) _n = (n); \
      if (v < _n) v = _n; \
    } while(0)

  #define NOMORE(v, n) \
    do { \
      __typeof__(n) _n = (n); \
      if (v > _n) v = _n; \
    } while(0)

  #define LIMIT(v, n1, n2) \
    do { \
      __typeof__(n1) _n1 = (n1); \
      __typeof__(n2) _n2 = (n2); \
      if (v < _n1) v = _n1; \
      else if (v > _n2) v = _n2; \
    } while(0)

  // NUM_ARGS(...) evaluates to the number of arguments
  #define _NUM_ARGS(X,X6,X5,X4,X3,X2,X1,N,...) N
  #define NUM_ARGS(...) _NUM_ARGS(0, __VA_ARGS__ ,6,5,4,3,2,1,0)

  #define MIN_2(a,b)      ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b;})
  #define MIN_3(a,...)    MIN_2(a,MIN_2(__VA_ARGS__))
  #define MIN_4(a,...)    MIN_2(a,MIN_3(__VA_ARGS__))
  #define MIN_5(a,...)    MIN_2(a,MIN_4(__VA_ARGS__))
  #define MIN_6(a,...)    MIN_2(a,MIN_5(__VA_ARGS__))
  #define __MIN_N(N, ...) MIN_ ## N(__VA_ARGS__)
  #define _MIN_N(N, ...)  __MIN_N(N, __VA_ARGS__)
  #define MIN(...)        _MIN_N(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

  #define MAX_2(a,b)      ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b;})
  #define MAX_3(a,...)    MAX_2(a,MAX_2(__VA_ARGS__))
  #define MAX_4(a,...)    MAX_2(a,MAX_3(__VA_ARGS__))
  #define MAX_5(a,...)    MAX_2(a,MAX_4(__VA_ARGS__))
  #define MAX_6(a,...)    MAX_2(a,MAX_5(__VA_ARGS__))
  #define __MAX_N(N, ...) MAX_ ## N(__VA_ARGS__)
  #define _MAX_N(N, ...)  __MAX_N(N, __VA_ARGS__)
  #define MAX(...)        _MAX_N(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#endif

#define NUMERIC(a) WITHIN(a, '0', '9')
#define DECIMAL(a) (NUMERIC(a) || a == '.')
#define NUMERIC_SIGNED(a) (NUMERIC(a) || (a) == '-' || (a) == '+')
#define DECIMAL_SIGNED(a) (DECIMAL(a) || (a) == '-' || (a) == '+')
#define COUNT(a) (sizeof(a)/sizeof(*a))
#define ZERO(a) memset(a,0,sizeof(a))
#define COPY(a,b) memcpy(a,b,MIN(sizeof(a),sizeof(b)))

#define PENDING(NOW,SOON) ((long)(NOW-(SOON))<0)
#define ELAPSED(NOW,SOON) (!PENDING(NOW,SOON))

#define MMM_TO_MMS(MM_M) ((MM_M)/60.0f)
#define MMS_TO_MMM(MM_S) ((MM_S)*60.0f)

#define NOOP do{} while(0)

#define CEILING(x,y) (((x) + (y) - 1) / (y))

#define UNEAR_ZERO(x) ((x) < 0.000001f)
#define NEAR_ZERO(x) WITHIN(x, -0.000001f, 0.000001f)
#define NEAR(x,y) NEAR_ZERO((x)-(y))

#define RECIPROCAL(x) (NEAR_ZERO(x) ? 0 : (1 / float(x)))
#define FIXFLOAT(f) (f + (f < 0 ? -0.00005f : 0.00005f))

//
// Maths macros that can be overridden by HAL
//
#define ATAN2(y, x) atan2f(y, x)
#define POW(x, y)   powf(x, y)
#define SQRT(x)     sqrtf(x)
#define RSQRT(x)    (1 / sqrtf(x))
#define CEIL(x)     ceilf(x)
#define FLOOR(x)    floorf(x)
#define LROUND(x)   lroundf(x)
#define FMOD(x, y)  fmodf(x, y)
#define HYPOT(x,y)  SQRT(HYPOT2(x,y))

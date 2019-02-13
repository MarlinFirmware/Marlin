/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#undef MIN
#undef MAX

#ifdef __cplusplus

  #ifndef _MINMAX_H_
  #define _MINMAX_H_

    extern "C++" {

      // C++11 solution that is standards compliant. Return type is deduced automatically
      template <class L, class R> static inline constexpr auto MIN(const L lhs, const R rhs) -> decltype(lhs + rhs) {
        return lhs < rhs ? lhs : rhs;
      }
      template <class L, class R> static inline constexpr auto MAX(const L lhs, const R rhs) -> decltype(lhs + rhs) {
        return lhs > rhs ? lhs : rhs;
      }
      template<class T, class ... Ts> static inline constexpr const T MIN(T V, Ts... Vs) { return MIN(V, MIN(Vs...)); }
      template<class T, class ... Ts> static inline constexpr const T MAX(T V, Ts... Vs) { return MAX(V, MAX(Vs...)); }

    }

  #endif

#else

  // NUM_ARGS(...) evaluates to the number of arguments
  #define _NUM_ARGS(X,X6,X5,X4,X3,X2,X1,N,...) N
  #define NUM_ARGS(...) _NUM_ARGS(0, __VA_ARGS__ ,6,5,4,3,2,1,0)

  #define MIN_2(a,b)      ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b;})
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

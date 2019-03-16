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

// Pass NUM_ARGS(__VA_ARGS__) to use the number of arguments
#define _NUM_ARGS(_0,_24_,_23,_22,_21,_20,_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,_1,N,...) N
#define NUM_ARGS(...) _NUM_ARGS(0, __VA_ARGS__ ,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

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

  #define MIN_2(a,b)      ((a)<(b)?(a):(b))
  #define MIN_3(a,...)    MIN_2(a,MIN_2(__VA_ARGS__))
  #define MIN_4(a,...)    MIN_2(a,MIN_3(__VA_ARGS__))
  #define MIN_5(a,...)    MIN_2(a,MIN_4(__VA_ARGS__))
  #define MIN_6(a,...)    MIN_2(a,MIN_5(__VA_ARGS__))
  #define MIN_7(a,...)    MIN_2(a,MIN_6(__VA_ARGS__))
  #define MIN_8(a,...)    MIN_2(a,MIN_7(__VA_ARGS__))
  #define MIN_9(a,...)    MIN_2(a,MIN_8(__VA_ARGS__))
  #define MIN_10(a,...)   MIN_2(a,MIN_9(__VA_ARGS__))
  #define __MIN_N(N, ...) MIN_##N(__VA_ARGS__)
  #define _MIN_N(N, ...)  __MIN_N(N,__VA_ARGS__)
  #define MIN(...)        _MIN_N(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

  #define MAX_2(a,b)      ((a)>(b)?(a):(b))
  #define MAX_3(a,...)    MAX_2(a,MAX_2(__VA_ARGS__))
  #define MAX_4(a,...)    MAX_2(a,MAX_3(__VA_ARGS__))
  #define MAX_5(a,...)    MAX_2(a,MAX_4(__VA_ARGS__))
  #define MAX_6(a,...)    MAX_2(a,MAX_5(__VA_ARGS__))
  #define MAX_7(a,...)    MAX_2(a,MAX_6(__VA_ARGS__))
  #define MAX_8(a,...)    MAX_2(a,MAX_7(__VA_ARGS__))
  #define MAX_9(a,...)    MAX_2(a,MAX_8(__VA_ARGS__))
  #define MAX_10(a,...)   MAX_2(a,MAX_9(__VA_ARGS__))
  #define __MAX_N(N, ...) MAX_##N(__VA_ARGS__)
  #define _MAX_N(N, ...)  __MAX_N(N,__VA_ARGS__)
  #define MAX(...)        _MAX_N(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#endif

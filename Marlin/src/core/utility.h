/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfigPre.h"
#include "../core/types.h"
#include "../core/millis_t.h"

// Delay that ensures heaters and watchdog are kept alive
void safe_delay(millis_t ms);

#if ENABLED(SERIAL_OVERRUN_PROTECTION)
  void serial_delay(const millis_t ms);
#else
  inline void serial_delay(const millis_t) {}
#endif

#if GRID_MAX_POINTS_X && GRID_MAX_POINTS_Y

  // 16x16 bit arrays
  template <int W, int H>
  struct FlagBits {
    typename IF<(W>8), uint16_t, uint8_t>::type bits[H];
    void fill()                                   { memset(bits, 0xFF, sizeof(bits)); }
    void reset()                                  { memset(bits, 0x00, sizeof(bits)); }
    void unmark(const uint8_t x, const uint8_t y) { CBI(bits[y], x); }
    void mark(const uint8_t x, const uint8_t y)   { SBI(bits[y], x); }
    bool marked(const uint8_t x, const uint8_t y) { return TEST(bits[y], x); }
    inline void unmark(const xy_int8_t &xy)       { unmark(xy.x, xy.y); }
    inline void mark(const xy_int8_t &xy)         { mark(xy.x, xy.y); }
    inline bool marked(const xy_int8_t &xy)       { return marked(xy.x, xy.y); }
  };

  typedef FlagBits<GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y> MeshFlags;

#endif

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void log_machine_info();
#else
  #define log_machine_info() NOOP
#endif

template<typename T>
class restorer {
  T& ref_;
  T  val_;
public:
  restorer(T& perm) : ref_(perm), val_(perm) {}
  restorer(T& perm, T temp_val) : ref_(perm), val_(perm) { perm = temp_val; }
  ~restorer() { restore(); }
  inline void restore() { ref_ = val_; }
};

#define REMEMBER(N,X,V...) const restorer<typeof(X)> restorer_##N(X, ##V)
#define RESTORE(N) restorer_##N.restore()

// Converts from an uint8_t in the range of 0-255 to an uint8_t
// in the range 0-100 while avoiding rounding artifacts
constexpr uint8_t ui8_to_percent(const uint8_t i) { return (int(i) * 100 + 127) / 255; }

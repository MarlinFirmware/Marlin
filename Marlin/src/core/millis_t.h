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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

typedef uint32_t millis_t;
extern "C" unsigned long millis(void);
void idle(const bool no_stepper_sleep);

#define SEC_TO_MS(N) millis_t((N)*1000UL)
#define MIN_TO_MS(N) SEC_TO_MS((N)*60UL)
#define MS_TO_SEC(N) millis_t((N)/1000UL)
#define MS_TO_SEC_PRECISE(N) (float(N)/1000.0f)

constexpr bool _PENDING(const millis_t now, const millis_t soon) { return (now < soon); }
constexpr bool _PENDING(const millis_t now, const millis_t start, const millis_t interval) { return ((now - start) < interval); }

#define PENDING(V...) _PENDING(V)
#define ELAPSED(V...) !PENDING(V)

template<typename T>
struct MTimeout {
  millis_t start = 0;
  T delay = 0;
  MTimeout() {}
  MTimeout(T interval) { start = millis(); delay = interval; }
  bool pending(const millis_t ms=millis()) { return PENDING(ms, start, delay); }
  bool elapsed(const millis_t ms=millis()) { return ELAPSED(ms, start, delay); }
  bool on_pending(const millis_t ms=millis()) { return delay && pending(ms); }
  bool on_elapsed(const millis_t ms=millis()) { return delay && elapsed(ms); }
  void prime(const millis_t ms=millis()) { start = ms; }
  void clear(const millis_t ms=millis()) { delay = 0; }
  void idle() { if (delay) { while(pending()) ::idle(false); } }
};

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

#include "../HAL/shared/Marduino.h"

typedef uint32_t millis_t;

void idle(const bool no_stepper_sleep);

#define SEC_TO_MS(N) millis_t((N)*1000UL)
#define MIN_TO_MS(N) SEC_TO_MS((N)*60UL)
#define MS_TO_SEC(N) millis_t((N)/1000UL)
#define MS_TO_SEC_PRECISE(N) (float(N)/1000.0f)

constexpr bool _PENDING(const millis_t now, const millis_t soon) { return (now < soon); }
constexpr bool _PENDING(const millis_t now, const millis_t start, const millis_t interval) { return ((now - start) < interval); }

#define PENDING(V...) _PENDING(V)
#define ELAPSED(V...) !PENDING(V)

typedef void (*timeoutFunc_t)();

template<typename T>
struct MTimeout {
  millis_t start_ms = 0;
  T delay_ms = 0;
  MTimeout() {}
  MTimeout(T interval, const millis_t ms=millis()) { start(interval, ms); }
  inline void prime(const millis_t ms=millis()) { start_ms = ms; }
  inline void clear(const millis_t ms=millis()) { delay_ms = 0; }
  inline void start(const T interval, const millis_t ms=millis()) { delay_ms = interval; prime(ms); }
  inline bool pending(const millis_t ms=millis()) const { return PENDING(ms, start_ms, delay_ms); }
  inline bool elapsed(const millis_t ms=millis()) const { return ELAPSED(ms, start_ms, delay_ms); }
  inline bool enabled() const { return delay_ms != 0; }
  inline bool on_pending(const millis_t ms=millis()) const { return enabled() && pending(ms); }
  inline bool on_elapsed(const millis_t ms=millis()) const { return enabled() && elapsed(ms); }
  inline void idle(const bool nss=false) const { if (enabled()) { while(pending()) ::idle(nss); } }
  inline void dofunc(timeoutFunc_t fn) const { if (enabled()) { while(pending()) fn(); } }
  millis_t remaining(const millis_t ms=millis()) const { return pending(ms) ? start_ms + delay_ms - ms : 0; }
};

template<const int D> struct MDelay : MTimeout<const int> {};

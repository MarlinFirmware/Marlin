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

#define SEC_TO_MS(N) millis_t((N)*1000UL)
#define MIN_TO_MS(N) SEC_TO_MS((N)*60UL)
#define MS_TO_SEC(N) millis_t((N)/1000UL)
#define MS_TO_SEC_PRECISE(N) (float(N)/1000.0f)

constexpr bool _PENDING(const millis_t now, const millis_t when) { return int32_t(when - now) > 0; }
constexpr bool _PENDING(const millis_t now, const millis_t start, const millis_t interval) { return (now - start) < interval; }
#define PENDING(V...)  _PENDING(V)
#define ELAPSED(V...) !_PENDING(V)

inline millis_t nz_millis() { const millis_t now = millis(); return now + !now; }
inline uint16_t millis16() { return uint16_t(millis()); }
inline uint16_t nz_millis16() { return uint16_t(nz_millis()); }

#define FI FORCE_INLINE

void marlin_idle(const bool no_stepper_sleep=false);

// A function to call continually the from run(fn) method
typedef void (*timeoutFunc_t)();

// Pass a test function to the wait_while(fn) / idle_while(fn) methods
typedef bool (*testFunc_t)();

// Single-use instant delay up to 65535 milliseconds. Example: TDelay<500> halfsec; halfsec.idle();
template<uint16_t DELAY_MS>
struct TDelay {
  uint16_t start_ms;
  TDelay(const uint16_t ms=millis16()) { prime(ms); }
  FI void prime(const uint16_t ms=millis16()) { start_ms = millis16(); }
  FI bool pending(const uint16_t ms=millis16()) const { return age() < DELAY_MS; }
  FI bool elapsed(const uint16_t ms=millis16()) const { return age() >= DELAY_MS; }
  FI void wait() const { while (pending()) { /* wait */ } }
  FI void idle(const bool nss=false) const { while (pending()) marlin_idle(nss); }
  FI void wait_while(testFunc_t fn) const { while (pending() && fn()) { /* wait */ }; }
  FI void wait_until(testFunc_t fn) const { while (pending() && !fn()) { /* wait */ }; }
  FI void idle_while(testFunc_t fn, const bool nss=false) const { while (pending() && fn()) marlin_idle(nss); }
  FI void run(timeoutFunc_t fn) const { while (pending()) fn(); }
  FI uint16_t age(const uint16_t ms=millis16()) const { return ms - start_ms; }
  FI uint16_t remaining(const uint16_t ms=millis16()) const { return pending(ms) ? DELAY_MS - age() : 0; }
};

// Reusable timeout in milliseconds units up to 24 days of duration.
template<typename T=millis_t>
struct TTimeout24 {
  T end_ms = 0;
  TTimeout24() {}
  TTimeout24(const T interval, const T ms=millis()) { start(interval, ms); }
  FI void start(const T interval, const T ms=millis()) { end_ms = ms + interval; }
  FI bool pending(const T ms=millis()) const { return _remaining() > 0; }
  FI bool elapsed(const T ms=millis()) const { return _remaining() <= 0; }
  FI void wait() const { while (pending()) { /* wait */ } }
  FI void idle(const bool nss=false) const { while (pending()) marlin_idle(nss); }
  FI void wait_while(testFunc_t fn) const { while (pending() && fn()) { /* wait */ }; }
  FI void wait_until(testFunc_t fn) const { while (pending() && !fn()) { /* wait */ }; }
  FI void idle_while(testFunc_t fn, const bool nss=false) const { while (pending() && fn()) marlin_idle(nss); }
  FI void run(timeoutFunc_t fn) const { while (pending()) fn(); }
  FI signed _remaining(const T ms=millis()) const { return end_ms - ms; }
  FI T remaining(const T ms=millis()) const { return pending(ms) ? _remaining() : 0; }
};

// Reusable timeout in milliseconds units up to 48 days of duration.
template<typename T=millis_t>
struct TTimeout {
  T start_ms = 0, delay_ms = 0;
  TTimeout() {}
  TTimeout(const T interval, const T ms=millis()) { start(interval, ms); }
  FI void prime(const T ms=millis()) { start_ms = ms; }
  FI void cancel() { delay_ms = 0; }
  FI bool enabled() const { return delay_ms != 0; }
  FI void start(const T interval, const T ms=millis()) { delay_ms = interval; prime(ms); }
  FI bool pending(const T ms=millis()) const { return age() < delay_ms; }
  FI bool elapsed(const T ms=millis()) const { return age() >= delay_ms; }
  FI bool on_pending(const T ms=millis()) const { return enabled() && pending(ms); }
  FI bool on_elapsed(const T ms=millis()) const { return enabled() && elapsed(ms); }
  FI void wait() const { while (pending()) { /* wait */ } }
  FI void idle(const bool nss=false) const { while (pending()) marlin_idle(nss); }
  FI void wait_while(testFunc_t fn) const { while (pending() && fn()) { /* wait */ }; }
  FI void wait_until(testFunc_t fn) const { while (pending() && !fn()) { /* wait */ }; }
  FI void idle_while(testFunc_t fn, const bool nss=false) const { while (pending() && fn()) marlin_idle(nss); }
  FI void run(timeoutFunc_t fn) const { while (on_pending()) fn(); }
  FI T age(const T ms=millis()) const { return ms - start_ms; }
  FI T remaining(const T ms=millis()) const { return on_pending(ms) ? delay_ms - age() : 0; }
};

// Reusable timeout in seconds units up to 48 days of duration.
template<typename T=millis_t>
struct TSeconds {
  T start_ms = 0;
  uint8_t delay_sec = 0;
  TSeconds() {}
  TSeconds(const T seconds, const T ms=millis()) { start(seconds, ms); }
  FI void prime(const T ms=millis()) { start_ms = ms; }
  FI void cancel() { delay_sec = 0; }
  FI bool enabled() const { return delay_sec != 0; }
  FI void start(const T seconds, const T ms=millis()) { delay_sec = seconds; prime(ms); }
  FI bool pending(const T ms=millis()) const { return age() < delay_sec; }
  FI bool elapsed(const T ms=millis()) const { return age() >= delay_sec; }
  FI bool on_pending(const T ms=millis()) const { return enabled() && pending(ms); }
  FI bool on_elapsed(const T ms=millis()) const { return enabled() && elapsed(ms); }
  FI void wait() const { while (pending()) { /* wait */ } }
  FI void idle(const bool nss=false) const { while (pending()) marlin_idle(nss); }
  FI void wait_while(testFunc_t fn) const { while (pending() && fn()) { /* wait */ }; }
  FI void wait_until(testFunc_t fn) const { while (pending() && !fn()) { /* wait */ }; }
  FI void idle_while(testFunc_t fn, const bool nss=false) const { while (pending() && fn()) marlin_idle(nss); }
  FI void run(timeoutFunc_t fn) const { while (on_pending()) fn(); }
  FI uint8_t age(const T ms=millis()) const { return MS_TO_SEC(ms - start_ms); }
  FI uint8_t remaining(const T ms=millis()) const { return on_pending(ms) ? delay_sec - age() : 0; }
};

typedef TTimeout24<> MTimeout24;
typedef TTimeout<> MTimeout;
typedef TSeconds<> MSeconds;

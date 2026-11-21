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

constexpr millis_t next_millis(const millis_t ms) { return ms ?: 1; }

constexpr bool _PENDING(const millis_t now, const millis_t when) { return int32_t(when - now) > 0; }
constexpr bool _PENDING(const millis_t now, const millis_t start, const millis_t interval) { return (now - start) < interval; }
#define PENDING(V...)  _PENDING(V)
#define ELAPSED(V...) !_PENDING(V)

inline millis_t nz_millis() { const millis_t now = millis(); return now + !now; }
inline uint16_t millis16() { return uint16_t(millis()); }
inline uint16_t nz_millis16() { return uint16_t(nz_millis()); }

#ifndef FI
  #define FI FORCE_INLINE
#endif

// Defined in MarlinCore.cpp so timers can idle without including MarlinCore.h
void marlin_idle(const bool no_stepper_sleep=false);

// A function to call continually from the run(fn) method
typedef void (*timeoutFunc_t)();

// A test function for the wait_while(fn) / wait_until(fn) / idle_while(fn) / idle_until(fn) methods
typedef bool (*testFunc_t)();

// The signed type of the same width, for wrap-safe 'remaining time' tests
template<typename T> struct TSigned { };
template<> struct TSigned<uint8_t>  { typedef int8_t  type; };
template<> struct TSigned<uint16_t> { typedef int16_t type; };
template<> struct TSigned<uint32_t> { typedef int32_t type; };

/**
 * Blocking helpers shared by all timer types.
 * The derived type provides pending(ms) -- true until the interval has passed.
 *
 *  wait()              Spin until the interval has passed.
 *  idle()              Run marlin_idle() until the interval has passed.
 *  wait_while(fn)      Spin while pending and fn() is true.
 *  wait_until(fn)      Spin while pending and fn() is false.
 *  idle_while(fn)      Idle while pending and fn() is true.
 *  idle_until(fn)      Idle while pending and fn() is false.
 *  run(fn)             Call fn() repeatedly until the interval has passed.
 */
template<typename D>
struct TTimerOps {
private:
  FI bool _pending() const { return static_cast<const D*>(this)->pending(); }
public:
  FI void wait() const { while (_pending()) { /* wait */ } }
  FI void idle(const bool nss=false) const { while (_pending()) marlin_idle(nss); }
  FI void wait_while(testFunc_t fn) const { while (_pending() && fn()) { /* wait */ } }
  FI void wait_until(testFunc_t fn) const { while (_pending() && !fn()) { /* wait */ } }
  FI void idle_while(testFunc_t fn, const bool nss=false) const { while (_pending() && fn()) marlin_idle(nss); }
  FI void idle_until(testFunc_t fn, const bool nss=false) const { while (_pending() && !fn()) marlin_idle(nss); }
  FI void run(timeoutFunc_t fn) const { while (_pending()) fn(); }
};

/**
 * A fixed delay up to 65535 ms, fixed at compile time. Stores only a 16-bit start time.
 * Example: TDelay<500> half_sec; half_sec.idle();
 *
 * - Starts when constructed or when prime() is called.
 * - Wrap-safe for any delay, provided it is re-primed before 65536 ms have passed since it elapsed.
 */
template<uint16_t DELAY_MS>
struct TDelay : TTimerOps<TDelay<DELAY_MS>> {
  uint16_t start_ms;
  TDelay(const uint16_t ms=millis16()) : start_ms(ms) { }
  FI void prime(const uint16_t ms=millis16()) { start_ms = ms; }
  FI uint16_t age(const uint16_t ms=millis16()) const { return uint16_t(ms - start_ms); }
  FI bool pending(const uint16_t ms=millis16()) const { return age(ms) < DELAY_MS; }
  FI bool elapsed(const uint16_t ms=millis16()) const { return age(ms) >= DELAY_MS; }
  FI uint16_t remaining(const uint16_t ms=millis16()) const { return pending(ms) ? uint16_t(DELAY_MS - age(ms)) : 0; }
};

/**
 * A reusable timeout stored as its end time. Works for periods up to half the range of T
 * (24 days for millis_t), and supports "elapsed since boot / never started" at any uptime.
 *
 * - end_ms == 0 means "not running." It is elapsed, not pending, and remaining() is 0.
 *   Use on_elapsed() to be true only when a started timeout has finished.
 * - start() never stores 0 (a start that would end at 0 ends at 1).
 * - Wrap-safe: pending() is a signed comparison, so it is valid so long as
 *   the timer is checked within half the range of T of its end time.
 * - Not suitable for tracking a time in the past. Use TTimeout (start + delay) for that.
 */
template<typename T=millis_t>
struct TTimeout24 : TTimerOps<TTimeout24<T>> {
  typedef typename TSigned<T>::type S;
  T end_ms = 0;
  TTimeout24() {}
  TTimeout24(const T interval, const T ms=millis()) { start(interval, ms); }
  FI void start(const T interval, const T ms=millis()) { end_ms = T(ms + interval); if (!end_ms) end_ms = 1; }
  FI void cancel() { end_ms = 0; }
  FI bool enabled() const { return end_ms != 0; }
  FI S _remaining(const T ms=millis()) const { return S(end_ms - ms); }
  FI bool pending(const T ms=millis()) const { return enabled() && _remaining(ms) > 0; }
  FI bool elapsed(const T ms=millis()) const { return !pending(ms); }
  FI bool on_elapsed(const T ms=millis()) const { return enabled() && !pending(ms); }  // Started, and now done
  FI T remaining(const T ms=millis()) const { return pending(ms) ? T(_remaining(ms)) : 0; }
};

/**
 * A reusable timeout stored as a start time and a delay, in milliseconds, up to the full range of T
 * (49 days for millis_t). Suitable for periodic tasks, since prime(ms) restarts the same interval.
 *
 * - delay_ms == 0 means "disabled." It is never pending, and on_elapsed() is never true.
 * - Wrap-safe provided it is re-primed (or cancelled) before the full range of T has passed
 *   since it elapsed. Otherwise the age wraps and it will appear to be pending again.
 */
template<typename T=millis_t>
struct TTimeout : TTimerOps<TTimeout<T>> {
  T start_ms = 0, delay_ms = 0;
  TTimeout() {}
  TTimeout(const T interval, const T ms=millis()) { start(interval, ms); }
  FI void prime(const T ms=millis()) { start_ms = ms; }
  FI void cancel() { delay_ms = 0; }
  FI bool enabled() const { return delay_ms != 0; }
  FI void start(const T interval, const T ms=millis()) { delay_ms = interval; prime(ms); }
  FI T age(const T ms=millis()) const { return T(ms - start_ms); }
  FI bool pending(const T ms=millis()) const { return age(ms) < delay_ms; }
  FI bool elapsed(const T ms=millis()) const { return age(ms) >= delay_ms; }
  FI bool on_elapsed(const T ms=millis()) const { return enabled() && elapsed(ms); }
  FI T remaining(const T ms=millis()) const { return pending(ms) ? T(delay_ms - age(ms)) : 0; }
};

/**
 * A reusable timeout in seconds, up to 65535 seconds. Stores a millis_t start time
 * and a 16-bit delay, and compares in milliseconds so there is no truncation.
 * Same disabled / re-prime rules as TTimeout.
 */
struct MSeconds : TTimerOps<MSeconds> {
  millis_t start_ms = 0;
  uint16_t delay_sec = 0;
  MSeconds() {}
  MSeconds(const uint16_t seconds, const millis_t ms=millis()) { start(seconds, ms); }
  FI void prime(const millis_t ms=millis()) { start_ms = ms; }
  FI void cancel() { delay_sec = 0; }
  FI bool enabled() const { return delay_sec != 0; }
  FI void start(const uint16_t seconds, const millis_t ms=millis()) { delay_sec = seconds; prime(ms); }
  FI millis_t age_ms(const millis_t ms=millis()) const { return ms - start_ms; }
  FI uint16_t age(const millis_t ms=millis()) const { return uint16_t(MS_TO_SEC(age_ms(ms))); }
  FI bool pending(const millis_t ms=millis()) const { return age_ms(ms) < SEC_TO_MS(delay_sec); }
  FI bool elapsed(const millis_t ms=millis()) const { return age_ms(ms) >= SEC_TO_MS(delay_sec); }
  FI bool on_elapsed(const millis_t ms=millis()) const { return enabled() && elapsed(ms); }
  FI uint16_t remaining(const millis_t ms=millis()) const { return pending(ms) ? uint16_t(delay_sec - age(ms)) : 0; }
};

typedef TTimeout24<> MTimeout24;
typedef TTimeout<> MTimeout;

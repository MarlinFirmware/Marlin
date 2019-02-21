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
#pragma once

#include <chrono>
#include <thread>

class Clock {
public:
  static uint64_t ticks(uint32_t frequency = Clock::frequency) {
    return (Clock::nanos() - Clock::startup.count()) / (1000000000ULL / frequency);
  }

  static uint64_t nanosToTicks(uint64_t ns, uint32_t frequency = Clock::frequency) {
    return ns / (1000000000ULL / frequency);
  }

  // Time acceleration compensated
  static uint64_t ticksToNanos(uint64_t tick, uint32_t frequency = Clock::frequency) {
    return (tick * (1000000000ULL / frequency)) / Clock::time_multiplier;
  }

  static void setFrequency(uint32_t freq) {
    Clock::frequency = freq;
  }

  // Time Acceleration compensated
  static uint64_t nanos() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return (now.count() - Clock::startup.count()) * Clock::time_multiplier;
  }

  static uint64_t micros() {
    return Clock::nanos() / 1000;
  }

  static uint64_t millis() {
    return Clock::micros() / 1000;
  }

  static double seconds() {
    return Clock::nanos() / 1000000000.0;
  }

  static void delayCycles(uint64_t cycles) {
    std::this_thread::sleep_for(std::chrono::nanoseconds( (1000000000L / frequency) * cycles) / Clock::time_multiplier );
  }

  static void delayMicros(uint64_t micros) {
    std::this_thread::sleep_for(std::chrono::microseconds( micros ) / Clock::time_multiplier);
  }

  static void delayMillis(uint64_t millis) {
    std::this_thread::sleep_for(std::chrono::milliseconds( millis ) / Clock::time_multiplier);
  }

  static void delaySeconds(double secs) {
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(secs * 1000) / Clock::time_multiplier);
  }

  // Will reduce timer resolution increasing likelihood of overflows
  static void setTimeMultiplier(double tm) {
    Clock::time_multiplier = tm;
  }

private:
  static std::chrono::nanoseconds startup;
  static uint32_t frequency;
  static double time_multiplier;
};

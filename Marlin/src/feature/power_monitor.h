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

#include "../inc/MarlinConfigPre.h"

template <int VOLTS>
typedef struct {
  static constexpr int volts = VOLTS;
  float value;
  uint32_t raw_value;
  uint16_t raw_value_filtered;
  void reset() { value = 0; raw_value = 0; raw_value_filtered = 0; }
  void add_sample(const uint16_t value) {
    raw_value -= raw_value >> 7;            // Subtract 1/128th of the raw value
    raw_value += uint32_t(value) << 7;      // Add new ADC reading, scaled by 128
    raw_value_filtered = raw_value >> 10;   // Divide to get to 0-16383 range since we used 1/128 IIR filter approach
  }
  void capture() { value = raw_value_filtered * (POWER_MONITOR_ADC_VREF / (volts * 16383)); }
} lpf_reading_t;

class PowerMonitor {
private:
  static uint8_t flags;  // M430, M431 set and clear flags to read/display system current

  #if ENABLED(POWER_MONITOR_CURRENT)
    static lpf_reading_t<POWER_MONITOR_VOLTS_PER_AMP> amps;
  #endif
  #if ENABLED(POWER_MONITOR_VOLTAGE)
    static lpf_reading_t<POWER_MONITOR_VOLTS_PER_VOLT> volts;
  #endif

  #if HAS_POWER_MONITOR_TIMEOUT
    static millis_t next_display_ms;
  #endif

public:
  PowerMonitor() { reset(); }

  FORCE_INLINE static bool display_enabled() { return flags != 0x00; }

  #if ENABLED(POWER_MONITOR_CURRENT)
    FORCE_INLINE static float getAmps() { return amps.value; }
    FORCE_INLINE static bool current_display_enabled() { return TEST(flags, 0); }
    FORCE_INLINE static void set_current_display_enabled(const bool b) { SET_BIT_TO(flags, 0, b); }
    static void add_current_sample(const uint16_t value) { amps.add_sample(value); }
  #endif

  #if ENABLED(POWER_MONITOR_VOLTAGE)
    FORCE_INLINE static float getVolts() { return volts.value; }
    FORCE_INLINE static bool voltage_display_enabled() { return TEST(flags, 1); }
    FORCE_INLINE static void set_voltage_display_enabled(const bool b) { SET_BIT_TO(flags, 1, b); }
    static void add_voltage_sample(const uint16_t value) { volts.add_sample(value); }
  #endif

  #if BOTH(POWER_MONITOR_CURRENT, POWER_MONITOR_VOLTAGE)
    FORCE_INLINE static bool power_display_enabled() { return flags == 0x03; }
  #endif

  static void reset() {

    flags = 0x00;

    #if ENABLED(POWER_MONITOR_CURRENT)
      amps.reset();
    #endif

    #if ENABLED(POWER_MONITOR_VOLTAGE)
      volts.reset();
    #endif
  }

  static void capture_values() {
    #if ENABLED(POWER_MONITOR_CURRENT)
      amps.capture();
    #endif
    #if ENABLED(POWER_MONITOR_VOLTAGE)
      volts.capture();
    #endif
  }
};

extern PowerMonitor power_monitor;

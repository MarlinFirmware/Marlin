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

template <const float& ADC_SCALE>
struct lpf_reading_t {
  float value;
  uint32_t raw_value;
  uint16_t raw_value_filtered;
  void reset() { value = 0; raw_value = 0; raw_value_filtered = 0; }
  void add_sample(const uint16_t sample) {
    raw_value -= raw_value >> 7;            // Subtract 1/128th of the raw value
    raw_value += uint32_t(sample) << 7;     // Add new ADC reading, scaled by 128
    raw_value_filtered = raw_value >> 10;   // Divide to get to 0-16383 range since we used 1/128 IIR filter approach
  }
  void capture() { value = raw_value_filtered * ADC_SCALE; }
};

class PowerMonitor {
private:
  #if ENABLED(POWER_MONITOR_CURRENT)
    static constexpr float amps_adc_scale = float(ADC_VREF) / (POWER_MONITOR_VOLTS_PER_AMP * 16384);
    static lpf_reading_t<amps_adc_scale> amps;
  #endif
  #if ENABLED(POWER_MONITOR_VOLTAGE)
    static constexpr float volts_adc_scale = float(ADC_VREF) / (POWER_MONITOR_VOLTS_PER_VOLT * 16384);
    static lpf_reading_t<volts_adc_scale> volts;
  #endif

public:
  static uint8_t flags;  // M430 flags to display current

  #if ENABLED(SDSUPPORT)
    static millis_t display_item_ms;
    static uint8_t display_item;
  #endif

  PowerMonitor() { reset(); }

  enum PM_Display_Bit : uint8_t {
    PM_DISP_BIT_I, // Current display enable bit
    PM_DISP_BIT_V, // Voltage display enable bit
    PM_DISP_BIT_P  // Power display enable bit
  };

  #if ENABLED(POWER_MONITOR_CURRENT)
    FORCE_INLINE static float getAmps() { return amps.value; }
    void add_current_sample(const uint16_t value) { amps.add_sample(value); }
  #endif

  #if HAS_POWER_MONITOR_VREF
    #if ENABLED(POWER_MONITOR_VOLTAGE)
      FORCE_INLINE static float getVolts() { return volts.value; }
    #else
      FORCE_INLINE static float getVolts() { return POWER_MONITOR_FIXED_VOLTAGE; }  // using a specified fixed valtage as the voltage measurement
    #endif
    #if ENABLED(POWER_MONITOR_VOLTAGE)
      void add_voltage_sample(const uint16_t value) { volts.add_sample(value); }
    #endif
  #endif

  #if HAS_POWER_MONITOR_WATTS
    FORCE_INLINE static float getPower() { return getAmps() * getVolts(); }
  #endif

  #if HAS_SPI_LCD
    FORCE_INLINE static bool display_enabled() { return flags != 0x00; }
    #if ENABLED(POWER_MONITOR_CURRENT)
      static void draw_current();
      FORCE_INLINE static bool current_display_enabled() { return TEST(flags, PM_DISP_BIT_I); }
      FORCE_INLINE static void set_current_display(const bool b) { SET_BIT_TO(flags, PM_DISP_BIT_I, b); }
      FORCE_INLINE static void toggle_current_display() { TBI(flags, PM_DISP_BIT_I); }
    #endif
    #if HAS_POWER_MONITOR_VREF
      static void draw_voltage();
      FORCE_INLINE static bool voltage_display_enabled() { return TEST(flags, PM_DISP_BIT_V); }
      FORCE_INLINE static void set_voltage_display(const bool b) { SET_BIT_TO(flags, PM_DISP_BIT_V, b); }
      FORCE_INLINE static void toggle_voltage_display() { TBI(flags, PM_DISP_BIT_I); }
    #endif
    #if HAS_POWER_MONITOR_WATTS
      static void draw_power();
      FORCE_INLINE static bool power_display_enabled() { return TEST(flags, PM_DISP_BIT_P); }
      FORCE_INLINE static void set_power_display(const bool b) { SET_BIT_TO(flags, PM_DISP_BIT_P, b); }
      FORCE_INLINE static void toggle_power_display() { TBI(flags, PM_DISP_BIT_I); }
    #endif
  #endif

  static void reset() {
    flags = 0x00;

    #if ENABLED(POWER_MONITOR_CURRENT)
      amps.reset();
    #endif

    #if ENABLED(POWER_MONITOR_VOLTAGE)
      volts.reset();
    #endif

    #if ENABLED(SDSUPPORT)
      display_item_ms = 0;
      display_item = 0;
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

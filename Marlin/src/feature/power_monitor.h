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

#include "../inc/MarlinConfig.h"

#if ENABLED(POWER_MONITOR)

  class PowerMonitor {
  private:
    #if HAS_POWER_MONITOR_CURRENT_SENSOR
      // LPF buffer
      uint32_t raw_current_value;
      uint16_t raw_current_value_filtered;
    #endif
    #if HAS_POWER_MONITOR_VOLTAGE_SENSOR
      // LPF buffer
      uint32_t raw_voltage_value;
      uint16_t raw_voltage_value_filtered;
    #endif

    float amps;   // Measured system current (in amps)
    float volts;  // Measured system voltage (in volts)

  public:
    bool current_display_enabled;  // M430 reads and enables/disables the real-time system current LCD display
    bool voltage_display_enabled;  // M431 reads and enables/disables the real-time system voltage LCD display

    PowerMonitor() {
      reset();
    }

    void reset() {
      #if HAS_POWER_MONITOR_CURRENT_SENSOR
        raw_current_value = 0;
        raw_current_value_filtered = 0;
      #endif
      #if HAS_POWER_MONITOR_VOLTAGE_SENSOR
        raw_voltage_value = 0;
        raw_voltage_value_filtered = 0;
      #endif

      current_display_enabled = false;
      voltage_display_enabled = false;

      amps = 0;
      volts = 0;
    }

    #if HAS_POWER_MONITOR_CURRENT_SENSOR
      void add_current_sample(uint16_t value) {
        // add the new sample into the LPF
         raw_current_value -= raw_current_value >> 7;            // Subtract 1/128th of the raw value
         raw_current_value += uint32_t(value) << 7;              // Add new ADC reading, scaled by 128
         raw_current_value_filtered = raw_current_value >> 10;   // Divide to get to 0-16383 range since we used 1/128 IIR filter approach
      }
    #endif
    #if HAS_POWER_MONITOR_VOLTAGE_SENSOR
      void add_voltage_sample(uint16_t value) {
        // add the new sample into the LPF
         raw_voltage_value -= raw_voltage_value >> 7;            // Subtract 1/128th of the raw value
         raw_voltage_value += uint32_t(value) << 7;              // Add new ADC reading, scaled by 128
         raw_voltage_value_filtered = raw_voltage_value >> 10;   // Divide to get to 0-16383 range since we used 1/128 IIR filter approach
      }
    #endif

    void capture_values() {
      #if HAS_POWER_MONITOR_CURRENT_SENSOR
        amps = raw_current_value_filtered * (POWER_MONITOR_ADC_VREF / (POWER_MONITOR_VOLTS_PER_AMP * 16383));
      #endif
      #if HAS_POWER_MONITOR_VOLTAGE_SENSOR
        volts = raw_voltage_value_filtered * (POWER_MONITOR_ADC_VREF / (POWER_MONITOR_VOLTS_PER_VOLT * 16383));
      #endif
    }

    float getAmps() {
      return amps;
    }

    float getVolts() {
      return volts;
    }
  };

  extern PowerMonitor power_monitor;

#endif

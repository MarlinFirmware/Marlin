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

#include "../inc/MarlinConfig.h"

#if HAS_POWER_MONITOR

#include "power_monitor.h"

uint8_t PowerMonitor::flags; // = 0

#if ENABLED(POWER_MONITOR_CURRENT)
  lpf_reading_t<&PowerMonitor::amps_adc_scale> PowerMonitor::amps;
#endif
#if ENABLED(POWER_MONITOR_VOLTAGE)
  lpf_reading_t<&PowerMonitor::volts_adc_scale> PowerMonitor::volts;
#endif

#if HAS_POWER_MONITOR_TIMEOUT
  millis_t PowerMonitor::next_display_ms;
#endif

PowerMonitor power_monitor; // Single instance - this calls the constructor

#endif

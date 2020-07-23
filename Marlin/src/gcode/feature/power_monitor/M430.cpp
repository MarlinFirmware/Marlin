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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_POWER_MONITOR

#include "../../../feature/power_monitor.h"
#include "../../../MarlinCore.h"
#include "../../gcode.h"

/**
 * M430: Enable/disable current LCD display
 *       With no parameters report the system current draw (in Amps)
 *
 *  I[bool] - Set Display of current on the LCD
 *  V[bool] - Set Display of voltage on the LCD
 *  W[bool] - Set Display of power on the LCD
 */
void GcodeSuite::M430() {
  bool do_report = true;
  #if HAS_SPI_LCD
    #if ENABLED(POWER_MONITOR_CURRENT)
      if (parser.seen('I')) { power_monitor.set_current_display(parser.value_bool()); do_report = false; }
    #endif
    #if HAS_POWER_MONITOR_VREF
      if (parser.seen('V')) { power_monitor.set_voltage_display(parser.value_bool()); do_report = false; }
    #endif
    #if HAS_POWER_MONITOR_WATTS
      if (parser.seen('W')) { power_monitor.set_power_display(parser.value_bool()); do_report = false; }
    #endif
  #endif
  if (do_report) {
    SERIAL_ECHOLNPAIR(
      #if ENABLED(POWER_MONITOR_CURRENT)
        "Current: ", power_monitor.getAmps(), "A"
        #if HAS_POWER_MONITOR_VREF
          "  "
        #endif
      #endif
      #if HAS_POWER_MONITOR_VREF
        "Voltage: ", power_monitor.getVolts(), "V"
      #endif
      #if HAS_POWER_MONITOR_WATTS
        "  Power: ", power_monitor.getPower(), "W"
      #endif
    );
  }
}

#endif // HAS_POWER_MONITOR

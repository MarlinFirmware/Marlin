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

#include "../../../inc/MarlinConfig.h"

#if HAS_POWER_MONITOR

#include "../../../feature/power_monitor.h"
#include "../../../Marlin.h"
#include "../../gcode.h"

#if ENABLED(POWER_MONITOR_CURRENT)

  /**
   * M430: Enable/disable current LCD display
   *       With no parameters report the system current draw (in Amps)
   *
   *  D[bool] - Set Display of current on the LCD
   */
  void GcodeSuite::M430() {
    if (parser.seen('D'))
      power_monitor.set_current_display_enabled(parser.value_bool());
    else
      SERIAL_ECHOLNPAIR("Current: ", power_monitor.getAmps(), "A");
  }

#endif

#if ENABLED(POWER_MONITOR_VOLTAGE)

  /**
   * M431: Enable/disable voltage LCD display
   *       With no parameters report the PSU voltage
   *
   *  D[bool] - Set Display of voltage on the LCD
   */
  void GcodeSuite::M431() {
    if (parser.seen('D'))
      power_monitor.set_voltage_display_enabled(parser.value_bool());
    else
      SERIAL_ECHOLNPAIR("Voltage: ", power_monitor.getVolts(), "V");
  }

#endif

#if ENABLED(POWER_MONITOR_CURRENT) && (ENABLED(POWER_MONITOR_VOLTAGE) || defined(POWER_MONITOR_FIXED_VOLTAGE))

  /**
   * M432: Enable/disable power LCD display
   *       With no parameters report the power
   *
   *  D[bool] - Set Display of power on the LCD
   */
  void GcodeSuite::M432() {
    if (parser.seen('D'))
      power_monitor.set_power_display_enabled(parser.value_bool());
    else
      SERIAL_ECHOLNPAIR("power: ", power_monitor.getPower(), "W");
  }

#endif

#endif // HAS_POWER_MONITOR

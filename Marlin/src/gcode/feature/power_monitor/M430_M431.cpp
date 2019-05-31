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

#if HAS_POWER_MONITOR_CURRENT_SENSOR || HAS_POWER_MONITOR_VOLTAGE_SENSOR

  #include "../../../feature/power_monitor.h"
  #include "../../../Marlin.h"
  #include "../../gcode.h"

  // M430: Display the current being drawn by the whole system (in amps), or enable/disable it
  //
  // M430     .. reads the current being drawn
  // M430 D0  .. disables the current LCD display
  // M430 D1  .. enables the current LCD display

  // M431: Display the system voltage, or enable/disable it
  //
  // M431     .. reads the PSU voltage
  // M431 D0  .. disables the voltage LCD display
  // M431 D1  .. enables the voltage LCD display

#if HAS_POWER_MONITOR_CURRENT_SENSOR
  void GcodeSuite::M430() {
    if (parser.seen('D'))
    {
      switch (parser.value_byte())
      {
        case 0:
          power_monitor.current_display_enabled = false;  // disable the LCD display
          break;
        case 1:
          power_monitor.current_display_enabled = true;   // enable the LCD display
          break;
      }
    }
    SERIAL_ECHOLNPAIR("current (amps): ", power_monitor.getAmps());
  }
#endif

#if HAS_POWER_MONITOR_VOLTAGE_SENSOR
  void GcodeSuite::M431() {
    if (parser.seen('D'))
    {
      switch (parser.value_byte())
      {
        case 0:
          power_monitor.voltage_display_enabled = false;  // disable the LCD display
          break;
        case 1:
          power_monitor.voltage_display_enabled = true;   // enable the LCD display
          break;
      }
    }
    SERIAL_ECHOLNPAIR("voltage (volts): ", power_monitor.getVolts());
  }
#endif

#endif

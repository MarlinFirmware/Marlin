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

#include "../inc/MarlinConfig.h"

#if EITHER(EXT_SOLENOID, MANUAL_SOLENOID_CONTROL)

#include "solenoid.h"

#include "../module/motion.h" // for active_extruder

// PARKING_EXTRUDER options alter the default behavior of solenoids, this ensures compliance of M380-381

#if ENABLED(PARKING_EXTRUDER)
  #include "../module/tool_change.h"
#endif

// Used primarily with MANUAL_SOLENOID_CONTROL
static void set_solenoid(const uint8_t num, const bool active) {
  const uint8_t value = active ? PE_MAGNET_ON_STATE : !PE_MAGNET_ON_STATE;
  #define _SOL_CASE(N) case N: TERN_(HAS_SOLENOID_##N, OUT_WRITE(SOL##N##_PIN, value)); break;
  switch (num) {
    REPEAT(8, _SOL_CASE)
    default: SERIAL_ECHO_MSG(STR_INVALID_SOLENOID); break;
  }

  #if ENABLED(PARKING_EXTRUDER)
    if (!active && active_extruder == num) // If active extruder's solenoid is disabled, carriage is considered parked
      parking_extruder_set_parked(true);
  #endif
}

void enable_solenoid(const uint8_t num) { set_solenoid(num, true); }
void disable_solenoid(const uint8_t num) { set_solenoid(num, false); }
void enable_solenoid_on_active_extruder() {  }

void disable_all_solenoids() {
  #define _SOL_DISABLE(N) TERN_(HAS_SOLENOID_##N, disable_solenoid(N));
  REPEAT(8, _SOL_DISABLE)
}

#endif // EXT_SOLENOID || MANUAL_SOLENOID_CONTROL

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

#if EITHER(EXT_SOLENOID, MANUAL_SOLENOID_CONTROL)

#include "solenoid.h"

#include "../module/motion.h" // for active_extruder

#if ENABLED(MANUAL_SOLENOID_CONTROL)
  #define HAS_SOLENOID(N) HAS_SOLENOID_##N
#else
  #define HAS_SOLENOID(N) (HAS_SOLENOID_##N && EXTRUDERS > N)
#endif

// Used primarily with MANUAL_SOLENOID_CONTROL
static void set_solenoid(const uint8_t num, const bool active) {
  const uint8_t value = active ? HIGH : LOW;
  switch (num) {
    case 0:
      OUT_WRITE(SOL0_PIN, value);
      break;
    #if HAS_SOLENOID(1)
      case 1:
        OUT_WRITE(SOL1_PIN, value);
        break;
    #endif
    #if HAS_SOLENOID(2)
      case 2:
        OUT_WRITE(SOL2_PIN, value);
        break;
    #endif
    #if HAS_SOLENOID(3)
      case 3:
        OUT_WRITE(SOL3_PIN, value);
        break;
    #endif
    #if HAS_SOLENOID(4)
      case 4:
        OUT_WRITE(SOL4_PIN, value);
        break;
    #endif
    #if HAS_SOLENOID(5)
      case 5:
        OUT_WRITE(SOL5_PIN, value);
        break;
    #endif
    default:
      SERIAL_ECHO_MSG(MSG_INVALID_SOLENOID);
      break;
  }
}

void enable_solenoid(const uint8_t num) { set_solenoid(num, true); }
void disable_solenoid(const uint8_t num) { set_solenoid(num, false); }
void enable_solenoid_on_active_extruder() { enable_solenoid(active_extruder); }

void disable_all_solenoids() {
  disable_solenoid(0);
  #if HAS_SOLENOID(1)
    disable_solenoid(1);
  #endif
  #if HAS_SOLENOID(2)
    disable_solenoid(2);
  #endif
  #if HAS_SOLENOID(3)
    disable_solenoid(3);
  #endif
  #if HAS_SOLENOID(4)
    disable_solenoid(4);
  #endif
  #if HAS_SOLENOID(5)
    disable_solenoid(5);
  #endif
}

#endif // EXT_SOLENOID || MANUAL_SOLENOID_CONTROL

/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(EXT_SOLENOID)

#include "solenoid.h"

#include "../module/motion.h" // for active_extruder

inline void enable_solenoid(const uint8_t num) {
  switch (num) {
    case 0:
      OUT_WRITE(SOL0_PIN, HIGH);
      break;
      #if HAS_SOLENOID_1 && EXTRUDERS > 1
        case 1:
          OUT_WRITE(SOL1_PIN, HIGH);
          break;
      #endif
      #if HAS_SOLENOID_2 && EXTRUDERS > 2
        case 2:
          OUT_WRITE(SOL2_PIN, HIGH);
          break;
      #endif
      #if HAS_SOLENOID_3 && EXTRUDERS > 3
        case 3:
          OUT_WRITE(SOL3_PIN, HIGH);
          break;
      #endif
      #if HAS_SOLENOID_4 && EXTRUDERS > 4
        case 4:
          OUT_WRITE(SOL4_PIN, HIGH);
          break;
      #endif
    default:
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM(MSG_INVALID_SOLENOID);
      break;
  }
}

void enable_solenoid_on_active_extruder() { enable_solenoid(active_extruder); }

void disable_all_solenoids() {
  OUT_WRITE(SOL0_PIN, LOW);
  #if HAS_SOLENOID_1 && EXTRUDERS > 1
    OUT_WRITE(SOL1_PIN, LOW);
  #endif
  #if HAS_SOLENOID_2 && EXTRUDERS > 2
    OUT_WRITE(SOL2_PIN, LOW);
  #endif
  #if HAS_SOLENOID_3 && EXTRUDERS > 3
    OUT_WRITE(SOL3_PIN, LOW);
  #endif
  #if HAS_SOLENOID_4 && EXTRUDERS > 4
    OUT_WRITE(SOL4_PIN, LOW);
  #endif
}

#endif // EXT_SOLENOID

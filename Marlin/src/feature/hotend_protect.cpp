/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Hotend Protection
 * Prevent filament in the nozzle from charring and causing a critical jam.
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(HOTEND_PROTECTION)

#include "hotend_protect.h"
#include "../gcode/gcode.h"

#include "../module/temperature.h"
#include "../module/motion.h"
#include "../lcd/ultralcd.h"

constexpr millis_t hp_interval = MIN_TO_MS(HP_TIMEOUT);
static millis_t next_protect_ms = 0;

inline void check_hotends(const millis_t &ms) {
  bool prot = false;
  HOTEND_LOOP() {
    if (thermalManager.degHotendNear(e, HP_MIN_TRIGGER)) {
      prot = true; break;
    }
  }
  if (bool(next_protect_ms) != prot)
    next_protect_ms = prot ? ms + hp_interval : 0;
}

inline void check_e_motion(const millis_t &ms) {
  static float old_e_position = 0;
  if (old_e_position != current_position.e) {
    old_e_position = current_position.e;          // Track filament motion
    if (next_protect_ms)                          // If some heater is on then...
      next_protect_ms = ms + hp_interval;         // ...delay the timeout till later
  }
}

// Lower (but don't raise) hotend / bed temperatures
inline void apply_hotend_protection() {
  next_protect_ms = 0;
  SERIAL_ECHOLNPGM("Hotend Protection");
  LCD_MESSAGEPGM(MSG_HOTEND_PROTECTION);
  HOTEND_LOOP() {
    if ((HP_NOZZLE_TARGET) < thermalManager.degTargetHotend(e))
      thermalManager.setTargetHotend(HP_NOZZLE_TARGET, e);
  }
  #if HAS_HEATED_BED
    if ((HP_BED_TARGET) < thermalManager.degTargetBed())
      thermalManager.setTargetBed(HP_BED_TARGET);
  #endif
}

void hotend_protection() {
  const millis_t ms = millis();                   // Shared millis

  check_hotends(ms);                              // Any hotends need protection?
  check_e_motion(ms);                             // Motion will protect them

  // Hot and not moving for too long...
  if (next_protect_ms && ELAPSED(ms, next_protect_ms))
    apply_hotend_protection();
}

#endif // HOTEND_PROTECTION

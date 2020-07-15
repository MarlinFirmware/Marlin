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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Hotend Idle Timeout
 * Prevent filament in the nozzle from charring and causing a critical jam.
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(HOTEND_IDLE_TIMEOUT)

#include "hotend_idle.h"
#include "../gcode/gcode.h"

#include "../module/temperature.h"
#include "../module/motion.h"
#include "../lcd/ultralcd.h"

extern HotendIdleProtection hotend_idle;

millis_t HotendIdleProtection::next_protect_ms = 0;

void HotendIdleProtection::check_hotends(const millis_t &ms) {
  bool do_prot = false;
  HOTEND_LOOP() {
    if (thermalManager.degHotend(active_extruder) >= HOTEND_IDLE_MIN_TRIGGER) {
      do_prot = true; break;
    }
  }
  if (bool(next_protect_ms) != do_prot)
    next_protect_ms = do_prot ? ms + hp_interval : 0;
}

void HotendIdleProtection::check_e_motion(const millis_t &ms) {
  static float old_e_position = 0;
  if (old_e_position != current_position.e) {
    old_e_position = current_position.e;          // Track filament motion
    if (next_protect_ms)                          // If some heater is on then...
      next_protect_ms = ms + hp_interval;         // ...delay the timeout till later
  }
}

void HotendIdleProtection::check() {
  const millis_t ms = millis();                   // Shared millis

  check_hotends(ms);                              // Any hotends need protection?
  check_e_motion(ms);                             // Motion will protect them

  // Hot and not moving for too long...
  if (next_protect_ms && ELAPSED(ms, next_protect_ms))
    timed_out();
}

// Lower (but don't raise) hotend / bed temperatures
void HotendIdleProtection::timed_out() {
  next_protect_ms = 0;
  SERIAL_ECHOLNPGM("Hotend Idle Timeout");
  LCD_MESSAGEPGM(MSG_HOTEND_IDLE_TIMEOUT);
  HOTEND_LOOP() {
    if ((HOTEND_IDLE_NOZZLE_TARGET) < thermalManager.degTargetHotend(e))
      thermalManager.setTargetHotend(HOTEND_IDLE_NOZZLE_TARGET, e);
  }
  #if HAS_HEATED_BED
    if ((HOTEND_IDLE_BED_TARGET) < thermalManager.degTargetBed())
      thermalManager.setTargetBed(HOTEND_IDLE_BED_TARGET);
  #endif
}

#endif // HOTEND_IDLE_TIMEOUT

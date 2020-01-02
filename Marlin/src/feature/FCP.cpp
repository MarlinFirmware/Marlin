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
 * Filament Carbonization Prevention
 * Prevent nozzle jam/clog as result from carbonized filament (which happens if the nozzle stay hot too long without extrusion) 
 * 
 * Logic:
 * Check if extruder have not moved during FCP_TIMEOUT(minutes), check if temperature is above FCP_MIN_TRIGGER(celsius)
 * if both conditions are true, then set nozzle and bed to their respective FCP_NOZZLE_TARGET and FCP_BED_TARGET
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(FCP_ENABLE)

#include "../module/stepper/indirection.h"
#include "../module/temperature.h"
#include "../module/motion.h"
#include "../gcode/gcode.h"

constexpr millis_t fcp_interval = (FCP_TIMEOUT) * 1000UL * 60UL; // convert minutes to ms

void fcp_update() {
  millis_t ms = millis();
  static millis_t nextMillis = ms + fcp_interval;
  static float old_e_position = 0;
  if (old_e_position != current_position.e) {
    old_e_position == current_position.e;
    nextMillis = ms + fcp_interval;
  }
  else if (nextMillis && ELAPSED(ms, nextMillis)) {
    bool trigger = false;
    HOTEND_LOOP() if (thermalManager.degHotend(e) >= (FCP_MIN_TRIGGER)) trigger = true;
    if (trigger) {
      SERIAL_ECHOLNPGM("WARNING: FCP WATCHDOG TRIGGERED");
      gcode.process_subcommands_now_P(PSTR("M117 FCP WATCHDOG TRIG"));

      HOTEND_LOOP() thermalManager.setTargetHotend(FCP_NOZZLE_TARGET, e);
      thermalManager.setTargetBed(FCP_BED_TARGET);
    }
    nextMillis = 0;
    //SERIAL_ECHOLNPGM("FCP WATCHDOG MONITORING");
  }
}

#endif // FCP_ENABLE

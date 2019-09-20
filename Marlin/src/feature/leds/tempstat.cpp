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
 * Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(TEMP_STAT_LEDS)

#include "tempstat.h"
#include "../../module/temperature.h"

void handle_status_leds() {
  static int8_t old_red = -1;  // Invalid value to force LED initialization
  static millis_t next_status_led_update_ms = 0;
  if (ELAPSED(millis(), next_status_led_update_ms)) {
    next_status_led_update_ms += 500; // Update every 0.5s
    float max_temp = 0.0;
    #if HAS_HEATED_BED
      max_temp = _MAX(thermalManager.degTargetBed(), thermalManager.degBed());
    #endif
    HOTEND_LOOP()
      max_temp = _MAX(max_temp, thermalManager.degHotend(e), thermalManager.degTargetHotend(e));
    const int8_t new_red = (max_temp > 55.0) ? HIGH : (max_temp < 54.0 || old_red < 0) ? LOW : old_red;
    if (new_red != old_red) {
      old_red = new_red;
      #if PIN_EXISTS(STAT_LED_RED)
        WRITE(STAT_LED_RED_PIN, new_red);
      #endif
      #if PIN_EXISTS(STAT_LED_BLUE)
        WRITE(STAT_LED_BLUE_PIN, !new_red);
      #endif
    }
  }
}

#endif // TEMP_STAT_LEDS

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

/**
 * Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(TEMP_STAT_LEDS) || ENABLED(HEAT_STAT_LEDS)

#include "tempstat.h"
#include "../../module/temperature.h"

void handle_status_leds(void) {
  #if ENABLED(TEMP_STAT_LEDS)
    static int8_t old_red = -1;  // Invalid value to force LED initialization
  #endif
  #if ENABLED(HEAT_STAT_LEDS)
    static uint8_t heating_state = -1;
    static uint8_t heat_led = -1;
  #endif
  static millis_t next_status_led_update_ms = 0;
  if (ELAPSED(millis(), next_status_led_update_ms)) {
    next_status_led_update_ms += 500; // Update every 0.5s
    #if ENABLED(TEMP_STAT_LEDS)
      float max_temp = 0.0;
      #if HAS_HEATED_BED
        max_temp = MAX(thermalManager.degTargetBed(), thermalManager.degBed());
      #endif
      HOTEND_LOOP()
        max_temp = MAX(max_temp, thermalManager.degHotend(e), thermalManager.degTargetHotend(e));
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
    #endif //end TEMP_STAT_LEDS
    #if ENABLED(HEAT_STAT_LEDS)
    uint8_t new_heating_state = 0;
    HOTEND_LOOP()
      new_heating_state = MAX((
      #if HAS_HEATED_BED
        thermalManager.degTargetBed() ||
      #endif
        thermalManager.degTargetHotend(e)) ? ((
      #if HAS_HEATED_BED
        thermalManager.isHeatingBed() ||
      #endif
      thermalManager.isHeatingHotend(e)) ? 2 : 1) : 0 ,new_heating_state);
    switch(new_heating_state) {
      case(0): //heaters off, led(s) off
        if(new_heating_state != heating_state) {
          heating_state = new_heating_state;
          #if PIN_EXISTS(HEAT_STAT_LED_GREEN) && PIN_EXISTS(HEAT_STAT_LED_RED)
            WRITE(HEAT_STAT_LED_GREEN_PIN, LOW);
            WRITE(HEAT_STAT_LED_RED_PIN, LOW);
          #else
            #if PIN_EXISTS(HEAT_STAT_LED_RED)
              WRITE(HEAT_STAT_LED_RED_PIN, LOW);
            #else
              WRITE(HEAT_STAT_LED_GREEN_PIN, LOW);
            #endif
          #endif
        } break;
      case(1): //heaters on target, led on solid
        if(new_heating_state != heating_state) {
          heating_state = new_heating_state;
          #if PIN_EXISTS(HEAT_STAT_LED_GREEN) && PIN_EXISTS(HEAT_STAT_LED_RED)
            WRITE(HEAT_STAT_LED_RED_PIN, LOW);
            WRITE(HEAT_STAT_LED_GREEN_PIN, HIGH);
          #else
            #if PIN_EXISTS(HEAT_STAT_LED_RED)
              WRITE(HEAT_STAT_LED_RED_PIN, HIGH);
            #else
              WRITE(HEAT_STAT_LED_GREEN_PIN,HIGH);
            #endif
          #endif
        } break;
      case(2): //heating up, led is blinking
        if(new_heating_state != heating_state) {
          heating_state = new_heating_state;
          #if PIN_EXISTS(HEAT_STAT_LED_GREEN) && PIN_EXISTS(HEAT_STAT_LED_RED)
            WRITE(HEAT_STAT_LED_GREEN_PIN, LOW);
          #endif
        }
        heat_led = !heat_led; //toggle led state
        #if PIN_EXISTS(HEAT_STAT_LED_RED)
          WRITE(HEAT_STAT_LED_RED_PIN, heat_led);
        #else
          WRITE(HEAT_STAT_LED_GREEN_PIN, heat_led);
        #endif
        break;
    }
    #endif //end HEAT_STAT_LEDS
  }
}
#endif // TEMP_STAT_LEDS || HEAT_STAT_LEDS

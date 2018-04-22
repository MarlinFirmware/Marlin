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

#include "../../inc/MarlinConfig.h"

#if HAS_HEATER_BED && HAS_TEMP_BED

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/motion.h"
#include "../../lcd/ultralcd.h"

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
  #include "../../module/printcounter.h"
#endif

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../../feature/leds/leds.h"
#endif

#include "../../Marlin.h" // for wait_for_heatup and idle()

/**
 * M140: Set bed temperature
 */
void GcodeSuite::M140() {
  if (DEBUGGING(DRYRUN)) return;
  if (parser.seenval('S')) thermalManager.setTargetBed(parser.value_celsius());
}

#ifndef MIN_COOLING_SLOPE_DEG_BED
  #define MIN_COOLING_SLOPE_DEG_BED 1.50
#endif
#ifndef MIN_COOLING_SLOPE_TIME_BED
  #define MIN_COOLING_SLOPE_TIME_BED 60
#endif

/**
 * M190: Sxxx Wait for bed current temp to reach target temp. Waits only when heating
 *       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
 */
void GcodeSuite::M190() {
  if (DEBUGGING(DRYRUN)) return;

  const bool no_wait_for_cooling = parser.seenval('S');
  if (no_wait_for_cooling || parser.seenval('R')) {
    thermalManager.setTargetBed(parser.value_celsius());
    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      if (parser.value_celsius() > BED_MINTEMP)
        print_job_timer.start();
    #endif
  }
  else return;

  lcd_setstatusPGM(thermalManager.isHeatingBed() ? PSTR(MSG_BED_HEATING) : PSTR(MSG_BED_COOLING));

  #if TEMP_BED_RESIDENCY_TIME > 0
    millis_t residency_start_ms = 0;
    // Loop until the temperature has stabilized
    #define TEMP_BED_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_BED_RESIDENCY_TIME) * 1000UL))
  #else
    // Loop until the temperature is very close target
    #define TEMP_BED_CONDITIONS (wants_to_cool ? thermalManager.isCoolingBed() : thermalManager.isHeatingBed())
  #endif

  float target_temp = -1.0, old_temp = 9999.0;
  bool wants_to_cool = false;
  wait_for_heatup = true;
  millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  target_extruder = active_extruder; // for print_heaterstates

  #if ENABLED(PRINTER_EVENT_LEDS)
    const float start_temp = thermalManager.degBed();
    uint8_t old_red = 127;
  #endif

  do {
    // Target temperature might be changed during the loop
    if (target_temp != thermalManager.degTargetBed()) {
      wants_to_cool = thermalManager.isCoolingBed();
      target_temp = thermalManager.degTargetBed();

      // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
      if (no_wait_for_cooling && wants_to_cool) break;
    }

    now = millis();
    if (ELAPSED(now, next_temp_ms)) { //Print Temp Reading every 1 second while heating up.
      next_temp_ms = now + 1000UL;
      thermalManager.print_heaterstates();
      #if TEMP_BED_RESIDENCY_TIME > 0
        SERIAL_PROTOCOLPGM(" W:");
        if (residency_start_ms)
          SERIAL_PROTOCOL(long((((TEMP_BED_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
        else
          SERIAL_PROTOCOLCHAR('?');
      #endif
      SERIAL_EOL();
    }

    idle();
    reset_stepper_timeout(); // Keep steppers powered

    const float temp = thermalManager.degBed();

    #if ENABLED(PRINTER_EVENT_LEDS)
      // Gradually change LED strip from blue to violet as bed heats up
      if (!wants_to_cool) {
        const uint8_t red = map(constrain(temp, start_temp, target_temp), start_temp, target_temp, 0, 255);
        if (red != old_red) {
          old_red = red;
          leds.set_color(
            MakeLEDColor(red, 0, 255, 0, pixels.getBrightness())
            #if ENABLED(NEOPIXEL_IS_SEQUENTIAL)
              , true
            #endif
          );
        }
      }
    #endif

    #if TEMP_BED_RESIDENCY_TIME > 0

      const float temp_diff = FABS(target_temp - temp);

      if (!residency_start_ms) {
        // Start the TEMP_BED_RESIDENCY_TIME timer when we reach target temp for the first time.
        if (temp_diff < TEMP_BED_WINDOW) residency_start_ms = now;
      }
      else if (temp_diff > TEMP_BED_HYSTERESIS) {
        // Restart the timer whenever the temperature falls outside the hysteresis.
        residency_start_ms = now;
      }

    #endif // TEMP_BED_RESIDENCY_TIME > 0

    // Prevent a wait-forever situation if R is misused i.e. M190 R0
    if (wants_to_cool) {
      // Break after MIN_COOLING_SLOPE_TIME_BED seconds
      // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG_BED
      if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
        if (old_temp - temp < MIN_COOLING_SLOPE_DEG_BED) break;
        next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME_BED;
        old_temp = temp;
      }
    }

  } while (wait_for_heatup && TEMP_BED_CONDITIONS);

  if (wait_for_heatup) lcd_reset_status();
  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(IN_HANDLER);
  #endif
}

#endif // HAS_HEATER_BED && HAS_TEMP_BED

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

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../lcd/ultralcd.h"
#include "../../Marlin.h"

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
  #include "../../module/printcounter.h"
#endif

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../../feature/leds/leds.h"
#endif

/**
 * M104: Set hot end temperature
 */
void GcodeSuite::M104() {
  if (get_target_extruder_from_command()) return;
  if (DEBUGGING(DRYRUN)) return;

  const uint8_t e = target_extruder;

  #if ENABLED(SINGLENOZZLE)
    if (e != active_extruder) return;
  #endif

  if (parser.seenval('S')) {
    const int16_t temp = parser.value_celsius();
    thermalManager.setTargetHotend(temp, e);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && e == 0)
        thermalManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Stop the timer at the end of print. Start is managed by 'heat and wait' M109.
       * We use half EXTRUDE_MINTEMP here to allow nozzles to be put into hot
       * standby mode, for instance in a dual extruder setup, without affecting
       * the running print timer.
       */
      if (parser.value_celsius() <= (EXTRUDE_MINTEMP) / 2) {
        print_job_timer.stop();
        LCD_MESSAGEPGM(WELCOME_MSG);
      }
    #endif

    if (parser.value_celsius() > thermalManager.degHotend(e))
      lcd_status_printf_P(0, PSTR("E%i %s"), e + 1, MSG_HEATING);
  }

  #if ENABLED(AUTOTEMP)
    planner.autotemp_M104_M109();
  #endif
}

/**
 * M109: Sxxx Wait for extruder(s) to reach temperature. Waits only when heating.
 *       Rxxx Wait for extruder(s) to reach temperature. Waits when heating and cooling.
 */

#ifndef MIN_COOLING_SLOPE_DEG
  #define MIN_COOLING_SLOPE_DEG 1.50
#endif
#ifndef MIN_COOLING_SLOPE_TIME
  #define MIN_COOLING_SLOPE_TIME 60
#endif

void GcodeSuite::M109() {

  if (get_target_extruder_from_command()) return;
  if (DEBUGGING(DRYRUN)) return;

  #if ENABLED(SINGLENOZZLE)
    if (target_extruder != active_extruder) return;
  #endif

  const bool no_wait_for_cooling = parser.seenval('S');
  if (no_wait_for_cooling || parser.seenval('R')) {
    const int16_t temp = parser.value_celsius();
    thermalManager.setTargetHotend(temp, target_extruder);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        thermalManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Use half EXTRUDE_MINTEMP to allow nozzles to be put into hot
       * standby mode, (e.g., in a dual extruder setup) without affecting
       * the running print timer.
       */
      if (parser.value_celsius() <= (EXTRUDE_MINTEMP) / 2) {
        print_job_timer.stop();
        LCD_MESSAGEPGM(WELCOME_MSG);
      }
      else
        print_job_timer.start();
    #endif

    if (thermalManager.isHeatingHotend(target_extruder)) lcd_status_printf_P(0, PSTR("E%i %s"), target_extruder + 1, MSG_HEATING);
  }
  else return;

  #if ENABLED(AUTOTEMP)
    planner.autotemp_M104_M109();
  #endif

  #if TEMP_RESIDENCY_TIME > 0
    millis_t residency_start_ms = 0;
    // Loop until the temperature has stabilized
    #define TEMP_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_RESIDENCY_TIME) * 1000UL))
  #else
    // Loop until the temperature is very close target
    #define TEMP_CONDITIONS (wants_to_cool ? thermalManager.isCoolingHotend(target_extruder) : thermalManager.isHeatingHotend(target_extruder))
  #endif

  float target_temp = -1.0, old_temp = 9999.0;
  bool wants_to_cool = false;
  wait_for_heatup = true;
  millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  #if ENABLED(PRINTER_EVENT_LEDS)
    const float start_temp = thermalManager.degHotend(target_extruder);
    uint8_t old_blue = 0;
  #endif

  do {
    // Target temperature might be changed during the loop
    if (target_temp != thermalManager.degTargetHotend(target_extruder)) {
      wants_to_cool = thermalManager.isCoolingHotend(target_extruder);
      target_temp = thermalManager.degTargetHotend(target_extruder);

      // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
      if (no_wait_for_cooling && wants_to_cool) break;
    }

    now = millis();
    if (ELAPSED(now, next_temp_ms)) { //Print temp & remaining time every 1s while waiting
      next_temp_ms = now + 1000UL;
      thermalManager.print_heaterstates();
      #if TEMP_RESIDENCY_TIME > 0
        SERIAL_PROTOCOLPGM(" W:");
        if (residency_start_ms)
          SERIAL_PROTOCOL(long((((TEMP_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
        else
          SERIAL_PROTOCOLCHAR('?');
      #endif
      SERIAL_EOL();
    }

    idle();
    refresh_cmd_timeout(); // to prevent stepper_inactive_time from running out

    const float temp = thermalManager.degHotend(target_extruder);

    #if ENABLED(PRINTER_EVENT_LEDS)
      // Gradually change LED strip from violet to red as nozzle heats up
      if (!wants_to_cool) {
        const uint8_t blue = map(constrain(temp, start_temp, target_temp), start_temp, target_temp, 255, 0);
        if (blue != old_blue) {
          old_blue = blue;
          set_led_color(255, 0, blue);
        }
      }
    #endif

    #if TEMP_RESIDENCY_TIME > 0

      const float temp_diff = FABS(target_temp - temp);

      if (!residency_start_ms) {
        // Start the TEMP_RESIDENCY_TIME timer when we reach target temp for the first time.
        if (temp_diff < TEMP_WINDOW) residency_start_ms = now;
      }
      else if (temp_diff > TEMP_HYSTERESIS) {
        // Restart the timer whenever the temperature falls outside the hysteresis.
        residency_start_ms = now;
      }

    #endif

    // Prevent a wait-forever situation if R is misused i.e. M109 R0
    if (wants_to_cool) {
      // break after MIN_COOLING_SLOPE_TIME seconds
      // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG
      if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
        if (old_temp - temp < MIN_COOLING_SLOPE_DEG) break;
        next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME;
        old_temp = temp;
      }
    }

  } while (wait_for_heatup && TEMP_CONDITIONS);

  if (wait_for_heatup) {
    LCD_MESSAGEPGM(MSG_HEATING_COMPLETE);
    #if ENABLED(PRINTER_EVENT_LEDS)
      #if ENABLED(RGB_LED) || ENABLED(BLINKM) || ENABLED(PCA9632) || ENABLED(RGBW_LED)
        set_led_color(LED_WHITE);
      #endif
      #if ENABLED(NEOPIXEL_LED)
        set_neopixel_color(pixels.Color(NEO_WHITE));
      #endif
    #endif
  }

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(IN_HANDLER);
  #endif
}

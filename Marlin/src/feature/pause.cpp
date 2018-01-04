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

/**
 * feature/pause.cpp - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE) || ENABLED(PARK_HEAD_ON_PAUSE)

#include "../Marlin.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../module/printcounter.h"
#include "../module/temperature.h"

#if ENABLED(FWRETRACT)
  #include "../feature/fwretract.h"
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../feature/runout.h"
#endif

#if ENABLED(ULTIPANEL)
  #include "../lcd/ultralcd.h"
#endif

#include "../libs/buzzer.h"
#include "../libs/nozzle.h"

// private:

static float resume_position[XYZE];

#if ENABLED(SDSUPPORT)
  #include "../sd/cardreader.h"
#endif

#if HAS_BUZZER
  static void filament_change_beep(const int8_t max_beep_count, const bool init=false) {
    static millis_t next_buzz = 0;
    static int8_t runout_beep = 0;

    if (init) next_buzz = runout_beep = 0;

    const millis_t ms = millis();
    if (ELAPSED(ms, next_buzz)) {
      if (max_beep_count < 0 || runout_beep < max_beep_count + 5) { // Only beep as long as we're supposed to
        next_buzz = ms + ((max_beep_count < 0 || runout_beep < max_beep_count) ? 2500 : 400);
        BUZZ(300, 2000);
        runout_beep++;
      }
    }
  }
#endif

static void ensure_safe_temperature() {
  bool heaters_heating = true;

  wait_for_heatup = true;    // M108 will clear this
  while (wait_for_heatup && heaters_heating) {
    idle();
    heaters_heating = false;
    HOTEND_LOOP() {
      if (thermalManager.degTargetHotend(e) && abs(thermalManager.degHotend(e) - thermalManager.degTargetHotend(e)) > TEMP_HYSTERESIS) {
        heaters_heating = true;
        #if ENABLED(ULTIPANEL)
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT);
        #endif
        break;
      }
    }
  }
}

void do_pause_e_move(const float &length, const float fr) {
  current_position[E_AXIS] += length / planner.e_factor[active_extruder];
  set_destination_from_current();
  #if IS_KINEMATIC
    planner.buffer_line_kinematic(destination, fr, active_extruder);
  #else
    buffer_line_to_destination(fr);
  #endif
  stepper.synchronize();
}

// public:

uint8_t did_pause_print = 0;

bool pause_print(const float &retract, const point_t &park_point, const float &unload_length/*=0*/,
                 const int8_t max_beep_count/*=0*/, const bool show_lcd/*=false*/
) {
  if (did_pause_print) return false; // already paused

  #ifdef ACTION_ON_PAUSE
    SERIAL_ECHOLNPGM("//action:" ACTION_ON_PAUSE);
  #endif

  if (!DEBUGGING(DRYRUN) && unload_length != 0) {
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      if (!thermalManager.allow_cold_extrude &&
          thermalManager.degTargetHotend(active_extruder) < thermalManager.extrude_min_temp) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_TOO_COLD_FOR_M600);
        return false;
      }
    #endif

    ensure_safe_temperature(); // wait for extruder to heat up before unloading
  }

  // Indicate that the printer is paused
  ++did_pause_print;

  // Pause the print job and timer
  #if ENABLED(SDSUPPORT)
    if (IS_SD_PRINTING) {
      card.pauseSDPrint();
      ++did_pause_print;
    }
  #endif
  print_job_timer.pause();

  // Show initial message and wait for synchronize steppers
  if (show_lcd) {
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INIT);
    #endif
  }

  stepper.synchronize();
  COPY(resume_position, current_position); // Save current position for later

  // Initial retract before move to filament change position
  if (retract && !thermalManager.tooColdToExtrude(active_extruder))
    do_pause_e_move(retract, PAUSE_PARK_RETRACT_FEEDRATE);

  // Park the nozzle by moving up by z_lift and then moving to (x_pos, y_pos)
  Nozzle::park(2, park_point);

  if (unload_length != 0) {
    if (show_lcd) {
      #if ENABLED(ULTIPANEL)
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_UNLOAD);
        idle();
      #endif
    }

    // Unload filament
    do_pause_e_move(unload_length, FILAMENT_CHANGE_UNLOAD_FEEDRATE);
  }

  if (show_lcd) {
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INSERT);
    #endif
  }

  #if HAS_BUZZER
    filament_change_beep(max_beep_count, true);
  #endif

  idle();

  // Disable extruders steppers for manual filament changing (only on boards that have separate ENABLE_PINS)
  #if E0_ENABLE_PIN != X_ENABLE_PIN && E1_ENABLE_PIN != Y_ENABLE_PIN
    disable_e_steppers();
    safe_delay(100);
  #endif

  // Start the heater idle timers
  const millis_t nozzle_timeout = (millis_t)(PAUSE_PARK_NOZZLE_TIMEOUT) * 1000UL;

  HOTEND_LOOP()
    thermalManager.start_heater_idle_timer(e, nozzle_timeout);

  return true;
}

void wait_for_filament_reload(const int8_t max_beep_count/*=0*/) {
  bool nozzle_timed_out = false;

  // Wait for filament insert by user and press button
  KEEPALIVE_STATE(PAUSED_FOR_USER);
  wait_for_user = true;    // LCD click or M108 will clear this
  while (wait_for_user) {
    #if HAS_BUZZER
      filament_change_beep(max_beep_count);
    #endif

    // If the nozzle has timed out, wait for the user to press the button to re-heat the nozzle, then
    // re-heat the nozzle, re-show the insert screen, restart the idle timers, and start over
    if (!nozzle_timed_out)
      HOTEND_LOOP()
        nozzle_timed_out |= thermalManager.is_heater_idle(e);

    if (nozzle_timed_out) {
      #if ENABLED(ULTIPANEL)
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE);
      #endif

      // Wait for LCD click or M108
      while (wait_for_user) idle(true);

      // Re-enable the heaters if they timed out
      HOTEND_LOOP() thermalManager.reset_heater_idle_timer(e);

      // Wait for the heaters to reach the target temperatures
      ensure_safe_temperature();

      #if ENABLED(ULTIPANEL)
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INSERT);
      #endif

      // Start the heater idle timers
      const millis_t nozzle_timeout = (millis_t)(PAUSE_PARK_NOZZLE_TIMEOUT) * 1000UL;

      HOTEND_LOOP()
        thermalManager.start_heater_idle_timer(e, nozzle_timeout);

      wait_for_user = true; /* Wait for user to load filament */
      nozzle_timed_out = false;

      #if HAS_BUZZER
        filament_change_beep(max_beep_count, true);
      #endif
    }

    idle(true);
  }
  KEEPALIVE_STATE(IN_HANDLER);
}

void resume_print(const float &load_length/*=0*/, const float &initial_extrude_length/*=0*/, const int8_t max_beep_count/*=0*/) {
  bool nozzle_timed_out = false;

  if (!did_pause_print) return;

  // Re-enable the heaters if they timed out
  HOTEND_LOOP() {
    nozzle_timed_out |= thermalManager.is_heater_idle(e);
    thermalManager.reset_heater_idle_timer(e);
  }

  if (nozzle_timed_out) ensure_safe_temperature();

  #if HAS_BUZZER
    filament_change_beep(max_beep_count, true);
  #endif

  if (load_length != 0) {
    #if ENABLED(ULTIPANEL)
      // Show "insert filament"
      if (nozzle_timed_out)
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INSERT);
    #endif

    KEEPALIVE_STATE(PAUSED_FOR_USER);
    wait_for_user = true;    // LCD click or M108 will clear this
    while (wait_for_user && nozzle_timed_out) {
      #if HAS_BUZZER
        filament_change_beep(max_beep_count);
      #endif
      idle(true);
    }
    KEEPALIVE_STATE(IN_HANDLER);

    #if ENABLED(ULTIPANEL)
      // Show "load" message
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_LOAD);
    #endif

    // Load filament
    do_pause_e_move(load_length, FILAMENT_CHANGE_LOAD_FEEDRATE);
  }

  #if ENABLED(ULTIPANEL) && ADVANCED_PAUSE_EXTRUDE_LENGTH > 0

    if (!thermalManager.tooColdToExtrude(active_extruder)) {
      float extrude_length = initial_extrude_length;

      do {
        if (extrude_length > 0) {
          // "Wait for filament extrude"
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_EXTRUDE);

          // Extrude filament to get into hotend
          do_pause_e_move(extrude_length, ADVANCED_PAUSE_EXTRUDE_FEEDRATE);
        }

        // Show "Extrude More" / "Resume" menu and wait for reply
        KEEPALIVE_STATE(PAUSED_FOR_USER);
        wait_for_user = false;
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_OPTION);
        while (advanced_pause_menu_response == ADVANCED_PAUSE_RESPONSE_WAIT_FOR) idle(true);
        KEEPALIVE_STATE(IN_HANDLER);

        extrude_length = ADVANCED_PAUSE_EXTRUDE_LENGTH;

        // Keep looping if "Extrude More" was selected
      } while (advanced_pause_menu_response == ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE);
    }

  #endif

  #if ENABLED(ULTIPANEL)
    // "Wait for print to resume"
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_RESUME);
  #endif

  // Intelligent resuming
  #if ENABLED(FWRETRACT)
    // If retracted before goto pause
    if (fwretract.retracted[active_extruder])
      do_pause_e_move(-fwretract.retract_length, fwretract.retract_feedrate_mm_s);
  #else
    // If resume_position negative
    if (resume_position[E_AXIS] < 0) do_pause_e_move(resume_position[E_AXIS], PAUSE_PARK_RETRACT_FEEDRATE);
  #endif

  // Move XY to starting position, then Z
  do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

  // Set Z_AXIS to saved position
  do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);

  // Now all extrusion positions are resumed and ready to be confirmed
  // Set extruder to saved position
  planner.set_e_position_mm(destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS]);

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    filament_ran_out = false;
  #endif

  #if ENABLED(ULTIPANEL)
    // Show pause status screen
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
  #endif

  #ifdef ACTION_ON_RESUME
    SERIAL_ECHOLNPGM("//action:" ACTION_ON_RESUME);
  #endif

  --did_pause_print;

  #if ENABLED(SDSUPPORT)
    if (did_pause_print) {
      card.startFileprint();
      --did_pause_print;
    }
  #endif
}

#endif // ADVANCED_PAUSE_FEATURE || PARK_HEAD_ON_PAUSE

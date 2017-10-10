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

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../feature/runout.h"
#endif

#if ENABLED(ULTIPANEL)
  #include "../lcd/ultralcd.h"
#endif

#include "../libs/buzzer.h"

// private:

static float resume_position[XYZE];

#if ENABLED(SDSUPPORT)
  #include "../sd/cardreader.h"
  static bool sd_print_paused = false;
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

// public:

bool move_away_flag = false;

#if IS_KINEMATIC
  #define RUNPLAN(RATE_MM_S) planner.buffer_line_kinematic(destination, RATE_MM_S, active_extruder)
#else
  #define RUNPLAN(RATE_MM_S) line_to_destination(RATE_MM_S)
#endif

bool pause_print(const float &retract, const float &z_lift, const float &x_pos, const float &y_pos,
                        const float &unload_length/*=0*/ , const int8_t max_beep_count/*=0*/, const bool show_lcd/*=false*/
) {
  if (move_away_flag) return false; // already paused

  if (!DEBUGGING(DRYRUN) && (unload_length != 0 || retract != 0)) {
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
  move_away_flag = true;

  // Pause the print job and timer
  #if ENABLED(SDSUPPORT)
    if (IS_SD_PRINTING) {
      card.pauseSDPrint();
      sd_print_paused = true;
    }
  #endif
  print_job_timer.pause();

  // Show initial message and wait for synchronize steppers
  if (show_lcd) {
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INIT);
    #endif
  }

  // Save current position
  stepper.synchronize();
  COPY(resume_position, current_position);

  if (retract) {
    // Initial retract before move to filament change position
    set_destination_to_current();
    destination[E_AXIS] += retract;
    RUNPLAN(PAUSE_PARK_RETRACT_FEEDRATE);
    stepper.synchronize();
  }

  // Lift Z axis
  if (z_lift > 0)
    do_blocking_move_to_z(current_position[Z_AXIS] + z_lift, PAUSE_PARK_Z_FEEDRATE);

  // Move XY axes to filament exchange position
  do_blocking_move_to_xy(x_pos, y_pos, PAUSE_PARK_XY_FEEDRATE);

  if (unload_length != 0) {
    if (show_lcd) {
      #if ENABLED(ULTIPANEL)
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_UNLOAD);
        idle();
      #endif
    }

    // Unload filament
    set_destination_to_current();
    destination[E_AXIS] += unload_length;
    RUNPLAN(FILAMENT_CHANGE_UNLOAD_FEEDRATE);
    stepper.synchronize();
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

  if (!move_away_flag) return;

  // Re-enable the heaters if they timed out
  HOTEND_LOOP() {
    nozzle_timed_out |= thermalManager.is_heater_idle(e);
    thermalManager.reset_heater_idle_timer(e);
  }

  if (nozzle_timed_out) ensure_safe_temperature();

  #if HAS_BUZZER
    filament_change_beep(max_beep_count, true);
  #endif

  set_destination_to_current();

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
    destination[E_AXIS] += load_length;
    RUNPLAN(FILAMENT_CHANGE_LOAD_FEEDRATE);
    stepper.synchronize();
  }

  #if ENABLED(ULTIPANEL) && ADVANCED_PAUSE_EXTRUDE_LENGTH > 0

    float extrude_length = initial_extrude_length;

    do {
      if (extrude_length > 0) {
        // "Wait for filament extrude"
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_EXTRUDE);

        // Extrude filament to get into hotend
        destination[E_AXIS] += extrude_length;
        RUNPLAN(ADVANCED_PAUSE_EXTRUDE_FEEDRATE);
        stepper.synchronize();
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

  #endif

  #if ENABLED(ULTIPANEL)
    // "Wait for print to resume"
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_RESUME);
  #endif

  // Set extruder to saved position
  destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS];
  planner.set_e_position_mm(current_position[E_AXIS]);

  // Move XY to starting position, then Z
  do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], PAUSE_PARK_XY_FEEDRATE);
  do_blocking_move_to_z(resume_position[Z_AXIS], PAUSE_PARK_Z_FEEDRATE);

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    filament_ran_out = false;
  #endif

  #if ENABLED(ULTIPANEL)
    // Show status screen
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
  #endif

  #if ENABLED(SDSUPPORT)
    if (sd_print_paused) {
      card.startFileprint();
      sd_print_paused = false;
    }
  #endif

  move_away_flag = false;
}

#endif // ADVANCED_PAUSE_FEATURE || PARK_HEAD_ON_PAUSE

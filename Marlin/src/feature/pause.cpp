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
 * feature/pause.cpp - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)

#include "../MarlinCore.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../module/printcounter.h"
#include "../module/temperature.h"

#if ENABLED(FWRETRACT)
  #include "fwretract.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "runout.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "host_actions.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

#include "../lcd/ultralcd.h"

#if HAS_BUZZER
  #include "../libs/buzzer.h"
#endif

#include "../libs/nozzle.h"
#include "pause.h"

// private:

static xyze_pos_t resume_position;

#if HAS_LCD_MENU
  PauseMenuResponse pause_menu_response;
  PauseMode pause_mode = PAUSE_MODE_PAUSE_PRINT;
#endif

fil_change_settings_t fc_settings[EXTRUDERS];

#if ENABLED(SDSUPPORT)
  #include "../sd/cardreader.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #define _PMSG(L) L##_M108
#else
  #define _PMSG(L) L##_LCD
#endif

#if HAS_BUZZER
  static void impatient_beep(const int8_t max_beep_count, const bool restart=false) {

    if (TERN0(HAS_LCD_MENU, pause_mode == PAUSE_MODE_PAUSE_PRINT)) return;

    static millis_t next_buzz = 0;
    static int8_t runout_beep = 0;

    if (restart) next_buzz = runout_beep = 0;

    const bool always = max_beep_count < 0;

    const millis_t ms = millis();
    if (ELAPSED(ms, next_buzz)) {
      if (always || runout_beep < max_beep_count + 5) { // Only beep as long as we're supposed to
        next_buzz = ms + ((always || runout_beep < max_beep_count) ? 1000 : 500);
        BUZZ(50, 880 - (runout_beep & 1) * 220);
        runout_beep++;
      }
    }
  }
  inline void first_impatient_beep(const int8_t max_beep_count) { impatient_beep(max_beep_count, true); }
#else
  inline void impatient_beep(const int8_t, const bool=false) {}
  inline void first_impatient_beep(const int8_t) {}
#endif

/**
 * Ensure a safe temperature for extrusion
 *
 * - Fail if the TARGET temperature is too low
 * - Display LCD placard with temperature status
 * - Return when heating is done or aborted
 *
 * Returns 'true' if heating was completed, 'false' for abort
 */
static bool ensure_safe_temperature(const PauseMode mode=PAUSE_MODE_SAME) {

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    if (!DEBUGGING(DRYRUN) && thermalManager.targetTooColdToExtrude(active_extruder)) {
      SERIAL_ECHO_MSG(STR_ERR_HOTEND_TOO_COLD);
      return false;
    }
  #endif

  #if HAS_LCD_MENU
    lcd_pause_show_message(PAUSE_MESSAGE_HEATING, mode);
  #else
    UNUSED(mode);
  #endif

  return thermalManager.wait_for_hotend(active_extruder);
}

/**
 * Load filament into the hotend
 *
 * - Fail if the a safe temperature was not reached
 * - If pausing for confirmation, wait for a click or M108
 * - Show "wait for load" placard
 * - Load and purge filament
 * - Show "Purge more" / "Continue" menu
 * - Return when "Continue" is selected
 *
 * Returns 'true' if load was completed, 'false' for abort
 */
bool load_filament(const float &slow_load_length/*=0*/, const float &fast_load_length/*=0*/, const float &purge_length/*=0*/, const int8_t max_beep_count/*=0*/,
                   const bool show_lcd/*=false*/, const bool pause_for_user/*=false*/,
                   const PauseMode mode/*=PAUSE_MODE_PAUSE_PRINT*/
                   DXC_ARGS
) {
  TERN(HAS_LCD_MENU,,UNUSED(show_lcd));

  if (!ensure_safe_temperature(mode)) {
    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_STATUS, mode);
    #endif
    return false;
  }

  if (pause_for_user) {
    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_INSERT, mode);
    #endif
    SERIAL_ECHO_MSG(_PMSG(STR_FILAMENT_CHANGE_INSERT));

    first_impatient_beep(max_beep_count);

    KEEPALIVE_STATE(PAUSED_FOR_USER);
    #if ENABLED(HOST_PROMPT_SUPPORT)
      const char tool = '0'
        #if NUM_RUNOUT_SENSORS > 1
          + active_extruder
        #endif
      ;
      host_action_prompt_begin(PROMPT_USER_CONTINUE, PSTR("Load Filament T"), tool);
      host_action_prompt_button(CONTINUE_STR);
      host_action_prompt_show();
    #endif
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("Load Filament")));
    while (wait_for_user) {
      impatient_beep(max_beep_count);
      idle_no_sleep();
    }
  }

  #if HAS_LCD_MENU
    if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_LOAD, mode);
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    const int8_t saved_ext        = active_extruder;
    const bool saved_ext_dup_mode = extruder_duplication_enabled;
    active_extruder = DXC_ext;
    extruder_duplication_enabled = false;
  #endif

  // Slow Load filament
  if (slow_load_length) unscaled_e_move(slow_load_length, FILAMENT_CHANGE_SLOW_LOAD_FEEDRATE);

  // Fast Load Filament
  if (fast_load_length) {
    #if FILAMENT_CHANGE_FAST_LOAD_ACCEL > 0
      const float saved_acceleration = planner.settings.retract_acceleration;
      planner.settings.retract_acceleration = FILAMENT_CHANGE_FAST_LOAD_ACCEL;
    #endif

    unscaled_e_move(fast_load_length, FILAMENT_CHANGE_FAST_LOAD_FEEDRATE);

    #if FILAMENT_CHANGE_FAST_LOAD_ACCEL > 0
      planner.settings.retract_acceleration = saved_acceleration;
    #endif
  }

  #if ENABLED(DUAL_X_CARRIAGE)      // Tie the two extruders movement back together.
    active_extruder = saved_ext;
    extruder_duplication_enabled = saved_ext_dup_mode;
    stepper.set_directions();
  #endif

  #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)

    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_PURGE);
    #endif

    TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Filament Purging..."), CONTINUE_STR));
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("Filament Purging...")));
    wait_for_user = true; // A click or M108 breaks the purge_length loop
    for (float purge_count = purge_length; purge_count > 0 && wait_for_user; --purge_count)
      unscaled_e_move(1, ADVANCED_PAUSE_PURGE_FEEDRATE);
    wait_for_user = false;

  #else

    do {
      if (purge_length > 0) {
        // "Wait for filament purge"
        #if HAS_LCD_MENU
          if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_PURGE);
        #endif

        // Extrude filament to get into hotend
        unscaled_e_move(purge_length, ADVANCED_PAUSE_PURGE_FEEDRATE);
      }

      TERN_(HOST_PROMPT_SUPPORT, filament_load_host_prompt()); // Initiate another host prompt. (NOTE: host_response_handler may also do this!)

      #if HAS_LCD_MENU
        if (show_lcd) {
          // Show "Purge More" / "Resume" menu and wait for reply
          KEEPALIVE_STATE(PAUSED_FOR_USER);
          wait_for_user = false;
          lcd_pause_show_message(PAUSE_MESSAGE_OPTION);
          while (pause_menu_response == PAUSE_RESPONSE_WAIT_FOR) idle_no_sleep();
        }
      #endif

      // Keep looping if "Purge More" was selected
    } while (TERN0(HAS_LCD_MENU, show_lcd && pause_menu_response == PAUSE_RESPONSE_EXTRUDE_MORE));

  #endif

  return true;
}

/**
 * Unload filament from the hotend
 *
 * - Fail if the a safe temperature was not reached
 * - Show "wait for unload" placard
 * - Retract, pause, then unload filament
 * - Disable E stepper (on most machines)
 *
 * Returns 'true' if unload was completed, 'false' for abort
 */
bool unload_filament(const float &unload_length, const bool show_lcd/*=false*/,
                     const PauseMode mode/*=PAUSE_MODE_PAUSE_PRINT*/
                     #if BOTH(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
                       , const float &mix_multiplier/*=1.0*/
                     #endif
) {
  TERN(HAS_LCD_MENU,,UNUSED(show_lcd));

  #if !BOTH(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
    constexpr float mix_multiplier = 1.0;
  #endif

  if (!ensure_safe_temperature(mode)) {
    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_STATUS);
    #endif
    return false;
  }

  #if HAS_LCD_MENU
    if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_UNLOAD, mode);
  #endif

  // Retract filament
  unscaled_e_move(-(FILAMENT_UNLOAD_PURGE_RETRACT) * mix_multiplier, (PAUSE_PARK_RETRACT_FEEDRATE) * mix_multiplier);

  // Wait for filament to cool
  safe_delay(FILAMENT_UNLOAD_PURGE_DELAY);

  // Quickly purge
  unscaled_e_move((FILAMENT_UNLOAD_PURGE_RETRACT + FILAMENT_UNLOAD_PURGE_LENGTH) * mix_multiplier,
                  (FILAMENT_UNLOAD_PURGE_FEEDRATE) * mix_multiplier);

  // Unload filament
  #if FILAMENT_CHANGE_UNLOAD_ACCEL > 0
    const float saved_acceleration = planner.settings.retract_acceleration;
    planner.settings.retract_acceleration = FILAMENT_CHANGE_UNLOAD_ACCEL;
  #endif

  unscaled_e_move(unload_length * mix_multiplier, (FILAMENT_CHANGE_UNLOAD_FEEDRATE) * mix_multiplier);

  #if FILAMENT_CHANGE_FAST_LOAD_ACCEL > 0
    planner.settings.retract_acceleration = saved_acceleration;
  #endif

  // Disable E steppers for manual change
  #if HAS_E_STEPPER_ENABLE
    disable_e_stepper(active_extruder);
    safe_delay(100);
  #endif

  return true;
}

// public:

/**
 * Pause procedure
 *
 * - Abort if already paused
 * - Send host action for pause, if configured
 * - Abort if TARGET temperature is too low
 * - Display "wait for start of filament change" (if a length was specified)
 * - Initial retract, if current temperature is hot enough
 * - Park the nozzle at the given position
 * - Call unload_filament (if a length was specified)
 *
 * Return 'true' if pause was completed, 'false' for abort
 */
uint8_t did_pause_print = 0;

bool pause_print(const float &retract, const xyz_pos_t &park_point, const float &unload_length/*=0*/, const bool show_lcd/*=false*/ DXC_ARGS) {
  TERN(HAS_LCD_MENU,,UNUSED(show_lcd));

  if (did_pause_print) return false; // already paused

  #if ENABLED(HOST_ACTION_COMMANDS)
    #ifdef ACTION_ON_PAUSED
      host_action_paused();
    #elif defined(ACTION_ON_PAUSE)
      host_action_pause();
    #endif
  #endif

  TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_INFO, PSTR("Pause"), DISMISS_STR));

  if (!DEBUGGING(DRYRUN) && unload_length && thermalManager.targetTooColdToExtrude(active_extruder)) {
    SERIAL_ECHO_MSG(STR_ERR_HOTEND_TOO_COLD);

    #if HAS_LCD_MENU
      if (show_lcd) { // Show status screen
        lcd_pause_show_message(PAUSE_MESSAGE_STATUS);
        LCD_MESSAGEPGM(MSG_M600_TOO_COLD);
      }
    #endif

    return false; // unable to reach safe temperature
  }

  // Indicate that the printer is paused
  ++did_pause_print;

  // Pause the print job and timer
  #if ENABLED(SDSUPPORT)
    if (IS_SD_PRINTING()) {
      card.pauseSDPrint();
      ++did_pause_print; // Indicate SD pause also
    }
  #endif

  print_job_timer.pause();

  // Save current position
  resume_position = current_position;

  // Wait for buffered blocks to complete
  planner.synchronize();

  #if ENABLED(ADVANCED_PAUSE_FANS_PAUSE) && HAS_FAN
    thermalManager.set_fans_paused(true);
  #endif

  // Initial retract before move to filament change position
  if (retract && thermalManager.hotEnoughToExtrude(active_extruder))
    unscaled_e_move(retract, PAUSE_PARK_RETRACT_FEEDRATE);

  // Park the nozzle by moving up by z_lift and then moving to (x_pos, y_pos)
  if (!axes_need_homing())
    nozzle.park(0, park_point);

  #if ENABLED(DUAL_X_CARRIAGE)
    const int8_t saved_ext        = active_extruder;
    const bool saved_ext_dup_mode = extruder_duplication_enabled;
    active_extruder = DXC_ext;
    extruder_duplication_enabled = false;
  #endif

  if (unload_length)   // Unload the filament
    unload_filament(unload_length, show_lcd, PAUSE_MODE_CHANGE_FILAMENT);

  #if ENABLED(DUAL_X_CARRIAGE)
    active_extruder = saved_ext;
    extruder_duplication_enabled = saved_ext_dup_mode;
    stepper.set_directions();
  #endif

  return true;
}

/**
 * For Paused Print:
 * - Show "Press button (or M108) to resume"
 *
 * For Filament Change:
 * - Show "Insert filament and press button to continue"
 *
 * - Wait for a click before returning
 * - Heaters can time out and must reheat before continuing
 *
 * Used by M125 and M600
 */

void show_continue_prompt(const bool is_reload) {
  TERN_(HAS_LCD_MENU, lcd_pause_show_message(is_reload ? PAUSE_MESSAGE_INSERT : PAUSE_MESSAGE_WAITING));
  SERIAL_ECHO_START();
  serialprintPGM(is_reload ? PSTR(_PMSG(STR_FILAMENT_CHANGE_INSERT) "\n") : PSTR(_PMSG(STR_FILAMENT_CHANGE_WAIT) "\n"));
}

void wait_for_confirmation(const bool is_reload/*=false*/, const int8_t max_beep_count/*=0*/ DXC_ARGS) {
  bool nozzle_timed_out = false;

  show_continue_prompt(is_reload);

  first_impatient_beep(max_beep_count);

  // Start the heater idle timers
  const millis_t nozzle_timeout = SEC_TO_MS(PAUSE_PARK_NOZZLE_TIMEOUT);

  HOTEND_LOOP() thermalManager.hotend_idle[e].start(nozzle_timeout);

  #if ENABLED(DUAL_X_CARRIAGE)
    const int8_t saved_ext        = active_extruder;
    const bool saved_ext_dup_mode = extruder_duplication_enabled;
    active_extruder = DXC_ext;
    extruder_duplication_enabled = false;
  #endif

  // Wait for filament insert by user and press button
  KEEPALIVE_STATE(PAUSED_FOR_USER);
  TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, GET_TEXT(MSG_NOZZLE_PARKED), CONTINUE_STR));
  TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(GET_TEXT(MSG_NOZZLE_PARKED)));
  wait_for_user = true;    // LCD click or M108 will clear this
  while (wait_for_user) {
    impatient_beep(max_beep_count);

    // If the nozzle has timed out...
    if (!nozzle_timed_out)
      HOTEND_LOOP() nozzle_timed_out |= thermalManager.hotend_idle[e].timed_out;

    // Wait for the user to press the button to re-heat the nozzle, then
    // re-heat the nozzle, re-show the continue prompt, restart idle timers, start over
    if (nozzle_timed_out) {
      TERN_(HAS_LCD_MENU, lcd_pause_show_message(PAUSE_MESSAGE_HEAT));
      SERIAL_ECHO_MSG(_PMSG(STR_FILAMENT_CHANGE_HEAT));

      TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, GET_TEXT(MSG_HEATER_TIMEOUT), GET_TEXT(MSG_REHEAT)));

      TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(GET_TEXT(MSG_HEATER_TIMEOUT)));

      wait_for_user_response(0, true); // Wait for LCD click or M108

      TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_INFO, GET_TEXT(MSG_REHEATING)));

      TERN_(EXTENSIBLE_UI, ExtUI::onStatusChanged_P(GET_TEXT(MSG_REHEATING)));

      // Re-enable the heaters if they timed out
      HOTEND_LOOP() thermalManager.reset_hotend_idle_timer(e);

      // Wait for the heaters to reach the target temperatures
      ensure_safe_temperature();

      // Show the prompt to continue
      show_continue_prompt(is_reload);

      // Start the heater idle timers
      const millis_t nozzle_timeout = SEC_TO_MS(PAUSE_PARK_NOZZLE_TIMEOUT);

      HOTEND_LOOP() thermalManager.hotend_idle[e].start(nozzle_timeout);
      TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Reheat Done"), CONTINUE_STR));
      TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("Reheat finished.")));
      wait_for_user = true;
      nozzle_timed_out = false;

      first_impatient_beep(max_beep_count);
    }
    idle_no_sleep();
  }
  #if ENABLED(DUAL_X_CARRIAGE)
    active_extruder = saved_ext;
    extruder_duplication_enabled = saved_ext_dup_mode;
    stepper.set_directions();
  #endif
}

/**
 * Resume or Start print procedure
 *
 * - If not paused, do nothing and return
 * - Reset heater idle timers
 * - Load filament if specified, but only if:
 *   - a nozzle timed out, or
 *   - the nozzle is already heated.
 * - Display "wait for print to resume"
 * - Retract to prevent oozing
 * - Move the nozzle back to resume_position
 * - Unretract
 * - Re-prime the nozzle...
 *   -  FWRETRACT: Recover/prime from the prior G10.
 *   - !FWRETRACT: Retract by resume_position.e, if negative.
 *                 Not sure how this logic comes into use.
 * - Sync the planner E to resume_position.e
 * - Send host action for resume, if configured
 * - Resume the current SD print job, if any
 */
void resume_print(const float &slow_load_length/*=0*/, const float &fast_load_length/*=0*/, const float &purge_length/*=ADVANCED_PAUSE_PURGE_LENGTH*/, const int8_t max_beep_count/*=0*/, int16_t targetTemp/*=0*/ DXC_ARGS) {
  /*
  SERIAL_ECHOLNPAIR(
    "start of resume_print()\ndual_x_carriage_mode:", dual_x_carriage_mode,
    "\nextruder_duplication_enabled:", extruder_duplication_enabled,
    "\nactive_extruder:", active_extruder,
    "\n"
  );
  //*/

  if (!did_pause_print) return;

  // Re-enable the heaters if they timed out
  bool nozzle_timed_out = false;
  HOTEND_LOOP() {
    nozzle_timed_out |= thermalManager.hotend_idle[e].timed_out;
    thermalManager.reset_hotend_idle_timer(e);
  }

  if (targetTemp > thermalManager.degTargetHotend(active_extruder))
    thermalManager.setTargetHotend(targetTemp, active_extruder);

  if (nozzle_timed_out || thermalManager.hotEnoughToExtrude(active_extruder)) // Load the new filament
    load_filament(slow_load_length, fast_load_length, purge_length, max_beep_count, true, nozzle_timed_out, PAUSE_MODE_SAME DXC_PASS);

  if (targetTemp > 0) {
    thermalManager.setTargetHotend(targetTemp, active_extruder);
    thermalManager.wait_for_hotend(active_extruder, false);
  }

  TERN_(HAS_LCD_MENU, lcd_pause_show_message(PAUSE_MESSAGE_RESUME));

  // Retract to prevent oozing
  unscaled_e_move(-(PAUSE_PARK_RETRACT_LENGTH), feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));

  // Move XY to starting position, then Z
  do_blocking_move_to_xy(resume_position, feedRate_t(NOZZLE_PARK_XY_FEEDRATE));

  // Move Z_AXIS to saved position
  do_blocking_move_to_z(resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));

  // Unretract
  unscaled_e_move(PAUSE_PARK_RETRACT_LENGTH, feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));

  // Intelligent resuming
  #if ENABLED(FWRETRACT)
    // If retracted before goto pause
    if (fwretract.retracted[active_extruder])
      unscaled_e_move(-fwretract.settings.retract_length, fwretract.settings.retract_feedrate_mm_s);
  #endif

  // If resume_position is negative
  if (resume_position.e < 0) unscaled_e_move(resume_position.e, feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));
  #if ADVANCED_PAUSE_RESUME_PRIME != 0
    unscaled_e_move(ADVANCED_PAUSE_RESUME_PRIME, feedRate_t(ADVANCED_PAUSE_PURGE_FEEDRATE));
  #endif

  // Now all extrusion positions are resumed and ready to be confirmed
  // Set extruder to saved position
  planner.set_e_position_mm((destination.e = current_position.e = resume_position.e));

  TERN_(HAS_LCD_MENU, lcd_pause_show_message(PAUSE_MESSAGE_STATUS));

  #ifdef ACTION_ON_RESUMED
    host_action_resumed();
  #elif defined(ACTION_ON_RESUME)
    host_action_resume();
  #endif

  --did_pause_print;

  TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_INFO, PSTR("Resuming"), DISMISS_STR));

  #if ENABLED(SDSUPPORT)
    if (did_pause_print) { card.startFileprint(); --did_pause_print; }
  #endif

  #if ENABLED(ADVANCED_PAUSE_FANS_PAUSE) && HAS_FAN
    thermalManager.set_fans_paused(false);
  #endif

  TERN_(HAS_FILAMENT_SENSOR, runout.reset());

  // Resume the print job timer if it was running
  if (print_job_timer.isPaused()) print_job_timer.start();

  TERN_(HAS_DISPLAY, ui.reset_status());
  TERN_(HAS_LCD_MENU, ui.return_to_status());
}

#endif // ADVANCED_PAUSE_FEATURE

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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * About Marlin
 *
 * This firmware is a mashup between Sprinter and grbl.
 *  - https://github.com/kliment/Sprinter
 *  - https://github.com/grbl/grbl
 */

#include "MarlinCore.h"

#if ENABLED(MARLIN_DEV_MODE)
  #warning "WARNING! Disable MARLIN_DEV_MODE for the final build!"
#endif

#include "HAL/shared/Delay.h"
#include "HAL/shared/esp_wifi.h"
#include "HAL/shared/cpu_exception/exception_hook.h"

#ifdef ARDUINO
  #include <pins_arduino.h>
#endif
#include <math.h>

#include "core/utility.h"

#include "module/motion.h"
#include "module/planner.h"
#include "module/endstops.h"
#include "module/temperature.h"
#include "module/settings.h"
#include "module/printcounter.h" // PrintCounter or Stopwatch

#include "module/stepper.h"
#include "module/stepper/indirection.h"

#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "gcode/queue.h"

#include "feature/pause.h"
#include "sd/cardreader.h"

#include "lcd/marlinui.h"
#if HAS_TOUCH_BUTTONS
  #include "lcd/touch/touch_buttons.h"
#endif

#if HAS_TFT_LVGL_UI
  #include "lcd/extui/mks_ui/tft_lvgl_configuration.h"
  #include "lcd/extui/mks_ui/draw_ui.h"
  #include "lcd/extui/mks_ui/mks_hardware_test.h"
  #include <lvgl.h>
#endif

#if ENABLED(DWIN_CREALITY_LCD)
  #include "lcd/dwin/e3v2/dwin.h"
  #include "lcd/dwin/e3v2/rotary_encoder.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "lcd/extui/ui_api.h"
#endif

#if HAS_ETHERNET
  #include "feature/ethernet.h"
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #include "libs/BL24CXX.h"
#endif

#if ENABLED(DIRECT_STEPPING)
  #include "feature/direct_stepping.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "feature/host_actions.h"
#endif

#if USE_BEEPER
  #include "libs/buzzer.h"
#endif

#if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
  #include "feature/closedloop.h"
#endif

#if HAS_MOTOR_CURRENT_I2C
  #include "feature/digipot/digipot.h"
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "feature/mixing.h"
#endif

#if ENABLED(MAX7219_DEBUG)
  #include "feature/max7219.h"
#endif

#if HAS_COLOR_LEDS
  #include "feature/leds/leds.h"
#endif

#if ENABLED(BLTOUCH)
  #include "feature/bltouch.h"
#endif

#if ENABLED(POLL_JOG)
  #include "feature/joystick.h"
#endif

#if HAS_SERVOS
  #include "module/servo.h"
#endif

#if ENABLED(HAS_MOTOR_CURRENT_DAC)
  #include "feature/dac/stepper_dac.h"
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "feature/twibus.h"
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  #include "feature/encoder_i2c.h"
#endif

#if HAS_TRINAMIC_CONFIG && DISABLED(PSU_DEFAULT_OFF)
  #include "feature/tmc_util.h"
#endif

#if HAS_CUTTER
  #include "feature/spindle_laser.h"
#endif

#if ENABLED(SDSUPPORT)
  CardReader card;
#endif

#if ENABLED(G38_PROBE_TARGET)
  uint8_t G38_move; // = 0
  bool G38_did_trigger; // = false
#endif

#if ENABLED(DELTA)
  #include "module/delta.h"
#elif IS_SCARA
  #include "module/scara.h"
#endif

#if HAS_LEVELING
  #include "feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(GCODE_REPEAT_MARKERS)
  #include "feature/repeat.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "feature/powerloss.h"
#endif

#if ENABLED(CANCEL_OBJECTS)
  #include "feature/cancel_object.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "feature/runout.h"
#endif

#if EITHER(PROBE_TARE, HAS_Z_SERVO_PROBE)
  #include "module/probe.h"
#endif

#if ENABLED(HOTEND_IDLE_TIMEOUT)
  #include "feature/hotend_idle.h"
#endif

#if ENABLED(TEMP_STAT_LEDS)
  #include "feature/leds/tempstat.h"
#endif

#if ENABLED(CASE_LIGHT_ENABLE)
  #include "feature/caselight.h"
#endif

#if HAS_FANMUX
  #include "feature/fanmux.h"
#endif

#include "module/tool_change.h"

#if ENABLED(USE_CONTROLLER_FAN)
  #include "feature/controllerfan.h"
#endif

#if HAS_PRUSA_MMU2
  #include "feature/mmu/mmu2.h"
#endif

#if HAS_L64XX
  #include "libs/L64XX/L64XX_Marlin.h"
#endif

#if ENABLED(PASSWORD_FEATURE)
  #include "feature/password/password.h"
#endif

#if ENABLED(DGUS_LCD_UI_MKS)
  #include "lcd/extui/dgus/DGUSScreenHandler.h"
#endif

#if HAS_DRIVER_SAFE_POWER_PROTECT
  #include "feature/stepper_driver_safety.h"
#endif

PGMSTR(M112_KILL_STR, "M112 Shutdown");

MarlinState marlin_state = MF_INITIALIZING;

// For M109 and M190, this flag may be cleared (by M108) to exit the wait loop
bool wait_for_heatup = true;

// For M0/M1, this flag may be cleared (by M108) to exit the wait-for-user loop
#if HAS_RESUME_CONTINUE
  bool wait_for_user; // = false;

  void wait_for_user_response(millis_t ms/*=0*/, const bool no_sleep/*=false*/) {
    UNUSED(no_sleep);
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    wait_for_user = true;
    if (ms) ms += millis(); // expire time
    while (wait_for_user && !(ms && ELAPSED(millis(), ms)))
      idle(TERN_(ADVANCED_PAUSE_FEATURE, no_sleep));
    wait_for_user = false;
  }

#endif

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */

/**
 * Stepper Reset (RigidBoard, et.al.)
 */
#if HAS_STEPPER_RESET
  void disableStepperDrivers() { OUT_WRITE(STEPPER_RESET_PIN, LOW); } // Drive down to keep motor driver chips in reset
  void enableStepperDrivers()  { SET_INPUT(STEPPER_RESET_PIN); }      // Set to input, allowing pullups to pull the pin high
#endif

/**
 * Sensitive pin test for M42, M226
 */

#include "pins/sensitive_pins.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"

#ifndef RUNTIME_ONLY_ANALOG_TO_DIGITAL
  template <pin_t ...D>
  constexpr pin_t OnlyPins<_SP_END, D...>::table[sizeof...(D)];
#endif

bool pin_is_protected(const pin_t pin) {
  #ifdef RUNTIME_ONLY_ANALOG_TO_DIGITAL
    static const pin_t sensitive_pins[] PROGMEM = { SENSITIVE_PINS };
    const size_t pincount = COUNT(sensitive_pins);
  #else
    static constexpr size_t pincount = OnlyPins<SENSITIVE_PINS>::size;
    static const pin_t (&sensitive_pins)[pincount] PROGMEM = OnlyPins<SENSITIVE_PINS>::table;
  #endif
  LOOP_L_N(i, pincount) {
    const pin_t * const pptr = &sensitive_pins[i];
    if (pin == (sizeof(pin_t) == 2 ? (pin_t)pgm_read_word(pptr) : (pin_t)pgm_read_byte(pptr))) return true;
  }
  return false;
}

#pragma GCC diagnostic pop

void enable_e_steppers() {
  #define _ENA_E(N) ENABLE_AXIS_E##N();
  REPEAT(E_STEPPERS, _ENA_E)
}

void enable_all_steppers() {
  TERN_(AUTO_POWER_CONTROL, powerManager.power_on());
  ENABLE_AXIS_X();
  ENABLE_AXIS_Y();
  ENABLE_AXIS_Z();
  ENABLE_AXIS_I(); // Marlin 6-axis support by DerAndere (https://github.com/DerAndere1/Marlin/wiki)
  ENABLE_AXIS_J();
  ENABLE_AXIS_K();
  enable_e_steppers();

  TERN_(EXTENSIBLE_UI, ExtUI::onSteppersEnabled());
}

void disable_e_steppers() {
  #define _DIS_E(N) DISABLE_AXIS_E##N();
  REPEAT(E_STEPPERS, _DIS_E)
}

void disable_e_stepper(const uint8_t e) {
  #define _CASE_DIS_E(N) case N: DISABLE_AXIS_E##N(); break;
  switch (e) {
    REPEAT(E_STEPPERS, _CASE_DIS_E)
  }
}

void disable_all_steppers() {
  DISABLE_AXIS_X();
  DISABLE_AXIS_Y();
  DISABLE_AXIS_Z();
  DISABLE_AXIS_I();
  DISABLE_AXIS_J();
  DISABLE_AXIS_K();
  disable_e_steppers();

  TERN_(EXTENSIBLE_UI, ExtUI::onSteppersDisabled());
}

/**
 * A Print Job exists when the timer is running or SD is printing
 */
bool printJobOngoing() { return print_job_timer.isRunning() || IS_SD_PRINTING(); }

/**
 * Printing is active when a job is underway but not paused
 */
bool printingIsActive() { return !did_pause_print && printJobOngoing(); }

/**
 * Printing is paused according to SD or host indicators
 */
bool printingIsPaused() {
  return did_pause_print || print_job_timer.isPaused() || IS_SD_PAUSED();
}

void startOrResumeJob() {
  if (!printingIsPaused()) {
    TERN_(GCODE_REPEAT_MARKERS, repeat.reset());
    TERN_(CANCEL_OBJECTS, cancelable.reset());
    TERN_(LCD_SHOW_E_TOTAL, e_move_accumulator = 0);
    #if BOTH(LCD_SET_PROGRESS_MANUALLY, USE_M73_REMAINING_TIME)
      ui.reset_remaining_time();
    #endif
  }
  print_job_timer.start();
}

#if ENABLED(SDSUPPORT)

  inline void abortSDPrinting() {
    IF_DISABLED(NO_SD_AUTOSTART, card.autofile_cancel());
    card.abortFilePrintNow(TERN_(SD_RESORT, true));

    queue.clear();
    quickstop_stepper();

    print_job_timer.abort();

    IF_DISABLED(SD_ABORT_NO_COOLDOWN, thermalManager.disable_all_heaters());

    TERN(HAS_CUTTER, cutter.kill(), thermalManager.zero_fan_speeds()); // Full cutter shutdown including ISR control

    wait_for_heatup = false;

    TERN_(POWER_LOSS_RECOVERY, recovery.purge());

    #ifdef EVENT_GCODE_SD_ABORT
      queue.inject_P(PSTR(EVENT_GCODE_SD_ABORT));
    #endif

    TERN_(PASSWORD_AFTER_SD_PRINT_ABORT, password.lock_machine());
  }

  inline void finishSDPrinting() {
    if (queue.enqueue_one_P(PSTR("M1001"))) { // Keep trying until it gets queued
      marlin_state = MF_RUNNING;              // Signal to stop trying
      TERN_(PASSWORD_AFTER_SD_PRINT_END, password.lock_machine());
      TERN_(DGUS_LCD_UI_MKS, ScreenHandler.SDPrintingFinished());
    }
  }

#endif // SDSUPPORT

/**
 * Minimal management of Marlin's core activities:
 *  - Keep the command buffer full
 *  - Check for maximum inactive time between commands
 *  - Check for maximum inactive time between stepper commands
 *  - Check if CHDK_PIN needs to go LOW
 *  - Check for KILL button held down
 *  - Check for HOME button held down
 *  - Check for CUSTOM USER button held down
 *  - Check if cooling fan needs to be switched on
 *  - Check if an idle but hot extruder needs filament extruded (EXTRUDER_RUNOUT_PREVENT)
 *  - Pulse FET_SAFETY_PIN if it exists
 */
inline void manage_inactivity(const bool no_stepper_sleep=false) {

  queue.get_available_commands();

  const millis_t ms = millis();

  // Prevent steppers timing-out
  const bool do_reset_timeout = no_stepper_sleep
                               || TERN0(PAUSE_PARK_NO_STEPPER_TIMEOUT, did_pause_print);

  // Reset both the M18/M84 activity timeout and the M85 max 'kill' timeout
  if (do_reset_timeout) gcode.reset_stepper_timeout(ms);

  if (gcode.stepper_max_timed_out(ms)) {
    SERIAL_ERROR_MSG(STR_KILL_INACTIVE_TIME, parser.command_ptr);
    kill();
  }

  // M18 / M84 : Handle steppers inactive time timeout
  if (gcode.stepper_inactive_time) {

    static bool already_shutdown_steppers; // = false

    // Any moves in the planner? Resets both the M18/M84
    // activity timeout and the M85 max 'kill' timeout
    if (planner.has_blocks_queued())
      gcode.reset_stepper_timeout(ms);
    else if (!do_reset_timeout && gcode.stepper_inactive_timeout()) {
      if (!already_shutdown_steppers) {
        already_shutdown_steppers = true;  // L6470 SPI will consume 99% of free time without this

        // Individual axes will be disabled if configured
        if (ENABLED(DISABLE_INACTIVE_X)) DISABLE_AXIS_X();
        if (ENABLED(DISABLE_INACTIVE_Y)) DISABLE_AXIS_Y();
        if (ENABLED(DISABLE_INACTIVE_Z)) DISABLE_AXIS_Z();
        if (ENABLED(DISABLE_INACTIVE_I)) DISABLE_AXIS_I();
        if (ENABLED(DISABLE_INACTIVE_J)) DISABLE_AXIS_J();
        if (ENABLED(DISABLE_INACTIVE_K)) DISABLE_AXIS_K();
        if (ENABLED(DISABLE_INACTIVE_E)) disable_e_steppers();

        TERN_(AUTO_BED_LEVELING_UBL, ubl.steppers_were_disabled());
      }
    }
    else
      already_shutdown_steppers = false;
  }

  #if ENABLED(PHOTO_GCODE) && PIN_EXISTS(CHDK)
    // Check if CHDK should be set to LOW (after M240 set it HIGH)
    extern millis_t chdk_timeout;
    if (chdk_timeout && ELAPSED(ms, chdk_timeout)) {
      chdk_timeout = 0;
      WRITE(CHDK_PIN, LOW);
    }
  #endif

  #if HAS_KILL

    // Check if the kill button was pressed and wait just in case it was an accidental
    // key kill key press
    // -------------------------------------------------------------------------------
    static int killCount = 0;   // make the inactivity button a bit less responsive
    const int KILL_DELAY = 750;
    if (kill_state())
      killCount++;
    else if (killCount > 0)
      killCount--;

    // Exceeded threshold and we can confirm that it was not accidental
    // KILL the machine
    // ----------------------------------------------------------------
    if (killCount >= KILL_DELAY) {
      SERIAL_ERROR_MSG(STR_KILL_BUTTON);
      kill();
    }
  #endif

  #if HAS_FREEZE_PIN
    Stepper::frozen = !READ(FREEZE_PIN);
  #endif

  #if HAS_HOME
    // Handle a standalone HOME button
    constexpr millis_t HOME_DEBOUNCE_DELAY = 1000UL;
    static millis_t next_home_key_ms; // = 0
    if (!IS_SD_PRINTING() && !READ(HOME_PIN)) { // HOME_PIN goes LOW when pressed
      const millis_t ms = millis();
      if (ELAPSED(ms, next_home_key_ms)) {
        next_home_key_ms = ms + HOME_DEBOUNCE_DELAY;
        LCD_MESSAGEPGM(MSG_AUTO_HOME);
        queue.inject_P(G28_STR);
      }
    }
  #endif

  #if ENABLED(CUSTOM_USER_BUTTONS)
    // Handle a custom user button if defined
    const bool printer_not_busy = !printingIsActive();
    #define HAS_CUSTOM_USER_BUTTON(N) (PIN_EXISTS(BUTTON##N) && defined(BUTTON##N##_HIT_STATE) && defined(BUTTON##N##_GCODE) && defined(BUTTON##N##_DESC))
    #define CHECK_CUSTOM_USER_BUTTON(N) do{                            \
      constexpr millis_t CUB_DEBOUNCE_DELAY_##N = 250UL;               \
      static millis_t next_cub_ms_##N;                                 \
      if (BUTTON##N##_HIT_STATE == READ(BUTTON##N##_PIN)               \
        && (ENABLED(BUTTON##N##_WHEN_PRINTING) || printer_not_busy)) { \
        const millis_t ms = millis();                                  \
        if (ELAPSED(ms, next_cub_ms_##N)) {                            \
          next_cub_ms_##N = ms + CUB_DEBOUNCE_DELAY_##N;               \
          if (strlen(BUTTON##N##_DESC))                                \
            LCD_MESSAGEPGM_P(PSTR(BUTTON##N##_DESC));                  \
          queue.inject_P(PSTR(BUTTON##N##_GCODE));                     \
        }                                                              \
      }                                                                \
    }while(0)

    #if HAS_CUSTOM_USER_BUTTON(1)
      CHECK_CUSTOM_USER_BUTTON(1);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(2)
      CHECK_CUSTOM_USER_BUTTON(2);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(3)
      CHECK_CUSTOM_USER_BUTTON(3);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(4)
      CHECK_CUSTOM_USER_BUTTON(4);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(5)
      CHECK_CUSTOM_USER_BUTTON(5);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(6)
      CHECK_CUSTOM_USER_BUTTON(6);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(7)
      CHECK_CUSTOM_USER_BUTTON(7);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(8)
      CHECK_CUSTOM_USER_BUTTON(8);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(9)
      CHECK_CUSTOM_USER_BUTTON(9);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(10)
      CHECK_CUSTOM_USER_BUTTON(10);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(11)
      CHECK_CUSTOM_USER_BUTTON(11);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(12)
      CHECK_CUSTOM_USER_BUTTON(12);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(13)
      CHECK_CUSTOM_USER_BUTTON(13);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(14)
      CHECK_CUSTOM_USER_BUTTON(14);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(15)
      CHECK_CUSTOM_USER_BUTTON(15);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(16)
      CHECK_CUSTOM_USER_BUTTON(16);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(17)
      CHECK_CUSTOM_USER_BUTTON(17);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(18)
      CHECK_CUSTOM_USER_BUTTON(18);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(19)
      CHECK_CUSTOM_USER_BUTTON(19);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(20)
      CHECK_CUSTOM_USER_BUTTON(20);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(21)
      CHECK_CUSTOM_USER_BUTTON(21);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(22)
      CHECK_CUSTOM_USER_BUTTON(22);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(23)
      CHECK_CUSTOM_USER_BUTTON(23);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(24)
      CHECK_CUSTOM_USER_BUTTON(24);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(25)
      CHECK_CUSTOM_USER_BUTTON(25);
    #endif
  #endif

  TERN_(USE_CONTROLLER_FAN, controllerFan.update()); // Check if fan should be turned on to cool stepper drivers down

  TERN_(AUTO_POWER_CONTROL, powerManager.check(!ui.on_status_screen() || printJobOngoing() || printingIsPaused()));

  TERN_(HOTEND_IDLE_TIMEOUT, hotend_idle.check());

  #if ENABLED(EXTRUDER_RUNOUT_PREVENT)
    if (thermalManager.degHotend(active_extruder) > (EXTRUDER_RUNOUT_MINTEMP)
      && ELAPSED(ms, gcode.previous_move_ms + SEC_TO_MS(EXTRUDER_RUNOUT_SECONDS))
      && !planner.has_blocks_queued()
    ) {
      #if ENABLED(SWITCHING_EXTRUDER)
        bool oldstatus;
        switch (active_extruder) {
          default: oldstatus = E0_ENABLE_READ(); ENABLE_AXIS_E0(); break;
          #if E_STEPPERS > 1
            case 2: case 3: oldstatus = E1_ENABLE_READ(); ENABLE_AXIS_E1(); break;
            #if E_STEPPERS > 2
              case 4: case 5: oldstatus = E2_ENABLE_READ(); ENABLE_AXIS_E2(); break;
              #if E_STEPPERS > 3
                case 6: case 7: oldstatus = E3_ENABLE_READ(); ENABLE_AXIS_E3(); break;
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #else // !SWITCHING_EXTRUDER
        bool oldstatus;
        switch (active_extruder) {
          default:
          #define _CASE_EN(N) case N: oldstatus = E##N##_ENABLE_READ(); ENABLE_AXIS_E##N(); break;
          REPEAT(E_STEPPERS, _CASE_EN);
        }
      #endif

      const float olde = current_position.e;
      current_position.e += EXTRUDER_RUNOUT_EXTRUDE;
      line_to_current_position(MMM_TO_MMS(EXTRUDER_RUNOUT_SPEED));
      current_position.e = olde;
      planner.set_e_position_mm(olde);
      planner.synchronize();

      #if ENABLED(SWITCHING_EXTRUDER)
        switch (active_extruder) {
          default: oldstatus = E0_ENABLE_WRITE(oldstatus); break;
          #if E_STEPPERS > 1
            case 2: case 3: oldstatus = E1_ENABLE_WRITE(oldstatus); break;
            #if E_STEPPERS > 2
              case 4: case 5: oldstatus = E2_ENABLE_WRITE(oldstatus); break;
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #else // !SWITCHING_EXTRUDER
        switch (active_extruder) {
          #define _CASE_RESTORE(N) case N: E##N##_ENABLE_WRITE(oldstatus); break;
          REPEAT(E_STEPPERS, _CASE_RESTORE);
        }
      #endif // !SWITCHING_EXTRUDER

      gcode.reset_stepper_timeout(ms);
    }
  #endif // EXTRUDER_RUNOUT_PREVENT

  #if ENABLED(DUAL_X_CARRIAGE)
    // handle delayed move timeout
    if (delayed_move_time && ELAPSED(ms, delayed_move_time) && IsRunning()) {
      // travel moves have been received so enact them
      delayed_move_time = 0xFFFFFFFFUL; // force moves to be done
      destination = current_position;
      prepare_line_to_destination();
      planner.synchronize();
    }
  #endif

  TERN_(TEMP_STAT_LEDS, handle_status_leds());

  TERN_(MONITOR_DRIVER_STATUS, monitor_tmc_drivers());

  TERN_(MONITOR_L6470_DRIVER_STATUS, L64xxManager.monitor_driver());

  // Limit check_axes_activity frequency to 10Hz
  static millis_t next_check_axes_ms = 0;
  if (ELAPSED(ms, next_check_axes_ms)) {
    planner.check_axes_activity();
    next_check_axes_ms = ms + 100UL;
  }

  #if PIN_EXISTS(FET_SAFETY)
    static millis_t FET_next;
    if (ELAPSED(ms, FET_next)) {
      FET_next = ms + FET_SAFETY_DELAY;  // 2µs pulse every FET_SAFETY_DELAY mS
      OUT_WRITE(FET_SAFETY_PIN, !FET_SAFETY_INVERTED);
      DELAY_US(2);
      WRITE(FET_SAFETY_PIN, FET_SAFETY_INVERTED);
    }
  #endif
}

/**
 * Standard idle routine keeps the machine alive:
 *  - Core Marlin activities
 *  - Manage heaters (and Watchdog)
 *  - Max7219 heartbeat, animation, etc.
 *
 *  Only after setup() is complete:
 *  - Handle filament runout sensors
 *  - Run HAL idle tasks
 *  - Handle Power-Loss Recovery
 *  - Run StallGuard endstop checks
 *  - Handle SD Card insert / remove
 *  - Handle USB Flash Drive insert / remove
 *  - Announce Host Keepalive state (if any)
 *  - Update the Print Job Timer state
 *  - Update the Beeper queue
 *  - Read Buttons and Update the LCD
 *  - Run i2c Position Encoders
 *  - Auto-report Temperatures / SD Status
 *  - Update the Průša MMU2
 *  - Handle Joystick jogging
 */
void idle(bool no_stepper_sleep/*=false*/) {
  #if ENABLED(MARLIN_DEV_MODE)
    static uint16_t idle_depth = 0;
    if (++idle_depth > 5) SERIAL_ECHOLNPAIR("idle() call depth: ", idle_depth);
  #endif

  // Core Marlin activities
  manage_inactivity(no_stepper_sleep);

  // Manage Heaters (and Watchdog)
  thermalManager.manage_heater();

  // Max7219 heartbeat, animation, etc
  TERN_(MAX7219_DEBUG, max7219.idle_tasks());

  // Return if setup() isn't completed
  if (marlin_state == MF_INITIALIZING) goto IDLE_DONE;

  // TODO: Still causing errors
  (void)check_tool_sensor_stats(active_extruder, true);

  // Handle filament runout sensors
  TERN_(HAS_FILAMENT_SENSOR, runout.run());

  // Run HAL idle tasks
  TERN_(HAL_IDLETASK, HAL_idletask());

  // Check network connection
  TERN_(HAS_ETHERNET, ethernet.check());

  // Handle Power-Loss Recovery
  #if ENABLED(POWER_LOSS_RECOVERY) && PIN_EXISTS(POWER_LOSS)
    if (IS_SD_PRINTING()) recovery.outage();
  #endif

  // Run StallGuard endstop checks
  #if ENABLED(SPI_ENDSTOPS)
    if (endstops.tmc_spi_homing.any
      && TERN1(IMPROVE_HOMING_RELIABILITY, ELAPSED(millis(), sg_guard_period))
    ) LOOP_L_N(i, 4) // Read SGT 4 times per idle loop
        if (endstops.tmc_spi_homing_check()) break;
  #endif

  // Handle SD Card insert / remove
  TERN_(SDSUPPORT, card.manage_media());

  // Handle USB Flash Drive insert / remove
  TERN_(USB_FLASH_DRIVE_SUPPORT, card.diskIODriver()->idle());

  // Announce Host Keepalive state (if any)
  TERN_(HOST_KEEPALIVE_FEATURE, gcode.host_keepalive());

  // Update the Print Job Timer state
  TERN_(PRINTCOUNTER, print_job_timer.tick());

  // Update the Beeper queue
  TERN_(USE_BEEPER, buzzer.tick());

  // Handle UI input / draw events
  TERN(DWIN_CREALITY_LCD, DWIN_Update(), ui.update());

  // Run i2c Position Encoders
  #if ENABLED(I2C_POSITION_ENCODERS)
  {
    static millis_t i2cpem_next_update_ms;
    if (planner.has_blocks_queued()) {
      const millis_t ms = millis();
      if (ELAPSED(ms, i2cpem_next_update_ms)) {
        I2CPEM.update();
        i2cpem_next_update_ms = ms + I2CPE_MIN_UPD_TIME_MS;
      }
    }
  }
  #endif

  // Auto-report Temperatures / SD Status
  #if HAS_AUTO_REPORTING
    if (!gcode.autoreport_paused) {
      TERN_(AUTO_REPORT_TEMPERATURES, thermalManager.auto_reporter.tick());
      TERN_(AUTO_REPORT_SD_STATUS, card.auto_reporter.tick());
      TERN_(AUTO_REPORT_POSITION, position_auto_reporter.tick());
    }
  #endif

  // Update the Průša MMU2
  TERN_(HAS_PRUSA_MMU2, mmu2.mmu_loop());

  // Handle Joystick jogging
  TERN_(POLL_JOG, joystick.inject_jog_moves());

  // Direct Stepping
  TERN_(DIRECT_STEPPING, page_manager.write_responses());

  // Update the LVGL interface
  TERN_(HAS_TFT_LVGL_UI, LV_TASK_HANDLER());

  IDLE_DONE:
  TERN_(MARLIN_DEV_MODE, idle_depth--);
  return;
}

/**
 * Kill all activity and lock the machine.
 * After this the machine will need to be reset.
 */
void kill(PGM_P const lcd_error/*=nullptr*/, PGM_P const lcd_component/*=nullptr*/, const bool steppers_off/*=false*/) {
  thermalManager.disable_all_heaters();

  TERN_(HAS_CUTTER, cutter.kill()); // Full cutter shutdown including ISR control

  // Echo the LCD message to serial for extra context
  if (lcd_error) { SERIAL_ECHO_START(); SERIAL_ECHOLNPGM_P(lcd_error); }

  #if HAS_DISPLAY
    ui.kill_screen(lcd_error ?: GET_TEXT(MSG_KILLED), lcd_component ?: NUL_STR);
  #else
    UNUSED(lcd_error); UNUSED(lcd_component);
  #endif

  TERN_(HAS_TFT_LVGL_UI, lv_draw_error_message(lcd_error));

  // "Error:Printer halted. kill() called!"
  SERIAL_ERROR_MSG(STR_ERR_KILLED);

  #ifdef ACTION_ON_KILL
    host_action_kill();
  #endif

  minkill(steppers_off);
}

void minkill(const bool steppers_off/*=false*/) {

  // Wait a short time (allows messages to get out before shutting down.
  for (int i = 1000; i--;) DELAY_US(600);

  cli(); // Stop interrupts

  // Wait to ensure all interrupts stopped
  for (int i = 1000; i--;) DELAY_US(250);

  // Reiterate heaters off
  thermalManager.disable_all_heaters();

  TERN_(HAS_CUTTER, cutter.kill());  // Reiterate cutter shutdown

  // Power off all steppers (for M112) or just the E steppers
  steppers_off ? disable_all_steppers() : disable_e_steppers();

  TERN_(PSU_CONTROL, PSU_OFF());

  TERN_(HAS_SUICIDE, suicide());

  #if EITHER(HAS_KILL, SOFT_RESET_ON_KILL)

    // Wait for both KILL and ENC to be released
    while (TERN0(HAS_KILL, !kill_state()) || TERN0(SOFT_RESET_ON_KILL, !ui.button_pressed()))
      watchdog_refresh();

    // Wait for either KILL or ENC press
    while (TERN1(HAS_KILL, kill_state()) && TERN1(SOFT_RESET_ON_KILL, ui.button_pressed()))
      watchdog_refresh();

    // Reboot the board
    HAL_reboot();

  #else

    for (;;) watchdog_refresh();  // Wait for RESET button or power-cycle

  #endif
}

/**
 * Turn off heaters and stop the print in progress
 * After a stop the machine may be resumed with M999
 */
void stop() {
  thermalManager.disable_all_heaters(); // 'unpause' taken care of in here

  print_job_timer.stop();

  #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
    thermalManager.set_fans_paused(false); // Un-pause fans for safety
  #endif

  if (!IsStopped()) {
    SERIAL_ERROR_MSG(STR_ERR_STOPPED);
    LCD_MESSAGEPGM(MSG_STOPPED);
    safe_delay(350);       // allow enough time for messages to get out before stopping
    marlin_state = MF_STOPPED;
  }
}

inline void tmc_standby_setup() {
  #if PIN_EXISTS(X_STDBY)
    SET_INPUT_PULLDOWN(X_STDBY_PIN);
  #endif
  #if PIN_EXISTS(X2_STDBY)
    SET_INPUT_PULLDOWN(X2_STDBY_PIN);
  #endif
  #if PIN_EXISTS(Y_STDBY)
    SET_INPUT_PULLDOWN(Y_STDBY_PIN);
  #endif
  #if PIN_EXISTS(Y2_STDBY)
    SET_INPUT_PULLDOWN(Y2_STDBY_PIN);
  #endif
  #if PIN_EXISTS(Z_STDBY)
    SET_INPUT_PULLDOWN(Z_STDBY_PIN);
  #endif
  #if PIN_EXISTS(Z2_STDBY)
    SET_INPUT_PULLDOWN(Z2_STDBY_PIN);
  #endif
  #if PIN_EXISTS(Z3_STDBY)
    SET_INPUT_PULLDOWN(Z3_STDBY_PIN);
  #endif
  #if PIN_EXISTS(Z4_STDBY)
    SET_INPUT_PULLDOWN(Z4_STDBY_PIN);
  #endif
  #if PIN_EXISTS(I_STDBY)
    SET_INPUT_PULLDOWN(I_STDBY_PIN);
  #endif
  #if PIN_EXISTS(J_STDBY)
    SET_INPUT_PULLDOWN(J_STDBY_PIN);
  #endif
  #if PIN_EXISTS(K_STDBY)
    SET_INPUT_PULLDOWN(K_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E0_STDBY)
    SET_INPUT_PULLDOWN(E0_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E1_STDBY)
    SET_INPUT_PULLDOWN(E1_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E2_STDBY)
    SET_INPUT_PULLDOWN(E2_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E3_STDBY)
    SET_INPUT_PULLDOWN(E3_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E4_STDBY)
    SET_INPUT_PULLDOWN(E4_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E5_STDBY)
    SET_INPUT_PULLDOWN(E5_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E6_STDBY)
    SET_INPUT_PULLDOWN(E6_STDBY_PIN);
  #endif
  #if PIN_EXISTS(E7_STDBY)
    SET_INPUT_PULLDOWN(E7_STDBY_PIN);
  #endif
}

/**
 * Marlin Firmware entry-point. Abandon Hope All Ye Who Enter Here.
 * Setup before the program loop:
 *
 *  - Call any special pre-init set for the board
 *  - Put TMC drivers into Low Power Standby mode
 *  - Init the serial ports (so setup can be debugged)
 *  - Set up the kill and suicide pins
 *  - Prepare (disable) board JTAG and Debug ports
 *  - Init serial for a connected MKS TFT with WiFi
 *  - Install Marlin custom Exception Handlers, if set.
 *  - Init Marlin's HAL interfaces (for SPI, i2c, etc.)
 *  - Init some optional hardware and features:
 *    • MAX Thermocouple pins
 *    • Duet Smart Effector
 *    • Filament Runout Sensor
 *    • TMC220x Stepper Drivers (Serial)
 *    • PSU control
 *    • Power-loss Recovery
 *    • L64XX Stepper Drivers (SPI)
 *    • Stepper Driver Reset: DISABLE
 *    • TMC Stepper Drivers (SPI)
 *    • Run BOARD_INIT if defined
 *    • ESP WiFi
 *  - Get the Reset Reason and report it
 *  - Print startup messages and diagnostics
 *  - Calibrate the HAL DELAY for precise timing
 *  - Init the buzzer, possibly a custom timer
 *  - Init more optional hardware:
 *    • Color LED illumination
 *    • Neopixel illumination
 *    • Controller Fan
 *    • Creality DWIN LCD (show boot image)
 *    • Tare the Probe if possible
 *  - Mount the (most likely external) SD Card
 *  - Load settings from EEPROM (or use defaults)
 *  - Init the Ethernet Port
 *  - Init Touch Buttons (for emulated DOGLCD)
 *  - Adjust the (certainly wrong) current position by the home offset
 *  - Init the Planner::position (steps) based on current (native) position
 *  - Initialize more managers and peripherals:
 *    • Temperatures
 *    • Print Job Timer
 *    • Endstops and Endstop Interrupts
 *    • Stepper ISR - Kind of Important!
 *    • Servos
 *    • Servo-based Probe
 *    • Photograph Pin
 *    • Laser/Spindle tool Power / PWM
 *    • Coolant Control
 *    • Bed Probe
 *    • Stepper Driver Reset: ENABLE
 *    • Digipot I2C - Stepper driver current control
 *    • Stepper DAC - Stepper driver current control
 *    • Solenoid (probe, or for other use)
 *    • Home Pin
 *    • Custom User Buttons
 *    • Red/Blue Status LEDs
 *    • Case Light
 *    • Prusa MMU filament changer
 *    • Fan Multiplexer
 *    • Mixing Extruder
 *    • BLTouch Probe
 *    • I2C Position Encoders
 *    • Custom I2C Bus handlers
 *    • Enhanced tools or extruders:
 *      • Switching Extruder
 *      • Switching Nozzle
 *      • Parking Extruder
 *      • Magnetic Parking Extruder
 *      • Switching Toolhead
 *      • Electromagnetic Switching Toolhead
 *    • Watchdog Timer - Also Kind of Important!
 *    • Closed Loop Controller
 *  - Run Startup Commands, if defined
 *  - Tell host to close Host Prompts
 *  - Test Trinamic driver connections
 *  - Init Prusa MMU2 filament changer
 *  - Init and test BL24Cxx EEPROM
 *  - Init Creality DWIN encoder, show faux progress bar
 *  - Reset Status Message / Show Service Messages
 *  - Init MAX7219 LED Matrix
 *  - Init Direct Stepping (Klipper-style motion control)
 *  - Init TFT LVGL UI (with 3D Graphics)
 *  - Apply Password Lock - Hold for Authentication
 *  - Open Touch Screen Calibration screen, if not calibrated
 *  - Set Marlin to RUNNING State
 */
void setup() {
  #ifdef BOARD_PREINIT
    BOARD_PREINIT(); // Low-level init (before serial init)
  #endif

  tmc_standby_setup();  // TMC Low Power Standby pins must be set early or they're not usable

  #if ENABLED(MARLIN_DEV_MODE)
    auto log_current_ms = [&](PGM_P const msg) {
      SERIAL_ECHO_START();
      SERIAL_CHAR('['); SERIAL_ECHO(millis()); SERIAL_ECHOPGM("] ");
      SERIAL_ECHOLNPGM_P(msg);
    };
    #define SETUP_LOG(M) log_current_ms(PSTR(M))
  #else
    #define SETUP_LOG(...) NOOP
  #endif
  #define SETUP_RUN(C) do{ SETUP_LOG(STRINGIFY(C)); C; }while(0)

  MYSERIAL1.begin(BAUDRATE);
  millis_t serial_connect_timeout = millis() + 1000UL;
  while (!MYSERIAL1.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }

  #if HAS_MULTI_SERIAL && !HAS_ETHERNET
    #ifndef BAUDRATE_2
      #define BAUDRATE_2 BAUDRATE
    #endif
    MYSERIAL2.begin(BAUDRATE_2);
    serial_connect_timeout = millis() + 1000UL;
    while (!MYSERIAL2.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
    #ifdef SERIAL_PORT_3
      #ifndef BAUDRATE_3
        #define BAUDRATE_3 BAUDRATE
      #endif
      MYSERIAL3.begin(BAUDRATE_3);
      serial_connect_timeout = millis() + 1000UL;
      while (!MYSERIAL3.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
    #endif
  #endif
  SERIAL_ECHOLNPGM("start");

  // Set up these pins early to prevent suicide
  #if HAS_KILL
    SETUP_LOG("KILL_PIN");
    #if KILL_PIN_STATE
      SET_INPUT_PULLDOWN(KILL_PIN);
    #else
      SET_INPUT_PULLUP(KILL_PIN);
    #endif
  #endif

  #if HAS_FREEZE_PIN
    SETUP_LOG("FREEZE_PIN");
    SET_INPUT_PULLUP(FREEZE_PIN);
  #endif

  #if HAS_SUICIDE
    SETUP_LOG("SUICIDE_PIN");
    OUT_WRITE(SUICIDE_PIN, !SUICIDE_PIN_INVERTING);
  #endif

  #ifdef JTAGSWD_RESET
    SETUP_LOG("JTAGSWD_RESET");
    JTAGSWD_RESET();
  #endif

  #if EITHER(DISABLE_DEBUG, DISABLE_JTAG)
    delay(10);
    // Disable any hardware debug to free up pins for IO
    #if ENABLED(DISABLE_DEBUG) && defined(JTAGSWD_DISABLE)
      SETUP_LOG("JTAGSWD_DISABLE");
      JTAGSWD_DISABLE();
    #elif defined(JTAG_DISABLE)
      SETUP_LOG("JTAG_DISABLE");
      JTAG_DISABLE();
    #else
      #error "DISABLE_(DEBUG|JTAG) is not supported for the selected MCU/Board."
    #endif
  #endif

  #if BOTH(HAS_TFT_LVGL_UI, MKS_WIFI_MODULE)
    mks_esp_wifi_init();
    WIFISERIAL.begin(WIFI_BAUDRATE);
    serial_connect_timeout = millis() + 1000UL;
    while (/*!WIFISERIAL && */PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
  #endif

  TERN_(DYNAMIC_VECTORTABLE, hook_cpu_exceptions()); // If supported, install Marlin exception handlers at runtime

  SETUP_RUN(HAL_init());

  // Init and disable SPI thermocouples; this is still needed
  #if TEMP_SENSOR_0_IS_MAX_TC || (TEMP_SENSOR_REDUNDANT_IS_MAX_TC && TEMP_SENSOR_REDUNDANT_SOURCE == 0)
    OUT_WRITE(MAX6675_SS_PIN, HIGH);  // Disable
  #endif
  #if TEMP_SENSOR_1_IS_MAX_TC || (TEMP_SENSOR_REDUNDANT_IS_MAX_TC && TEMP_SENSOR_REDUNDANT_SOURCE == 1)
    OUT_WRITE(MAX6675_SS2_PIN, HIGH); // Disable
  #endif

  #if ENABLED(DUET_SMART_EFFECTOR) && PIN_EXISTS(SMART_EFFECTOR_MOD)
    OUT_WRITE(SMART_EFFECTOR_MOD_PIN, LOW);   // Put Smart Effector into NORMAL mode
  #endif

  #if HAS_FILAMENT_SENSOR
    SETUP_RUN(runout.setup());
  #endif

  #if HAS_TMC220x
    SETUP_RUN(tmc_serial_begin());
  #endif

  #if ENABLED(PSU_CONTROL)
    SETUP_LOG("PSU_CONTROL");
    powersupply_on = ENABLED(PSU_DEFAULT_OFF);
    if (ENABLED(PSU_DEFAULT_OFF)) PSU_OFF(); else PSU_ON();
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    SETUP_RUN(recovery.setup());
  #endif

  #if HAS_L64XX
    SETUP_RUN(L64xxManager.init());  // Set up SPI, init drivers
  #endif

  #if HAS_STEPPER_RESET
    SETUP_RUN(disableStepperDrivers());
  #endif

  #if HAS_TMC_SPI
    #if DISABLED(TMC_USE_SW_SPI)
      SETUP_RUN(SPI.begin());
    #endif
    SETUP_RUN(tmc_init_cs_pins());
  #endif

  #ifdef BOARD_INIT
    SETUP_LOG("BOARD_INIT");
    BOARD_INIT();
  #endif

  SETUP_RUN(esp_wifi_init());

  // Check startup - does nothing if bootloader sets MCUSR to 0
  const byte mcu = HAL_get_reset_source();
  if (mcu & RST_POWER_ON) SERIAL_ECHOLNPGM(STR_POWERUP);
  if (mcu & RST_EXTERNAL) SERIAL_ECHOLNPGM(STR_EXTERNAL_RESET);
  if (mcu & RST_BROWN_OUT) SERIAL_ECHOLNPGM(STR_BROWNOUT_RESET);
  if (mcu & RST_WATCHDOG) SERIAL_ECHOLNPGM(STR_WATCHDOG_RESET);
  if (mcu & RST_SOFTWARE) SERIAL_ECHOLNPGM(STR_SOFTWARE_RESET);
  HAL_clear_reset_source();

  SERIAL_ECHOPGM_P(GET_TEXT(MSG_MARLIN));
  SERIAL_CHAR(' ');
  SERIAL_ECHOLNPGM(SHORT_BUILD_VERSION);
  SERIAL_EOL();
  #if defined(STRING_DISTRIBUTION_DATE) && defined(STRING_CONFIG_H_AUTHOR)
    SERIAL_ECHO_MSG(
      " Last Updated: " STRING_DISTRIBUTION_DATE
      " | Author: " STRING_CONFIG_H_AUTHOR
    );
  #endif
  SERIAL_ECHO_MSG("Compiled: " __DATE__);
  SERIAL_ECHO_MSG(STR_FREE_MEMORY, freeMemory(), STR_PLANNER_BUFFER_BYTES, sizeof(block_t) * (BLOCK_BUFFER_SIZE));

  // Some HAL need precise delay adjustment
  calibrate_delay_loop();

  // Init buzzer pin(s)
  #if USE_BEEPER
    SETUP_RUN(buzzer.init());
  #endif

  // Set up LEDs early
  #if HAS_COLOR_LEDS
    SETUP_RUN(leds.setup());
  #endif

  #if ENABLED(NEOPIXEL2_SEPARATE)
    SETUP_RUN(leds2.setup());
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)     // Set up fan controller to initialize also the default configurations.
    SETUP_RUN(controllerFan.setup());
  #endif

  // UI must be initialized before EEPROM
  // (because EEPROM code calls the UI).

  #if ENABLED(DWIN_CREALITY_LCD)
    delay(800);   // Required delay (since boot?)
    SERIAL_ECHOPGM("\nDWIN handshake ");
    if (DWIN_Handshake()) SERIAL_ECHOLNPGM("ok."); else SERIAL_ECHOLNPGM("error.");
    DWIN_Frame_SetDir(1); // Orientation 90°
    DWIN_UpdateLCD();     // Show bootscreen (first image)
  #else
    SETUP_RUN(ui.init());
    #if BOTH(HAS_WIRED_LCD, SHOW_BOOTSCREEN)
      SETUP_RUN(ui.show_bootscreen());
      const millis_t bootscreen_ms = millis();
    #endif
    SETUP_RUN(ui.reset_status());     // Load welcome message early. (Retained if no errors exist.)
  #endif

  #if PIN_EXISTS(SAFE_POWER)
    #if HAS_DRIVER_SAFE_POWER_PROTECT
      SETUP_RUN(stepper_driver_backward_check());
    #else
      SETUP_LOG("SAFE_POWER");
      OUT_WRITE(SAFE_POWER_PIN, HIGH);
    #endif
  #endif

  #if ENABLED(PROBE_TARE)
    SETUP_RUN(probe.tare_init());
  #endif

  #if BOTH(SDSUPPORT, SDCARD_EEPROM_EMULATION)
    SETUP_RUN(card.mount());          // Mount media with settings before first_load
  #endif

  SETUP_RUN(settings.first_load());   // Load data from EEPROM if available (or use defaults)
                                      // This also updates variables in the planner, elsewhere

  #if HAS_ETHERNET
    SETUP_RUN(ethernet.init());
  #endif

  #if HAS_TOUCH_BUTTONS
    SETUP_RUN(touch.init());
  #endif

  TERN_(HAS_M206_COMMAND, current_position += home_offset); // Init current position based on home_offset

  sync_plan_position();               // Vital to init stepper/planner equivalent for current_position

  SETUP_RUN(thermalManager.init());   // Initialize temperature loop

  SETUP_RUN(print_job_timer.init());  // Initial setup of print job timer

  SETUP_RUN(endstops.init());         // Init endstops and pullups

  SETUP_RUN(stepper.init());          // Init stepper. This enables interrupts!

  #if HAS_SERVOS
    SETUP_RUN(servo_init());
  #endif

  #if HAS_Z_SERVO_PROBE
    SETUP_RUN(probe.servo_probe_init());
  #endif

  #if HAS_PHOTOGRAPH
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif

  #if HAS_CUTTER
    SETUP_RUN(cutter.init());
  #endif

  #if ENABLED(COOLANT_MIST)
    OUT_WRITE(COOLANT_MIST_PIN, COOLANT_MIST_INVERT);   // Init Mist Coolant OFF
  #endif
  #if ENABLED(COOLANT_FLOOD)
    OUT_WRITE(COOLANT_FLOOD_PIN, COOLANT_FLOOD_INVERT); // Init Flood Coolant OFF
  #endif

  #if HAS_BED_PROBE
    SETUP_RUN(endstops.enable_z_probe(false));
  #endif

  #if HAS_STEPPER_RESET
    SETUP_RUN(enableStepperDrivers());
  #endif

  #if HAS_MOTOR_CURRENT_I2C
    SETUP_RUN(digipot_i2c.init());
  #endif

  #if ENABLED(HAS_MOTOR_CURRENT_DAC)
    SETUP_RUN(stepper_dac.init());
  #endif

  #if EITHER(Z_PROBE_SLED, SOLENOID_PROBE) && HAS_SOLENOID_1
    OUT_WRITE(SOL1_PIN, LOW); // OFF
  #endif

  #if HAS_HOME
    SET_INPUT_PULLUP(HOME_PIN);
  #endif

  #if ENABLED(CUSTOM_USER_BUTTONS)
    #define INIT_CUSTOM_USER_BUTTON_PIN(N) do{ SET_INPUT(BUTTON##N##_PIN); WRITE(BUTTON##N##_PIN, !BUTTON##N##_HIT_STATE); }while(0)

    #if HAS_CUSTOM_USER_BUTTON(1)
      INIT_CUSTOM_USER_BUTTON_PIN(1);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(2)
      INIT_CUSTOM_USER_BUTTON_PIN(2);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(3)
      INIT_CUSTOM_USER_BUTTON_PIN(3);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(4)
      INIT_CUSTOM_USER_BUTTON_PIN(4);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(5)
      INIT_CUSTOM_USER_BUTTON_PIN(5);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(6)
      INIT_CUSTOM_USER_BUTTON_PIN(6);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(7)
      INIT_CUSTOM_USER_BUTTON_PIN(7);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(8)
      INIT_CUSTOM_USER_BUTTON_PIN(8);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(9)
      INIT_CUSTOM_USER_BUTTON_PIN(9);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(10)
      INIT_CUSTOM_USER_BUTTON_PIN(10);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(11)
      INIT_CUSTOM_USER_BUTTON_PIN(11);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(12)
      INIT_CUSTOM_USER_BUTTON_PIN(12);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(13)
      INIT_CUSTOM_USER_BUTTON_PIN(13);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(14)
      INIT_CUSTOM_USER_BUTTON_PIN(14);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(15)
      INIT_CUSTOM_USER_BUTTON_PIN(15);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(16)
      INIT_CUSTOM_USER_BUTTON_PIN(16);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(17)
      INIT_CUSTOM_USER_BUTTON_PIN(17);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(18)
      INIT_CUSTOM_USER_BUTTON_PIN(18);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(19)
      INIT_CUSTOM_USER_BUTTON_PIN(19);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(20)
      INIT_CUSTOM_USER_BUTTON_PIN(20);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(21)
      INIT_CUSTOM_USER_BUTTON_PIN(21);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(22)
      INIT_CUSTOM_USER_BUTTON_PIN(22);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(23)
      INIT_CUSTOM_USER_BUTTON_PIN(23);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(24)
      INIT_CUSTOM_USER_BUTTON_PIN(24);
    #endif
    #if HAS_CUSTOM_USER_BUTTON(25)
      INIT_CUSTOM_USER_BUTTON_PIN(25);
    #endif
  #endif

  #if PIN_EXISTS(STAT_LED_RED)
    OUT_WRITE(STAT_LED_RED_PIN, LOW); // OFF
  #endif
  #if PIN_EXISTS(STAT_LED_BLUE)
    OUT_WRITE(STAT_LED_BLUE_PIN, LOW); // OFF
  #endif

  #if ENABLED(CASE_LIGHT_ENABLE)
    SETUP_RUN(caselight.init());
  #endif

  #if HAS_PRUSA_MMU1
    SETUP_RUN(mmu_init());
  #endif

  #if HAS_FANMUX
    SETUP_RUN(fanmux_init());
  #endif

  #if ENABLED(MIXING_EXTRUDER)
    SETUP_RUN(mixer.init());
  #endif

  #if ENABLED(BLTOUCH)
    SETUP_RUN(bltouch.init(/*set_voltage=*/true));
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    SETUP_RUN(I2CPEM.init());
  #endif

  #if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0
    SETUP_LOG("i2c...");
    i2c.onReceive(i2c_on_receive);
    i2c.onRequest(i2c_on_request);
  #endif

  #if DO_SWITCH_EXTRUDER
    SETUP_RUN(move_extruder_servo(0));  // Initialize extruder servo
  #endif

  #if ENABLED(SWITCHING_NOZZLE)
    SETUP_LOG("SWITCHING_NOZZLE");
    // Initialize nozzle servo(s)
    #if SWITCHING_NOZZLE_TWO_SERVOS
      lower_nozzle(0);
      raise_nozzle(1);
    #else
      move_nozzle_servo(0);
    #endif
  #endif

  #if ENABLED(PARKING_EXTRUDER)
    SETUP_RUN(pe_solenoid_init());
  #elif ENABLED(MAGNETIC_PARKING_EXTRUDER)
    SETUP_RUN(mpe_settings_init());
  #elif ENABLED(SWITCHING_TOOLHEAD)
    SETUP_RUN(swt_init());
  #elif ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
    SETUP_RUN(est_init());
  #endif

  #if ENABLED(USE_WATCHDOG)
    SETUP_RUN(watchdog_init());       // Reinit watchdog after HAL_get_reset_source call
  #endif

  #if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
    SETUP_RUN(closedloop.init());
  #endif

  #ifdef STARTUP_COMMANDS
    SETUP_LOG("STARTUP_COMMANDS");
    queue.inject_P(PSTR(STARTUP_COMMANDS));
  #endif

  #if ENABLED(HOST_PROMPT_SUPPORT)
    SETUP_RUN(host_action_prompt_end());
  #endif

  #if HAS_TRINAMIC_CONFIG && DISABLED(PSU_DEFAULT_OFF)
    SETUP_RUN(test_tmc_connection());
  #endif

  #if HAS_DRIVER_SAFE_POWER_PROTECT
    SETUP_RUN(stepper_driver_backward_report());
  #endif

  #if HAS_PRUSA_MMU2
    SETUP_RUN(mmu2.init());
  #endif

  #if ENABLED(IIC_BL24CXX_EEPROM)
    BL24CXX::init();
    const uint8_t err = BL24CXX::check();
    SERIAL_ECHO_TERNARY(err, "BL24CXX Check ", "failed", "succeeded", "!\n");
  #endif

  #if ENABLED(DWIN_CREALITY_LCD)
    Encoder_Configuration();
    HMI_Init();
    DWIN_JPG_CacheTo1(Language_English);
    HMI_StartFrame(true);
    DWIN_StatusChanged(GET_TEXT(WELCOME_MSG));
  #endif

  #if HAS_SERVICE_INTERVALS && DISABLED(DWIN_CREALITY_LCD)
    ui.reset_status(true);  // Show service messages or keep current status
  #endif

  #if ENABLED(MAX7219_DEBUG)
    SETUP_RUN(max7219.init());
  #endif

  #if ENABLED(DIRECT_STEPPING)
    SETUP_RUN(page_manager.init());
  #endif

  #if HAS_TFT_LVGL_UI
    #if ENABLED(SDSUPPORT)
      if (!card.isMounted()) SETUP_RUN(card.mount()); // Mount SD to load graphics and fonts
    #endif
    SETUP_RUN(tft_lvgl_init());
  #endif

  #if BOTH(HAS_WIRED_LCD, SHOW_BOOTSCREEN)
    const millis_t elapsed = millis() - bootscreen_ms;
    #if ENABLED(MARLIN_DEV_MODE)
      SERIAL_ECHOLNPAIR("elapsed=", elapsed);
    #endif
    SETUP_RUN(ui.bootscreen_completion(elapsed));
  #endif

  #if ENABLED(PASSWORD_ON_STARTUP)
    SETUP_RUN(password.lock_machine());      // Will not proceed until correct password provided
  #endif

  #if BOTH(HAS_LCD_MENU, TOUCH_SCREEN_CALIBRATION) && EITHER(TFT_CLASSIC_UI, TFT_COLOR_UI)
    ui.check_touch_calibration();
  #endif

  marlin_state = MF_RUNNING;

  SETUP_LOG("setup() completed.");
}

/**
 * The main Marlin program loop
 *
 *  - Call idle() to handle all tasks between G-code commands
 *      Note that no G-codes from the queue can be executed during idle()
 *      but many G-codes can be called directly anytime like macros.
 *  - Check whether SD card auto-start is needed now.
 *  - Check whether SD print finishing is needed now.
 *  - Run one G-code command from the immediate or main command queue
 *    and open up one space. Commands in the main queue may come from sd
 *    card, host, or by direct injection. The queue will continue to fill
 *    as long as idle() or manage_inactivity() are being called.
 */
void loop() {
  do {
    idle();

    #if ENABLED(SDSUPPORT)
      if (card.flag.abort_sd_printing) abortSDPrinting();
      if (marlin_state == MF_SD_COMPLETE) finishSDPrinting();
    #endif

    queue.advance();

    endstops.event_handler();

    TERN_(HAS_TFT_LVGL_UI, printer_state_polling());

  } while (ENABLED(__AVR__)); // Loop forever on slower (AVR) boards
}

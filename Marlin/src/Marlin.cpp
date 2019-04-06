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
 * About Marlin
 *
 * This firmware is a mashup between Sprinter and grbl.
 *  - https://github.com/kliment/Sprinter
 *  - https://github.com/grbl/grbl
 */

#include "Marlin.h"

#include "core/utility.h"
#include "lcd/ultralcd.h"
#include "module/motion.h"
#include "module/planner.h"
#include "module/stepper.h"
#include "module/endstops.h"
#include "module/probe.h"
#include "module/temperature.h"
#include "sd/cardreader.h"
#include "module/configuration_store.h"
#include "module/printcounter.h" // PrintCounter or Stopwatch
#include "feature/closedloop.h"

#include "HAL/shared/Delay.h"

#include "module/stepper_indirection.h"

#ifdef ARDUINO
  #include <pins_arduino.h>
#endif
#include <math.h>
#include "libs/nozzle.h"

#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "gcode/queue.h"

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "feature/host_actions.h"
#endif

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "libs/buzzer.h"
#endif

#if ENABLED(DIGIPOT_I2C)
  #include "feature/digipot/digipot.h"
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "feature/mixing.h"
#endif

#if ENABLED(BEZIER_CURVE_SUPPORT)
  #include "module/planner_bezier.h"
#endif

#if ENABLED(MAX7219_DEBUG)
  #include "feature/Max7219_Debug_LEDs.h"
#endif

#if HAS_COLOR_LEDS
  #include "feature/leds/leds.h"
#endif

#if ENABLED(BLTOUCH)
  #include "feature/bltouch.h"
#endif

#if HAS_SERVOS
  #include "module/servo.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  #include "feature/dac/stepper_dac.h"
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "feature/twibus.h"
  TWIBus i2c;
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  #include "feature/I2CPositionEncoder.h"
#endif

#if HAS_TRINAMIC
  #include "feature/tmc_util.h"
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

#if BOTH(ADVANCED_PAUSE_FEATURE, PAUSE_PARK_NO_STEPPER_TIMEOUT)
  #include "feature/pause.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "feature/power_loss_recovery.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "feature/runout.h"
#endif

#if ENABLED(TEMP_STAT_LEDS)
  #include "feature/leds/tempstat.h"
#endif

#if HAS_CASE_LIGHT
  #include "feature/caselight.h"
#endif

#if HAS_FANMUX
  #include "feature/fanmux.h"
#endif

#if DO_SWITCH_EXTRUDER || ANY(SWITCHING_NOZZLE, PARKING_EXTRUDER, MAGNETIC_PARKING_EXTRUDER)
  #include "module/tool_change.h"
#endif

#if ENABLED(USE_CONTROLLER_FAN)
  #include "feature/controllerfan.h"
#endif

#if ENABLED(PRUSA_MMU2)
  #include "feature/prusa_MMU2/mmu2.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "lcd/extensible_ui/ui_api.h"
#endif

#if HAS_DRIVER(L6470)
  #include "libs/L6470/L6470_Marlin.h"
#endif

bool Running = true;

#if ENABLED(TEMPERATURE_UNITS_SUPPORT)
  TempUnit input_temp_units = TEMPUNIT_C;
#endif

// For M109 and M190, this flag may be cleared (by M108) to exit the wait loop
bool wait_for_heatup = true;

// For M0/M1, this flag may be cleared (by M108) to exit the wait-for-user loop
#if HAS_RESUME_CONTINUE
  bool wait_for_user; // = false;
#endif

#if HAS_AUTO_REPORTING || ENABLED(HOST_KEEPALIVE_FEATURE)
  bool suspend_auto_report; // = false
#endif

// Inactivity shutdown
millis_t max_inactive_time, // = 0
         stepper_inactive_time = (DEFAULT_STEPPER_DEACTIVE_TIME) * 1000UL;

#if PIN_EXISTS(CHDK)
  extern millis_t chdk_timeout;
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  I2CPositionEncodersMgr I2CPEM;
#endif

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */

void setup_killpin() {
  #if HAS_KILL
    SET_INPUT_PULLUP(KILL_PIN);
  #endif
}

void setup_powerhold() {
  #if HAS_SUICIDE
    OUT_WRITE(SUICIDE_PIN, HIGH);
  #endif
  #if HAS_POWER_SWITCH
    #if ENABLED(PS_DEFAULT_OFF)
      powersupply_on = true;  PSU_OFF();
    #else
      powersupply_on = false; PSU_ON();
    #endif
  #endif
}

/**
 * Stepper Reset (RigidBoard, et.al.)
 */
#if HAS_STEPPER_RESET
  void disableStepperDrivers() { OUT_WRITE(STEPPER_RESET_PIN, LOW); } // Drive down to keep motor driver chips in reset
  void enableStepperDrivers()  { SET_INPUT(STEPPER_RESET_PIN); }      // Set to input, allowing pullups to pull the pin high
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0

  void i2c_on_receive(int bytes) { // just echo all bytes received to serial
    i2c.receive(bytes);
  }

  void i2c_on_request() {          // just send dummy data for now
    i2c.reply("Hello World!\n");
  }

#endif

/**
 * Sensitive pin test for M42, M226
 */

#include "pins/sensitive_pins.h"

bool pin_is_protected(const pin_t pin) {
  static const pin_t sensitive_pins[] PROGMEM = SENSITIVE_PINS;
  for (uint8_t i = 0; i < COUNT(sensitive_pins); i++) {
    pin_t sensitive_pin;
    memcpy_P(&sensitive_pin, &sensitive_pins[i], sizeof(pin_t));
    if (pin == sensitive_pin) return true;
  }
  return false;
}

void protected_pin_err() {
  SERIAL_ERROR_MSG(MSG_ERR_PROTECTED_PIN);
}

void quickstop_stepper() {
  planner.quick_stop();
  planner.synchronize();
  set_current_from_steppers_for_axis(ALL_AXES);
  sync_plan_position();
}

void enable_all_steppers() {
  #if ENABLED(AUTO_POWER_CONTROL)
    powerManager.power_on();
  #endif
  enable_X();
  enable_Y();
  enable_Z();
  enable_E0();
  enable_E1();
  enable_E2();
  enable_E3();
  enable_E4();
  enable_E5();
}

void disable_e_steppers() {
  disable_E0();
  disable_E1();
  disable_E2();
  disable_E3();
  disable_E4();
  disable_E5();
}

void disable_e_stepper(const uint8_t e) {
  switch (e) {
    case 0: disable_E0(); break;
    case 1: disable_E1(); break;
    case 2: disable_E2(); break;
    case 3: disable_E3(); break;
    case 4: disable_E4(); break;
    case 5: disable_E5(); break;
  }
}

void disable_all_steppers() {
  disable_X();
  disable_Y();
  disable_Z();
  disable_e_steppers();
}

#if HAS_FILAMENT_SENSOR

  void event_filament_runout() {

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      if (did_pause_print) return;  // Action already in progress. Purge triggered repeated runout.
    #endif

    #if ENABLED(EXTENSIBLE_UI)
      ExtUI::onFilamentRunout(ExtUI::getActiveTool());
    #endif

    #if EITHER(HOST_PROMPT_SUPPORT, HOST_ACTION_COMMANDS)
      const char tool = '0'
        #if NUM_RUNOUT_SENSORS > 1
          + active_extruder
        #endif
      ;
    #endif

    //action:out_of_filament
    #if ENABLED(HOST_PROMPT_SUPPORT)
      host_prompt_reason = PROMPT_FILAMENT_RUNOUT;
      host_action_prompt_end();
      host_action_prompt_begin(PSTR("FilamentRunout T"), false);
      SERIAL_CHAR(tool);
      SERIAL_EOL();
      host_action_prompt_show();
    #endif

    const bool run_runout_script = !runout.host_handling;

    #if ENABLED(HOST_ACTION_COMMANDS)
      if (run_runout_script
        && ( strstr(FILAMENT_RUNOUT_SCRIPT, "M600")
          || strstr(FILAMENT_RUNOUT_SCRIPT, "M125")
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            || strstr(FILAMENT_RUNOUT_SCRIPT, "M25")
          #endif
        )
      ) {
        host_action_paused(false);
      }
      else {
        // Legacy Repetier command for use until newer version supports standard dialog
        // To be removed later when pause command also triggers dialog
        #ifdef ACTION_ON_FILAMENT_RUNOUT
          host_action(PSTR(ACTION_ON_FILAMENT_RUNOUT " T"), false);
          SERIAL_CHAR(tool);
          SERIAL_EOL();
        #endif

        host_action_pause(false);
      }
      SERIAL_ECHOPGM(" " ACTION_REASON_ON_FILAMENT_RUNOUT " ");
      SERIAL_CHAR(tool);
      SERIAL_EOL();
    #endif // HOST_ACTION_COMMANDS

    if (run_runout_script)
      enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
  }

#endif // HAS_FILAMENT_SENSOR

#if ENABLED(G29_RETRY_AND_RECOVER)

  void event_probe_failure() {
    #ifdef ACTION_ON_G29_FAILURE
      host_action(PSTR(ACTION_ON_G29_FAILURE));
    #endif
    #ifdef G29_FAILURE_COMMANDS
      gcode.process_subcommands_now_P(PSTR(G29_FAILURE_COMMANDS));
    #endif
    #if ENABLED(G29_HALT_ON_FAILURE)
      #ifdef ACTION_ON_CANCEL
        host_action_cancel();
      #endif
      kill(PSTR(MSG_ERR_PROBING_FAILED));
    #endif
  }

  void event_probe_recover() {
    #if ENABLED(HOST_PROMPT_SUPPORT)
      host_prompt_do(PROMPT_INFO, PSTR("G29 Retrying"));
    #endif
    #ifdef ACTION_ON_G29_RECOVER
      host_action(PSTR(ACTION_ON_G29_RECOVER));
    #endif
    #ifdef G29_RECOVER_COMMANDS
      gcode.process_subcommands_now_P(PSTR(G29_RECOVER_COMMANDS));
    #endif
  }

#endif

/**
 * Manage several activities:
 *  - Check for Filament Runout
 *  - Keep the command buffer full
 *  - Check for maximum inactive time between commands
 *  - Check for maximum inactive time between stepper commands
 *  - Check if CHDK_PIN needs to go LOW
 *  - Check for KILL button held down
 *  - Check for HOME button held down
 *  - Check if cooling fan needs to be switched on
 *  - Check if an idle but hot extruder needs filament extruded (EXTRUDER_RUNOUT_PREVENT)
 *  - Pulse FET_SAFETY_PIN if it exists
 */

void manage_inactivity(const bool ignore_stepper_queue/*=false*/) {

  #if HAS_FILAMENT_SENSOR
    runout.run();
  #endif

  if (commands_in_queue < BUFSIZE) get_available_commands();

  const millis_t ms = millis();

  if (max_inactive_time && ELAPSED(ms, gcode.previous_move_ms + max_inactive_time)) {
    SERIAL_ERROR_START();
    SERIAL_ECHOLNPAIR(MSG_KILL_INACTIVE_TIME, parser.command_ptr);
    kill();
  }

  // Prevent steppers timing-out in the middle of M600
  #if BOTH(ADVANCED_PAUSE_FEATURE, PAUSE_PARK_NO_STEPPER_TIMEOUT)
    #define MOVE_AWAY_TEST !did_pause_print
  #else
    #define MOVE_AWAY_TEST true
  #endif

  if (stepper_inactive_time) {
    static bool already_shutdown_steppers; // = false
    if (planner.has_blocks_queued())
      gcode.previous_move_ms = ms; // reset_stepper_timeout to keep steppers powered
    else if (MOVE_AWAY_TEST && !ignore_stepper_queue && ELAPSED(ms, gcode.previous_move_ms + stepper_inactive_time)) {
      if (!already_shutdown_steppers) {
        already_shutdown_steppers = true;  // L6470 SPI will consume 99% of free time without this
        #if ENABLED(DISABLE_INACTIVE_X)
          disable_X();
        #endif
        #if ENABLED(DISABLE_INACTIVE_Y)
          disable_Y();
        #endif
        #if ENABLED(DISABLE_INACTIVE_Z)
          disable_Z();
        #endif
        #if ENABLED(DISABLE_INACTIVE_E)
          disable_e_steppers();
        #endif
        #if HAS_LCD_MENU
          ui.status_screen();
          #if ENABLED(AUTO_BED_LEVELING_UBL)
            if (ubl.lcd_map_control) {
              ubl.lcd_map_control = false;
              ui.defer_status_screen(false);
            }
          #endif
        #endif
      }
    }
    else
      already_shutdown_steppers = false;
  }

  #if PIN_EXISTS(CHDK) // Check if pin should be set to LOW (after M240 set it HIGH)
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
    if (!READ(KILL_PIN))
      killCount++;
    else if (killCount > 0)
      killCount--;

    // Exceeded threshold and we can confirm that it was not accidental
    // KILL the machine
    // ----------------------------------------------------------------
    if (killCount >= KILL_DELAY) {
      SERIAL_ERROR_MSG(MSG_KILL_BUTTON);
      kill();
    }
  #endif

  #if HAS_HOME
    // Check to see if we have to home, use poor man's debouncer
    // ---------------------------------------------------------
    static int homeDebounceCount = 0;   // poor man's debouncing count
    const int HOME_DEBOUNCE_DELAY = 2500;
    if (!IS_SD_PRINTING() && !READ(HOME_PIN)) {
      if (!homeDebounceCount) {
        enqueue_and_echo_commands_P(PSTR("G28"));
        LCD_MESSAGEPGM(MSG_AUTO_HOME);
      }
      if (homeDebounceCount < HOME_DEBOUNCE_DELAY)
        homeDebounceCount++;
      else
        homeDebounceCount = 0;
    }
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)
    controllerfan_update(); // Check if fan should be turned on to cool stepper drivers down
  #endif

  #if ENABLED(AUTO_POWER_CONTROL)
    powerManager.check();
  #endif

  #if ENABLED(EXTRUDER_RUNOUT_PREVENT)
    if (thermalManager.degHotend(active_extruder) > EXTRUDER_RUNOUT_MINTEMP
      && ELAPSED(ms, gcode.previous_move_ms + (EXTRUDER_RUNOUT_SECONDS) * 1000UL)
      && !planner.has_blocks_queued()
    ) {
      #if ENABLED(SWITCHING_EXTRUDER)
        bool oldstatus;
        switch (active_extruder) {
          default: oldstatus = E0_ENABLE_READ; enable_E0(); break;
          #if E_STEPPERS > 1
            case 2: case 3: oldstatus = E1_ENABLE_READ; enable_E1(); break;
            #if E_STEPPERS > 2
              case 4: case 5: oldstatus = E2_ENABLE_READ; enable_E2(); break;
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #else // !SWITCHING_EXTRUDER
        bool oldstatus;
        switch (active_extruder) {
          default: oldstatus = E0_ENABLE_READ; enable_E0(); break;
          #if E_STEPPERS > 1
            case 1: oldstatus = E1_ENABLE_READ; enable_E1(); break;
            #if E_STEPPERS > 2
              case 2: oldstatus = E2_ENABLE_READ; enable_E2(); break;
              #if E_STEPPERS > 3
                case 3: oldstatus = E3_ENABLE_READ; enable_E3(); break;
                #if E_STEPPERS > 4
                  case 4: oldstatus = E4_ENABLE_READ; enable_E4(); break;
                  #if E_STEPPERS > 5
                    case 5: oldstatus = E5_ENABLE_READ; enable_E5(); break;
                  #endif // E_STEPPERS > 5
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #endif // !SWITCHING_EXTRUDER

      const float olde = current_position[E_AXIS];
      current_position[E_AXIS] += EXTRUDER_RUNOUT_EXTRUDE;
      planner.buffer_line(current_position, MMM_TO_MMS(EXTRUDER_RUNOUT_SPEED), active_extruder);
      current_position[E_AXIS] = olde;
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
          case 0: E0_ENABLE_WRITE(oldstatus); break;
          #if E_STEPPERS > 1
            case 1: E1_ENABLE_WRITE(oldstatus); break;
            #if E_STEPPERS > 2
              case 2: E2_ENABLE_WRITE(oldstatus); break;
              #if E_STEPPERS > 3
                case 3: E3_ENABLE_WRITE(oldstatus); break;
                #if E_STEPPERS > 4
                  case 4: E4_ENABLE_WRITE(oldstatus); break;
                  #if E_STEPPERS > 5
                    case 5: E5_ENABLE_WRITE(oldstatus); break;
                  #endif // E_STEPPERS > 5
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #endif // !SWITCHING_EXTRUDER

      gcode.previous_move_ms = ms; // reset_stepper_timeout to keep steppers powered
    }
  #endif // EXTRUDER_RUNOUT_PREVENT

  #if ENABLED(DUAL_X_CARRIAGE)
    // handle delayed move timeout
    if (delayed_move_time && ELAPSED(ms, delayed_move_time + 1000UL) && IsRunning()) {
      // travel moves have been received so enact them
      delayed_move_time = 0xFFFFFFFFUL; // force moves to be done
      set_destination_from_current();
      prepare_move_to_destination();
    }
  #endif

  #if ENABLED(TEMP_STAT_LEDS)
    handle_status_leds();
  #endif

  #if ENABLED(MONITOR_DRIVER_STATUS)
    monitor_tmc_driver();
  #endif

  #if ENABLED(MONITOR_L6470_DRIVER_STATUS)
    L6470.monitor_driver();
  #endif

  // Limit check_axes_activity frequency to 10Hz
  static millis_t next_check_axes_ms = 0;
  if (ELAPSED(ms, next_check_axes_ms)) {
    planner.check_axes_activity();
    next_check_axes_ms = ms + 100UL;
  }

  #if PIN_EXISTS(FET_SAFETY)
    static millis_t FET_next;
    if (ELAPSED(ms, FET_next)) {
      FET_next = ms + FET_SAFETY_DELAY;  // 2uS pulse every FET_SAFETY_DELAY mS
      OUT_WRITE(FET_SAFETY_PIN, !FET_SAFETY_INVERTED);
      DELAY_US(2);
      WRITE(FET_SAFETY_PIN, FET_SAFETY_INVERTED);
    }
  #endif
}

/**
 * Standard idle routine keeps the machine alive
 */
void idle(
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    bool no_stepper_sleep/*=false*/
  #endif
) {
  #if ENABLED(MAX7219_DEBUG)
    max7219.idle_tasks();
  #endif

  ui.update();

  #if ENABLED(HOST_KEEPALIVE_FEATURE)
    gcode.host_keepalive();
  #endif

  manage_inactivity(
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      no_stepper_sleep
    #endif
  );

  thermalManager.manage_heater();

  #if ENABLED(PRINTCOUNTER)
    print_job_timer.tick();
  #endif

  #if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
    buzzer.tick();
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    static millis_t i2cpem_next_update_ms;
    if (planner.has_blocks_queued() && ELAPSED(millis(), i2cpem_next_update_ms)) {
      I2CPEM.update();
      i2cpem_next_update_ms = millis() + I2CPE_MIN_UPD_TIME_MS;
    }
  #endif

  #ifdef HAL_IDLETASK
    HAL_idletask();
  #endif

  #if HAS_AUTO_REPORTING
    if (!suspend_auto_report) {
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        thermalManager.auto_report_temperatures();
      #endif
      #if ENABLED(AUTO_REPORT_SD_STATUS)
        card.auto_report_sd_status();
      #endif
    }
  #endif

  #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
    Sd2Card::idle();
  #endif

  #if ENABLED(PRUSA_MMU2)
    mmu2.mmuLoop();
  #endif
}

/**
 * Kill all activity and lock the machine.
 * After this the machine will need to be reset.
 */
void kill(PGM_P const lcd_msg/*=NULL*/) {
  thermalManager.disable_all_heaters();

  SERIAL_ERROR_MSG(MSG_ERR_KILLED);

  #if HAS_SPI_LCD || ENABLED(EXTENSIBLE_UI)
    ui.kill_screen(lcd_msg ? lcd_msg : PSTR(MSG_KILLED));
  #else
    UNUSED(lcd_msg);
  #endif

  #ifdef ACTION_ON_KILL
    host_action_kill();
  #endif

  minkill();
}

void minkill() {

  // Wait a short time (allows messages to get out before shutting down.
  for (int i = 1000; i--;) DELAY_US(600);

  cli(); // Stop interrupts

  // Wait to ensure all interrupts stopped
  for (int i = 1000; i--;) DELAY_US(250);

  thermalManager.disable_all_heaters(); // turn off heaters again

  #if HAS_POWER_SWITCH
    PSU_OFF();
  #endif

  #if HAS_SUICIDE
    suicide();
  #endif

  #if HAS_KILL

    // Wait for kill to be released
    while (!READ(KILL_PIN)) {
      #if ENABLED(USE_WATCHDOG)
        watchdog_reset();
      #endif
    }

    // Wait for kill to be pressed
    while (READ(KILL_PIN)) {
      #if ENABLED(USE_WATCHDOG)
        watchdog_reset();
      #endif
    }

    void(*resetFunc)(void) = 0; // Declare resetFunc() at address 0
    resetFunc();                // Jump to address 0

  #else // !HAS_KILL

    for (;;) {
      #if ENABLED(USE_WATCHDOG)
        watchdog_reset();
      #endif
    } // Wait for reset

  #endif // !HAS_KILL
}

/**
 * Turn off heaters and stop the print in progress
 * After a stop the machine may be resumed with M999
 */
void stop() {
  thermalManager.disable_all_heaters(); // 'unpause' taken care of in here
  print_job_timer.stop();

  #if ENABLED(PROBING_FANS_OFF)
    if (thermalManager.fans_paused) thermalManager.set_fans_paused(false); // put things back the way they were
  #endif

  if (IsRunning()) {
    Stopped_gcode_LastN = gcode_LastN; // Save last g_code for restart
    SERIAL_ERROR_MSG(MSG_ERR_STOPPED);
    LCD_MESSAGEPGM(MSG_STOPPED);
    safe_delay(350);       // allow enough time for messages to get out before stopping
    Running = false;
  }
}

/**
 * Marlin entry-point: Set up before the program loop
 *  - Set up the kill pin, filament runout, power hold
 *  - Start the serial port
 *  - Print startup messages and diagnostics
 *  - Get EEPROM or default settings
 *  - Initialize managers for:
 *    • temperature
 *    • planner
 *    • watchdog
 *    • stepper
 *    • photo pin
 *    • servos
 *    • LCD controller
 *    • Digipot I2C
 *    • Z probe sled
 *    • status LEDs
 */
void setup() {

  #ifdef HAL_INIT
    HAL_init();
  #endif

  #if HAS_DRIVER(L6470)
    L6470.init();         // setup SPI and then init chips
  #endif

  #if ENABLED(MAX7219_DEBUG)
    max7219.init();
  #endif

  #if ENABLED(DISABLE_DEBUG)
    // Disable any hardware debug to free up pins for IO
    #ifdef JTAGSWD_DISABLE
      JTAGSWD_DISABLE();
    #elif defined(JTAG_DISABLE)
      JTAG_DISABLE();
    #else
      #error "DISABLE_DEBUG is not supported for the selected MCU/Board"
    #endif
  #elif ENABLED(DISABLE_JTAG)
    // Disable JTAG to free up pins for IO
    #ifdef JTAG_DISABLE
      JTAG_DISABLE();
    #else
      #error "DISABLE_JTAG is not supported for the selected MCU/Board"
    #endif
  #endif

  #if HAS_FILAMENT_SENSOR
    runout.setup();
  #endif

  setup_killpin();

  setup_powerhold();

  #if HAS_STEPPER_RESET
    disableStepperDrivers();
  #endif

  #if NUM_SERIAL > 0
    MYSERIAL0.begin(BAUDRATE);
    #if NUM_SERIAL > 1
      MYSERIAL1.begin(BAUDRATE);
    #endif
  #endif

  #if NUM_SERIAL > 0
    uint32_t serial_connect_timeout = millis() + 1000UL;
    while (!MYSERIAL0 && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
    #if NUM_SERIAL > 1
      serial_connect_timeout = millis() + 1000UL;
      while (!MYSERIAL1 && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
    #endif
  #endif

  SERIAL_ECHOLNPGM("start");
  SERIAL_ECHO_START();

  #if TMC_HAS_SPI
    #if DISABLED(TMC_USE_SW_SPI)
      SPI.begin();
    #endif
    tmc_init_cs_pins();
  #endif
  #if HAS_DRIVER(TMC2208)
    tmc2208_serial_begin();
  #endif

  #ifdef BOARD_INIT
    BOARD_INIT();
  #endif

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = HAL_get_reset_source();
  if (mcu &  1) SERIAL_ECHOLNPGM(MSG_POWERUP);
  if (mcu &  2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
  if (mcu &  4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
  if (mcu &  8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
  if (mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
  HAL_clear_reset_source();

  SERIAL_ECHOPGM(MSG_MARLIN);
  SERIAL_CHAR(' ');
  SERIAL_ECHOLNPGM(SHORT_BUILD_VERSION);
  SERIAL_EOL();

  #if defined(STRING_DISTRIBUTION_DATE) && defined(STRING_CONFIG_H_AUTHOR)
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
    SERIAL_ECHOPGM(STRING_DISTRIBUTION_DATE);
    SERIAL_ECHOLNPGM(MSG_AUTHOR STRING_CONFIG_H_AUTHOR);
    SERIAL_ECHO_MSG("Compiled: " __DATE__);
  #endif

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPAIR(MSG_FREE_MEMORY, freeMemory(), MSG_PLANNER_BUFFER_BYTES, (int)sizeof(block_t) * (BLOCK_BUFFER_SIZE));

  queue_setup();

  // Load data from EEPROM if available (or use defaults)
  // This also updates variables in the planner, elsewhere
  (void)settings.load();

  #if HAS_M206_COMMAND
    // Initialize current position based on home_offset
    LOOP_XYZ(a) current_position[a] += home_offset[a];
  #endif

  // Vital to init stepper/planner equivalent for current_position
  sync_plan_position();

  thermalManager.init();    // Initialize temperature loop

  print_job_timer.init();   // Initial setup of print job timer

  endstops.init();          // Init endstops and pullups

  stepper.init();           // Init stepper. This enables interrupts!

  #if HAS_SERVOS
    servo_init();
  #endif

  #if HAS_Z_SERVO_PROBE
    servo_probe_init();
  #endif

  #if HAS_PHOTOGRAPH
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif

  #if ENABLED(SPINDLE_LASER_ENABLE)
    OUT_WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);  // init spindle to off
    #if SPINDLE_DIR_CHANGE
      OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);  // init rotation to clockwise (M3)
    #endif
    #if ENABLED(SPINDLE_LASER_PWM) && defined(SPINDLE_LASER_PWM_PIN) && SPINDLE_LASER_PWM_PIN >= 0
      SET_PWM(SPINDLE_LASER_PWM_PIN);
      analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);  // set to lowest speed
    #endif
  #endif

  #if HAS_BED_PROBE
    endstops.enable_z_probe(false);
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)
    SET_OUTPUT(CONTROLLER_FAN_PIN);
  #endif

  #if HAS_STEPPER_RESET
    enableStepperDrivers();
  #endif

  #if ENABLED(DIGIPOT_I2C)
    digipot_i2c_init();
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT)
    dac_init();
  #endif

  #if EITHER(Z_PROBE_SLED, SOLENOID_PROBE) && HAS_SOLENOID_1
    OUT_WRITE(SOL1_PIN, LOW); // OFF
  #endif

  #if HAS_HOME
    SET_INPUT_PULLUP(HOME_PIN);
  #endif

  #if PIN_EXISTS(STAT_LED_RED)
    OUT_WRITE(STAT_LED_RED_PIN, LOW); // OFF
  #endif

  #if PIN_EXISTS(STAT_LED_BLUE)
    OUT_WRITE(STAT_LED_BLUE_PIN, LOW); // OFF
  #endif

  #if HAS_COLOR_LEDS
    leds.setup();
  #endif

  #if HAS_CASE_LIGHT
    #if DISABLED(CASE_LIGHT_USE_NEOPIXEL)
      if (PWM_PIN(CASE_LIGHT_PIN)) SET_PWM(CASE_LIGHT_PIN); else SET_OUTPUT(CASE_LIGHT_PIN);
    #endif
    update_case_light();
  #endif

  #if ENABLED(MK2_MULTIPLEXER)
    SET_OUTPUT(E_MUX0_PIN);
    SET_OUTPUT(E_MUX1_PIN);
    SET_OUTPUT(E_MUX2_PIN);
  #endif

  #if HAS_FANMUX
    fanmux_init();
  #endif

  ui.init();
  ui.reset_status();

  #if HAS_SPI_LCD && ENABLED(SHOW_BOOTSCREEN)
    ui.show_bootscreen();
  #endif

  #if ENABLED(MIXING_EXTRUDER)
    mixer.init();
  #endif

  #if ENABLED(BLTOUCH)
    bltouch.init();
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    I2CPEM.init();
  #endif

  #if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0
    i2c.onReceive(i2c_on_receive);
    i2c.onRequest(i2c_on_request);
  #endif

  #if DO_SWITCH_EXTRUDER
    move_extruder_servo(0);   // Initialize extruder servo
  #endif

  #if ENABLED(SWITCHING_NOZZLE)
    // Initialize nozzle servo(s)
    #if SWITCHING_NOZZLE_TWO_SERVOS
      lower_nozzle(0);
      raise_nozzle(1);
    #else
      move_nozzle_servo(0);
    #endif
  #endif

  #if ENABLED(MAGNETIC_PARKING_EXTRUDER)
    mpe_settings_init();
  #endif

  #if ENABLED(PARKING_EXTRUDER)
    pe_solenoid_init();
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    recovery.check();
  #endif

  #if ENABLED(USE_WATCHDOG)
    watchdog_init();          // Reinit watchdog after HAL_get_reset_source call
  #endif

  #if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
    init_closedloop();
  #endif

  #if ENABLED(SDSUPPORT) && DISABLED(ULTRA_LCD)
    card.beginautostart();
  #endif

  #if HAS_TRINAMIC && DISABLED(PS_DEFAULT_OFF)
    test_tmc_connection(true, true, true, true);
  #endif

  #if ENABLED(PRUSA_MMU2)
    mmu2.init();
  #endif
}

/**
 * The main Marlin program loop
 *
 *  - Save or log commands to SD
 *  - Process available commands (if not saving)
 *  - Call endstop manager
 *  - Call inactivity manager
 */
void loop() {

  for (;;) {

    #if ENABLED(SDSUPPORT)
      card.checkautostart();

      if (card.flag.abort_sd_printing) {
        card.stopSDPrint(
          #if SD_RESORT
            true
          #endif
        );
        clear_command_queue();
        quickstop_stepper();
        print_job_timer.stop();
        thermalManager.disable_all_heaters();
        thermalManager.zero_fan_speeds();
        wait_for_heatup = false;
        #if ENABLED(POWER_LOSS_RECOVERY)
          card.removeJobRecoveryFile();
        #endif
        #ifdef EVENT_GCODE_SD_STOP
          enqueue_and_echo_commands_P(PSTR(EVENT_GCODE_SD_STOP));
        #endif
      }
    #endif // SDSUPPORT

    if (commands_in_queue < BUFSIZE) get_available_commands();
    advance_command_queue();
    endstops.event_handler();
    idle();
  }
}

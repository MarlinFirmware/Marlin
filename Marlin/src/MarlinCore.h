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
#pragma once

#include "inc/MarlinConfig.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Global State of the firmware
enum MarlinState : uint8_t {
  MF_INITIALIZING = 0,
  MF_STOPPED,
  MF_KILLED,
  MF_RUNNING,
  MF_SD_COMPLETE,
  MF_PAUSED,
  MF_WAITING,
};

typedef bool (*testFunc_t)();

// Delay ensuring that temperatures are updated and the watchdog is kept alive
void safe_delay(millis_t ms);

// Singleton for Marlin global data and methods

class Marlin {
public:
  #if ENABLED(CONFIGURABLE_MACHINE_NAME)
    static MString<64> machine_name;
  #endif

  static MarlinState state;
  static void setState(const MarlinState s) { state = s; }
  static bool is(const MarlinState s) { return state == s; }
  static bool isStopped() { return is(MF_STOPPED); }
  static bool isRunning() { return state >= MF_RUNNING; }

  static bool printingIsActive();
  static bool printJobOngoing();
  static bool printingIsPaused();
  static void startOrResumeJob();

  static bool printer_busy();

  static void stop();

  // Maintain all important activities
  static void manage_inactivity(const bool no_stepper_sleep=false);

  // Pass true to keep steppers from timing out
  static void idle(const bool no_stepper_sleep=false);
  static void idle_no_sleep() { idle(true); }

  static void kill(FSTR_P const lcd_error=nullptr, FSTR_P const lcd_component=nullptr, const bool steppers_off=false);
  static void minkill(const bool steppers_off=false);

  #if HAS_RESUME_CONTINUE
    // Global waiting for user response
    static bool wait_for_user;
    static void wait_start() { wait_for_user = true; }
    static void user_resume() { wait_for_user = false; }
    static void wait_for_user_response(millis_t ms=0, const bool no_sleep=false);
  #endif

  // Global waiting for heatup state
  static bool wait_for_heatup;
  static bool is_heating() { return wait_for_heatup; }
  static void heatup_start() { wait_for_heatup = true; }
  static void heatup_done() { wait_for_heatup = false; }
  static void end_waiting() { TERN_(HAS_RESUME_CONTINUE, wait_for_user =) wait_for_heatup = false; }

  // Shared function for M42 / M43
  static bool pin_is_protected(const pin_t pin);

  #if HAS_SUICIDE
    static void suicide() { OUT_WRITE(SUICIDE_PIN, SUICIDE_PIN_STATE); }
  #endif

  static bool kill_state() {
    return (
      #if HAS_KILL
        #ifndef KILL_PIN_STATE
          #define KILL_PIN_STATE LOW
        #endif
        READ(KILL_PIN) == KILL_PIN_STATE
      #else
        false
      #endif
    );
  }

};

extern Marlin marlin;

extern const char M112_KILL_STR[];

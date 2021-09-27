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

#include "../inc/MarlinConfig.h"

#if ENABLED(HOST_ACTION_COMMANDS)

#include "host_actions.h"

//#define DEBUG_HOST_ACTIONS

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "pause.h"
  #include "../gcode/queue.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "runout.h"
#endif

void host_action(FSTR_P const fstr, const bool eol) {
  PORT_REDIRECT(SerialMask::All);
  SERIAL_ECHOPGM("//action:");
  SERIAL_ECHOF(fstr);
  if (eol) SERIAL_EOL();
}

#ifdef ACTION_ON_KILL
  void host_action_kill() { host_action(F(ACTION_ON_KILL)); }
#endif
#ifdef ACTION_ON_PAUSE
  void host_action_pause(const bool eol/*=true*/) { host_action(F(ACTION_ON_PAUSE), eol); }
#endif
#ifdef ACTION_ON_PAUSED
  void host_action_paused(const bool eol/*=true*/) { host_action(F(ACTION_ON_PAUSED), eol); }
#endif
#ifdef ACTION_ON_RESUME
  void host_action_resume() { host_action(F(ACTION_ON_RESUME)); }
#endif
#ifdef ACTION_ON_RESUMED
  void host_action_resumed() { host_action(F(ACTION_ON_RESUMED)); }
#endif
#ifdef ACTION_ON_CANCEL
  void host_action_cancel() { host_action(F(ACTION_ON_CANCEL)); }
#endif
#ifdef ACTION_ON_START
  void host_action_start() { host_action(F(ACTION_ON_START)); }
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)

  PGMSTR(CONTINUE_STR, "Continue");
  PGMSTR(DISMISS_STR, "Dismiss");

  #if HAS_RESUME_CONTINUE
    extern bool wait_for_user;
  #endif

  PromptReason host_prompt_reason = PROMPT_NOT_DEFINED;

  void host_action_notify(const char * const cstr) {
    PORT_REDIRECT(SerialMask::All);
    host_action(F("notification "), false);
    SERIAL_ECHOLN(cstr);
  }

  void host_action_notify(FSTR_P const fstr) {
    PORT_REDIRECT(SerialMask::All);
    host_action(F("notification "), false);
    SERIAL_ECHOLNF(fstr);
  }

  void host_action_prompt(FSTR_P const ptype, const bool eol=true) {
    PORT_REDIRECT(SerialMask::All);
    host_action(F("prompt_"), false);
    SERIAL_ECHOF(ptype);
    if (eol) SERIAL_EOL();
  }

  void host_action_prompt_plus(FSTR_P const ptype, FSTR_P const fstr, const char extra_char='\0') {
    host_action_prompt(ptype, false);
    PORT_REDIRECT(SerialMask::All);
    SERIAL_CHAR(' ');
    SERIAL_ECHOF(fstr);
    if (extra_char != '\0') SERIAL_CHAR(extra_char);
    SERIAL_EOL();
  }
  void host_action_prompt_begin(const PromptReason reason, FSTR_P const fstr, const char extra_char/*='\0'*/) {
    host_action_prompt_end();
    host_prompt_reason = reason;
    host_action_prompt_plus(F("begin"), fstr, extra_char);
  }
  void host_action_prompt_button(FSTR_P const fstr) { host_action_prompt_plus(F("button"), fstr); }
  void host_action_prompt_end() { host_action_prompt(F("end")); }
  void host_action_prompt_show() { host_action_prompt(F("show")); }

  void _host_prompt_show(FSTR_P const btn1/*=nullptr*/, FSTR_P const btn2/*=nullptr*/) {
    if (btn1) host_action_prompt_button(btn1);
    if (btn2) host_action_prompt_button(btn2);
    host_action_prompt_show();
  }
  void host_prompt_do(const PromptReason reason, FSTR_P const fstr, FSTR_P const btn1/*=nullptr*/, FSTR_P const btn2/*=nullptr*/) {
    host_action_prompt_begin(reason, fstr);
    _host_prompt_show(btn1, btn2);
  }
  void host_prompt_do(const PromptReason reason, FSTR_P const fstr, const char extra_char, FSTR_P const btn1/*=nullptr*/, FSTR_P const btn2/*=nullptr*/) {
    host_action_prompt_begin(reason, fstr, extra_char);
    _host_prompt_show(btn1, btn2);
  }

  void filament_load_host_prompt() {
    const bool disable_to_continue = TERN0(HAS_FILAMENT_SENSOR, runout.filament_ran_out);
    host_prompt_do(PROMPT_FILAMENT_RUNOUT, F("Paused"), F("PurgeMore"),
      disable_to_continue ? F("DisableRunout") : FPSTR(CONTINUE_STR)
    );
  }

  //
  // Handle responses from the host, such as:
  //  - Filament runout responses: Purge More, Continue
  //  - General "Continue" response
  //  - Resume Print response
  //  - Dismissal of info
  //
  void host_response_handler(const uint8_t response) {
    const PromptReason hpr = host_prompt_reason;
    host_prompt_reason = PROMPT_NOT_DEFINED;  // Reset now ahead of logic
    switch (hpr) {
      case PROMPT_FILAMENT_RUNOUT:
        switch (response) {

          case 0: // "Purge More" button
            #if BOTH(M600_PURGE_MORE_RESUMABLE, ADVANCED_PAUSE_FEATURE)
              pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // Simulate menu selection (menu exits, doesn't extrude more)
            #endif
            break;

          case 1: // "Continue" / "Disable Runout" button
            #if BOTH(M600_PURGE_MORE_RESUMABLE, ADVANCED_PAUSE_FEATURE)
              pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;  // Simulate menu selection
            #endif
            #if HAS_FILAMENT_SENSOR
              if (runout.filament_ran_out) {                      // Disable a triggered sensor
                runout.enabled = false;
                runout.reset();
              }
            #endif
            break;
        }
        break;
      case PROMPT_USER_CONTINUE:
        TERN_(HAS_RESUME_CONTINUE, wait_for_user = false);
        break;
      case PROMPT_PAUSE_RESUME:
        #if BOTH(ADVANCED_PAUSE_FEATURE, SDSUPPORT)
          extern const char M24_STR[];
          queue.inject_P(M24_STR);
        #endif
        break;
      case PROMPT_INFO:
        break;
      default: break;
    }
  }

#endif // HOST_PROMPT_SUPPORT

#endif // HOST_ACTION_COMMANDS

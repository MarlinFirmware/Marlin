/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

void host_action(const char * const pstr, const bool eol) {
  SERIAL_ECHOPGM("//action:");
  serialprintPGM(pstr);
  if (eol) SERIAL_EOL();
}

#ifdef ACTION_ON_KILL
  void host_action_kill() { host_action(PSTR(ACTION_ON_KILL)); }
#endif
#ifdef ACTION_ON_PAUSE
  void host_action_pause(const bool eol/*=true*/) { host_action(PSTR(ACTION_ON_PAUSE), eol); }
#endif
#ifdef ACTION_ON_PAUSED
  void host_action_paused(const bool eol/*=true*/) { host_action(PSTR(ACTION_ON_PAUSED), eol); }
#endif
#ifdef ACTION_ON_RESUME
  void host_action_resume() { host_action(PSTR(ACTION_ON_RESUME)); }
#endif
#ifdef ACTION_ON_RESUMED
  void host_action_resumed() { host_action(PSTR(ACTION_ON_RESUMED)); }
#endif
#ifdef ACTION_ON_CANCEL
  void host_action_cancel() { host_action(PSTR(ACTION_ON_CANCEL)); }
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)

  #if HAS_RESUME_CONTINUE
    extern bool wait_for_user;
  #endif

  PromptReason host_prompt_reason = PROMPT_NOT_DEFINED;

  void host_action_prompt(const char * const ptype, const bool eol=true) {
    host_action(PSTR("prompt_"), false);
    serialprintPGM(ptype);
    if (eol) SERIAL_EOL();
  }

  void host_action_prompt_plus(const char * const ptype, const char * const pstr, const bool eol=true) {
    host_action_prompt(ptype, false);
    SERIAL_CHAR(' ');
    serialprintPGM(pstr);
    if (eol) SERIAL_EOL();
  }
  void host_action_prompt_begin(const char * const pstr, const bool eol/*=true*/) { host_action_prompt_plus(PSTR("begin"), pstr, eol); }
  void host_action_prompt_button(const char * const pstr) { host_action_prompt_plus(PSTR("button"), pstr); }
  void host_action_prompt_end() { host_action_prompt(PSTR("end")); }
  void host_action_prompt_show() { host_action_prompt(PSTR("show")); }
  void host_prompt_do(const PromptReason reason, const char * const pstr, const char * const pbtn/*=nullptr*/) {
    host_prompt_reason = reason;
    host_action_prompt_end();
    host_action_prompt_begin(pstr);
    if (pbtn) host_action_prompt_button(pbtn);
    host_action_prompt_show();
  }

  inline void say_m876_response(const char * const pstr) {
    SERIAL_ECHOPGM("M876 Responding PROMPT_");
    serialprintPGM(pstr);
    SERIAL_EOL();
  }

  void host_response_handler(const uint8_t response) {
    #ifdef DEBUG_HOST_ACTIONS
      SERIAL_ECHOLNPAIR("M86 Handle Reason: ", host_prompt_reason);
      SERIAL_ECHOLNPAIR("M86 Handle Response: ", response);
    #endif
    const char *msg = PSTR("UNKNOWN STATE");
    const PromptReason hpr = host_prompt_reason;
    host_prompt_reason = PROMPT_NOT_DEFINED;
    switch (hpr) {
      case PROMPT_FILAMENT_RUNOUT:
        msg = PSTR("FILAMENT_RUNOUT");
        if (response == 0) {
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
          #endif
          host_action_prompt_end();   // Close current prompt
          host_action_prompt_begin(PSTR("Paused"));
          host_action_prompt_button(PSTR("Purge More"));
          if (false
            #if HAS_FILAMENT_SENSOR
              || runout.filament_ran_out
            #endif
          )
            host_action_prompt_button(PSTR("DisableRunout"));
          else {
            host_prompt_reason = PROMPT_FILAMENT_RUNOUT;
            host_action_prompt_button(PSTR("Continue"));
          }
          host_action_prompt_show();
        }
        else if (response == 1) {
          #if HAS_FILAMENT_SENSOR
            if (runout.filament_ran_out) {
              runout.enabled = false;
              runout.reset();
            }
          #endif
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
          #endif
        }
        break;
      case PROMPT_USER_CONTINUE:
        #if HAS_RESUME_CONTINUE
          wait_for_user = false;
        #endif
        msg = PSTR("FILAMENT_RUNOUT_CONTINUE");
        break;
      case PROMPT_PAUSE_RESUME:
        msg = PSTR("LCD_PAUSE_RESUME");
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          queue.inject_P(PSTR("M24"));
        #endif
        break;
      case PROMPT_INFO:
        msg = PSTR("GCODE_INFO");
        break;
      default: break;
    }
    say_m876_response(msg);
  }

#endif // HOST_PROMPT_SUPPORT

#endif // HOST_ACTION_COMMANDS

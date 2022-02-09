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

//#define DEBUG_HOST_ACTIONS

#include "host_actions.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "pause.h"
  #include "../gcode/queue.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "runout.h"
#endif

HostUI hostui;

flag_t HostUI::flag;

void HostUI::action(FSTR_P const faction, const bool send_LF) {
  if (!flag.bits) return;
  PORT_REDIRECT(SerialMask::All);
  SERIAL_ECHOPGM("//action:");
  SERIAL_ECHOF(faction);
  if (send_LF) SERIAL_EOL();
}

#ifdef ACTION_ON_KILL
  void HostUI::kill() { action(F(ACTION_ON_KILL)); }
#endif
#ifdef ACTION_ON_PAUSE
  void HostUI::pause(const bool send_LF/*=true*/) { action(F(ACTION_ON_PAUSE), send_LF); }
#endif
#ifdef ACTION_ON_PAUSED
  void HostUI::paused(const bool send_LF/*=true*/) { action(F(ACTION_ON_PAUSED), send_LF); }
#endif
#ifdef ACTION_ON_RESUME
  void HostUI::resume() { action(F(ACTION_ON_RESUME)); }
#endif
#ifdef ACTION_ON_RESUMED
  void HostUI::resumed() { action(F(ACTION_ON_RESUMED)); }
#endif
#ifdef ACTION_ON_CANCEL
  void HostUI::cancel() { action(F(ACTION_ON_CANCEL)); }
#endif
#ifdef ACTION_ON_START
  void HostUI::start() { action(F(ACTION_ON_START)); }
#endif

#if ENABLED(G29_RETRY_AND_RECOVER)
  #ifdef ACTION_ON_G29_RECOVER
    void HostUI::g29_recover() { action(F(ACTION_ON_G29_RECOVER)); }
  #endif
  #ifdef ACTION_ON_G29_FAILURE
    void HostUI::g29_failure() { action(F(ACTION_ON_G29_FAILURE)); }
  #endif
#endif

#ifdef SHUTDOWN_ACTION
  void HostUI::shutdown() { action(F(SHUTDOWN_ACTION)); }
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)

  PromptType HostUI::host_prompt_type = PROMPT_NOT_DEFINED;

  PGMSTR(CONTINUE_STR, "Continue");
  PGMSTR(DISMISS_STR, "Dismiss");

  #if HAS_RESUME_CONTINUE
    extern bool wait_for_user;
  #endif

  void HostUI::notify(const char * const cmsg) {
    if (!flag.bits) return;
    PORT_REDIRECT(SerialMask::All);
    action(F("notification "), false);
    SERIAL_ECHOLN(cmsg);
  }

  void HostUI::notify_P(PGM_P const pstr) {
    if (!flag.bits) return;
    PORT_REDIRECT(SerialMask::All);
    action(F("notification "), false);
    SERIAL_ECHOLNPGM_P(pstr);
  }

  void HostUI::prompt(FSTR_P const ftype, const bool send_LF/*=true*/) {
    if (!flag.bits) return;
    PORT_REDIRECT(SerialMask::All);
    action(F("prompt_"), false);
    SERIAL_ECHOF(ftype);
    if (send_LF) SERIAL_EOL();
  }

  void HostUI::prompt_plus(FSTR_P const ftype, FSTR_P const fstr, const char final_char/*='\0'*/) {
    if (!flag.bits) return;
    prompt(ftype, false);
    PORT_REDIRECT(SerialMask::All);
    SERIAL_CHAR(' ');
    SERIAL_ECHOF(fstr);
    if (final_char != '\0') SERIAL_CHAR(final_char);
    SERIAL_EOL();
  }
  void HostUI::prompt_begin(const PromptType type, FSTR_P const fstr, const char final_char/*='\0'*/) {
    if (!flag.bits) return;
    prompt_end();
    host_prompt_type = type;
    prompt_plus(F("begin"), fstr, final_char);
  }
  void HostUI::prompt_button(FSTR_P const ftitle) { prompt_plus(F("button"), ftitle); }
  void HostUI::prompt_end() { prompt(F("end")); }
  void HostUI::prompt_show() { prompt(F("show")); }

  void HostUI::_prompt_show(FSTR_P const fbtn1, FSTR_P const fbtn2) {
    if (fbtn1) prompt_button(fbtn1);
    if (fbtn2) prompt_button(fbtn2);
    prompt_show();
  }
  void HostUI::prompt_do(const PromptType type, FSTR_P const fstr, FSTR_P const fbtn1/*=nullptr*/, FSTR_P const fbtn2/*=nullptr*/) {
    prompt_begin(type, fstr);
    _prompt_show(fbtn1, fbtn2);
  }
  void HostUI::prompt_do(const PromptType type, FSTR_P const fstr, const char final_char, FSTR_P const fbtn1/*=nullptr*/, FSTR_P const fbtn2/*=nullptr*/) {
    prompt_begin(type, fstr, final_char);
    _prompt_show(fbtn1, fbtn2);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void HostUI::filament_load_prompt() {
      const bool disable_to_continue = TERN0(HAS_FILAMENT_SENSOR, runout.filament_ran_out);
      prompt_do(PROMPT_FILAMENT_RUNOUT, F("Paused"), F("PurgeMore"),
        disable_to_continue ? F("DisableRunout") : FPSTR(CONTINUE_STR)
      );
    }
  #endif

  //
  // Handle responses from the host, such as:
  //  - Filament runout responses: Purge More, Continue
  //  - General "Continue" response
  //  - Resume Print response
  //  - Dismissal of info
  //
  void HostUI::handle_response(const uint8_t response) {
    const PromptType type = host_prompt_type;
    host_prompt_type = PROMPT_NOT_DEFINED;  // Reset now ahead of logic
    switch (type) {
      case PROMPT_FILAMENT_RUNOUT: // Runout button press response (1)
        switch (response) {

          case 0: // "Purge More" button
            #if BOTH(M600_PURGE_MORE_RESUMABLE, ADVANCED_PAUSE_FEATURE)
              pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // Simulate menu selection (menu exits, doesn't extrude more)
            #endif
            break;

          case 1: // "Continue" / "Disable Runout" button
            #if BOTH(M600_PURGE_MORE_RESUMABLE, ADVANCED_PAUSE_FEATURE)
              pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;  // Simulate menu selection
              hostui.prompt_end();
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
      case PROMPT_USER_CONTINUE: // Resume (2)
        wait_for_user = false;
        hostui.prompt_end();
        break;
      #if BOTH(ADVANCED_PAUSE_FEATURE, SDSUPPORT)
        case PROMPT_PAUSE_RESUME:
          extern const char M24_STR[];
          queue.inject_P(M24_STR);
          hostui.prompt_end();
          break;
      #endif
      case PROMPT_INFO: // Acknowledge (5)
        hostui.prompt_end();
        break;
      default:
        hostui.prompt_end();
        hostui.notify(F("Unexpected host response"));
        break;
    }
  }

#endif // HOST_PROMPT_SUPPORT

#endif // HOST_ACTION_COMMANDS

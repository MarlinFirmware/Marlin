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

#include "../inc/MarlinConfigPre.h"
#include "../HAL/shared/Marduino.h"

typedef union {
  uint8_t bits;
  struct { bool info:1, errors:1, debug:1; };
} flag_t;

#if ENABLED(HOST_PROMPT_SUPPORT)

  enum PromptType : uint8_t {
    PROMPT_NOT_DEFINED,
    PROMPT_FILAMENT_RUNOUT,
    PROMPT_USER_CONTINUE,
    PROMPT_FILAMENT_RUNOUT_REHEAT,
    PROMPT_PAUSE_RESUME,
    PROMPT_INFO
  };

#endif

class HostUI {
  public:

  static flag_t flag;
  HostUI() { flag.bits = 0xFF; }

  static void action(FSTR_P const faction, const bool send_LF=true);

  #ifdef ACTION_ON_KILL
    static void kill();
  #endif
  #ifdef ACTION_ON_PAUSE
    static void pause(const bool send_LF=true);
  #endif
  #ifdef ACTION_ON_PAUSED
    static void paused(const bool send_LF=true);
  #endif
  #ifdef ACTION_ON_RESUME
    static void resume();
  #endif
  #ifdef ACTION_ON_RESUMED
    static void resumed();
  #endif
  #ifdef ACTION_ON_CANCEL
    static void cancel();
  #endif
  #ifdef ACTION_ON_START
    static void start();
  #endif
  #ifdef SHUTDOWN_ACTION
    static void shutdown();
  #endif

  #if ENABLED(G29_RETRY_AND_RECOVER)
    #ifdef ACTION_ON_G29_RECOVER
      static void g29_recover();
    #endif
    #ifdef ACTION_ON_G29_FAILURE
      static void g29_failure();
    #endif
  #endif

  #if ENABLED(HOST_PROMPT_SUPPORT)
    private:
    static void prompt(FSTR_P const ftype, const bool send_LF=true);
    static void prompt_plus(FSTR_P const ftype, FSTR_P const p_message, const char final_char='\0');
    static void prompt_show();
    static void _prompt_show(FSTR_P const fbtn1, FSTR_P const fbtn2);

    public:
    static PromptType host_prompt_type;

    static void handle_response(const uint8_t response);

    static void notify_P(PGM_P const pstr);
    static void notify(FSTR_P const fmsg) { notify_P(FTOP(fmsg)); }
    static void notify(const char * const cmsg);

    static void prompt_begin(const PromptType type, FSTR_P const fmsg, const char final_char='\0');
    static void prompt_button(FSTR_P const ftitle);
    static void prompt_end();
    static void prompt_do(const PromptType type, FSTR_P const fstr, FSTR_P const fbtn1=nullptr, FSTR_P const fbtn2=nullptr);
    static void prompt_do(const PromptType type, FSTR_P const fstr, const char final_char, FSTR_P const fbtn1=nullptr, FSTR_P const fbtn2=nullptr);
    static void prompt_open(const PromptType type, FSTR_P const fstr, FSTR_P const fbtn1=nullptr, FSTR_P const fbtn2=nullptr) {
      if (host_prompt_type == PROMPT_NOT_DEFINED) prompt_do(type, fstr, button1_label, button2_label);
    }

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      static void filament_load_prompt();
    #endif

  #endif

};

extern HostUI hostui;

extern const char CONTINUE_STR[], DISMISS_STR[];

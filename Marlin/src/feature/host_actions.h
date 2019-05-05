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
#pragma once

#include "../inc/MarlinConfigPre.h"

#include <stdint.h>

void host_action(const char * const pstr, const bool eol=true);

#ifdef ACTION_ON_KILL
  void host_action_kill();
#endif
#ifdef ACTION_ON_PAUSE
  void host_action_pause(const bool eol=true);
#endif
#ifdef ACTION_ON_PAUSED
  void host_action_paused(const bool eol=true);
#endif
#ifdef ACTION_ON_RESUME
  void host_action_resume();
#endif
#ifdef ACTION_ON_RESUMED
  void host_action_resumed();
#endif
#ifdef ACTION_ON_CANCEL
  void host_action_cancel();
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)

  enum PromptReason : uint8_t {
    PROMPT_NOT_DEFINED,
    PROMPT_FILAMENT_RUNOUT,
    PROMPT_USER_CONTINUE,
    PROMPT_FILAMENT_RUNOUT_REHEAT,
    PROMPT_PAUSE_RESUME,
    PROMPT_INFO
  };

  extern PromptReason host_prompt_reason;

  void host_response_handler(const uint8_t response);
  void host_action_prompt_begin(const char * const pstr, const bool eol=true);
  void host_action_prompt_button(const char * const pstr);
  void host_action_prompt_end();
  void host_action_prompt_show();
  void host_prompt_do(const PromptReason type, const char * const pstr, const char * const pbtn=NULL);
  inline void host_prompt_open(const PromptReason reason, const char * const pstr, const char * const pbtn=NULL) {
    if (host_prompt_reason == PROMPT_NOT_DEFINED) host_prompt_do(reason, pstr, pbtn);
  }

#endif

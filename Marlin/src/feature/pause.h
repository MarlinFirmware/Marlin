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
#pragma once

/**
 * feature/pause.h - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

typedef struct {
  float unload_length, load_length;
} fil_change_settings_t;

#include "../inc/MarlinConfigPre.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)

#include "../libs/nozzle.h"

enum AdvancedPauseMode : char {
  ADVANCED_PAUSE_MODE_SAME,
  ADVANCED_PAUSE_MODE_PAUSE_PRINT,
  ADVANCED_PAUSE_MODE_LOAD_FILAMENT,
  ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT
};

enum AdvancedPauseMessage : char {
  ADVANCED_PAUSE_MESSAGE_INIT,
  ADVANCED_PAUSE_MESSAGE_WAITING,
  ADVANCED_PAUSE_MESSAGE_UNLOAD,
  ADVANCED_PAUSE_MESSAGE_INSERT,
  ADVANCED_PAUSE_MESSAGE_LOAD,
  ADVANCED_PAUSE_MESSAGE_PURGE,
  ADVANCED_PAUSE_MESSAGE_OPTION,
  ADVANCED_PAUSE_MESSAGE_RESUME,
  ADVANCED_PAUSE_MESSAGE_STATUS,
  ADVANCED_PAUSE_MESSAGE_HEAT,
  ADVANCED_PAUSE_MESSAGE_HEATING
};

enum AdvancedPauseMenuResponse : char {
  ADVANCED_PAUSE_RESPONSE_WAIT_FOR,
  ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE,
  ADVANCED_PAUSE_RESPONSE_RESUME_PRINT
};

extern AdvancedPauseMenuResponse advanced_pause_menu_response;

extern fil_change_settings_t fc_settings[EXTRUDERS];

extern uint8_t did_pause_print;

#if ENABLED(DUAL_X_CARRIAGE)
  #define DXC_PARAMS , const int8_t DXC_ext=-1
  #define DXC_ARGS   , const int8_t DXC_ext
  #define DXC_PASS   , DXC_ext
#else
  #define DXC_PARAMS
  #define DXC_ARGS
  #define DXC_PASS
#endif

void do_pause_e_move(const float &length, const float &fr);

bool pause_print(const float &retract, const point_t &park_point, const float &unload_length=0, const bool show_lcd=false DXC_PARAMS);

void wait_for_confirmation(const bool is_reload=false, const int8_t max_beep_count=0 DXC_PARAMS);

void resume_print(const float &slow_load_length=0, const float &fast_load_length=0, const float &extrude_length=ADVANCED_PAUSE_PURGE_LENGTH, const int8_t max_beep_count=0 DXC_PARAMS);

bool load_filament(const float &slow_load_length=0, const float &fast_load_length=0, const float &extrude_length=0, const int8_t max_beep_count=0, const bool show_lcd=false,
                          const bool pause_for_user=false, const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT DXC_PARAMS);

bool unload_filament(const float &unload_length, const bool show_lcd=false, const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT);

#endif // ADVANCED_PAUSE_FEATURE

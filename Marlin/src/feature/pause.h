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

/**
 * feature/pause.h - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "../libs/nozzle.h"

#include "../inc/MarlinConfigPre.h"

enum AdvancedPauseMode : char {
  ADVANCED_PAUSE_MODE_PAUSE_PRINT,
  ADVANCED_PAUSE_MODE_LOAD_FILAMENT,
  ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT
};

enum AdvancedPauseMessage : char {
  ADVANCED_PAUSE_MESSAGE_INIT,
  ADVANCED_PAUSE_MESSAGE_UNLOAD,
  ADVANCED_PAUSE_MESSAGE_INSERT,
  ADVANCED_PAUSE_MESSAGE_LOAD,
  ADVANCED_PAUSE_MESSAGE_PURGE,
  #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
    ADVANCED_PAUSE_MESSAGE_CONTINUOUS_PURGE,
  #endif
  ADVANCED_PAUSE_MESSAGE_OPTION,
  ADVANCED_PAUSE_MESSAGE_RESUME,
  ADVANCED_PAUSE_MESSAGE_STATUS,
  ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE,
  ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT
};

enum AdvancedPauseMenuResponse : char {
  ADVANCED_PAUSE_RESPONSE_WAIT_FOR,
  ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE,
  ADVANCED_PAUSE_RESPONSE_RESUME_PRINT
};

extern AdvancedPauseMenuResponse advanced_pause_menu_response;

extern float filament_change_unload_length[EXTRUDERS],
             filament_change_load_length[EXTRUDERS];

extern uint8_t did_pause_print;

bool pause_print(const float &retract, const point_t &park_point, const float &unload_length=0, const bool show_lcd=false);

void wait_for_filament_reload(const int8_t max_beep_count=0);

void resume_print(const float &slow_load_length=0, const float &fast_load_length=0, const float &extrude_length=ADVANCED_PAUSE_PURGE_LENGTH, const int8_t max_beep_count=0);

bool load_filament(const float &slow_load_length=0, const float &fast_load_length=0, const float &extrude_length=0, const int8_t max_beep_count=0, const bool show_lcd=false,
                          const bool pause_for_user=false, const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT);

bool unload_filament(const float &unload_length, const bool show_lcd=false, const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT);

#endif // _PAUSE_H_

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

enum PauseMode : char {
  PAUSE_MODE_SAME,
  PAUSE_MODE_PAUSE_PRINT,
  PAUSE_MODE_CHANGE_FILAMENT,
  PAUSE_MODE_LOAD_FILAMENT,
  PAUSE_MODE_UNLOAD_FILAMENT
};

enum PauseMessage : char {
  PAUSE_MESSAGE_PARKING,
  PAUSE_MESSAGE_CHANGING,
  PAUSE_MESSAGE_WAITING,
  PAUSE_MESSAGE_UNLOAD,
  PAUSE_MESSAGE_INSERT,
  PAUSE_MESSAGE_LOAD,
  PAUSE_MESSAGE_PURGE,
  PAUSE_MESSAGE_OPTION,
  PAUSE_MESSAGE_RESUME,
  PAUSE_MESSAGE_STATUS,
  PAUSE_MESSAGE_HEAT,
  PAUSE_MESSAGE_HEATING
};

#if HAS_LCD_MENU
  enum PauseMenuResponse : char {
    PAUSE_RESPONSE_WAIT_FOR,
    PAUSE_RESPONSE_EXTRUDE_MORE,
    PAUSE_RESPONSE_RESUME_PRINT
  };
  extern PauseMenuResponse pause_menu_response;
  extern PauseMode pause_mode;
#endif

extern fil_change_settings_t fc_settings[EXTRUDERS];

extern uint8_t did_pause_print;

#if ENABLED(DUAL_X_CARRIAGE)
  #define DXC_PARAMS , const int8_t DXC_ext=-1
  #define DXC_ARGS   , const int8_t DXC_ext
  #define DXC_PASS   , DXC_ext
  #define DXC_SAY    , " dxc:", int(DXC_ext)
#else
  #define DXC_PARAMS
  #define DXC_ARGS
  #define DXC_PASS
  #define DXC_SAY
#endif

bool pause_print(const float &retract, const xyz_pos_t &park_point, const float &unload_length=0, const bool show_lcd=false DXC_PARAMS);

void wait_for_confirmation(const bool is_reload=false, const int8_t max_beep_count=0 DXC_PARAMS);

void resume_print(const float &slow_load_length=0, const float &fast_load_length=0, const float &extrude_length=ADVANCED_PAUSE_PURGE_LENGTH,
                    const int8_t max_beep_count=0, int16_t targetTemp=0 DXC_PARAMS);

bool load_filament(const float &slow_load_length=0, const float &fast_load_length=0, const float &extrude_length=0, const int8_t max_beep_count=0,
                    const bool show_lcd=false, const bool pause_for_user=false, const PauseMode mode=PAUSE_MODE_PAUSE_PRINT DXC_PARAMS);

bool unload_filament(const float &unload_length, const bool show_lcd=false, const PauseMode mode=PAUSE_MODE_PAUSE_PRINT
  #if BOTH(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
    , const float &mix_multiplier=1.0
  #endif
);

#endif // ADVANCED_PAUSE_FEATURE

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

#pragma once

#include "../../inc/MarlinConfig.h"

#if HAS_FILAMENT_SENSOR
  #include "../runout.h"
#endif

struct E_Step;

class MMU2 {
public:
  MMU2();

  static void init();
  static void reset();
  static void mmu_loop();
  static void tool_change(uint8_t index);
  static void tool_change(const char* special);
  static uint8_t get_current_tool();
  static void set_filament_type(uint8_t index, uint8_t type);

  #if HAS_LCD_MENU && ENABLED(MMU2_MENUS)
    static bool unload();
    static void load_filament(uint8_t);
    static void load_all();
    static bool load_filament_to_nozzle(uint8_t index);
    static bool eject_filament(uint8_t index, bool recover);
  #endif

private:
  static bool rx_str_P(const char* str);
  static void tx_str_P(const char* str);
  static void tx_printf_P(const char* format, int argument);
  static void tx_printf_P(const char* format, int argument1, int argument2);
  static void clear_rx_buffer();

  static bool rx_ok();
  static bool rx_start();
  static void check_version();

  static void command(const uint8_t cmd);
  static bool get_response(void);
  static void manage_response(const bool move_axes, const bool turn_off_nozzle);

  #if HAS_LCD_MENU && ENABLED(MMU2_MENUS)
    static void load_to_nozzle();
    static void filament_ramming();
    static void execute_extruder_sequence(const E_Step * sequence, int steps);
  #endif

  static void filament_runout();

  static bool enabled, ready, mmu_print_saved;
  static uint8_t cmd, cmd_arg, last_cmd, extruder;
  static int8_t state;
  static volatile int8_t finda;
  static volatile bool finda_runout_valid;
  static int16_t version, buildnr;
  static millis_t last_request, next_P0_request;
  static char rx_buffer[16], tx_buffer[16];

  static inline void set_runout_valid(const bool valid) {
    finda_runout_valid = valid;
    #if HAS_FILAMENT_SENSOR
      if (valid) runout.reset();
    #endif
  }

};

extern MMU2 mmu2;

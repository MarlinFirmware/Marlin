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

#include "../../inc/MarlinConfig.h"

#if HAS_FILAMENT_SENSOR
  #include "../runout.h"
#endif

#if SERIAL_USB
  #define MMU_RX_SIZE 256
  #define MMU_TX_SIZE 256
#else
  #define MMU_RX_SIZE  16
  #define MMU_TX_SIZE  16
#endif

struct E_Step;

class MMU2 {
public:
  MMU2();

  static void init();
  static void reset();
  static void mmu_loop();
  static void tool_change(const uint8_t index);
  static void tool_change(const char *special);
  static uint8_t get_current_tool();
  static void set_filament_type(const uint8_t index, const uint8_t type);

  static bool unload();
  static void load_filament(uint8_t);
  static void load_all();
  static bool load_filament_to_nozzle(const uint8_t index);
  static bool eject_filament(const uint8_t index, const bool recover);

private:
  static bool rx_str_P(const char *str);
  static void tx_str_P(const char *str);
  static void tx_printf_P(const char *format, const int argument);
  static void tx_printf_P(const char *format, const int argument1, const int argument2);
  static void clear_rx_buffer();

  static bool rx_ok();
  static bool rx_start();
  static void check_version();

  static void command(const uint8_t cmd);
  static bool get_response();
  static void manage_response(const bool move_axes, const bool turn_off_nozzle);

  static void load_to_nozzle();
  static void execute_extruder_sequence(const E_Step * sequence, int steps);

  static void filament_runout();

  #if HAS_PRUSA_MMU2S
    static bool mmu2s_triggered;
    static void check_filament();
    static bool can_load();
    static bool load_to_gears();
  #else
    FORCE_INLINE static bool load_to_gears() { return true; }
  #endif

  #if ENABLED(MMU_EXTRUDER_SENSOR)
    static void mmu_continue_loading();
  #endif

  static bool enabled, ready, mmu_print_saved;

  static uint8_t cmd, cmd_arg, last_cmd, extruder;
  static int8_t state;
  static volatile int8_t finda;
  static volatile bool finda_runout_valid;
  static int16_t version, buildnr;
  static millis_t prev_request, prev_P0_request;
  static char rx_buffer[MMU_RX_SIZE], tx_buffer[MMU_TX_SIZE];

  static inline void set_runout_valid(const bool valid) {
    finda_runout_valid = valid;
    #if HAS_FILAMENT_SENSOR
      if (valid) runout.reset();
    #endif
  }

};

extern MMU2 mmu2;

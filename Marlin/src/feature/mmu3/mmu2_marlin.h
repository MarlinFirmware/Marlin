/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * mmu2_marlin.h
 */

/// The sole purpose of this interface is to separate Marlin1/Marlin2 from the MMU top logic layer.
/// Why?
/// - unify implementation among MK3 and Buddy FW
/// - enable unit testing of MMU top layer
#pragma once
#include <stdint.h>
#include "src/MarlinCore.h"

#if HAS_PRUSA_MMU3
  namespace MMU2 {

  void extruder_move(const float distance, const float feed_rate, const bool sync=true);
  void extruder_schedule_turning(float feed_rate);

  float move_raise_z(float delta);

  void planner_abort_queued_moves();
  void planner_synchronize();
  bool planner_any_moves();
  float stepper_get_machine_position_E_mm();
  float planner_get_current_position_E();
  void planner_set_current_position_E(float e);
  xyz_pos_t planner_current_position();

  void motion_do_blocking_move_to_xy(float rx, float ry, float feedRate_mm_s);
  void motion_do_blocking_move_to_z(float z, float feedRate_mm_s);

  void nozzle_park();

  bool marlin_printingIsActive();
  void marlin_manage_heater();
  void marlin_manage_inactivity(bool b);
  void marlin_idle(bool b);
  void marlin_refresh_print_state_in_ram();
  void marlin_clear_print_state_in_ram();
  void marlin_stop_and_save_print_to_ram();

  int16_t thermal_degTargetHotend();
  int16_t thermal_degHotend();
  void thermal_setExtrudeMintemp(int16_t t);
  void thermal_setTargetHotend(int16_t t);

  void safe_delay_keep_alive(uint16_t t);

  void Enable_E0();
  void Disable_E0();

  bool all_axes_homed();

  } // namespace MMU2
#endif // HAS_PRUSA_MMU3

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

#include "../../inc/MarlinConfig.h"

#if ENABLED(DIRECT_STEPPING)

#include "../../feature/direct_stepping.h"

#include "../gcode.h"
#include "../../module/planner.h"

/**
 * G6: Direct Stepper Move
 */
void GcodeSuite::G6() {
  // TODO: feedrate support?
  if (parser.seen('R'))
    planner.last_page_step_rate = parser.value_ulong();

  if (!DirectStepping::Config::DIRECTIONAL) {
    #define PAGE_DIR_SET(N,A) do{ if (parser.seen(N)) planner.last_page_dir.A = !!parser.value_byte(); } while(0)
    LOGICAL_AXIS_CODE(
      PAGE_DIR_SET('E',E),
      PAGE_DIR_SET('X',X), PAGE_DIR_SET('Y',Y), PAGE_DIR_SET('Z',Z),
      PAGE_DIR_SET(AXIS4_NAME,I), PAGE_DIR_SET(AXIS5_NAME,J), PAGE_DIR_SET(AXIS6_NAME,K),
      PAGE_DIR_SET(AXIS5_NAME,U), PAGE_DIR_SET(AXIS6_NAME,V), PAGE_DIR_SET(AXIS7_NAME,W)
    );
  }

  // No index means we just set the state
  if (!parser.seen('I')) return;

  // No speed is set, can't schedule the move
  if (!planner.last_page_step_rate) return;

  const page_idx_t page_idx = (page_idx_t)parser.value_ulong();

  uint16_t num_steps = DirectStepping::Config::TOTAL_STEPS;
  if (parser.seen('S')) num_steps = parser.value_ushort();

  planner.buffer_page(page_idx, 0, num_steps);
  reset_stepper_timeout();
}

#endif // DIRECT_STEPPING

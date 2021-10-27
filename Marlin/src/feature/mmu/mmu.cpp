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

#if HAS_PRUSA_MMU1

#include "../MarlinCore.h"
#include "../module/planner.h"
#include "../module/stepper.h"

void mmu_init() {
  SET_OUTPUT(E_MUX0_PIN);
  SET_OUTPUT(E_MUX1_PIN);
  SET_OUTPUT(E_MUX2_PIN);
}

void select_multiplexed_stepper(const uint8_t e) {
  planner.synchronize();
  stepper.disable_e_steppers();
  WRITE(E_MUX0_PIN, TEST(e, 0) ? HIGH : LOW);
  WRITE(E_MUX1_PIN, TEST(e, 1) ? HIGH : LOW);
  WRITE(E_MUX2_PIN, TEST(e, 2) ? HIGH : LOW);
  safe_delay(100);
}

#endif // HAS_PRUSA_MMU1

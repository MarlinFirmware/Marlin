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
 * mmu2_power.cpp
 */

#include "mmu2.h"
#include "mmu2_power.h"

#include "src/MarlinCore.h"

#include "src/core/macros.h"
#include "src/core/boards.h"
#include "src/pins/pins.h"

#if HAS_PRUSA_MMU3
  namespace MMU2 {

  // On MK3 we cannot do actual power cycle on HW. Instead trigger a hardware reset.
  void power_on() {
    #if PIN_EXISTS(MMU2_RST)
      WRITE(MMU2_RST_PIN, 1);
      SET_OUTPUT(MMU2_RST_PIN); // setup reset pin
    #endif // MMU_HWRESET

    reset();
  }

  void power_off() {
  }

  void reset() {
    #if PIN_EXISTS(MMU2_RST) // HW - pulse reset pin
      WRITE(MMU2_RST_PIN, 0);
      safe_delay(100);
      WRITE(MMU2_RST_PIN, 1);
    #else
      mmu2.Reset(MMU2::Software); // @@TODO needs to be redesigned, this power implementation shall not know anything about the MMU itself
    #endif
    // otherwise HW reset is not available
  }

  } // namespace MMU2
#endif // HAS_PRUSA_MMU3

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

#include "../../inc/MarlinConfigPre.h"

#if HAS_PRUSA_MMU3

#include "mmu2.h"
#include "mmu2_power.h"

#include "../../MarlinCore.h"

#include "../../core/macros.h"
#include "../../core/boards.h"
#include "../../pins/pins.h"

namespace MMU2 {

// On MK3 we cannot do actual power cycle on HW. Instead trigger a hardware reset.
void power_on() {
  #if PIN_EXISTS(MMU2_RST)
    OUT_WRITE(MMU2_RST_PIN, HIGH);
  #endif
  reset();
}

void power_off() {}

void reset() {
  #if PIN_EXISTS(MMU2_RST) // HW - pulse reset pin
    WRITE(MMU2_RST_PIN, LOW);
    safe_delay(100);
    WRITE(MMU2_RST_PIN, HIGH);
  #else
    mmu2.Reset(MMU2::Software); // TODO: Needs redesign. This power implementation shouldn't know anything about the MMU itself
  #endif
  // otherwise HW reset is not available
}

} // namespace MMU2

#endif // HAS_PRUSA_MMU3

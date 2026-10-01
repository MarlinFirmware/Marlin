/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "DGUSDisplay.h"
#include "definition/DGUS_VP.h"

namespace DGUSRxHandler {
  void printSpeedPercentage(DGUS_VP &vp, void *data);
  void zOffset(DGUS_VP &vp, void *data);
  void extruderTargetTemp(DGUS_VP &vp, void *data);
  void bedTargetTemp(DGUS_VP &vp, void *data);
  void axis_X(DGUS_VP &vp, void *data);
  void axis_Y(DGUS_VP &vp, void *data);
  void axis_Z(DGUS_VP &vp, void *data);
  void filamentLength(DGUS_VP &vp, void *data);
  void setLanguage(DGUS_VP &vp, void *data);
  void refresh(DGUS_VP &vp, void *data);

  template<typename T>
  void integerToExtra(DGUS_VP &vp, void *data_ptr) {
    if (!vp.size || !vp.extra) return;
    switch (vp.size) {
      default: return;
      case 1: {
        const uint8_t data = *(uint8_t*)data_ptr;
        *(T*)vp.extra = (T)data;
        break;
      }
      case 2: {
        const uint16_t data = Endianness::fromBE_P<uint16_t>(data_ptr);
        *(T*)vp.extra = (T)data;
        break;
      }
      case 4: {
        const uint32_t data = Endianness::fromBE_P<uint32_t>(data_ptr);
        *(T*)vp.extra = (T)data;
        break;
      }
    }
  }
}

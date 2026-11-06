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

namespace DGUSTxHandler {
  void bootAnimation(DGUS_VP &);

  void zOffset(DGUS_VP &);
  void elapsedHours(DGUS_VP &);
  void elapsedMinutes(DGUS_VP &);
  void printPercentage(DGUS_VP &);
  void printPercentageIcon(DGUS_VP &);
  void printSpeedPercentage(DGUS_VP &);
  void fanIcon(DGUS_VP &);
  void extruderTargetTemp(DGUS_VP &);
  void extruderCurrentTemp(DGUS_VP &);
  void bedTargetTemp(DGUS_VP &);
  void bedCurrentTemp(DGUS_VP &);
  void axis_X(DGUS_VP &);
  void axis_Y(DGUS_VP &);
  void axis_Z(DGUS_VP &);
  void filamentLength(DGUS_VP &);
  void levelingProgressIcon(DGUS_VP &);
  void filamentMissingIcon(DGUS_VP &);
  void stepperStatus(DGUS_VP &);
  void printFilename(DGUS_VP &);
  void fileSelectionIcon(DGUS_VP &);
  void fileSelectionColor(DGUS_VP &);

  void extraToString(DGUS_VP &);
  void extraPGMToString(DGUS_VP &);

  template<typename T>
  void extraToInteger(DGUS_VP &vp) {
    if (!vp.size || !vp.extra) return;
    switch (vp.size) {
      default: return;
      case 1: {
        const uint8_t data = uint8_t(*(T*)vp.extra);
        dgus.write(uint16_t(vp.addr), data);
        break;
      }
      case 2: {
        const uint16_t data = uint16_t(*(T*)vp.extra);
        dgus.write(uint16_t(vp.addr), Endianness::toBE(data));
        break;
      }
      case 4: {
        const uint32_t data = uint32_t(*(T*)vp.extra);
        dgus.write(uint16_t(vp.addr), Endianness::toBE(data));
        break;
      }
    }
  }
}

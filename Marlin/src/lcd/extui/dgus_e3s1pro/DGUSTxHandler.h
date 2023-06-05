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

#define Swap16(val) (uint16_t((uint16_t(val) >> 8) | (uint16_t(val) << 8)))

namespace DGUSTxHandler {
  void bootAnimation(DGUS_VP &);

  void zOffset(DGUS_VP &);
  void elapsedHours(DGUS_VP &);
  void elapsedMinutes(DGUS_VP &);
  void levelingProgress(DGUS_VP &vp);
  void fanSpeed(DGUS_VP &vp);
  void levelingProgressIcon(DGUS_VP &vp);
  #if HAS_FILAMENT_SENSOR
    void filamentInsertionStatus(DGUS_VP &vp);
  #endif
  void sdCardInsertionStatus(DGUS_VP &vp);

  template <typename T, T axis>
  void maxFeedrate(DGUS_VP &vp) {
    const uint16_t data = (uint16_t)ExtUI::getAxisMaxFeedrate_mm_s(axis);
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }
  template <typename T, T axis>
  void maxAcceleration(DGUS_VP &vp) {
    const uint16_t data = (uint16_t)ExtUI::getAxisMaxAcceleration_mm_s2(axis);
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }
  template <typename T, T axis>
  void maxJerk(DGUS_VP &vp) {
    const uint16_t data = dgus.toFixedPoint<float, uint16_t, 2>(ExtUI::getAxisMaxJerk_mm_s(axis));
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }
  template <typename T, T axis>
  void stepsPerMM(DGUS_VP &vp) {
    const uint16_t data = dgus.toFixedPoint<float, uint16_t, 1>(ExtUI::getAxisSteps_per_mm(axis));
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }

  #if ENABLED(PIDTEMP)
    template<ExtUI::extruder_t extruder>
    void PID_P(DGUS_VP &vp) {
      const uint16_t data = dgus.toFixedPoint<float, uint16_t, 2>(ExtUI::getPID_Kp(extruder));
      dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
    }
    template<ExtUI::extruder_t extruder>
    void PID_I(DGUS_VP &vp) {
      const uint16_t data = dgus.toFixedPoint<float, uint16_t, 2>(ExtUI::getPID_Ki(extruder));
      dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
    }
    template<ExtUI::extruder_t extruder>
    void PID_D(DGUS_VP &vp) {
      const uint16_t data = dgus.toFixedPoint<float, uint16_t, 2>(ExtUI::getPID_Kd(extruder));
      dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
    }
  #endif // PIDTEMP

  #if ENABLED(PIDTEMPBED)
    void bed_PID_P(DGUS_VP &vp);
    void bed_PID_I(DGUS_VP &vp);
    void bed_PID_D(DGUS_VP &vp);
  #endif

  void printRemainingHours(DGUS_VP &);
  void printRemainingMinutes(DGUS_VP &);
  void printPercentage(DGUS_VP &);
  void printSpeedPercentage(DGUS_VP &);
  void extruderTargetTemp(DGUS_VP &);
  void extruderCurrentTemp(DGUS_VP &);
  void bedTargetTemp(DGUS_VP &);
  void bedCurrentTemp(DGUS_VP &);
  void axis_X(DGUS_VP &);
  void axis_Y(DGUS_VP &);
  void axis_Z(DGUS_VP &);
  void stepperStatus(DGUS_VP &);
  void enableIfLanguageMatchesExtra(DGUS_VP &);

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

  template<typename T, uint8_t decimals>
  void extraToFixedPoint(DGUS_VP &vp) {
    if (!vp.size || !vp.extra) return;
    switch (vp.size) {
      default: return;
      case 1: {
        const uint8_t data = dgus.toFixedPoint<T, uint8_t, decimals>(*(T*)vp.extra);
        dgus.write(uint16_t(vp.addr), data);
        break;
      }
      case 2: {
        const uint16_t data = dgus.toFixedPoint<T, uint16_t, decimals>(*(T*)vp.extra);
        dgus.write(uint16_t(vp.addr), Endianness::toBE(data));
        break;
      }
      case 4: {
        const uint32_t data = dgus.toFixedPoint<T, uint32_t, decimals>(*(T*)vp.extra);
        dgus.write(uint16_t(vp.addr), Endianness::toBE(data));
        break;
      }
    }
  }
}

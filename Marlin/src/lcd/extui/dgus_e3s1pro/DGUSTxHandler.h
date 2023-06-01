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
  void BootAnimation(DGUS_VP &);

  void ZOffset(DGUS_VP &);
  void ElapsedHours(DGUS_VP &);
  void ElapsedMinutes(DGUS_VP &);
  void Leveling_Progress(DGUS_VP &vp);
  void FanSpeed(DGUS_VP &vp);
  void Leveling_ProgressIcon(DGUS_VP &vp);
  void Filament_InsertionStatus(DGUS_VP &vp);
  void SDCard_InsertionStatus(DGUS_VP &vp);


  template <typename T, T axis>
  void MaxFeedrate(DGUS_VP &vp)
  {
    uint16_t data = (uint16_t)ExtUI::getAxisMaxFeedrate_mm_s(axis);
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
  }
  template <typename T, T axis>
  void MaxAcceleration(DGUS_VP &vp)
  {
    uint16_t data = (uint16_t)ExtUI::getAxisMaxAcceleration_mm_s2(axis);
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
  }
  template <typename T, T axis>
  void MaxJerk(DGUS_VP &vp)
  {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getAxisMaxJerk_mm_s(axis));
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
  }
  template <typename T, T axis>
  void StepsPerMm(DGUS_VP &vp)
  {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 1>(ExtUI::getAxisSteps_per_mm(axis));
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
  }

  #if ENABLED(PIDTEMP)
    template<ExtUI::extruder_t extruder>
    void PID_P(DGUS_VP &vp)
    {
      uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getPID_Kp(extruder));
      dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
    }
    template<ExtUI::extruder_t extruder>
    void PID_I(DGUS_VP &vp)
    {
      uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getPID_Ki(extruder));
      dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
    }
    template<ExtUI::extruder_t extruder>
    void PID_D(DGUS_VP &vp)
    {
      uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getPID_Kd(extruder));
      dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
    }
  #endif // PIDTEMP

  #if ENABLED(PIDTEMPBED)
    void Bed_PID_P(DGUS_VP &vp);
    void Bed_PID_I(DGUS_VP &vp);
    void Bed_PID_D(DGUS_VP &vp);
  #endif // PIDTEMPBED

  void PrintRemainingHours(DGUS_VP &);
  void PrintRemainingMinutes(DGUS_VP &);
  void PrintPercentage(DGUS_VP &);
  void PrintSpeedPercentage(DGUS_VP &);
  void ExtruderTargetTemp(DGUS_VP &);
  void ExtruderCurrentTemp(DGUS_VP &);
  void BedTargetTemp(DGUS_VP &);
  void BedCurrentTemp(DGUS_VP &);
  void Axis_X(DGUS_VP &);
  void Axis_Y(DGUS_VP &);
  void Axis_Z(DGUS_VP &);
  void StepperStatus(DGUS_VP &);
  void EnableIfLanguageMatchesExtra(DGUS_VP &);

  void ExtraToString(DGUS_VP &);
  void ExtraPGMToString(DGUS_VP &);

  template<typename T>
  void ExtraToInteger(DGUS_VP &vp) {
    if (!vp.size || !vp.extra) return;
    switch (vp.size) {
      default: return;
      case 1: {
        const uint8_t data = uint8_t(*(T*)vp.extra);
        dgus_display.Write(uint16_t(vp.addr), data);
        break;
      }
      case 2: {
        const uint16_t data = uint16_t(*(T*)vp.extra);
        dgus_display.Write(uint16_t(vp.addr), Swap16(data));
        break;
      }
      case 4: {
        const uint32_t data = uint32_t(*(T*)vp.extra);
        dgus_display.Write(uint16_t(vp.addr), dgus_display.SwapBytes(data));
        break;
      }
    }
  }

  template<typename T, uint8_t decimals>
  void ExtraToFixedPoint(DGUS_VP &vp) {
    if (!vp.size || !vp.extra) return;
    switch (vp.size) {
      default: return;
      case 1: {
        const uint8_t data = dgus_display.ToFixedPoint<T, uint8_t, decimals>(*(T*)vp.extra);
        dgus_display.Write(uint16_t(vp.addr), data);
        break;
      }
      case 2: {
        const uint16_t data = dgus_display.ToFixedPoint<T, uint16_t, decimals>(*(T*)vp.extra);
        dgus_display.Write(uint16_t(vp.addr), Swap16(data));
        break;
      }
      case 4: {
        const uint32_t data = dgus_display.ToFixedPoint<T, uint32_t, decimals>(*(T*)vp.extra);
        dgus_display.Write(uint16_t(vp.addr), dgus_display.SwapBytes(data));
        break;
      }
    }
  }
}

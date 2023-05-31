/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  void PrintSpeedPercentage(DGUS_VP &vp, void *data);
  void ZOffset(DGUS_VP &vp, void *data);
  void ExtruderTargetTemp(DGUS_VP &vp, void *data);
  void BedTargetTemp(DGUS_VP &vp, void *data);
  void Axis_X(DGUS_VP &vp, void *data);
  void Axis_Y(DGUS_VP &vp, void *data);
  void Axis_Z(DGUS_VP &vp, void *data);
  void ExtrudeLength(DGUS_VP &vp, void *data);
  void RetractLength(DGUS_VP &vp, void *data);
  void Language_SetLanguage(DGUS_VP &vp, void *data);

  template <typename T, T axis>
  void MaxFeedrate(DGUS_VP &vp, void *data)
  {
    feedRate_t maxSpeed = (float)BE16_P(data);
    ExtUI::setAxisMaxFeedrate_mm_s(maxSpeed, axis);
  }  
  template <typename T, T axis>
  void MaxAcceleration(DGUS_VP &vp, void *data)
  {
    float maxAcceleration = (float)BE16_P(data);
    ExtUI::setAxisMaxAcceleration_mm_s2(maxAcceleration, axis);
  }
  template <typename T, T axis>
  void MaxJerk(DGUS_VP &vp, void *data)
  {
    float maxJerk = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
    ExtUI::setAxisMaxJerk_mm_s(maxJerk, axis);
  }  
  template <typename T, T axis>
  void StepsPerMm(DGUS_VP &vp, void *data)
  {
    float stepsPerMm = dgus_display.FromFixedPoint<uint16_t, float, 1>(BE16_P(data));
    ExtUI::setAxisSteps_per_mm(stepsPerMm, axis);
  }


  #if ENABLED(PIDTEMP)
      template <ExtUI::extruder_t extruder>
    void PID_P(DGUS_VP &vp, void *data)
    {
      float pidValue = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
      ExtUI::setPID(
        pidValue,
        ExtUI::getPID_Ki(extruder),
        ExtUI::getPID_Kd(extruder),
        extruder
      );
    }
    template <ExtUI::extruder_t extruder>
    void PID_I(DGUS_VP &vp, void *data)
    {
      float pidValue = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
      ExtUI::setPID(
        ExtUI::getPID_Kp(extruder),
        pidValue,
        ExtUI::getPID_Kd(extruder),
        extruder
      );
    }
    template <ExtUI::extruder_t extruder>
    void PID_D(DGUS_VP &vp, void *data)
    {
      float pidValue = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
      ExtUI::setPID(
        ExtUI::getPID_Kp(extruder),
        ExtUI::getPID_Ki(extruder),
        pidValue,
        extruder
      );
    }
  #endif // PIDTEMP

  #if ENABLED(PIDTEMPBED)
    void Bed_PID_P(DGUS_VP &vp, void *data);
    void Bed_PID_I(DGUS_VP &vp, void *data);
    void Bed_PID_D(DGUS_VP &vp, void *data);
  #endif // PIDTEMPBED
  
  void Control_SetFanSpeed(DGUS_VP &vp, void *data);
  void SDCard_FileSelection(DGUS_VP &vp, void *data);

  void StringToExtra(DGUS_VP &vp, void *data);
  void Disabled(DGUS_VP &vp, void *data);

  template<typename T>
  void IntegerToExtra(DGUS_VP &vp, void *data_ptr) {
    if (!vp.size || !vp.extra) return;
    switch (vp.size) {
      default: return;
      case 1: {
        const uint8_t data = *(uint8_t*)data_ptr;
        *(T*)vp.extra = (T)data;
        break;
      }
      case 2: {
        const uint16_t data = BE16_P(data_ptr);
        *(T*)vp.extra = (T)data;
        break;
      }
      case 4: {
        const uint32_t data = dgus_display.SwapBytes(*(uint32_t*)data_ptr);
        *(T*)vp.extra = (T)data;
        break;
      }
    }
  }

}

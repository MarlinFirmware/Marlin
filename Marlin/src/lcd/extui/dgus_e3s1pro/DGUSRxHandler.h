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
  void extrudeLength(DGUS_VP &vp, void *data);
  void retractLength(DGUS_VP &vp, void *data);
  void setLanguage(DGUS_VP &vp, void *data);

  template <typename T, T axis>
  void maxFeedrate(DGUS_VP &vp, void *data) {
    feedRate_t maxSpeed = (float)Endianness::fromBE_P<uint16_t>(data);
    ExtUI::setAxisMaxFeedrate_mm_s(maxSpeed, axis);
  }
  template <typename T, T axis>
  void maxAcceleration(DGUS_VP &vp, void *data) {
    float maxAcceleration = (float)Endianness::fromBE_P<uint16_t>(data);
    ExtUI::setAxisMaxAcceleration_mm_s2(maxAcceleration, axis);
  }
  template <typename T, T axis>
  void maxJerk(DGUS_VP &vp, void *data) {
    float maxJerk = dgus.fromFixedPoint<uint16_t, float, 2>(Endianness::fromBE_P<uint16_t>(data));
    ExtUI::setAxisMaxJerk_mm_s(maxJerk, axis);
  }

  #if ENABLED(EDITABLE_STEPS_PER_UNIT)
    template <typename T, T axis>
    void stepsPerMM(DGUS_VP &vp, void *data) {
      float stepsPerMm = dgus.fromFixedPoint<uint16_t, float, 1>(Endianness::fromBE_P<uint16_t>(data));
      ExtUI::setAxisSteps_per_mm(stepsPerMm, axis);
    }
  #endif

  #if ENABLED(PIDTEMP)
    template <ExtUI::extruder_t extruder>
    void PID_P(DGUS_VP &vp, void *data) {
      float pidValue = dgus.fromFixedPoint<uint16_t, float, 2>(Endianness::fromBE_P<uint16_t>(data));
      ExtUI::setPID(
        pidValue,
        ExtUI::getPID_Ki(extruder),
        ExtUI::getPID_Kd(extruder),
        extruder
      );
    }
    template <ExtUI::extruder_t extruder>
    void PID_I(DGUS_VP &vp, void *data) {
      float pidValue = dgus.fromFixedPoint<uint16_t, float, 2>(Endianness::fromBE_P<uint16_t>(data));
      ExtUI::setPID(
        ExtUI::getPID_Kp(extruder),
        pidValue,
        ExtUI::getPID_Kd(extruder),
        extruder
      );
    }
    template <ExtUI::extruder_t extruder>
    void PID_D(DGUS_VP &vp, void *data) {
      float pidValue = dgus.fromFixedPoint<uint16_t, float, 2>(Endianness::fromBE_P<uint16_t>(data));
      ExtUI::setPID(
        ExtUI::getPID_Kp(extruder),
        ExtUI::getPID_Ki(extruder),
        pidValue,
        extruder
      );
    }
  #endif // PIDTEMP

  #if ENABLED(PIDTEMPBED)
    void bed_PID_P(DGUS_VP &vp, void *data);
    void bed_PID_I(DGUS_VP &vp, void *data);
    void bed_PID_D(DGUS_VP &vp, void *data);
  #endif

  void fanSpeed(DGUS_VP &vp, void *data);
  void sdCardFileSection(DGUS_VP &vp, void *data);

  void stringToExtra(DGUS_VP &vp, void *data);
  void disabled(DGUS_VP &vp, void *data);

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

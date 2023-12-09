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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_E3S1PRO)

#include "DGUSTxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Data.h"

#include "../../../module/stepper.h" // for axis_enabled
#include "../../../libs/duration_t.h"

void DGUSTxHandler::bootAnimation(DGUS_VP &vp) {
  static uint16_t bootIcon = 0;

  dgus.write((uint16_t)vp.addr, Endianness::toBE(bootIcon));

  if (++bootIcon > 100) {
    bootIcon = 0;
    screen.triggerScreenChange(DGUS_ScreenID::HOME);
  }
}

void DGUSTxHandler::zOffset(DGUS_VP &vp) {
  const float position = ExtUI::getZOffset_mm();
  const int16_t data = dgus.toFixedPoint<float, int16_t, 2>(position); // Round to 0.01
  dgus.write((int16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::elapsedHours(DGUS_VP &vp) {
  const duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  const int16_t data = elapsedtime.hour();
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::elapsedMinutes(DGUS_VP &vp) {
  const duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  const int16_t data = elapsedtime.minute() % 60;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::levelingProgress(DGUS_VP &vp) {
  const uint16_t data = (100 * screen.currentMeshPointIndex) / GRID_MAX_POINTS;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::fanSpeed(DGUS_VP &vp) {
  const int16_t data = (int16_t)ExtUI::getActualFan_percent(ExtUI::FAN0);
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::levelingProgressIcon(DGUS_VP &vp) {
  const uint16_t data = 1 + (50 * screen.currentMeshPointIndex) / GRID_MAX_POINTS;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

#if HAS_FILAMENT_SENSOR
  void DGUSTxHandler::filamentInsertionStatus(DGUS_VP &vp) {
    const uint16_t data = ExtUI::getFilamentRunoutState() ? 1 : 0;
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }
#endif

void DGUSTxHandler::sdCardInsertionStatus(DGUS_VP &vp) {
  const uint16_t data = ExtUI::isMediaInserted() ? 1 : 0;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

#if ENABLED(PIDTEMPBED)
  void DGUSTxHandler::bed_PID_P(DGUS_VP &vp) {
    const int16_t data = dgus.toFixedPoint<float, int16_t, 2>(ExtUI::getBedPID_Kp());
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }

  void DGUSTxHandler::bed_PID_I(DGUS_VP &vp) {
    const int16_t data = dgus.toFixedPoint<float, int16_t, 2>(ExtUI::getBedPID_Ki());
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }

  void DGUSTxHandler::bed_PID_D(DGUS_VP &vp) {
    const int16_t data = dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getBedPID_Kd());
    dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
  }
#endif // PIDTEMPBED

static duration_t _printRemainingDurationEstimate() {
  duration_t remainingDuration = 0;

  if (ExtUI::isPrinting()) {
    const uint8_t progressPercentage = ExtUI::getProgress_percent();
    remainingDuration = duration_t(3600);

    if (progressPercentage >= 2)
      remainingDuration = ExtUI::getProgress_seconds_elapsed() * ((100.0 / (float)progressPercentage) - 1.0);
  }

  return remainingDuration;
}

void DGUSTxHandler::printRemainingHours(DGUS_VP &vp) {
  const int16_t data = _printRemainingDurationEstimate().hour();
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::printRemainingMinutes(DGUS_VP &vp) {
  const int16_t data = _printRemainingDurationEstimate().minute() % 60;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::printPercentage(DGUS_VP &vp) {
  const int16_t data = ExtUI::getProgress_percent();
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::printSpeedPercentage(DGUS_VP &vp) {
  const int16_t data = ExtUI::getFeedrate_percent();
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::extruderTargetTemp(DGUS_VP &vp) {
  const int16_t data = ExtUI::getTargetTemp_celsius(ExtUI::H0);
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::extruderCurrentTemp(DGUS_VP &vp) {
  const int16_t data = ExtUI::getActualTemp_celsius(ExtUI::H0);
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::bedTargetTemp(DGUS_VP &vp) {
  const int16_t data = ExtUI::getTargetTemp_celsius(ExtUI::BED);
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::bedCurrentTemp(DGUS_VP &vp) {
  const int16_t data = ExtUI::getActualTemp_celsius(ExtUI::BED);
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::axis_X(DGUS_VP &vp) {
  const int16_t data = dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::X));
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::axis_Y(DGUS_VP &vp) {
  const int16_t data = dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Y));
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::axis_Z(DGUS_VP &vp) {
  const int16_t data = dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Z));
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::stepperStatus(DGUS_VP &vp) {
  const bool areSteppersEnabled = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
  const uint16_t data = areSteppersEnabled ? 1 : 0;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::enableIfLanguageMatchesExtra(DGUS_VP &vp) {
  const DGUS_Data::Language targetLanguage = (DGUS_Data::Language)reinterpret_cast<uintptr_t>(vp.extra);
  const uint16_t data = targetLanguage == screen.config.language ? 1 : 0;
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::extraToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  #if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
    if (vp.flags & VPFLAG_TXSTRING_AUTOSCROLL) {
      const char* stringStart = (const char*)vp.extra;
      const size_t stringLength = strlen(stringStart);
      screen.addCurrentPageStringLength(stringLength, vp.size);

      if (stringLength <= vp.size || screen.getScrollIndex() <= 0) {
        // no scroll needed
      }
      else if ((ssize_t)stringLength - screen.getScrollIndex() <= vp.size) {
        stringStart += (stringLength - vp.size);
      }
      else
        stringStart += screen.getScrollIndex();

      dgus.writeString((uint16_t)vp.addr,
        stringStart,
        vp.size, true, false, false);
      return;
    }
  #endif // DGUS_SOFTWARE_AUTOSCROLL

  dgus.writeString((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

void DGUSTxHandler::extraPGMToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus.writeStringPGM((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

#endif // DGUS_LCD_UI_E3S1PRO

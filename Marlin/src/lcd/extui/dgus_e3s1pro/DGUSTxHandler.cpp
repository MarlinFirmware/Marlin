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

#include "../../../module/stepper.h"

void DGUSTxHandler::bootAnimation(DGUS_VP &vp) {
  static uint16_t bootIcon = 0;

  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(bootIcon));

  if (++bootIcon > 100) {
    bootIcon = 0;
    screen.triggerScreenChange(DGUS_Screen::HOME);
  }
}

void DGUSTxHandler::zOffset(DGUS_VP &vp) {
  const float position = ExtUI::getZOffset_mm();
  const int16_t data = dgus_display.ToFixedPoint<float, int16_t, 2>(position); // Round to 0.01
  dgus_display.write((int16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::elapsedHours(DGUS_VP &vp) {
  duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  int16_t data = elapsedtime.hour();
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::elapsedMinutes(DGUS_VP &vp) {
  duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  int16_t data = elapsedtime.minute() % 60;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::levelingProgress(DGUS_VP &vp) {
  uint16_t data = (100 * screen.currentMeshPointIndex) / GRID_MAX_POINTS;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::fanSpeed(DGUS_VP &vp) {
  uint16_t data = (uint16_t)ExtUI::getActualFan_percent(ExtUI::FAN0);
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::levelingProgressIcon(DGUS_VP &vp) {
  uint16_t data = 1 + (50 * screen.currentMeshPointIndex) / GRID_MAX_POINTS;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::filamentInsertionStatus(DGUS_VP &vp) {
  uint16_t data = ExtUI::getFilamentRunoutState() ? 1 : 0;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::sdCardInsertionStatus(DGUS_VP &vp) {
  uint16_t data = ExtUI::isMediaInserted() ? 1 : 0;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

#if ENABLED(PIDTEMPBED)
  void DGUSTxHandler::bed_PID_P(DGUS_VP &vp) {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getBedPID_Kp());
    dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
  }

  void DGUSTxHandler::bed_PID_I(DGUS_VP &vp) {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getBedPID_Ki());
    dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
  }

  void DGUSTxHandler::bed_PID_D(DGUS_VP &vp) {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getBedPID_Kd());
    dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
  }
#endif // PIDTEMPBED

static duration_t _PrintRemainingDurationEstimate() {
  duration_t remainingDuration = 0;

  if (ExtUI::isPrinting()) {
    uint8_t progressPercentage = ExtUI::getProgress_percent();
    remainingDuration = duration_t(3600);

    if (progressPercentage >= 2)
      remainingDuration = ExtUI::getProgress_seconds_elapsed() * (100.0 / progressPercentage);
  }

  return remainingDuration;
}

void DGUSTxHandler::printRemainingHours(DGUS_VP &vp) {
  int16_t data = _PrintRemainingDurationEstimate().hour();
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::printRemainingMinutes(DGUS_VP &vp) {
  int16_t data = _PrintRemainingDurationEstimate().minute() % 60;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::printPercentage(DGUS_VP &vp) {
  int16_t data = ExtUI::getProgress_percent();
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::printSpeedPercentage(DGUS_VP &vp) {
  int16_t data = ExtUI::getFeedrate_percent();
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::extruderTargetTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getTargetTemp_celsius(ExtUI::H0);
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::extruderCurrentTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getActualTemp_celsius(ExtUI::H0);
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::bedTargetTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getTargetTemp_celsius(ExtUI::BED);
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::bedCurrentTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getActualTemp_celsius(ExtUI::BED);
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::axis_X(DGUS_VP &vp) {
  int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::X));
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::axis_Y(DGUS_VP &vp) {
  int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Y));
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::axis_Z(DGUS_VP &vp) {
  int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Z));
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::stepperStatus(DGUS_VP &vp) {
  const bool areSteppersEnabled = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
  uint16_t data = areSteppersEnabled ? 1 : 0;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::enableIfLanguageMatchesExtra(DGUS_VP &vp) {
  DGUS_Data::Language targetLanguage = (DGUS_Data::Language)reinterpret_cast<uintptr_t>(vp.extra);
  uint16_t data = targetLanguage == screen.config.language ? 1 : 0;
  dgus_display.write((uint16_t)vp.addr, dgus_display.swapBytes(data));
}

void DGUSTxHandler::extraToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

#if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
  if (vp.flags & VPFLAG_TXSTRING_AUTOSCROLL) {
    const char* stringStart = (const char*)vp.extra;
    size_t stringLength = strlen(stringStart);
    screen.addCurrentPageStringLength(stringLength, vp.size);

    if (stringLength <= vp.size || screen.getScrollIndex() <= 0) {
      // no scroll needed
    }
    else if ((ssize_t)stringLength - screen.getScrollIndex() <= vp.size) {
      stringStart += (stringLength - vp.size);
    }
    else
      stringStart += screen.getScrollIndex();

    dgus_display.writeString((uint16_t)vp.addr,
      stringStart,
      vp.size, true, false, false);
  }
  else
#endif // DGUS_SOFTWARE_AUTOSCROLL
    dgus_display.writeString((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

void DGUSTxHandler::extraPGMToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus_display.writeStringPGM((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

#endif // DGUS_LCD_UI_RELOADED

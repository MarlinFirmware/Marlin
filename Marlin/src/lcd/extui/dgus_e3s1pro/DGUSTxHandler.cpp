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

void DGUSTxHandler::BootAnimation(DGUS_VP &vp) {
  static uint16_t bootIcon = 0;

  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(bootIcon));

  if (++bootIcon > 100) {
    bootIcon = 0;
    dgus_screen_handler.TriggerScreenChange(DGUS_Screen::HOME);
  }
}

void DGUSTxHandler::ZOffset(DGUS_VP &vp) {
  const float position = ExtUI::getZOffset_mm();
  const int16_t data = dgus_display.ToFixedPoint<float, int16_t, 2>(position); // Round to 0.01
  dgus_display.Write((int16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::ElapsedHours(DGUS_VP &vp) {
  duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  int16_t data = elapsedtime.hour();
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::ElapsedMinutes(DGUS_VP &vp) {
  duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  int16_t data = elapsedtime.minute() % 60;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::Leveling_Progress(DGUS_VP &vp) {
  uint16_t data = (100 * dgus_screen_handler.currentMeshPointIndex) / GRID_MAX_POINTS;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::FanSpeed(DGUS_VP &vp) {
  uint16_t data = (uint16_t)ExtUI::getActualFan_percent(ExtUI::FAN0);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::Leveling_ProgressIcon(DGUS_VP &vp) {
  uint16_t data = 1 + (50 * dgus_screen_handler.currentMeshPointIndex) / GRID_MAX_POINTS;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::Filament_InsertionStatus(DGUS_VP &vp) {
  uint16_t data = ExtUI::getFilamentRunoutState() ? 1 : 0;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::SDCard_InsertionStatus(DGUS_VP &vp) {
  uint16_t data = ExtUI::isMediaInserted() ? 1 : 0;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

#if ENABLED(PIDTEMPBED)
  void DGUSTxHandler::Bed_PID_P(DGUS_VP &vp) {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getBedPID_Kp());
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
  }

  void DGUSTxHandler::Bed_PID_I(DGUS_VP &vp) {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getBedPID_Ki());
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
  }

  void DGUSTxHandler::Bed_PID_D(DGUS_VP &vp) {
    uint16_t data = dgus_display.ToFixedPoint<float, uint16_t, 2>(ExtUI::getBedPID_Kd());
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
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

void DGUSTxHandler::PrintRemainingHours(DGUS_VP &vp) {
  int16_t data = _PrintRemainingDurationEstimate().hour();
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::PrintRemainingMinutes(DGUS_VP &vp) {
  int16_t data = _PrintRemainingDurationEstimate().minute() % 60;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::PrintPercentage(DGUS_VP &vp) {
  int16_t data = ExtUI::getProgress_percent();
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::PrintSpeedPercentage(DGUS_VP &vp) {
  int16_t data = ExtUI::getFeedrate_percent();
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::ExtruderTargetTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getTargetTemp_celsius(ExtUI::H0);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::ExtruderCurrentTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getActualTemp_celsius(ExtUI::H0);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::BedTargetTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getTargetTemp_celsius(ExtUI::BED);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::BedCurrentTemp(DGUS_VP &vp) {
  int16_t data = ExtUI::getActualTemp_celsius(ExtUI::BED);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::Axis_X(DGUS_VP &vp) {
  int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::X));
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::Axis_Y(DGUS_VP &vp) {
  int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Y));
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::Axis_Z(DGUS_VP &vp) {
  int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Z));
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::StepperStatus(DGUS_VP &vp) {
  const bool areSteppersEnabled = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
  uint16_t data = areSteppersEnabled ? 1 : 0;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::EnableIfLanguageMatchesExtra(DGUS_VP &vp) {
  DGUS_Data::Language targetLanguage = (DGUS_Data::Language)reinterpret_cast<uintptr_t>(vp.extra);
  uint16_t data = targetLanguage == dgus_screen_handler.config.language ? 1 : 0;
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::ExtraToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

#if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
  if (vp.flags & VPFLAG_TXSTRING_AUTOSCROLL) {
    const char* stringStart = (const char*)vp.extra;
    size_t stringLength = strlen(stringStart);
    dgus_screen_handler.AddCurrentPageStringLength(stringLength, vp.size);

    if (stringLength <= vp.size || dgus_screen_handler.GetScrollIndex() <= 0) {
      // no scroll needed
    }
    else if ((ssize_t)stringLength - dgus_screen_handler.GetScrollIndex() <= vp.size) {
      stringStart += (stringLength - vp.size);
    }
    else
      stringStart += dgus_screen_handler.GetScrollIndex();

    dgus_display.WriteString((uint16_t)vp.addr,
      stringStart,
      vp.size, true, false, false);
  }
  else
#endif // DGUS_SOFTWARE_AUTOSCROLL
    dgus_display.WriteString((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

void DGUSTxHandler::ExtraPGMToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus_display.WriteStringPGM((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

#endif // DGUS_LCD_UI_RELOADED

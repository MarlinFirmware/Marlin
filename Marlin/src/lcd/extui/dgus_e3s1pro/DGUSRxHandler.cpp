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

#include "DGUSRxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Screen.h"

#include "../ui_api.h"
#include "../../../core/language.h"
#include "../../../module/stepper.h"

void DGUSRxHandler::printSpeedPercentage(DGUS_VP &vp, void *data) {
  const_float_t feedratePercentage = dgus.fromFixedPoint<int16_t, const_float_t, 0>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setFeedrate_percent(feedratePercentage);
}

void DGUSRxHandler::zOffset(DGUS_VP &vp, void *data) {
  const_float_t zoffset = dgus.fromFixedPoint<int16_t, const_float_t, 2>(Endianness::fromBE_P<int16_t>(data));
  const_float_t currentzOffset = ExtUI::getZOffset_mm();
  const_float_t zStepsPerMm = ExtUI::getAxisSteps_per_mm(ExtUI::Z);
  int16_t zStepsDiff = zStepsPerMm * (zoffset - currentzOffset);

  ExtUI::babystepAxis_steps(zStepsDiff, ExtUI::Z);
  ExtUI::setZOffset_mm(zoffset);
}

void DGUSRxHandler::extruderTargetTemp(DGUS_VP &vp, void *data) {
  const_float_t temperature = dgus.fromFixedPoint<int16_t, const_float_t, 0>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setTargetTemp_celsius(temperature, ExtUI::H0);
}

void DGUSRxHandler::bedTargetTemp(DGUS_VP &vp, void *data) {
  const_float_t temperature = dgus.fromFixedPoint<int16_t, const_float_t, 0>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setTargetTemp_celsius(temperature, ExtUI::BED);
}

void DGUSRxHandler::axis_X(DGUS_VP &vp, void *data) {
  const_float_t axisValue = dgus.fromFixedPoint<int16_t, float, 1>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setAxisPosition_mm(axisValue, ExtUI::X);
}

void DGUSRxHandler::axis_Y(DGUS_VP &vp, void *data) {
  const_float_t axisValue = dgus.fromFixedPoint<int16_t, float, 1>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setAxisPosition_mm(axisValue, ExtUI::Y);
}

void DGUSRxHandler::axis_Z(DGUS_VP &vp, void *data) {
  const_float_t axisValue = dgus.fromFixedPoint<int16_t, float, 1>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setAxisPosition_mm(axisValue, ExtUI::Z);
}

void DGUSRxHandler::extrudeLength(DGUS_VP &vp, void *data) {
  const_float_t length = dgus.fromFixedPoint<int16_t, const_float_t, 1>(Endianness::fromBE_P<int16_t>(data));
  const_float_t currentPosition = ExtUI::getAxisPosition_mm(ExtUI::E0);

  #if HAS_FILAMENT_SENSOR
    if (ExtUI::getFilamentRunoutEnabled() && ExtUI::getFilamentRunoutState()) {
      screen.triggerTempScreenChange(DGUS_ScreenID::FILAMENTCHECK, DGUS_ScreenID::CONTROL_DEVICE);
      return;
    }
  #endif
  ExtUI::setAxisPosition_mm(currentPosition+length, ExtUI::E0);
}

void DGUSRxHandler::retractLength(DGUS_VP &vp, void *data) {
  const_float_t length = dgus.fromFixedPoint<int16_t, const_float_t, 1>(Endianness::fromBE_P<int16_t>(data));
  const_float_t currentPosition = ExtUI::getAxisPosition_mm(ExtUI::E0);

  #if HAS_FILAMENT_SENSOR
    if (ExtUI::getFilamentRunoutEnabled() && ExtUI::getFilamentRunoutState()) {
      screen.triggerTempScreenChange(DGUS_ScreenID::FILAMENTCHECK, DGUS_ScreenID::CONTROL_DEVICE);
      return;
    }
  #endif
  ExtUI::setAxisPosition_mm(currentPosition-length, ExtUI::E0);
}

void DGUSRxHandler::setLanguage(DGUS_VP &vp, void *data) {
  DGUS_Data::Language language = (DGUS_Data::Language)Endianness::fromBE_P<uint16_t>(data);
  screen.config.language = language;
  screen.triggerEEPROMSave();
  screen.triggerFullUpdate();
}

#if ENABLED(PIDTEMPBED)

  void DGUSRxHandler::bed_PID_P(DGUS_VP &vp, void *data) {
    float pidValue = dgus.fromFixedPoint<int16_t, float, 2>(Endianness::fromBE_P<int16_t>(data));
    ExtUI::setBedPID(
      pidValue,
      ExtUI::getBedPID_Ki(),
      ExtUI::getBedPID_Kd()
    );
  }

  void DGUSRxHandler::bed_PID_I(DGUS_VP &vp, void *data) {
    float pidValue = dgus.fromFixedPoint<int16_t, float, 2>(Endianness::fromBE_P<int16_t>(data));
    ExtUI::setBedPID(
      ExtUI::getBedPID_Kp(),
      pidValue,
      ExtUI::getBedPID_Kd()
    );
  }

  void DGUSRxHandler::bed_PID_D(DGUS_VP &vp, void *data) {
    float pidValue = dgus.fromFixedPoint<int16_t, float, 1>(Endianness::fromBE_P<int16_t>(data));
    ExtUI::setBedPID(
      ExtUI::getBedPID_Kp(),
      ExtUI::getBedPID_Ki(),
      pidValue
    );
  }

#endif // PIDTEMPBED

void DGUSRxHandler::fanSpeed(DGUS_VP &vp, void *data) {
  const_float_t percentage = dgus.fromFixedPoint<int16_t, float, 0>(Endianness::fromBE_P<int16_t>(data));
  ExtUI::setTargetFan_percent(percentage, ExtUI::FAN0);
}

void DGUSRxHandler::sdCardFileSection(DGUS_VP &vp, void *data) {
  uint8_t sdFileIndex = Endianness::fromBE_P<int16_t>(data) - 1;

  #if ENABLED(DGUS_USERCONFIRM)
    if (screen.isOnUserConfirmationScreen()) {
      screen.userConfirmation();
      return;
    }
  #endif
  dgus_sdcard_handler.onFileSelect(DGUS_FILE_FROM_INDEX(sdFileIndex % 5));
}

void DGUSRxHandler::stringToExtra(DGUS_VP &vp, void *data_ptr) {
  if (!vp.size || !vp.extra) return;
  memcpy(vp.extra, data_ptr, vp.size);
}

void DGUSRxHandler::disabled(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);
  screen.angryBeeps(2);
}

#endif // DGUS_LCD_UI_E3S1PRO

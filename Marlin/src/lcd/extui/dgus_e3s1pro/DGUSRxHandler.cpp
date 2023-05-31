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

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_E3S1PRO

#include "DGUSRxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Screen.h"

#include "../ui_api.h"
#include "../../../core/language.h"
#include "../../../module/stepper.h"

void DGUSRxHandler::PrintSpeedPercentage(DGUS_VP &vp, void *data)
{
  const_float_t feedratePercentage = dgus_display.FromFixedPoint<int16_t, const_float_t, 0>(BE16_P(data));
  ExtUI::setFeedrate_percent(feedratePercentage);
}

void DGUSRxHandler::ZOffset(DGUS_VP &vp, void *data)
{
  const_float_t zoffset = dgus_display.FromFixedPoint<int16_t, const_float_t, 2>(BE16_P(data));
  const_float_t currentzOffset = ExtUI::getZOffset_mm();
  const_float_t zStepsPerMm = ExtUI::getAxisSteps_per_mm(ExtUI::Z);
  int16_t zStepsDiff = zStepsPerMm * (zoffset - currentzOffset);

  ExtUI::babystepAxis_steps(zStepsDiff, ExtUI::Z);
  ExtUI::setZOffset_mm(zoffset);
}

void DGUSRxHandler::ExtruderTargetTemp(DGUS_VP &vp, void *data)
{
  const_float_t temperature = BE16_P(data);
  ExtUI::setTargetTemp_celsius(temperature, ExtUI::H0);
}

void DGUSRxHandler::BedTargetTemp(DGUS_VP &vp, void *data)
{
  const_float_t temperature = BE16_P(data);
  ExtUI::setTargetTemp_celsius(temperature, ExtUI::BED);
}

void DGUSRxHandler::Axis_X(DGUS_VP &vp, void *data)
{
  const_float_t axisValue = dgus_display.FromFixedPoint<int16_t, float, 1>(BE16_P(data));
  ExtUI::setAxisPosition_mm(axisValue, ExtUI::X);
}

void DGUSRxHandler::Axis_Y(DGUS_VP &vp, void *data)
{
  const_float_t axisValue = dgus_display.FromFixedPoint<int16_t, float, 1>(BE16_P(data));
  ExtUI::setAxisPosition_mm(axisValue, ExtUI::Y);
}

void DGUSRxHandler::Axis_Z(DGUS_VP &vp, void *data)
{
  const_float_t axisValue = dgus_display.FromFixedPoint<int16_t, float, 1>(BE16_P(data));
  ExtUI::setAxisPosition_mm(axisValue, ExtUI::Z);
}

void DGUSRxHandler::ExtrudeLength(DGUS_VP &vp, void *data)
{
  const_float_t length = dgus_display.FromFixedPoint<int16_t, const_float_t, 1>(BE16_P(data));
  const_float_t currentPosition = ExtUI::getAxisPosition_mm(ExtUI::E0);
  ExtUI::setAxisPosition_mm(currentPosition+length, ExtUI::E0);
}

void DGUSRxHandler::RetractLength(DGUS_VP &vp, void *data)
{
  const_float_t length = dgus_display.FromFixedPoint<int16_t, const_float_t, 1>(BE16_P(data));
  const_float_t currentPosition = ExtUI::getAxisPosition_mm(ExtUI::E0);
  ExtUI::setAxisPosition_mm(currentPosition-length, ExtUI::E0);
}

void DGUSRxHandler::Language_SetLanguage(DGUS_VP &vp, void *data)
{
  DGUS_Data::Language language = (DGUS_Data::Language)BE16_P(data);
  dgus_screen_handler.config.language = language;
  dgus_screen_handler.TriggerEEPROMSave();
  dgus_screen_handler.TriggerFullUpdate();
}

#if ENABLED(PIDTEMPBED)
  void DGUSRxHandler::Bed_PID_P(DGUS_VP &vp, void *data)
  {
    float pidValue = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
    ExtUI::setBedPID(
      pidValue,
      ExtUI::getBedPID_Ki(),
      ExtUI::getBedPID_Kd()
    );
  }

  void DGUSRxHandler::Bed_PID_I(DGUS_VP &vp, void *data)
  {
    float pidValue = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
    ExtUI::setBedPID(
      ExtUI::getBedPID_Kp(),
      pidValue,
      ExtUI::getBedPID_Kd()
    );
  }

  void DGUSRxHandler::Bed_PID_D(DGUS_VP &vp, void *data)
  {
    float pidValue = dgus_display.FromFixedPoint<uint16_t, float, 2>(BE16_P(data));
    ExtUI::setBedPID(
      ExtUI::getBedPID_Kp(),
      ExtUI::getBedPID_Ki(),
      pidValue
    );
  }
#endif // PIDTEMPBED

void DGUSRxHandler::Control_SetFanSpeed(DGUS_VP &vp, void *data)
{
  const_float_t percentage = BE16_P(data);
  ExtUI::setTargetFan_percent(percentage, ExtUI::FAN0);
}

void DGUSRxHandler::SDCard_FileSelection(DGUS_VP &vp, void *data)
{
  uint8_t sdFileIndex = BE16_P(data) - 1;

#if ENABLED(DGUS_USERCONFIRM)
  if (dgus_screen_handler.IsOnUserConfirmationScreen())
  {
    dgus_screen_handler.UserConfirmation();
  }
  else
#endif
    dgus_sdcard_handler.onFileSelect(DGUS_FILE_FROM_INDEX(sdFileIndex % 5));
}

void DGUSRxHandler::StringToExtra(DGUS_VP &vp, void *data_ptr) {
  if (!vp.size || !vp.extra) return;
  memcpy(vp.extra, data_ptr, vp.size);
}

void DGUSRxHandler::Disabled(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);
  dgus_screen_handler.AngryBeeps(2);
}

#endif // DGUS_LCD_UI_RELOADED

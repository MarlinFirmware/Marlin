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

#if ENABLED(DGUS_LCD_UI_E3V2_TOUCH)

#include "DGUSRxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Screen.h"

#include "../ui_api.h"

static int16_t readWord(void *data) { return Endianness::fromBE_P<int16_t>(data); }

void DGUSRxHandler::printSpeedPercentage(DGUS_VP &vp, void *data) {
  ExtUI::setFeedrate_percent(readWord(data));
}

/**
 * The screen sends the new offset in 0.01mm. With a probe it is the probe offset,
 * and the difference is babystepped so it applies at once. Without a probe
 * ExtUI babysteps the difference itself.
 */
void DGUSRxHandler::zOffset(DGUS_VP &vp, void *data) {
  const float zoffset = dgus.fromFixedPoint<int16_t, float, 2>(readWord(data));
  #if HAS_BED_PROBE
    const int16_t zStepsDiff = ExtUI::getAxisSteps_per_mm(ExtUI::Z) * (zoffset - ExtUI::getZOffset_mm());
    ExtUI::babystepAxis_steps(zStepsDiff, ExtUI::Z);
  #endif
  ExtUI::setZOffset_mm(zoffset);
  screen.triggerEEPROMSave();
}

void DGUSRxHandler::extruderTargetTemp(DGUS_VP &vp, void *data) {
  ExtUI::setTargetTemp_celsius(readWord(data), ExtUI::H0);
}

void DGUSRxHandler::bedTargetTemp(DGUS_VP &vp, void *data) {
  ExtUI::setTargetTemp_celsius(readWord(data), ExtUI::BED);
}

void DGUSRxHandler::axis_X(DGUS_VP &vp, void *data) {
  ExtUI::setAxisPosition_mm(dgus.fromFixedPoint<int16_t, float, 1>(readWord(data)), ExtUI::X);
}

void DGUSRxHandler::axis_Y(DGUS_VP &vp, void *data) {
  ExtUI::setAxisPosition_mm(dgus.fromFixedPoint<int16_t, float, 1>(readWord(data)), ExtUI::Y);
}

void DGUSRxHandler::axis_Z(DGUS_VP &vp, void *data) {
  ExtUI::setAxisPosition_mm(dgus.fromFixedPoint<int16_t, float, 1>(readWord(data)), ExtUI::Z);
}

void DGUSRxHandler::filamentLength(DGUS_VP &vp, void *data) {
  screen.filamentLength = dgus.fromFixedPoint<int16_t, float, 1>(readWord(data));
}

void DGUSRxHandler::setLanguage(DGUS_VP &vp, void *data) {
  const DGUS_Data::Language language = (DGUS_Data::Language)readWord(data);
  if (!WITHIN(language, DGUS_Data::Language::Chinese_Simplified, DGUS_Data::Language::Turkish)) return;
  screen.config.language = language;
  screen.triggerEEPROMSave();
  screen.triggerScreenChange(DGUS_ScreenID::CONTROL);
}

void DGUSRxHandler::refresh(DGUS_VP &vp, void *data) {
  screen.triggerFullUpdate();
}

#endif // DGUS_LCD_UI_E3V2_TOUCH

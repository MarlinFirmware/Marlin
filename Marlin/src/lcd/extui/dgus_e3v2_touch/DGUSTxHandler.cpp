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

#include "DGUSTxHandler.h"

#include "DGUSScreenHandler.h"
#include "DGUSSDCardHandler.h"
#include "config/DGUS_Data.h"

#include "../../../module/stepper.h" // for axis_enabled
#include "../../../libs/duration_t.h"
#if HAS_MEDIA
  #include "../../../sd/cardreader.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

static void writeWord(const DGUS_VP &vp, const uint16_t data) {
  dgus.write((uint16_t)vp.addr, Endianness::toBE(data));
}

void DGUSTxHandler::bootAnimation(DGUS_VP &vp) {
  static uint16_t bootIcon = 0;

  writeWord(vp, bootIcon);

  if (++bootIcon > 100) {
    bootIcon = 0;
    screen.triggerScreenChange(DGUS_ScreenID::HOME);
  }
}

void DGUSTxHandler::zOffset(DGUS_VP &vp) {
  const int16_t data = dgus.toFixedPoint<float, int16_t, 2>(ExtUI::getZOffset_mm()); // Round to 0.01
  writeWord(vp, data);
}

void DGUSTxHandler::elapsedHours(DGUS_VP &vp) {
  const duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  writeWord(vp, elapsedtime.hour());
}

void DGUSTxHandler::elapsedMinutes(DGUS_VP &vp) {
  const duration_t elapsedtime = ExtUI::getProgress_seconds_elapsed();
  writeWord(vp, elapsedtime.minute() % 60);
}

void DGUSTxHandler::printPercentage(DGUS_VP &vp) {
  writeWord(vp, ExtUI::getProgress_percent());
}

// The bar icon shows nothing at 0, so a running job starts at 1
void DGUSTxHandler::printPercentageIcon(DGUS_VP &vp) {
  const uint8_t percent = ExtUI::getProgress_percent();
  writeWord(vp, ExtUI::isPrinting() ? _MIN(percent + 1, 100) : percent);
}

void DGUSTxHandler::printSpeedPercentage(DGUS_VP &vp) {
  writeWord(vp, ExtUI::getFeedrate_percent());
}

void DGUSTxHandler::fanIcon(DGUS_VP &vp) {
  #if HAS_FAN
    const bool fanOn = ExtUI::getTargetFan_percent(ExtUI::FAN0) > 0;
  #else
    constexpr bool fanOn = false;
  #endif
  writeWord(vp, fanOn ? DGUS_FAN_ICON_ON : DGUS_FAN_ICON_OFF);
}

void DGUSTxHandler::extruderTargetTemp(DGUS_VP &vp) {
  writeWord(vp, (int16_t)ExtUI::getTargetTemp_celsius(ExtUI::H0));
}

void DGUSTxHandler::extruderCurrentTemp(DGUS_VP &vp) {
  writeWord(vp, (int16_t)ExtUI::getActualTemp_celsius(ExtUI::H0));
}

void DGUSTxHandler::bedTargetTemp(DGUS_VP &vp) {
  writeWord(vp, (int16_t)ExtUI::getTargetTemp_celsius(ExtUI::BED));
}

void DGUSTxHandler::bedCurrentTemp(DGUS_VP &vp) {
  writeWord(vp, (int16_t)ExtUI::getActualTemp_celsius(ExtUI::BED));
}

void DGUSTxHandler::axis_X(DGUS_VP &vp) {
  writeWord(vp, dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::X)));
}

void DGUSTxHandler::axis_Y(DGUS_VP &vp) {
  writeWord(vp, dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Y)));
}

void DGUSTxHandler::axis_Z(DGUS_VP &vp) {
  writeWord(vp, dgus.toFixedPoint<float, int16_t, 1>(ExtUI::getAxisPosition_mm(ExtUI::Z)));
}

void DGUSTxHandler::filamentLength(DGUS_VP &vp) {
  writeWord(vp, dgus.toFixedPoint<float, int16_t, 1>(screen.filamentLength));
}

// One icon per probed point, up to the 16 the screen has
void DGUSTxHandler::levelingProgressIcon(DGUS_VP &vp) {
  writeWord(vp, constrain(screen.currentMeshPointIndex, 1, 16));
}

// Language-specific "no filament" label, or a blank icon when filament is loaded
void DGUSTxHandler::filamentMissingIcon(DGUS_VP &vp) {
  #if HAS_FILAMENT_SENSOR
    const bool missing = ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE;
  #else
    constexpr bool missing = false;
  #endif
  writeWord(vp, missing ? (uint16_t)screen.config.language : DGUS_FILAMENT_ICON_BLANK);
}

void DGUSTxHandler::stepperStatus(DGUS_VP &vp) {
  const bool areSteppersEnabled = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
  writeWord(vp, areSteppersEnabled ? DGUS_STEPPER_ICON_ON : DGUS_STEPPER_ICON_OFF);
}

void DGUSTxHandler::printFilename(DGUS_VP &vp) {
  const char *name = nullptr;
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (screen.getCurrentScreen() == DGUS_ScreenID::POWERCONTINUE)
      name = recovery.info.sd_filename;
    else
  #endif
  TERN_(HAS_MEDIA, if (ExtUI::isPrintingFromMedia()) name = card.longest_filename());
  dgus.writeString((uint16_t)vp.addr, name ?: "", vp.size, true, true);
}

// File rows 1-20 map onto consecutive icon and colour addresses
void DGUSTxHandler::fileSelectionIcon(DGUS_VP &vp) {
  const int8_t row = (uint16_t)vp.addr - (uint16_t)DGUS_Addr::SDCARD_Selection_File1;
  writeWord(vp, row == dgus_sdcard_handler.selected ? DGUS_FILE_ICON_SELECTED : DGUS_FILE_ICON_NORMAL);
}

void DGUSTxHandler::fileSelectionColor(DGUS_VP &vp) {
  const int8_t row = ((uint16_t)vp.addr - (uint16_t)DGUS_Addr::SDCARD_Color_File1) / 0x10;
  writeWord(vp, row == dgus_sdcard_handler.selected ? DGUS_FILE_COLOR_SELECTED : DGUS_FILE_COLOR_NORMAL);
}

void DGUSTxHandler::extraToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;
  dgus.writeString((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

void DGUSTxHandler::extraPGMToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;
  dgus.writeStringPGM((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

#endif // DGUS_LCD_UI_E3V2_TOUCH

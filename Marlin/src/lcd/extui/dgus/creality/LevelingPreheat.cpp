/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../../inc/MarlinConfig.h" // HAS_HEATED_BED needs the full config

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "LevelingPreheat.h"

#if CREALITY_LEVEL_PREHEAT

#include "../DGUSScreenHandler.h"
#include "../../ui_api.h"
#include "../../../../module/temperature.h"

LevelingPreheat::State LevelingPreheat::state = LevelingPreheat::IDLE;
FSTR_P LevelingPreheat::commands;
millis_t LevelingPreheat::soak_end_ms, LevelingPreheat::next_message_ms;

static bool atTemperature() {
  const bool hotend_ok = !thermalManager.leveling_temp.hotend
                      || ExtUI::getActualTemp_celsius(ExtUI::E0) + (TEMP_WINDOW) >= thermalManager.leveling_temp.hotend,
             bed_ok    = !thermalManager.leveling_temp.bed
                      || ExtUI::getActualTemp_celsius(ExtUI::BED) + (TEMP_BED_WINDOW) >= thermalManager.leveling_temp.bed;
  return hotend_ok && bed_ok;
}

void LevelingPreheat::startThen(FSTR_P const cmds) {
  commands = cmds;

  // Raise the targets, never lower them (as Marlin's preheat does)
  if (thermalManager.leveling_temp.hotend > ExtUI::getTargetTemp_celsius(ExtUI::E0))
    ExtUI::setTargetTemp_celsius(thermalManager.leveling_temp.hotend, ExtUI::E0);
  if (thermalManager.leveling_temp.bed > ExtUI::getTargetTemp_celsius(ExtUI::BED))
    ExtUI::setTargetTemp_celsius(thermalManager.leveling_temp.bed, ExtUI::BED);

  if (atTemperature()) {                // Nothing to wait for, so nothing to soak
    state = IDLE;
    ExtUI::injectCommands(commands);
    return;
  }

  state = HEATING;
  next_message_ms = 0;
  screen.setStatusMessage(F("Heating before leveling..."));
}

void LevelingPreheat::loop() {
  if (state == IDLE) return;

  const millis_t ms = millis();

  if (state == HEATING) {
    if (!atTemperature()) return;
    #ifdef LEVELING_HEAT_SOAK_TIME
      state = SOAKING;
      soak_end_ms = ms + SEC_TO_MS(LEVELING_HEAT_SOAK_TIME);
      next_message_ms = 0;
    #else
      state = IDLE;
      ExtUI::injectCommands(commands);
      return;
    #endif
  }

  #ifdef LEVELING_HEAT_SOAK_TIME
    if (ELAPSED(ms, soak_end_ms)) {
      state = IDLE;
      screen.setStatusMessage(F("Heat soak done"));
      ExtUI::injectCommands(commands);
    }
    else if (ELAPSED(ms, next_message_ms)) {
      next_message_ms = ms + 1000UL;
      const uint16_t secs = uint16_t((soak_end_ms - ms + 999UL) / 1000UL);
      MString<32> msg;
      msg.setf(F("Heat soak %u:%02u"), secs / 60, secs % 60);
      screen.setStatusMessage(msg);
    }
  #endif
}

void LevelingPreheat::cancel() {
  if (state == IDLE) return;
  state = IDLE;
  screen.setStatusMessage(F("Canceled heating"));
}

#endif // CREALITY_LEVEL_PREHEAT
#endif // DGUS_LCD_UI_CREALITY_TOUCH

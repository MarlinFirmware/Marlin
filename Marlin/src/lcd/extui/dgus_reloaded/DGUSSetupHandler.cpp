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

#if DGUS_LCD_UI_RELOADED

#include "DGUSSetupHandler.h"

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"

#include "../../../gcode/queue.h"

#if HAS_MEDIA
  bool DGUSSetupHandler::print() {
    screen.filelist.refresh();

    while (!screen.filelist.isAtRootDir()) {
      screen.filelist.upDir();
    }

    screen.filelist_offset = 0;
    screen.filelist_selected = -1;

    return true;
  }
#endif

bool DGUSSetupHandler::printStatus() {
  if (ExtUI::isPrinting() || ExtUI::isPrintingPaused()) {
    return true;
  }

  screen.triggerScreenChange(DGUS_ScreenID::PRINT_FINISHED);
  return false;
}

bool DGUSSetupHandler::printAdjust() {
  if (ExtUI::isPrinting() || ExtUI::isPrintingPaused()) {
    return true;
  }

  screen.triggerScreenChange(DGUS_ScreenID::PRINT_FINISHED);
  return false;
}

bool DGUSSetupHandler::levelingMenu() {
  ExtUI::setLevelingActive(screen.leveling_active);

  if (!screen.isPrinterIdle()) {
    screen.setStatusMessage(GET_TEXT_F(DGUS_MSG_BUSY));
    return false;
  }

  if (ExtUI::isPositionKnown()) {
    if (ExtUI::getAxisPosition_mm(ExtUI::Z) < 10.0f) {
      queue.enqueue_now(F("G0Z10"));
    }

    return true;
  }

  screen.showWaitScreen(GET_TEXT_F(DGUS_MSG_HOMING), DGUS_ScreenID::LEVELING_MENU);

  queue.enqueue_now(F("G28"));

  return false;
}

bool DGUSSetupHandler::levelingManual() {
  ExtUI::setLevelingActive(false);

  if (ExtUI::isPositionKnown()) {
    return true;
  }

  if (!screen.isPrinterIdle()) {
    screen.setStatusMessage(GET_TEXT_F(DGUS_MSG_BUSY));
    return false;
  }

  screen.showWaitScreen(GET_TEXT_F(DGUS_MSG_HOMING), DGUS_ScreenID::LEVELING_MANUAL);

  queue.enqueue_now(F("G28"));

  return false;
}

bool DGUSSetupHandler::levelingOffset() {
  screen.offset_steps = DGUS_Data::StepSize::MMP1;

  if (!screen.isPrinterIdle()) {
    screen.setStatusMessage(GET_TEXT_F(DGUS_MSG_BUSY));
    return false;
  }

  if (ExtUI::isPositionKnown()) {
    if (ExtUI::getAxisPosition_mm(ExtUI::Z) < 4.0f) {
      queue.enqueue_now(F("G0Z4"));
    }

    char buffer[20];
    snprintf_P(buffer, sizeof(buffer), PSTR("G0X%dY%d"), DGUS_LEVEL_CENTER_X, DGUS_LEVEL_CENTER_Y);

    queue.enqueue_one_now(buffer);
    queue.enqueue_now(F("G0Z0"));

    return true;
  }

  screen.showWaitScreen(GET_TEXT_F(DGUS_MSG_HOMING), DGUS_ScreenID::LEVELING_OFFSET);

  queue.enqueue_now(F("G28"));

  return false;
}

bool DGUSSetupHandler::levelingAutomatic() {
  if (ExtUI::getLevelingIsValid()) {
    screen.leveling_active = true;

    ExtUI::setLevelingActive(true);
  }

  return true;
}

bool DGUSSetupHandler::levelingProbing() {
  screen.probing_icons[0] = 0;
  screen.probing_icons[1] = 0;

  return true;
}

bool DGUSSetupHandler::filament() {
  screen.filament_extruder = DGUS_Data::Extruder::CURRENT;
  screen.filament_length = DGUS_DEFAULT_FILAMENT_LEN;

  return true;
}

bool DGUSSetupHandler::move() {
  screen.move_steps = DGUS_Data::StepSize::MM10;

  if (!screen.isPrinterIdle()) {
    screen.setStatusMessage(GET_TEXT_F(DGUS_MSG_BUSY));
    return false;
  }

  return true;
}

bool DGUSSetupHandler::gcode() {
  ZERO(screen.gcode);

  if (dgus.gui_version < 0x30 || dgus.os_version < 0x21) {
    screen.setStatusMessage(GET_TEXT_F(DGUS_MSG_FW_OUTDATED));
    return false;
  }

  return true;
}

bool DGUSSetupHandler::pid() {
  screen.pid_heater = DGUS_Data::Heater::H0;
  screen.pid_temp = DGUS_PLA_TEMP_HOTEND;

  return true;
}

bool DGUSSetupHandler::infos() {
  screen.debug_count = 0;

  return true;
}

#endif // DGUS_LCD_UI_RELOADED

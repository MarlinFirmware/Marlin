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

#if ENABLED(DGUS_LCD_UI_RELOADED)

#include "DGUSSetupHandler.h"

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"

#include "../../../gcode/queue.h"

#if ENABLED(SDSUPPORT)
  bool DGUSSetupHandler::Print() {
    dgus_screen_handler.filelist.refresh();

    while (!dgus_screen_handler.filelist.isAtRootDir()) {
      dgus_screen_handler.filelist.upDir();
    }

    dgus_screen_handler.filelist_offset = 0;
    dgus_screen_handler.filelist_selected = -1;

    return true;
  }
#endif

bool DGUSSetupHandler::PrintStatus() {
  if (ExtUI::isPrinting() || ExtUI::isPrintingPaused()) {
    return true;
  }

  dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PRINT_FINISHED);
  return false;
}

bool DGUSSetupHandler::PrintAdjust() {
  if (ExtUI::isPrinting() || ExtUI::isPrintingPaused()) {
    return true;
  }

  dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PRINT_FINISHED);
  return false;
}

bool DGUSSetupHandler::LevelingMenu() {
  ExtUI::setLevelingActive(dgus_screen_handler.leveling_active);

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessage(FPSTR(DGUS_MSG_BUSY));
    return false;
  }

  if (ExtUI::isPositionKnown()) {
    if (ExtUI::getAxisPosition_mm(ExtUI::Z) < 10.0f) {
      queue.enqueue_now(F("G0Z10"));
    }

    return true;
  }

  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 1);
  dgus_screen_handler.SetMessageLinePGM(DGUS_MSG_HOMING, 2);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 3);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 4);
  dgus_screen_handler.ShowWaitScreen(DGUS_Screen::LEVELING_MENU);

  queue.enqueue_now_P(DGUS_CMD_HOME);

  return false;
}

bool DGUSSetupHandler::LevelingManual() {
  ExtUI::setLevelingActive(false);

  if (ExtUI::isPositionKnown()) {
    return true;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessage(FPSTR(DGUS_MSG_BUSY));
    return false;
  }

  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 1);
  dgus_screen_handler.SetMessageLinePGM(DGUS_MSG_HOMING, 2);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 3);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 4);
  dgus_screen_handler.ShowWaitScreen(DGUS_Screen::LEVELING_MANUAL);

  queue.enqueue_now_P(DGUS_CMD_HOME);

  return false;
}

bool DGUSSetupHandler::LevelingOffset() {
  dgus_screen_handler.offset_steps = DGUS_Data::StepSize::MMP1;

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessage(FPSTR(DGUS_MSG_BUSY));
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

  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 1);
  dgus_screen_handler.SetMessageLinePGM(DGUS_MSG_HOMING, 2);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 3);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 4);
  dgus_screen_handler.ShowWaitScreen(DGUS_Screen::LEVELING_OFFSET);

  queue.enqueue_now_P(DGUS_CMD_HOME);

  return false;
}

bool DGUSSetupHandler::LevelingAutomatic() {
  if (ExtUI::getMeshValid()) {
    dgus_screen_handler.leveling_active = true;

    ExtUI::setLevelingActive(true);
  }

  return true;
}

bool DGUSSetupHandler::LevelingProbing() {
  dgus_screen_handler.probing_icons[0] = 0;
  dgus_screen_handler.probing_icons[1] = 0;

  return true;
}

bool DGUSSetupHandler::Filament() {
  dgus_screen_handler.filament_extruder = DGUS_Data::Extruder::CURRENT;
  dgus_screen_handler.filament_length = DGUS_DEFAULT_FILAMENT_LEN;

  return true;
}

bool DGUSSetupHandler::Move() {
  dgus_screen_handler.move_steps = DGUS_Data::StepSize::MM10;

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessage(FPSTR(DGUS_MSG_BUSY));
    return false;
  }

  return true;
}

bool DGUSSetupHandler::Gcode() {
  ZERO(dgus_screen_handler.gcode);

  if (dgus_display.gui_version < 0x30 || dgus_display.os_version < 0x21) {
    dgus_screen_handler.SetStatusMessage(FPSTR(DGUS_MSG_FW_OUTDATED));
    return false;
  }

  return true;
}

bool DGUSSetupHandler::PID() {
  dgus_screen_handler.pid_heater = DGUS_Data::Heater::H0;
  dgus_screen_handler.pid_temp = DGUS_PLA_TEMP_HOTEND;

  return true;
}

bool DGUSSetupHandler::Infos() {
  dgus_screen_handler.debug_count = 0;

  return true;
}

#endif // DGUS_LCD_UI_RELOADED

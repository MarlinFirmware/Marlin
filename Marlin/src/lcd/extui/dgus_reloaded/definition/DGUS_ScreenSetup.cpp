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

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_RELOADED

#include "DGUS_ScreenSetup.h"

#include "../DGUSSetupHandler.h"

#include "../../ui_api.h"

#define SETUP_HELPER(SCREEN, SETUP) { .screenID = SCREEN, .setup_fn = SETUP }

const struct DGUS_ScreenSetup screen_setup_list[] PROGMEM = {
  #if HAS_MEDIA
    SETUP_HELPER(DGUS_ScreenID::PRINT,            &DGUSSetupHandler::print),
  #endif
  SETUP_HELPER(DGUS_ScreenID::PRINT_STATUS,       &DGUSSetupHandler::printStatus),
  SETUP_HELPER(DGUS_ScreenID::PRINT_ADJUST,       &DGUSSetupHandler::printAdjust),
  SETUP_HELPER(DGUS_ScreenID::LEVELING_MENU,      &DGUSSetupHandler::levelingMenu),
  SETUP_HELPER(DGUS_ScreenID::LEVELING_OFFSET,    &DGUSSetupHandler::levelingOffset),
  SETUP_HELPER(DGUS_ScreenID::LEVELING_MANUAL,    &DGUSSetupHandler::levelingManual),
  SETUP_HELPER(DGUS_ScreenID::LEVELING_AUTOMATIC, &DGUSSetupHandler::levelingAutomatic),
  SETUP_HELPER(DGUS_ScreenID::LEVELING_PROBING,   &DGUSSetupHandler::levelingProbing),
  SETUP_HELPER(DGUS_ScreenID::FILAMENT,           &DGUSSetupHandler::filament),
  SETUP_HELPER(DGUS_ScreenID::MOVE,               &DGUSSetupHandler::move),
  SETUP_HELPER(DGUS_ScreenID::GCODE,              &DGUSSetupHandler::gcode),
  SETUP_HELPER(DGUS_ScreenID::PID,                &DGUSSetupHandler::pid),
  SETUP_HELPER(DGUS_ScreenID::INFOS,              &DGUSSetupHandler::infos),

  SETUP_HELPER((DGUS_ScreenID)0, nullptr)
};

#endif // DGUS_LCD_UI_RELOADED

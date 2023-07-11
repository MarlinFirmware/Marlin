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

#include "DGUSSetupHandler.h"

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"

typedef void (*controlSet_t)(const DGUS_ScreenID, DGUSDisplay::DGUS_ControlType, DGUS_Control);

bool DGUSSetupHandler::sdCardPrepare() {
  bool isConfirm = false;

  #if ENABLED(DGUS_USERCONFIRM)
    isConfirm = screen.isOnUserConfirmationScreen();
    controlSet_t controlSet = isConfirm ? dgus.disableControl : dgus.enableControl;

    // Only allow the fifth line used as the confirm button to be clicked
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File1);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File2);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File3);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File4);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Start);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_End);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Prev);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Next);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Home);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Print);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Ready);
    controlSet(DGUS_ScreenID::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Settings);
  #endif

  if (!isConfirm) dgus_sdcard_handler.onPageLoad(DGUS_SCREEN_TO_PAGE(screen.getCurrentScreen()));
  return true;
}

#endif // DGUS_LCD_UI_E3S1PRO

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

#include "DGUSSetupHandler.h"

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"

bool DGUSSetupHandler::SDCardPrepare()
{
  #if ENABLED(DGUS_USERCONFIRM)
    if (dgus_screen_handler.IsOnUserConfirmationScreen())
    {
      // Only allow the fifth line used as the confirm button to be clicked
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File1);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File2);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File3);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File4);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Start);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_End);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Prev);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Next);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Home);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Print);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Ready);
      dgus_display.DisableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Settings);

      return true; // skip the file update
    }
    else
    {
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File1);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File2);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File3);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_File4);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Start);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_End);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Prev);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Next);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Home);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Print);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Ready);
      dgus_display.EnableControl(DGUS_Screen::FILE1, DGUSDisplay::DGUS_ControlType::RETURN_KEY_CODE, DGUS_Control::FILE1_Settings);
    }
  #endif
  
  dgus_sdcard_handler.onPageLoad(DGUS_SCREEN_TO_PAGE(dgus_screen_handler.GetCurrentScreen()));
  return true;
}

#endif // DGUS_LCD_UI_RELOADED

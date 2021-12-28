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

/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.8.1
 * Date: 2021/11/06
 */

#include "dwin.h"
#include "dwin_popup.h"

void Draw_Select_Highlight(const bool sel) {
  HMI_flag.select_flag = sel;
  const uint16_t c1 = sel ? HMI_data.Highlight_Color : HMI_data.PopupBg_color,
                 c2 = sel ? HMI_data.PopupBg_color : HMI_data.Highlight_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void DWIN_Popup_Continue(const uint8_t icon, FSTR_P const fmsg1, FSTR_P const fmsg2) {
  HMI_SaveProcessID(WaitResponse);
  DWIN_Draw_Popup(icon, fmsg1, fmsg2, ICON_Continue_E);  // Button Continue
  DWIN_UpdateLCD();
}

void DWIN_Popup_ConfirmCancel(const uint8_t icon, FSTR_P const fmsg2) {
  DWIN_Draw_Popup(ICON_BLTouch, F("Please confirm"), fmsg2);
  DWINUI::Draw_Icon(ICON_Confirm_E, 26, 280);
  DWINUI::Draw_Icon(ICON_Cancel_E, 146, 280);
  Draw_Select_Highlight(true);
  DWIN_UpdateLCD();
}
